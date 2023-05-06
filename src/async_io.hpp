#ifndef ASYNC_CONNECTION_HPP
#define ASYNC_CONNECTION_HPP

#include "async_base.hpp"
#include "io_service.hpp"
#include "socket_handler.hpp"
#include "logging.hpp"

#include <vector>
#include <memory>

/**
 * @class AsyncIO
 * @author Michael Griffin
 * @date 12/09/2017
 * @file async_connection.hpp
 * @brief Handles Shared Sockets Async Request Jobs to IOService.
 */
class AsyncIO
    : public AsyncBase
{
public:

    AsyncIO(IOService& io_service, socket_handler_ptr socket_handler)
        : AsyncBase(io_service, socket_handler)
    {
    }

    ~AsyncIO()
    {
        std::cout << "~AsyncIO()" << std::endl;
    }

    /**
     * @brief Async Read Callback for IOService Work
     * @param BufferSequence - populates will data retrieved
     * @param Callback - returns error code
     */
    template <typename BufferSequence, typename Callback>
    void asyncRead(BufferSequence &buffer, const Callback &callback)
    {
        m_io_service.addAsyncJob(buffer, nullptr, m_socket_handler, callback, SERVICE_TYPE_READ);
    }

    /**
     * @brief Async Write Callback for IOService Work
     * @param StringSequence - Data to Pass to Socket
     * @param Callback - returns error code
     */
    template <typename StringSequence, typename Callback>
    void asyncWrite(StringSequence string_seq, const Callback &callback)
    {
        // Place Holder is used for template parameters, string_seq is used in writes
        // Where the Buffer Place Holder in the above method is used for reads.
        // nullptr can't be passed as reference for vector
        std::vector<unsigned char> place_holder;
        m_io_service.addAsyncJob(place_holder, string_seq, m_socket_handler, callback, SERVICE_TYPE_WRITE);
    }

    /**
     * @brief Async Connection Callback for IOService Work
     * @param StringSequence - Host:Port
     * @param Callback - return error code and handles to new session
     */
    template <typename StringSequence, typename Protocol, typename Callback>
    void asyncConnect(StringSequence string_seq, Protocol protocol, const Callback &callback)
    {
        // Place Holder is used for template parameters, string_seq is used in writes
        // Where the Buffer Place Holder in the above method is used for reads.
        // nullptr can't be passed as reference for vector
        std::vector<unsigned char> place_holder;
        int service_type = SERVICE_TYPE_NONE;

        if(protocol == "TELNET")
        {
            service_type = SERVICE_TYPE_CONNECT_TELNET;
        }
        else if(protocol == "SSH")
        {
            service_type = SERVICE_TYPE_CONNECT_SSH;
        }
        else if(protocol == "IRC")
        {
            service_type = SERVICE_TYPE_CONNECT_IRC;
        }

        m_io_service.addAsyncJob(place_holder, string_seq, m_socket_handler, callback, service_type);
    }

    /**
     * @brief Async Listener Callback for IOService Work
     * @param StringSequence - Host:Port
     * @param Callback - return error code and handles to new session
     */
    template <typename StringSequence, typename Protocol, typename Callback>
    void asyncHandshake(StringSequence string_seq, Protocol protocol, const Callback &callback)
    {
        // Place Holder is used for template parameters, string_seq is used in writes
        // Where the Buffer Place Holder in the above method is used for reads.
        // nullptr can't be passed as reference for vector
        std::vector<unsigned char> place_holder;
        int service_type = SERVICE_TYPE_NONE;

        if(protocol == "TELNET")
        {
            service_type = SERVICE_TYPE_LISTENER_TELNET;
        }
        else if(protocol == "SSH")
        {
            service_type = SERVICE_TYPE_LISTENER_SSH;
        }
        else if(protocol == "IRC")
        {
            service_type = SERVICE_TYPE_LISTENER_IRC;
        }

        m_io_service.addAsyncJob(place_holder, string_seq, m_socket_handler, callback, service_type);
    }

};

typedef std::shared_ptr<AsyncIO> connection_ptr;

#endif // ASYNC_CONNECTION_HPP
