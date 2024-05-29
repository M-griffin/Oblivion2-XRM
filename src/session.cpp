#include "session.hpp"

#include <memory>
#include <list>
#include <string>
#include <cassert>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "model-sys/users.hpp"
#include "data-sys/users_dao.hpp"

#include "state_manager.hpp"
#include "socket_handler.hpp"
#include "session_manager.hpp"
#include "telnet_decoder.hpp"
//#include "menu_system.hpp"
#include "menu_shell.hpp"

#include "logging.hpp"
#include "encoding.hpp"
#include "uuid.hpp"
#include "connection_base.hpp"
#include "communicator.hpp"

#include "libSqliteWrapped.h"

#include "mods/mod_prelogon.hpp"


/**
 * @brief Session Constructor
 * @param tcp_connection
 * @param session_manager
 * @return
 */
Session::Session(boost::asio::io_service& io_service, connection_ptr connection, session_manager_ptr my_session_manager)
    : m_log(Logging::getInstance())   
    , m_connection(connection)
    , m_session_manager(my_session_manager)
    , m_state_manager(nullptr)
    , m_telnet_decoder(nullptr)
    , m_user_record(nullptr)
    , m_node_number(0)
    , m_is_leaving(false)
    , m_raw_data()
    , m_parsed_data("")
    , m_session_id("")
    , m_encoding_text(Encoding::ENCODING_TEXT_UTF8)
    , m_encoding(Encoding::ENCODE_UTF8)    
    , m_is_use_ansi(true)
    , m_is_esc_timer(false)
    , m_is_session_authorized(false)
    , m_detection_deadline(io_service)
    , m_user_database(USERS_DATABASE, &m_database_log)
{
    //m_log.write<Logging::CONSOLE_LOG>("Session() Started.");
    std::cout << "Session() Started." << std::endl;
    
    // Setup Shared Pointers
    m_state_manager = std::make_shared<StateManager>();
    m_telnet_decoder = std::make_shared<TelnetDecoder>(connection);
    m_user_record = std::make_shared<Users>();  
    
}

Session::~Session()
{
    //m_log.write<Logging::CONSOLE_LOG>("~Session() Closed.");
    std::cout << "~Session() Closed." << std::endl;

    // Free the menu system state and modules when session closes.
    m_state_manager->clean();     
    m_connection.reset();
    m_telnet_decoder.reset();
    m_user_record.reset();
    m_session_manager.reset();    
}


/**
 * @brief Start ESC Key input timer
 */
void Session::startTelnetOptionNegoiation()
{
    // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences reponses
    m_detection_deadline.expires_from_now(boost::posix_time::milliseconds(2000));   
    
    auto self(shared_from_this());
    m_detection_deadline.async_wait(
    [this, self](const boost::system::error_code& ec)
    {
        if (!ec)
        {
            if(m_connection->is_open())
            {
                handleTelnetOptionNegoiation();
            }            
        }
        else 
        {
            std::cout << "Deadline Timer Exepction: " << ec.message() << std::endl;
            disconnectUser();
        }
    });
    
    
}

/**
 * @brief Timer Handler, When Called Starts up the Login State
 *        
 * @param timer
 */
void Session::handleTelnetOptionNegoiation()
{   
    std::cout << "handleTelnetOptionNegoiation - Deadline Terminal Detection, EXPIRED!" << std::endl;
    // Setup Node Number for Separate Thread.
    m_log.setUserInfo(m_node_number);
    
    // Starts Up the Menu System Then Loads up the PreLogin Sequence.   
    //state_ptr new_state = std::make_shared<MenuSystem>(shared_from_this());
    
    // TESTING FIXME
    if(m_connection->is_open())
    {
        state_ptr new_state = std::make_shared<MenuShell>(shared_from_this());
    
        std::cout << "handleTelnetOptionNegoiation - Starting Menu System State!" << std::endl;
        m_state_manager->changeState(new_state);    
    }
}

/**
 * @brief Deadline Detection Timer for Negotiation
 * @param timer
 */
void Session::handlePyBind11State()
{
    // Work in Progress, not yet in use for main develop branch
    // Also requries Dependencies and Extra setup on Make Files!
#ifdef HAVE_PYHON

    // Detection Completed, start ip the Pre-Logon Sequence State.
    state_ptr new_state = std::make_shared<PythonSystem>(m_session_data);
    
    m_state_manager->changeState(new_state);
    
#endif // HAVE_PYTHON
}

/**
 * @brief Callback from The Broadcaster to write data to the active sessions.
 * @param msg
 */
