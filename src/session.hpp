#ifndef SESSION_HPP
#define SESSION_HPP

#include "state_manager.hpp"
#include "async_io.hpp"
#include "deadline_timer.hpp"
#include "socket_handler.hpp"
#include "session_manager.hpp"
#include "telnet_decoder.hpp"
#include "logging.hpp"

#include <memory>
#include <list>
#include <string>
#include <cassert>

/*
 * NOTES TODO: Sessions should also have incoming notification buffer
 * or OLM Buffer for message from other nodes and sysops
 * when not in chat.
 */
class Session;
typedef std::shared_ptr<Session> session_ptr;

/**
 * @class Session
 * @author Michael Griffin
 * @date 15/08/2015
 * @file chat_session.hpp
 * @brief handles TCP and SSL individual connection Sessions.
 */
class Session
    : public std::enable_shared_from_this<Session>
{
public:

    /**
     * @brief Session Constructor
     * @param tcp_connection
     * @param session_manager
     * @return
     */
    Session(async_io_ptr const &my_async_io, session_manager_ptr const &my_session_manager)
        : m_async_io(my_async_io)        
        , m_session_manager(my_session_manager)
        , m_state_manager(new StateManager())
        , m_deadline_timer(new DeadlineTimer())
        , m_telnet_decoder(new TelnetDecoder())
        , m_node_number(0)
        , m_is_leaving(false)
    {
        Logging *log = Logging::instance();
       
        if(m_async_io->isActive())
        {
            try
            {
                log->write<Logging::DEBUG_LOG>("New Session Accepted", __LINE__, __FILE__);
            }
            catch(std::exception &ex)
            {
                log->write<Logging::ERROR_LOG>("Exception remote_endpoint()=", ex.what(), __LINE__, __FILE__);
            }
        }

        // Get The First available node number. Needs Rework from session manager!
        m_node_number = 0;
        
        log->write<Logging::CONSOLE_LOG>("New Session ConnectionNode Number=", m_node_number);
    }
    
    ~Session()
    {
        std::cout << "~Session()" << std::endl;
        // Free the Node Number from this session.
        //TheCommunicator::instance()->freeNodeNumber(m_session_data->m_node_number);

        // Free the menu system state and modules when session closes.
        m_state_manager->clean();
        m_async_io.reset();
        m_session_manager.reset();
        m_deadline_timer.reset();
        m_telnet_decoder.reset();    
    }

    /**
     * @brief Startup Session (Connection Listener)
     *        Handle the initial Session Creation, Also start the
     *        Telnet Option Negotiation with the client.
     * @param tcp_connection
     * @param session_manager
     * @return
     */
    static session_ptr create(async_io_ptr my_async_io, session_manager_ptr my_session_manager)
    {
        session_ptr new_session(new Session(my_async_io, my_session_manager));
       
        if(my_async_io->isActive())
        {
            /* new_session->m_session_data->startUpSessionStats("Telnet"); - Disable Database for now! */
            new_session->waitingForData();
        }

        // Send out Telnet Negotiation Options
        if(my_async_io->isActive())
        {
            // On initial Session Connection,  setup and send TELNET Options to
            // start the negotiation of client features.

            // On initial connection, clear and home cursor
            std::string clear_screen = "\x1b[1;1H\x1b[2J";
            new_session->deliver(clear_screen);

            /*
            new_session->m_telnet_decoder->sendIACSequences(DONT, TELOPT_OLD_ENVIRON);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_SGA);
            new_session->m_telnet_decoder->addReply(TELOPT_SGA);

            new_session->m_telnet_decoder->sendIACSequences(WILL, TELOPT_ECHO);
            new_session->m_telnet_decoder->addReply(TELOPT_ECHO);

            new_session->m_telnet_decoder->sendIACSequences(WILL, TELOPT_SGA);
            new_session->m_telnet_decoder->addReply(TELOPT_SGA);

            new_session->m_telnet_decoder->sendIACSequences(WILL, TELOPT_BINARY);
            new_session->m_telnet_decoder->addReply(TELOPT_BINARY);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_BINARY);
            new_session->m_telnet_decoder->addReply(TELOPT_BINARY);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_TTYPE);
            new_session->m_telnet_decoder->addReply(TELOPT_TTYPE);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_NAWS);
            new_session->m_telnet_decoder->addReply(TELOPT_NAWS);

            // Wait 1.5 Seconds for respones.
            new_session->startDetectionTimer();
            */
        }

        return new_session;
    }

    /**
     * @brief Telnet option Sequence Timer
     */
    void startDetectionTimer()
    {
        // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences responses
        m_deadline_timer->setWaitInMilliseconds(1500);
        m_deadline_timer->asyncWait(
            std::bind(&Session::handleDetectionTimer, shared_from_this())
        );
    }

    /**
     * @brief Detection Timer Handler, once Completed Starts up the Menu System
     *        
     * @param timer
     */
    void handleDetectionTimer()
    {
        // Detection Completed, start ip the Pre-Logon Sequence State.
        // Comment out for Now.
        
        /*
        state_ptr new_state(new MenuSystem(m_session_data));
        m_state_manager->changeState(new_state);
        */
    }
    
    /**
     * @brief Deadline Detection Timer for Negotiation
     * @param timer
     */
    void handlePyBind11State()
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
    void deliver(const std::string &msg)
    {
        if(msg.size() == 0 || msg[0] == '\0')
        {
            return;
        }

        // handle output encoding, if utf-8 translate data accordingly.
        // Temp Disabled, in general any done is translated prior to this
        // But lets double check this!
        std::string outputBuffer = std::string(msg);

        /*
        // On Output, We have internal UTF8 now, translate to CP437
        if(m_encoding == Encoding::ENCODE_CP437)
        {
            outputBuffer = msg; //Encoding::instance()->utf8Decode(msg);
        }
        else
        {
            outputBuffer = msg;
        }*/

        

        if(m_async_io->isActive()) // && TheCommunicator::instance()->isActive())
        {
            m_async_io->asyncWrite(outputBuffer,
                                     std::bind(
                                         &Session::handleWrite,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         std::placeholders::_2));
        }
    }

    /**
     * @brief Callback after Writing Data, If error/hangup notifies
     *        Everyone this person has left.
     * @param error
     */
    void handleWrite(const std::error_code& error, socket_handler_ptr)
    {
        Logging *log = Logging::instance();

        if(error)
        {
            log->write<Logging::ERROR_LOG>("Async_HandleWrite Session Closed() error=", error.message(), __LINE__, __FILE__);
        }

        session_manager_ptr session_mgr = m_session_manager.lock();

        if(session_mgr && error) //&& (!m_session_data->m_is_leaving))
        {
            //m_is_leaving = true;

            log->write<Logging::DEBUG_LOG>("Async_HandleWrite Removing Sesison from Manager", __LINE__, __FILE__);
            
            if(m_async_io->isActive())
            {
                try
                {
                    log->write<Logging::DEBUG_LOG>("Async_HandleWrite Leaving (NORMAL SESSION)", __LINE__, __FILE__);
                    m_async_io->shutdown();
                }
                catch(std::exception &ex)
                {
                    log->write<Logging::ERROR_LOG>("Async_HandleWrite Exception closing socket()", ex.what(), __LINE__, __FILE__);
                }
            }
            
            // Disconnect the session.
            session_mgr->leave(shared_from_this());
        }
    }
    
    // Previous SessionData Methods
    
    /**
     * @brief Data Handler for incoming Data (From Client)
     */
    void waitingForData()
    {
        // Important, clear out buffer before each read.
        std::vector<unsigned char>().swap(m_in_data_vector);
        
        if(m_async_io->isActive()) // && TheCommunicator::instance()->isActive())
        {
            m_async_io->asyncRead(m_in_data_vector,
                                    std::bind(
                                        &Session::handleRead,
                                        shared_from_this(),
                                        std::placeholders::_1,
                                        std::placeholders::_2));
        }
    }
    
    /**
     * @brief Callback after data received. handles telnet options
     * Then parses out normal text data from client to server.
     * @param error
     * @param bytes_transferred
     */
    void handleRead(const std::error_code& error, socket_handler_ptr)
    {
        
        Logging *log = Logging::instance();
        log->write<Logging::DEBUG_LOG>("handleRead - After Incoming Data.", __FILE__, __LINE__);

        if (error) 
        {
            session_manager_ptr session_manager = m_session_manager.lock();

            if(session_manager && error)
            {
                m_is_leaving = true;

                // Disconnect the session.
                std::cout << "handleRead - Leaving Session on Error" << std::endl;
                session_manager->leave(shared_from_this());
            }
            
            return;
        }
        

        std::cout << "handleRead - waitingForData" << std::endl;

        // Testing ASIO Call Back after reading data. 
        //async_io_ptr async_io = m_async_io.lock();
        //if(async_io->isActive())
        {
            waitingForData();
        }
            
        /*
        if(!error)
        {
            // Part I: Parse Out Telnet Options and handle responses back to client.
            handleTeloptCodes();
        }

        session_manager_ptr session_manager = m_session_manager.lock();

        if(!session_manager)
        {
            Logging *log = Logging::instance();
            log->write<Logging::ERROR_LOG>("Unable to load session_manager", __FILE__, __LINE__);
            return;
        }

        if(!error)
        {
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
                }           
            }

            // Restart Callback to wait for more data.
            // If this step is skipped, then the node will exit
            // since io_service will have no more work!
            if(m_async_io->isActive())
            {
                waitingForData();
            }
        }
        else
        {
            // Socket Read Error we mark leaving for the session.
            Logging *log = Logging::instance();
            log->write<Logging::INFO_LOG>("Clearing SessionData on Lost Connection Msg=", error.message(), __FILE__, __LINE__);

            session_manager_ptr session_manager = m_session_manager.lock();

            if(session_manager && error)
            {
                m_is_leaving = true;

                // Disconnect the session.
                session_manager->leave(m_session);
            }        
        }
        */
    }
    
    // Local Member Definitions Weak Pointers
    async_io_ptr               m_async_io;
    session_manager_wptr       m_session_manager;
    
    // Local Member Definitions Unique Pointers
    state_manager_uptr         m_state_manager;
    deadline_timer_uptr        m_deadline_timer;    
    telnet_decoder_uptr        m_telnet_decoder;
    
    // Local Member Variables
    int                        m_node_number;
    bool                       m_is_leaving;
    std::vector<unsigned char> m_in_data_vector;


};


#endif // SESSION_HPP
