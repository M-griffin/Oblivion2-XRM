#ifndef SERVER_HPP
#define SERVER_HPP

#include "session_manager.hpp"
#include "session.hpp"
#include "async_io.hpp"

// New Rework for SDL2_net and Asyc io.
#include "io_service.hpp"
#include "socket_handler.hpp"
#include "async_acceptor.hpp"
#include "logging.hpp"

// For Startup.
#include <sdl2_net/SDL_net.hpp>
#include <thread>

class Interface;
typedef std::unique_ptr<Interface> interface_ptr;

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
     * @brief Create IO_Service Worker Thread for AsyncIO (Temp Commented Out!)
     * @return
     */
    std::thread create_thread()
    {
        return std::thread([&] { m_io_service.run(); });
    }

    /**
     * @brief Main interface constructor.
     * @param io_service
     * @param protocol
     * @param port
     * @return
     */
    Interface(IOService &io_service, const std::string &protocol, const int &port)
        : m_log(Logging::getInstance())
        , m_io_service(io_service)
        , m_session_manager(nullptr)
        , m_socket_acceptor(nullptr)
        , m_async_listener(nullptr)
        , m_protocol(protocol)
    {
        // Setup Shared Pointers
        m_session_manager = std::make_shared<SessionManager>();
        m_socket_acceptor = std::make_shared<SocketHandler>();
        m_async_listener  = std::make_shared<AsyncAcceptor>(io_service, m_socket_acceptor);        
        
        // Startup SDL NET. Custom version Tweaked for KEEP Alive's
        if(SDLNet_Init() == -1)
        {
            m_log.write<Logging::ERROR_LOG>("SDLNet_Init", SDLNet_GetError());
            return;
        }

        unsigned int num_threads = std::thread::hardware_concurrency();
        m_log.write<Logging::CONSOLE_LOG>("Concurrent CPU Core(s) Supported", num_threads);

        // Start up worker thread of ASIO. We want socket communications in a separate thread.
        // We only spawn a single thread for IO_Service on start up
 //       std::thread my_thread = create_thread();
//        my_thread.detach();

        // Setup Telnet Server Connection Listener.
        if(!m_socket_acceptor->createTelnetAcceptor("127.0.0.1", port))
        {
            m_log.write<Logging::ERROR_LOG>("Unable to start Telnet Acceptor");
            return;
        }

        m_log.write<Logging::CONSOLE_LOG>("Telnet Server Waiting for Connection.");
        waitingForConnection();
        
        // run the Service Loop.
        m_io_service.run();
    }

    ~Interface()
    {
        m_log.write<Logging::DEBUG_LOG>("~Interface()");
        m_io_service.stop();
        //m_thread.join();  // Not good pratice in destructor.
        
        // Clear Smart Pointers, We Should be good here but cleaner is better.
        m_session_manager.reset();
        m_socket_acceptor.reset();
        m_async_listener.reset();
        
        SDLNet_Quit();
    }

    /**
     * @brief Handles incoming connections.
     */
    void waitingForConnection()
    {        
        m_log.write<Logging::DEBUG_LOG>("Waiting For Connection, Adding Async Job to Listener");
        m_async_listener->asyncAccept(
            m_protocol,
            std::bind(&Interface::handle_accept,
                      this,
                      std::placeholders::_1,
                      std::placeholders::_2));
    }
    
    /**
     * @brief Shutdown Sessions by Session Manager.
     */
    void shutdown() 
    {
        m_session_manager->shutdown();        
    }

private:

    /**
     * @brief Callback on connections, creates new sessions
     * @param new_connection
     * @param error
     */
    void handle_accept(const std::error_code& error, socket_handler_ptr socket_handler)
    {
        if(!error)
        {
            async_io_ptr async_conn = std::make_shared<AsyncIO>(m_io_service, socket_handler);
            m_log.write<Logging::DEBUG_LOG>("Handle-Accept Create New Session");

            // Create the new Session
            session_ptr new_session = Session::create(async_conn, m_session_manager);
            if (new_session) 
            {                
                m_log.write<Logging::DEBUG_LOG>("Handle-Accept Attached Session to Manager");

                // Attach Session to Session Manager.
                m_session_manager->join(new_session);

                m_log.setUserInfo(new_session->m_node_number);
                m_log.write<Logging::INFO_LOG>("Handle-Accept TCP Connection accepted from=", 
                    async_conn->getSocketHandle()->getIpAddress(), "Node=", new_session->m_node_number); 
            }
        }
        else
        {
            m_log.write<Logging::ERROR_LOG>("Handle-Accept Connection refused", error.message());
        }        
    }

    Logging             &m_log;
    IOService           &m_io_service;
    session_manager_ptr  m_session_manager;
    socket_handler_ptr   m_socket_acceptor;
    acceptor_ptr         m_async_listener;
    std::string          m_protocol;
    //std::thread          m_thread;

};


#endif
