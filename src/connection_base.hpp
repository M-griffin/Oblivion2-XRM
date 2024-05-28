#ifndef CONNECTION_BASE_HPP
#define CONNECTION_BASE_HPP

#include <iostream>
#include <string>
#include <memory>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> secure_socket;
typedef boost::asio::ip::tcp::socket normal_socket;

/**
 * @class ConnectionBase
 * @author Michael Griffin
 * @date 10/18/2015
 * @file connection_base.hpp
 * @brief Virtual Class for Socket Sesssions.
 */
class ConnectionBase
{
public:

    virtual ~ConnectionBase()
    {
        std::cout << "~ConnectionBase." << std::endl;
    }

    virtual bool is_open() = 0;

    ConnectionBase(//boost::asio::io_service& io_service,
                   normal_socket socket,
                   //boost::asio::ssl::context& context,
                   bool is_secure)
        //: m_normal_socket(io_service)
        //, m_secure_socket(io_service, context)
        : m_normal_socket(std::move(socket))
        , m_is_secure(is_secure)
    { }

    normal_socket m_normal_socket;
    //secure_socket m_secure_socket;
    bool          m_is_secure;

};

typedef std::shared_ptr<ConnectionBase> connection_ptr;

#endif // CONNECTION_BASE_HPP
