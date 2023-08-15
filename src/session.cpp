#include "session.hpp"

#include "model-sys/users.hpp"
#include "data-sys/users_dao.hpp"

#include "state_manager.hpp"
#include "async_io.hpp"
#include "deadline_timer.hpp"
#include "socket_handler.hpp"
#include "session_manager.hpp"
#include "telnet_decoder.hpp"
#include "menu_system.hpp"

#include "logging.hpp"
#include "encoding.hpp"

#include "libSqliteWrapped.h"

#include <memory>
#include <list>
#include <string>
#include <cassert>
#include <thread>


/**
 * @brief Session Constructor
 * @param tcp_connection
 * @param session_manager
 * @return
 */
Session::Session(async_io_ptr const &my_async_io, session_manager_ptr const &my_session_manager)
    : m_log(Logging::getInstance())
    , m_async_io(my_async_io)        
    , m_session_manager(my_session_manager)
    , m_state_manager(new StateManager())
    , m_deadline_timer(new DeadlineTimer())
    , m_esc_input_timer(new DeadlineTimer())
    , m_telnet_decoder(new TelnetDecoder(my_async_io))
    , m_user_record(new Users())
    , m_node_number(0)
    , m_is_leaving(false)
    , m_parsed_data("")
    , m_encoding_text(Encoding::ENCODING_TEXT_UTF8)
    , m_encoding(Encoding::ENCODE_UTF8)
    , m_is_use_ansi(true)
    , m_is_esc_timer(false)
    , m_is_session_authorized(false)
    , m_user_database(USERS_DATABASE, &m_database_log)
{
    if(m_async_io->isActive())
    {
        try
        {
            m_log.write<Logging::DEBUG_LOG>("New Session Accepted", __LINE__, __FILE__);
        }
        catch(std::exception &ex)
        {
            m_log.write<Logging::ERROR_LOG>("New Session Exception=", ex.what(), __LINE__, __FILE__);
        }
    }

    // Get The First available node number. Needs Rework from session manager!
    m_node_number = 0;            
    m_log.write<Logging::CONSOLE_LOG>("New Session ConnectionNode Number=", m_node_number);
        
}

Session::~Session()
{
    std::cout << "~Session()" << std::endl;

    // Free the menu system state and modules when session closes.
    m_state_manager->clean();
    m_async_io.reset();    
    m_deadline_timer.reset();
    m_esc_input_timer.reset();
    m_telnet_decoder.reset();    
    m_user_record.reset();
    m_session_manager.reset();
    std::vector<unsigned char>().swap(m_in_data_vector);
}


/**
 * @brief Telnet Option Negoiation Timer
 */
void Session::startTelnetOptionNegoiation()
{
    std::cout << "startTelnetOptionNegoiation Called()" << std::endl;
    m_deadline_timer->setWaitInMilliseconds(2000);
    
    // Deprecated bind, look at replacing with lambda and std::function
    m_deadline_timer->asyncWait(
        std::bind(&Session::handleTelnetOptionNegoiation, shared_from_this())
    );        
}

/**
 * @brief Timer Handler, When Called Starts up the Login State
 *        
 * @param timer
 */
void Session::handleTelnetOptionNegoiation()
{        
    std::cout << "handleTelnetOptionNegoiation Called()" << std::endl;
    
    // Starts Up the Menu System Then Loads up the PreLogin Sequence.   
    state_ptr new_state(new MenuSystem(shared_from_this()));
    
    //state_ptr new_state(new MenuShell(shared_from_this()));    
    m_state_manager->changeState(new_state);
    
}

/**
 * @brief Deadline Detection Timer for Negotiation
 * @param timer
 */
void Session::handlePyBind11State()
{
// Work in Progress, not yet in use for main develop branch
#ifdef HAVE_PYHON
    // Detection Completed, start ip the Pre-Logon Sequence State.
    state_ptr new_state(new PythonSystem(m_session_data));
    m_state_manager->changeState(new_state);
#endif // HAVE_PYTHON
}

