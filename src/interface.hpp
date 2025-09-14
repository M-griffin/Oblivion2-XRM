#ifndef SERVER_HPP
#define SERVER_HPP

// For Startup.
#include <chrono>

#include "sdl2_net/SDL_net.hpp"
#include "logging.hpp"
#include "libSqliteWrapped.h"


/**
 * @class Interface
 * @author Michael Griffin
 * @date 13/03/2018
 * @file interface.hpp
 * @brief Handling Async IO Startup and connections
 */
class Interface
{

public:

    /**
     * @brief Main interface constructor.
     * @param io_service
     * @param protocol
     * @param port
     * @return
     */
    Interface(IOService &io_service, const std::string &protocol, const int &port)
        : m_log(Logging::getInstance())
        , m_protocol(protocol)
        , m_user_database(USERS_DATABASE, &m_database_log)
    {        
        // Startup SDL NET. Custom version Tweaked for KEEP Alive's
        if(SDLNet_Init() == -1)
        {
            m_log.write<Logging::ERROR_LOG>("SDLNet_Init", SDLNet_GetError());
            return;
        }

        unsigned int num_threads = std::thread::hardware_concurrency();
        m_log.write<Logging::CONSOLE_LOG>("Concurrent CPU Core(s) Supported", num_threads);


        // Setup Telnet Server Connection Listener.
        if(!m_socket_acceptor->createTelnetAcceptor("127.0.0.1", port))
        {
            m_log.write<Logging::ERROR_LOG>("Unable to start Telnet Acceptor");
            return;
        }

        m_log.write<Logging::CONSOLE_LOG>("Telnet Server Waiting for Connection.");
        waitingForConnection();
        
    }

    ~Interface()
    {
        m_log.write<Logging::DEBUG_LOG>("~Interface()");        
        SDLNet_Quit();
    }

private:

    /**
     * @brief Callback on connections, creates new sessions
     * @param new_connection
     * @param error
     */
    void handle_accept(const std::error_code& error, socket_handler_ptr socket_handler)
    {
        
        // Avoid Incoming Hamming, or space it out a but, give 2 seconds intervals
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        
        if(!error)
        {
            async_io_ptr async_conn = std::make_shared<AsyncIO>(m_io_service, socket_handler);
            m_log.write<Logging::CONSOLE_LOG>("Handle-Accept NEw Incoming TCP Connection");
            
            int node_number = m_session_manager->getNodeNumber();
            m_log.setUserInfo(node_number);
            
            // Ignore Creating a session and close connection
            if (node_number > 10)
            {
                m_log.write<Logging::INFO_LOG>("Handle-Accept TCP Connection OVER MAX 10 from=",                 
                    async_conn->getSocketHandle()->getIpAddress(), "Node=", node_number); 
                    socket_handler->close();
                    return;
            }

            // Create the new Session
            session_ptr new_session = Session::create(async_conn, m_session_manager, m_user_database);
            if (new_session) 
            {                
                m_log.write<Logging::DEBUG_LOG>("Handle-Accept Attached Session to Manager");

                // Attach Session to Session Manager.
                new_session->m_node_number = node_number;
                m_session_manager->join(new_session);
                                
                m_log.setUserInfo(node_number);
                m_log.write<Logging::INFO_LOG>("Handle-Accept TCP Connection accepted from=", 
                    async_conn->getSocketHandle()->getIpAddress(), "Node=", node_number); 
            }
        }
        else
        {
            m_log.write<Logging::ERROR_LOG>("Handle-Accept Connection refused", error.message());
        }        
    }

    Logging             &m_log;
    IOService           &m_io_service;
        
    SQLW::Database       m_user_database;
    SQLW::StderrLog      m_database_log;  

};


#endif
