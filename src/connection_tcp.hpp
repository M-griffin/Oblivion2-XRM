#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include "connection_base.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <string>
#include <vector>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
typedef boost::asio::ip::tcp::socket tcp_socket;

/**
 * @class tcp_connection
 * @author Michael Griffin
 * @date 15/08/2015
 * @file tcp_connection.hpp
 * @brief Handles a shared socket per individual session
 */
class tcp_connection
    : public connection_base
{
public:

    tcp_connection(boost::asio::io_service& io_service,
                   boost::asio::ssl::context& context)
        : connection_base(io_service, context, false)
    {
    }

/*
    boost::asio::ip::tcp::socket& socket()
    {
        return m_normal_socket;
    }*/

    bool is_open()
    {
        return m_normal_socket.is_open();
    }
};

//typedef boost::shared_ptr<tcp_connection> connection_ptr;

#endif // TCP_CONNECTION_HPP
