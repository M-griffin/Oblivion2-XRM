#ifndef SERVER_HPP
#define SERVER_HPP

#include "model-sys/config.hpp"
#include "session_manager.hpp"
#include "session.hpp"
#include "connection_tcp.hpp"
#include "communicator.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/**
 * @class Server
 * @author Michael Griffin
 * @date 15/08/2015
 * @file chat_server.hpp
 * @brief Handling the Incoming Connections and starts session creation.
 */
class Server
{

public:
    Server(boost::asio::io_service& io_service, int port) //, const tcp::endpoint& endpoint)
        : m_io_service(io_service)
        , m_acceptor_v6(io_service)
        , m_acceptor_v4(io_service)
        , m_room(new SessionManager())
        , m_is_using_ipv6(true)
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
            m_acceptor_v6.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            m_acceptor_v6.set_option(v6_only, ec);
            m_acceptor_v6.get_option(v6_only);

            // Listen to localhost connections only
            //m_acceptor_v6.bind(tcp::endpoint(ip::address::from_string("127.0.0.1"), "5555"));
            m_acceptor_v6.bind(tcp::endpoint(boost::asio::ip::tcp::v6(), port));
            m_acceptor_v6.listen();
        }
        else
        {
            std::cout << "Unable to use IPv6 acceptor" << std::endl;
        }

        // Fallback to ipv4 acceptor.
        if (!m_acceptor_v6.is_open() || v6_only)
        {
            m_is_using_ipv6 = false;
            m_acceptor_v4.open(tcp::v4(), ec);
            if (!ec)
            {
                m_acceptor_v4.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
                // Listen to localhost connections only
                //m_acceptor_v4.bind(tcp::endpoint(ip::address::from_string("127.0.0.1"), "5555"));
                m_acceptor_v4.bind(tcp::endpoint(boost::asio::ip::tcp::v4(), port));
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
            std::cout << "Server Accepts both ipv6 and ipv4 connections." << std::endl;
        }
        else
        {
            std::cout << "Server Accepts only ipv4 connections." << std::endl;
        }

        // Setup the communicator to allow rest of program to talk with
        // And send messages to other nodes.
        TheCommunicator::instance()->setupServer(m_room);

        std::cout << "Telnet Server Ready." << std::endl;
        wait_for_connection();
    }

    ~Server()
    {
        std::cout << "~Server" << std::endl;
    }

    /**
     * @brief Handles incoming connections.
     */
    void wait_for_connection()
    {
        connection_ptr new_connection(new tcp_connection(m_io_service, m_context, false));

        /*  Looks like UDP only!
        // Setup connections to allow Both ipv4 and ipv6!
        boost::asio::ip::v6_only option(false);
        boost::asio::ip::tcp::socket the_socket = new_connection->socket();
        the_socket.set_option(option);*/

        // Accept The Connection
        if (m_is_using_ipv6)
        {
            m_acceptor_v6.async_accept(new_connection->m_normal_socket,
                                       boost::bind(&Server::handle_accept, this,
                                                   new_connection,
                                                   boost::asio::placeholders::error));

        }
        else
        {
            m_acceptor_v4.async_accept(new_connection->m_normal_socket,
                                       boost::bind(&Server::handle_accept, this,
                                                   new_connection,
                                                   boost::asio::placeholders::error));
        }
    }

private:

    /**
     * @brief Callback on connections, creates new sessions
     * @param new_connection
     * @param error
     */
    void handle_accept(connection_ptr new_connection, const boost::system::error_code& error)
    {
        if(!error)
        {
            std::cout << "TCP Connection accepted" << std::endl;
            session_ptr new_session = Session::create(m_io_service, new_connection, m_room);
            m_room->join(new_session);
            wait_for_connection();
        }
        else
        {
            std::cout << "Connection refused: " << error.message() << std::endl;
        }
    }

    boost::asio::io_service&    m_io_service;
    tcp::acceptor               m_acceptor_v6;
    tcp::acceptor               m_acceptor_v4;
    session_manager_ptr         m_room;
    bool m_is_using_ipv6;

    // Place Holder Not used!
    boost::asio::ssl::context   m_context;
};


#endif // CHAT_SERVER_HPP
