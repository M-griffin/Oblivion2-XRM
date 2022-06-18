#ifndef SERVER_HPP
#define SERVER_HPP

//#include "model-sys/config.hpp"
#include "session_manager.hpp"
#include "session.hpp"
#include "communicator.hpp"

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
     * @brief Create IO_Service Working thread for socket communications.
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
    Interface(IOService& io_service, std::string protocol, int port)
        : m_io_service(io_service)
        , m_session_manager(new SessionManager())
        , m_socket_acceptor(new SocketHandler())
        , m_async_listener(new AsyncAcceptor(io_service, m_socket_acceptor))
        , m_protocol(protocol)
    {

        Logging *log = Logging::instance();

        // Startup SDL NET. Custom version Tweaked for KEEP Alive's
        if(SDLNet_Init() == -1)
        {
            log->write<Logging::ERROR_LOG>("SDLNet_Init", SDLNet_GetError());
            TheCommunicator::instance()->shutdown();
            return;
        }

        unsigned int num_threads = std::thread::hardware_concurrency();
        log->write<Logging::INFO_LOG>("concurrent threads supported", num_threads);

        // Start up worker thread of ASIO. We want socket communications in a separate thread.
        // We only spawn a single thread for IO_Service on start up
        m_thread = create_thread();

        // Setup Telnet Server Connection Listener.
        if(!m_socket_acceptor->createTelnetAcceptor("127.0.0.1", port))
        {
            log->write<Logging::ERROR_LOG>("Unable to start Telnet Acceptor");
            TheCommunicator::instance()->shutdown();
            return;
        }

        // Setup the communicator to allow rest of program to talk with
        // And send messages to other nodes.
        TheCommunicator::instance()->setupServer(m_session_manager);

        log->write<Logging::CONSOLE_LOG>("Telnet Server Waiting for Connection.");
        waitingForConnection();
    }

    ~Interface()
    {
        m_io_service.stop();
        m_thread.join();
        SDLNet_Quit();
    }

    /**
     * @brief Handles incoming connections.
     */
    void waitingForConnection()
    {
        Logging *log = Logging::instance();
        log->write<Logging::DEBUG_LOG>("Waiting For Connection, Adding Async Job to Listener");
        m_async_listener->asyncAccept(
            m_protocol,
            std::bind(&Interface::handle_accept,
                      this,
                      std::placeholders::_1,
                      std::placeholders::_2));
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
            Logging *log = Logging::instance();
            log->write<Logging::DEBUG_LOG>("TCP Connection accepted");

            connection_ptr async_conn(new AsyncIO(m_io_service, socket_handler));

            // Create DeadlineTimer and attach to new session
            // This timer is specific for waiting .25 seconds on
            // ESC sequences check for single esc, vs key sequences
            deadline_timer_ptr deadline_timer(new DeadlineTimer());

            // Create the new Session
            session_ptr new_session = Session::create(m_io_service,
                                      async_conn,
                                      deadline_timer,
                                      m_session_manager);

            // Attach Session to Session Manager.
            m_session_manager->join(new_session);
        }
        else
        {
            Logging *log = Logging::instance();
            log->write<Logging::ERROR_LOG>("Connection refused", error.message());
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