void Session::deliver(const std::string &msg, bool is_disconnection)
{
    std::cout << "deliver()" << std::endl;
    if(msg.size() == 0 || msg[0] == '\0')
    {
        std::cout << "deliver() size 0 - return" << std::endl;
        return;
    }
    
    std::string outputBuffer = "";
    
    // On Output, We have internal UTF8 now, translate to CP437
    if(m_encoding == Encoding::ENCODE_CP437)
    {
        outputBuffer = Encoding::getInstance().utf8Decode(msg);
    }
    else
    {
        outputBuffer = msg;
    }    

    if(m_connection->is_open())
    {                                         
        auto self(shared_from_this());        
        boost::asio::async_write(m_connection->m_normal_socket, boost::asio::buffer(outputBuffer, outputBuffer.size()),
        [this, self, is_disconnection](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (ec)
            {
                std::cout << "Async Write Error: " << ec.message() << std::endl;
                disconnectUser();
                return;
            }
        });
    }
    else
    {
        std::cout << "deliver - Error Code (CONNECTION CLOSED!) "
                  << " Client IP: "
                  << m_connection->m_normal_socket.remote_endpoint().address().to_string()
                  << std::endl;
        disconnectUser();
    }     
}

/**
 * @brief Data Handler for incoming Data (From Client)
 */
void Session::waitingForData()
{
    std::cout << "waiting For Data()" << std::endl;
    memset(&m_raw_data, 0, max_length); // * sizeof(m_raw_data));
    
    if(m_connection->is_open())
    {
        auto self(shared_from_this());        
        m_connection->m_normal_socket.async_read_some(boost::asio::buffer(m_raw_data, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (ec)
            {
                std::cout << "Async Read Error: " << ec.message() << " : " << m_is_leaving << std::endl;
                disconnectUser();                
                return;
            }
            
            if(m_connection->is_open())
            {
                handleRead(ec, length);                 
            }
        });
    }
}

/**
 * @brief Resolves the Hostname
 * @param ec
 * @param it
 */
void Session::resolveHandler(const boost::system::error_code &ec, tcp::resolver::iterator it)
{
    if(!ec)
    {
        tcp::resolver::iterator end;
        for(; it != end; ++it)
        {
            std::cout << "hostname: " << it->host_name() << std::endl;
        }
    }
    else
    {
        std::cout << "Error resolving hostname: " << ec.message() << std::endl;
    }
}

/**
 * @brief Passed data Though the State, and Checks ESC Timer
 */
void Session::updateState()
{
    
    // FIXME 
    std::cout << "FIX ME updateState " << std::endl;
    
    // Setup Logging per Current Node.
    m_log.setUserInfo(m_node_number);
    
    // Last Character Received is ESC, then Check for
    // ESC Sequence, or Lone ESC Key.
    if(m_parsed_data[m_parsed_data.size()-1] == '\x1b')
    {
        //startEscapeTimer();
        //m_is_esc_timer = true;
    }
    else if(!m_is_esc_timer)
    {
        //m_state_manager->update();
    }        
}



/**
 * @brief ESC Sequence Timer for Determining Single Key vs. ESC Sequence
 */
void Session::startEscapeTimer()
{
    std::cout << "startEscapeTimer " << std::endl;
    
    // Add Deadline Timer for .400 milliseconds for complete ESC Sequences.
    // Is no other input or part of ESC Sequences ie.. [A following the ESC
    // Then it's an ESC key, otherwise capture the rest of the sequence.
    //m_esc_input_timer->setWaitInMilliseconds(400);
    //m_esc_input_timer->asyncWait(
    //    std::bind(&Session::handleEscTimer, shared_from_this())
    //);
    
    
    //auto callback_function = std::bind(&Session::handleEscTimer, shared_from_this());
    //asyncWait(400, callback_function);
}
    
/**
 * @brief Deadline Input Timer for ESC vs ESC Sequence.
 * @param timer
 */
void Session::handleEscTimer()
{
    std::cout << "handleEscTimer " << std::endl;
    m_log.setUserInfo(m_node_number);
    
    // Move text to State Machine, Timer has passed, or remainder of Sequence caught up!
    m_state_manager->update();
    m_is_esc_timer = false;
}

/**
 * @brief Callback after data received. handles telnet options
 * Then parses out normal text data from client to server.
 * @param ec
 * @param length
 */
