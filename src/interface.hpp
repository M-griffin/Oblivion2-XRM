#ifndef SERVER_HPP
#define SERVER_HPP

#include "model-sys/config.hpp"
#include "session_manager.hpp"
#include "session.hpp"
#include "communicator.hpp"

// For Startup.
#include <sdl2_net/SDL_net.hpp>

// New Rework for SDL2_net and Asyc io.
#include "io_service.hpp"
#include "socket_handler.hpp"
#include "async_acceptor.hpp"


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
     * @brief Create IO_Service Working thread for socket communications.
     * @return
     */
    std::thread create_thread()
    {
        return std::thread([&] { m_io_service.run(); });
    }


    Interface(IOService& io_service, std::string protocol, int port) //, const tcp::endpoint& endpoint)
        : m_io_service(io_service)
        , m_session_manager(new SessionManager())
        , m_socket_acceptor(new SocketHandler())
        , m_async_listener(new AsyncAcceptor(io_service, m_socket_acceptor))
        , m_protocol(protocol)
    {

        // Startup SDL NET.
        if(SDLNet_Init() == -1) 
        {
            fprintf(stderr, "ER: SDLNet_Init: %s\n", SDLNet_GetError());
            exit(-1);
        }
        
        std::cout << "Interface Created" << std::endl;
        // Start up worker thread of ASIO. We want socket communications in a separate thread.
        // We only spawn a single thread for IO_Service on start up
        m_thread = create_thread();

        std::cout << "Starting Telnet Listener on port: " << port << std::endl;

        // Setup Telnet Server Connection Listener.
        if (!m_socket_acceptor->createTelnetAcceptor("127.0.0.1", port))
        {
             std::cout << "Unable to start Telnet Acceptor" << std::endl;
             TheCommunicator::instance()->shutdown();
             return;
        }
                
        // Setup the communicator to allow rest of program to talk with
        // And send messages to other nodes.
        TheCommunicator::instance()->setupServer(m_session_manager);

        std::cout << "Telnet Server Ready." << std::endl;
        wait_for_connection();
    }

    ~Interface()
    {
        std::cout << "~Interface" << std::endl;
        m_io_service.stop();
        m_thread.join();
        SDLNet_Quit();
    }

    /**
     * @brief Handles incoming connections.
     */
    void wait_for_connection()
    {
        
        // Setup call back jobs for new connections.   
        m_async_listener->asyncListen(
            m_protocol, 
            std::bind(&Interface::handle_accept, 
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2));

        // Setup a seperate Socket Handler specific for connections.                

        // Check for incomming connations, if so, spawn new socket connection.
  //      connection_listener->asyncHandshake()

        /*
        // Accept The Connection
        if (m_is_using_ipv6)
        {
            m_acceptor_v6.async_accept(new_connection->m_normal_socket,
                                       boost::bind(&Server::handle_accept, this,
                                                   new_connection,
                                                   boost::asio::placeholders::error));
        */
    }

private:

    /**
     * @brief Callback on connections, creates new sessions
     * @param new_connection
     * @param error
     */
    void handle_accept(const std::error_code& error, connection_ptr new_connection)
    {
        if(!error)
        {
            std::cout << "TCP Connection accepted" << std::endl;
                        
            // Create DeadlineTimer and attach to new session
            deadline_timer_ptr deadline_timer(new DeadlineTimer(
                                          m_io_service,
                                          new_connection->m_socket_handler
                                      ));
                                      
            // Create the new Session
            session_ptr new_session = Session::create(m_io_service, 
                                      new_connection,
                                      deadline_timer,
                                      m_session_manager);

            // Attach Session to Session Manager.
            m_session_manager->join(new_session);
            wait_for_connection();
        }
        else
        {
            std::cout << "Connection refused: " << error.message() << std::endl;
        }
    }

    IOService&          m_io_service;
    session_manager_ptr m_session_manager;
    socket_handler_ptr  m_socket_acceptor;
    acceptor_ptr        m_async_listener;
    std::string         m_protocol;
    std::thread         m_thread;

};


#endif // CHAT_SERVER_HPP
