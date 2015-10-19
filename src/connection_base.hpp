#ifndef CONNECTION_BASE_HPP
#define CONNECTION_BASE_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <string>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> secure_socket;
typedef boost::asio::ip::tcp::socket normal_socket;

/**
 * @class connection_base
 * @author Michael Griffin
 * @date 10/18/2015
 * @file connection_base.hpp
 * @brief Virtual Class for Socket Sesssions.
 */
class connection_base
    : public boost::enable_shared_from_this<connection_base>
{
public:

    virtual ~connection_base()
    {
        std::cout << "~connection_base." << std::endl;
    }

    virtual bool is_open() = 0;

    connection_base(boost::asio::io_service& io_service,
                   boost::asio::ssl::context& context,
                   bool is_secure)
        : m_normal_socket(io_service)
        , m_secure_socket(io_service, context)
        , m_is_secure(is_secure)
    { }

    normal_socket m_normal_socket;
    secure_socket m_secure_socket;
    bool          m_is_secure;

};

typedef boost::shared_ptr<connection_base> connection_ptr;

#endif // CONNECTION_BASE_HPP