/**
 * @brief Callback from The Broadcaster to write data to the active sessions.
 * @param msg
 */
void Session::deliver(const std::string &msg, bool is_disconnection)
{
    if(msg.size() == 0 || msg[0] == '\0')
    {
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

    if(m_async_io->isActive()) // && TheCommunicator::instance()->isActive())
    {
        // Deprecated bind, look at replacing with lambda and std::function
        if (!is_disconnection)
        {
            m_async_io->asyncWrite(outputBuffer,
                                     std::bind(
                                         &Session::handleWrite,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         std::placeholders::_2));
        }
        else
        {
            m_async_io->asyncWrite(outputBuffer,
                                     std::bind(
                                         &Session::handleWriteThenDisconnect,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         std::placeholders::_2));
        }
    }
}

/**
 * @brief Callback after Writing Data, If error/hangup notifies
 *        Everyone this person has left.
 * @param error
 */
void Session::handleWrite(const std::error_code& error, socket_handler_ptr)
{
    if(error)
    {
        m_log.write<Logging::ERROR_LOG>("Async_HandleWrite Session Closed() error=", error.message(), __LINE__, __FILE__);
    }

    session_manager_ptr session_mgr = m_session_manager.lock();

    if(session_mgr && error) //&& (!m_session_data->m_is_leaving))
    {
        //m_is_leaving = true;

        m_log.write<Logging::DEBUG_LOG>("Async_HandleWrite Removing Sesison from Manager", __LINE__, __FILE__);
        
        if(m_async_io->isActive())
        {
            try
            {
                m_log.write<Logging::DEBUG_LOG>("Async_HandleWrite Leaving (NORMAL SESSION)", __LINE__, __FILE__);
                m_async_io->shutdown();
            }
            catch(std::exception &ex)
            {
                m_log.write<Logging::ERROR_LOG>("Async_HandleWrite Exception closing socket()", ex.what(), __LINE__, __FILE__);
            }
        }
        
        // Disconnect the session.
        if (m_async_io->isActive())
        {
            session_mgr->leave(shared_from_this());
            session_mgr.reset();
        }
    }
}

/**
 * @brief Callback after Writing Data, If error/hangup notifies
 *        Everyone this person has left.
 * @param error
 */
void Session::handleWriteThenDisconnect(const std::error_code& error, socket_handler_ptr ptr)
{
    handleWrite(error, ptr);
    disconnectUser();    
}


// Previous SessionData Methods

/**
 * @brief Data Handler for incoming Data (From Client)
 */
void Session::waitingForData()
{
    // Important, clear out buffer before each read.
    std::vector<unsigned char>().swap(m_in_data_vector);
    
    if(m_async_io->isActive()) // && TheCommunicator::instance()->isActive())
    {
        // Deprecated bind, look at replacing with lambda and std::function
        m_async_io->asyncRead(m_in_data_vector,
                                std::bind(
                                    &Session::handleRead,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
    }
}

/**
 * @brief Passed data Though the State, and Checks ESC Timer
 */
void Session::updateState()
{
    // Last Character Received is ESC, then Check for
    // ESC Sequence, or Lone ESC Key.
    std::cout << " Session::updateState()" << std::endl;
    if(m_parsed_data[m_parsed_data.size()-1] == '\x1b')
    {
        std::cout << " Session::updateState() - startEscapeTimer" << std::endl;
        startEscapeTimer();
        m_is_esc_timer = true;
    }
    else if(!m_is_esc_timer)
    {
        std::cout << " Session::updateState() - update!" << std::endl;
        m_state_manager->update();
    }
}

/**
 * @brief ESC Sequence Timer for Determining Single Key vs. ESC Sequence
 */
void Session::startEscapeTimer()
{
    // Add Deadline Timer for .400 milliseconds for complete ESC Sequences.
    // Is no other input or part of ESC Sequences ie.. [A following the ESC
    // Then it's an ESC key, otherwise capture the rest of the sequence.
    m_esc_input_timer->setWaitInMilliseconds(400);
    m_esc_input_timer->asyncWait(
        std::bind(&Session::handleEscTimer, shared_from_this())
    );
}
    
/**
 * @brief Deadline Input Timer for ESC vs ESC Sequence.
 * @param timer
 */
void Session::handleEscTimer()
{
    // Move text to State Machine, Timer has passed, or remainder of Sequence caught up!
    m_state_manager->update();
    m_is_esc_timer = false;
}

/**
 * @brief Callback after data received. handles telnet options
 * Then parses out normal text data from client to server.
 * @param error
 * @param bytes_transferred
 */
void Session::handleRead(const std::error_code& error, socket_handler_ptr)
{
    m_log.write<Logging::DEBUG_LOG>("handleRead - After Incoming Data.", __FILE__, __LINE__);

    session_manager_ptr session_manager = m_session_manager.lock();
    if(!session_manager)
    {
        m_log.write<Logging::ERROR_LOG>("handleRead - Unable to load session_manager", __FILE__, __LINE__);
        return;
    }

    if (error) 
    {        
        m_is_leaving = true;

            // Disconnect the session.
        std::cout << "handleRead - Leaving Session on Error" << std::endl;
        session_manager->leave(shared_from_this());            
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
        
        // PART II: Standalone For Now, Pushes Input to the State or Menu System.
        std::cout << "handleRead - updateState" << std::endl;
        updateState();
    }

    // Restart Callback to wait for more data.
    // If this step is skipped, then the node will exit
    // since io_service will have no more work!
    if(m_async_io->isActive())
    {
        waitingForData();
    }

}

/**
 * @brief Handle Telnet Options in incoming data
 * raw data is read in from socket
 * m_parsed_data is filled with parsed out options.
 */
void Session::handleTeloptCodes()
{
    std::string incoming_data = "";
    
    if (m_in_data_vector.size() == 0) 
    {
        return;
    }

    for(auto c : m_in_data_vector)
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
            
            // we see (2) 27 Keys here in double ESC!! 
            std::cout << "SESSION INPUT: " << int(ch) << std::endl;

            // Incoming Buffer is filled and Telnet options are parsed out.
            incoming_data += ch;
            
        }    
        catch(std::exception& e)
        {
            m_log.write<Logging::ERROR_LOG>("Exception telnet_process_char", e.what(), __LINE__, __FILE__);
        }
    }

    // Encode all incoming data as UTF8 unless we are not utf8
    // Updated, append Incoming to Parsed Data so we don't miss double ESC Sequences.
    // This catches items still in Buffer where sequences happen quickly. like Double ESC's.
    if(m_encoding != Encoding::ENCODE_UTF8)
    {
        m_parsed_data += Encoding::getInstance().utf8Encode(incoming_data);        
    }
    else
    {
        m_parsed_data += incoming_data;
    
    }
}

/**
 * @brief User Logoff
 */
void Session::logoff()
{
    
    // This might be doing double the deallocation    
    if(m_async_io && m_async_io->getSocketHandle()->isActive())
    {
        try
        {
            m_log.write<Logging::DEBUG_LOG>("m_async_io->getSocketHandle() is ACTIVE!", __LINE__, __FILE__);
            m_async_io->getSocketHandle()->setInactive();
            
        }
        catch(std::exception &e)
        {
            // Sometime this doesn't close when it's already existed, just extra checking here.            
            m_log.write<Logging::DEBUG_LOG>("Exception connection shutdown()", e.what(), __LINE__, __FILE__);
        }
    }
    
    session_manager_ptr session_manager = m_session_manager.lock();

    if(session_manager)
    {
        m_is_leaving = true;
        m_log.write<Logging::DEBUG_LOG>("Logoff Session Manager", __LINE__, __FILE__);
                
        session_manager->leave(shared_from_this());            
        session_manager.reset();        
    }
    
}

/**
 * @brief Shutdown Socket Connections by User Rquests / Logoff.
 */
void Session::disconnectUser() 
{
    m_async_io->getSocketHandle()->disconnectUser();
}