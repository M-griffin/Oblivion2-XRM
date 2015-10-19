#ifndef SSL_CONNECTION_HPP
#define SSL_CONNECTION_HPP

#include "connection_base.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/tuple/tuple.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <string>
#include <vector>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
typedef boost::asio::ip::tcp::socket tcp_socket;

/**
 * @class ssl_connection
 * @author Michael Griffin
 * @date 15/10/2015
 * @file ssl_connection.hpp
 * @brief Handles a shared socket per individual session
 */
class ssl_connection
    : public connection_base
{
public:

    ssl_connection(boost::asio::io_service& io_service,
                   boost::asio::ssl::context& context,
                   bool is_ssecure)
        : connection_base(io_service, context, is_ssecure)
    {
    }

/*
    //ssl_socket::lowest_layer_type& socket()
    boost::asio::ip::tcp::socket& socket()
    {
        std::cout << "ssl socket get" << std::endl;
        return m_secure_socket.next_layer();
    }*/

    bool is_open()
    {
        return m_secure_socket.lowest_layer().is_open();
    }

};

//typedef boost::shared_ptr<ssl_connection> ssl_connection_ptr;

#endif // TCP_CONNECTION_HPP