void Session::handleRead(const boost::system::error_code& ec, std::size_t length)
{
    m_log.setUserInfo(m_node_number);

    session_manager_ptr session_manager = m_session_manager.lock();
    if(!session_manager)
    {
        m_log.write<Logging::ERROR_LOG>("handleRead - Unable to load session_manager", length, __FILE__, __LINE__);
        disconnectUser();
        return;
    }

    if (ec || !m_connection->is_open())
    {        
        std::cout << "handleRead (ERROR)" << ((ec) ? ec.message() : "") << " : " << m_is_leaving << std::endl;
        disconnectUser();
        return;
    }
              
    // Part I: Parse Out Telnet Options and handle responses back to client.
    handleTeloptCodes();

    if(m_parsed_data.size() > 0)
    {
        // Windows Console Telnet sends [CR\LF] for ENTER!
        // search and replace input buffer we only need one!
        std::string::size_type id1 = 0;

        do
        {
            // Convert CR\LF to LF!
            id1 = m_parsed_data.find("\r\n", 0);

            if(id1 != std::string::npos)
            {
                m_parsed_data.erase(id1, 1);
                id1 = m_parsed_data.find("\r\n", 0);
            }
        }
        while(id1 != std::string::npos);


        // If were in a process, skip stat and ESC timer.
        /*
        if(m_is_process_running)
        {
            if(m_processes.size() > 0)
            {
                m_processes.back()->update();
            }
            else
            {
                // If no processes then reset.
                m_is_process_running = false;
                updateState();
            }
        }
        else
        {
            // Check for state cleanup
            if(m_processes.size() > 0)
            {
                clearProcess();
            }

            updateState();
        }*/
        
        std::cout << "updateState " << std::endl;
        updateState();
    }

    // Restart Callback to wait for more data.
    // If this step is skipped, then the node will exit
    // since io_service will have no more work!
    if(!m_connection->is_open())
    {
        m_log.write<Logging::WARN_LOG>("handleRead - m_connection no longer active");
        disconnectUser();
        return;
    }
    
    std::cout << "look back to  waitingForData" << std::endl;
    waitingForData();
}

/**
 * @brief Handle Telnet Options in incoming data
 * raw data is read in from socket
 * m_parsed_data is filled with parsed out options.
 */
void Session::handleTeloptCodes()
{
    std::cout << "handleTeloptCodes - inside" << std::endl;
    
    m_log.setUserInfo(m_node_number);
    
    std::string incoming_data = "";
    
    for(auto c : m_raw_data)
    {
        try
        {
            // Enter the Telnet_State and handle parsing options.
            unsigned char ch = m_telnet_decoder->telnetOptionParse(c);
            
            // Skip any incoming nulls, nulls are also return on Telnet options received
            // So we know that there is no valid text data to send to the client.
            if(ch == '\0')
            {
                continue;
            }

            // Incoming Buffer is filled and Telnet options are parsed out.
            incoming_data += ch;
            
        }    
        catch(std::exception& e)
        {
            m_log.write<Logging::ERROR_LOG>("Exception telnet_process_char", e.what(), __LINE__, __FILE__);
        }
    }
    
    // Clear the Session's Socket Buffer for next set of data.
    memset(&m_raw_data, 0, max_length); // * sizeof(m_raw_data));
    
    // Encoding is isolated to Output Translations of CP437 Textfiles.
    // For Incoming data we shouldn't translate as Parsing methods should handle.
    m_parsed_data += incoming_data;
}

/**
 * @brief Closes Socket Logging the user off, then gets routed to DisconnectUser() Below.
 */
void Session::logoff() 
{    
    std::cout << "logoff() : is_leaving=" << m_is_leaving << " : is_open=" << m_connection->is_open() << std::endl;
    
    try 
    {
        m_connection->m_normal_socket.shutdown(tcp::socket::shutdown_both);
        m_connection->m_normal_socket.close();
    }
    catch (std::exception &msg) 
    {
        std::cout << "Logoff() Exception: " << msg.what() << std::endl;
    }
                
    return; 
}

/**
 * @brief Handles Errors and Closeing the Session
 */
void Session::disconnectUser() 
{
    std::cout << "disconnectUser() : is_leaving=" << m_is_leaving << " : is_open=" << m_connection->is_open() << std::endl;
    if (m_is_leaving)
    {
        return;        
    }
    m_is_leaving = true;       

    // Close Down Sockets If they are Open Still.
    if (m_connection->is_open())
    {
        try 
        {            
            m_connection->m_normal_socket.shutdown(tcp::socket::shutdown_both);
            m_connection->m_normal_socket.close();
        }
        catch (std::exception &msg) 
        {
            std::cout << "DisconnectUser() Exception: " << msg.what() << std::endl;
        }
    }
    
    // Remove Session from Session Manager so it will Exit Cleanly.
    // Cancle The Timer incase it's running it can block proper session shutdown
    m_detection_deadline.cancel();
    session_manager_ptr session_manager = m_session_manager.lock();
    if(!session_manager)
    {
        m_log.write<Logging::ERROR_LOG>("disconnectUser - Unable to load session_manager", __FILE__, __LINE__);
        return;
    }
            
    // Review Sessions are being freed properly.
    std::cout << "Shutting Down Session : is_leaving=" << m_is_leaving << " : is_open=" << m_connection->is_open() << std::endl;
    std::cout << "Current Sessions cnt=" << session_manager->connections() << std::endl;    
    session_manager->leave(shared_from_this());
    std::cout << "Removed Session cnt=" << session_manager->connections() << std::endl;
    
}