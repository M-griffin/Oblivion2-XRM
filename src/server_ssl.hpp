
#include "model/config.hpp"
#include "session_manager.hpp"
#include "communicator.hpp"
#include "connection_base.hpp"
#include "connection_ssl.hpp"
#include "session.hpp"

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

/**
 * @class ServerSSL
 * @author Michael Griffin
 * @date 10/17/2015
 * @file server_ssl.hpp
 * @brief Handles SSL negoiations and spawns sessions.
 */
class ServerSSL
{
public:
    ServerSSL(boost::asio::io_service& io_service, int port)
        : m_io_service(io_service)
        , m_acceptor(io_service
                     , boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4()
                     , port))
        , m_context(boost::asio::ssl::context::sslv23)
        , m_room(new SessionManager())
    {
        std::cout << "Starting SSL Server" << std::endl;
        m_context.set_options(
            boost::asio::ssl::context::default_workarounds
            | boost::asio::ssl::context::no_sslv2
            | boost::asio::ssl::context::single_dh_use);
        m_context.set_password_callback(boost::bind(&ServerSSL::get_password, this));
        m_context.use_certificate_chain_file("KEYS/ca.pem");
        //m_context.use_certificate_chain_file_path ...
        m_context.use_private_key_file("KEYS/server.key", boost::asio::ssl::context::pem);
        m_context.use_tmp_dh_file("KEYS/dh2048.pem");


        // Setup the communicator to allow rest of program to talk with
        // And send messages to other nodes.
        TheCommunicator::instance()->setupServer(m_room);

        std::cout << "SSL Server Ready." << std::endl;
        wait_for_connection();
    }

    ~ServerSSL()
    {
        std::cout << "~ServerSSL" << std::endl;
    }

    /**
     * @brief Handles incoming connections.
     */
    void wait_for_connection()
    {
        connection_ptr new_connection(new ssl_connection(m_io_service, m_context, true));
        m_acceptor.async_accept(new_connection->m_secure_socket.lowest_layer(),
                                boost::bind(&ServerSSL::handle_accept, this,
                                            new_connection,
                                            boost::asio::placeholders::error));
    }

    /**
     * @brief Gets Passed if cert verify is turn off.
     * @return
     */
    std::string get_password() const
    {
        std::cout << "get_password()" << std::endl;
        return "private";
    }

    /**
     * @brief On Connections, creates and spawns the SSL session.
     * @param new_connection
     * @param error
     */
    void handle_accept(connection_ptr new_connection,
                       const boost::system::error_code& error)
    {
        if(!error)
        {
            std::cout << "SSL Connection Accepted" << std::endl;
            session_ptr new_session = Session::create(m_io_service, new_connection, m_room);
            m_room->join(new_session); // keep it alive!

            // Loop back and handle next connection
            wait_for_connection();
        }
        else
        {
            std::cout << "Error SSL Connection." << std::endl;
        }
    }

private:
    boost::asio::io_service&        m_io_service;
    boost::asio::ip::tcp::acceptor  m_acceptor;
    boost::asio::ssl::context       m_context;
    session_manager_ptr                m_room;
};
