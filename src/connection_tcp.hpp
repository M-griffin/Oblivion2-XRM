#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include "connection_base.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include <boost/asio.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
typedef boost::asio::ip::tcp::socket tcp_socket;

/**
 * @class TCP_Connection
 * @author Michael Griffin
 * @date 15/08/2015
 * @file tcp_connection.hpp
 * @brief Handles a shared socket per individual session
 */
class TCP_Connection
    : public ConnectionBase
{
public:

    TCP_Connection(//boost::asio::io_service& io_service,
                   tcp_socket socket)
                   //boost::asio::ssl::context& context)
        : ConnectionBase(std::move(socket), false)
    {
    }

    /*
    boost::asio::ip::tcp::socket& socket()
    {
        return m_normal_socket();
    }*/

    bool is_open()
    {
        return m_normal_socket.is_open();
    }
};

#endif // TCP_CONNECTION_HPP
