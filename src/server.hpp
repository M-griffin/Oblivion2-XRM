
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

#include "model-sys/config.hpp"
#include "session_manager.hpp"
#include "session.hpp"
#include "connection_base.hpp"
#include "connection_tcp.hpp"

using boost::asio::ip::tcp;

/**
 * @class Server Template (Default Telnet)
 * @author Michael Griffin
 * @date 15/08/2015
 * @file chat_server.hpp
 * @brief Handling the Incoming Connections and starts session creation.
 */
class Server
{

public:
    Server(boost::asio::io_service& io_service, int port) //, const tcp::endpoint& endpoint)
        : m_io_service(io_service)   // Pass to Session for Deadline Timers
        , m_acceptor_v6(io_service)
        , m_acceptor_v4(io_service)
        , m_session_manager(std::make_shared<SessionManager>())
        , m_is_using_ipv6(false)
        , m_context(boost::asio::ssl::context::sslv23)
    {
        std::cout << "Starting Telnet Server" << std::endl;

        // Defaults v6_Only to false to accept both v4 and v6 connections.
        boost::asio::ip::v6_only v6_only(false);
        boost::system::error_code ec;


        // Try to Setup Listen Socket with IPv6 + IPv4 Support.
        m_acceptor_v6.open(boost::asio::ip::tcp::v6(), ec);
        if (!ec)
        {
            std::cout << "Attempting IPv6 acceptor" << std::endl;            
            m_acceptor_v6.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            m_acceptor_v6.set_option(v6_only, ec);
            m_acceptor_v6.get_option(v6_only);

            // Listen to localhost connections only
            //m_acceptor_v6.bind(tcp::endpoint(ip::address::from_string("127.0.0.1"), "5555"));
            m_acceptor_v6.bind(tcp::endpoint(tcp::v6(), port));            
            std::cout << "Listening on IPv6 acceptor" << std::endl;
            m_acceptor_v6.listen();
            
            m_is_using_ipv6 = true;
        }
        else
        {
            std::cout << "Unable to use IPv6 acceptor::" << std::endl;
        }

        // Fallback to ipv4 acceptor.
        if (!m_is_using_ipv6)
        {
            std::cout << "Attempting IPv4 acceptor" << std::endl;            
            m_is_using_ipv6 = false;
            m_acceptor_v4.open(tcp::v4(), ec);
            if (!ec)
            {
                m_acceptor_v4.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
                // Listen to localhost connections only
                //m_acceptor_v4.bind(tcp::endpoint(ip::address::from_string("127.0.0.1"), "5555"));
                m_acceptor_v4.bind(tcp::endpoint(tcp::v4(), port));                
                std::cout << "Listening on IPv4 acceptor" << std::endl;
                m_acceptor_v4.listen();
            }
            else
            {
                // This is the fallback, if it deoesn't work, then good luck!
                std::cout << "Error: Unable to use IPv4 acceptor, No connections will be accepted." << std::endl;
                exit(2);
            }
        }

        // Give a notice on connection protocols.
        if (m_is_using_ipv6)
        {
            std::cout << "Server Accepts both IPv6 and IPv4 connections." << std::endl;
        }
        else
        {
            std::cout << "Server Accepts only IPv4 connections." << std::endl;
        }

        // Bind session manager to Communicator
        // Setup the communicator to allow rest of program to talk with with nodes, and keeps sessions alive.
        Communicator::getInstance().setupServer(m_session_manager);

        std::cout << "Telnet Server Ready." << std::endl;
        waitingForConnection();
    }

    ~Server()
    {
        std::cout << "~Server" << std::endl;
    }

    /**
     * @brief Managers Incoming Connections on ASIO Acceptors / Listeners
     * @param acceptor
     */
    void manageAsyncAcceptors(tcp::acceptor& acceptor)
    {
        acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (ec)
            {
              std::cout << "Connection refused: " << ec.message() << std::endl;
            }
            else
            {
                // Setup Keep Alives for Incoming Client Sockets.
                //boost::asio::socket_base::keep_alive option(true);
                //socket.set_option(option);

                std::cout << "TCP Connection Session Joined "
                          << " Client IP: "
                          << socket.remote_endpoint().address().to_string()
                          << std::endl;

                // Setup a Connection Handle, and Attach to the Session
                connection_ptr connection = std::make_shared<TCP_Connection>(std::move(socket));                                  
                session_ptr session = createSession(m_io_service, connection, m_session_manager);
                m_session_manager->join(session);
            }            
            
            waitingForConnection();
        });
    }


    /**
     * @brief Handles incoming connections.
     */
    void waitingForConnection()
    {
        std::cout << "wait_for_connection" << std::endl;

        // Accept The Connection, v6 Handles Both V4 and V6, Default to V4 if V6 Fails.
        if (m_is_using_ipv6)
        {      
            manageAsyncAcceptors(m_acceptor_v6);
        }
        else
        {
            manageAsyncAcceptors(m_acceptor_v4);
        }
    }

    /**
     * @brief Startup Session (Connection Listener)
     *        Handle the initial Session Creation, Also start the
     *        Telnet Option Negotiation with the client.
     * @param io_service
     * @param connection
     * @param my_session_manager
     * @return
     */
    session_ptr createSession(boost::asio::io_service& io_service, connection_ptr connection, session_manager_ptr my_session_manager)
    {
        session_ptr new_session = std::make_shared<Session>(io_service, connection, my_session_manager);
        
        // Setup Unique Session Id's.    
        Uuid uuid;
        new_session->m_session_id = uuid.createUuidString();
        
        try        
        {
            // Setup Async Input on Session for User Input
            std::cout << "SETUP waitingForData()" << std::endl;
            new_session->waitingForData();      
            
            // On initial Session Connection,  setup and send TELNET Options to
            // start the negotiation of client features.
            // On initial connection, clear and home cursor
            std::string clear_screen = "\x1b[1;1H\x1b[2J\x1b[0m - \x1b[0;34mTesting - Will Disconnect on Matrix Menu!\x1b[0m";
            
            std::cout << "New Session clear screen" << std::endl;
            new_session->deliver(clear_screen);
                                    
            std::cout << "deliver Telnet IAC Sequences" << std::endl;
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
            std::cout << "deliver startTelnetOptionNegoiation" << std::endl;
            new_session->startTelnetOptionNegoiation();
            
        }
        catch(std::exception &ex)
        {
            Logging &log = Logging::getInstance();
            log.write<Logging::ERROR_LOG>("TelnetOptionNegoiation Exception=", ex.what(), __LINE__, __FILE__);            
        }
        
        return new_session;
    }

private:
    boost::asio::io_service&    m_io_service;
    tcp::acceptor               m_acceptor_v6;
    tcp::acceptor               m_acceptor_v4;
    session_manager_ptr         m_session_manager;
    bool m_is_using_ipv6;

    // Place Holder Not used!
    boost::asio::ssl::context   m_context;
};


#endif // CHAT_SERVER_HPP
