#ifndef ASYNC_CONNECTION_HPP
#define ASYNC_CONNECTION_HPP

#include "async_base.hpp"
#include "io_service.hpp"
#include "socket_handler.hpp"
#include "logging.hpp"

#include <vector>
#include <memory>
#include <chrono>

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

    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point<clock, std::chrono::milliseconds>;
        

    AsyncIO(IOService& io_service, socket_handler_ptr socket_handler)
        : AsyncBase(io_service, socket_handler)
    {
    }

    ~AsyncIO()
    {
        m_log.write<Logging::DEBUG_LOG>("~AsyncIO()");
    }

    /**
     * @brief Async Read Callback for IOService Work
     * @param BufferSequence - populates will data retrieved
     * @param Callback - returns error code
     */
    template <typename BufferSequence, typename Callback>
    void asyncRead(BufferSequence &buffer, const Callback &callback)
    {
        m_log.write<Logging::DEBUG_LOG>("Async Read Job Added To Queue");
        m_io_service.addAsyncJob(buffer, nullptr, m_socket_handler, callback, SERVICE_TYPE_READ);
    }

    /**
     * @brief Async Write Callback for IOService Work
     * @param StringSequence - Data to Pass to Socket
     * @param Callback - returns error code
     */
    template <typename StringSequence, typename Callback>
    void asyncWrite(StringSequence &string_seq, const Callback &callback)
    {
        m_log.write<Logging::DEBUG_LOG>("Async Write Job Added To Queue");
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

        m_log.write<Logging::DEBUG_LOG>("Async Connect Job Added To Queue");
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

        m_log.write<Logging::DEBUG_LOG>("Async Listener Job Added To Queue");
        m_io_service.addAsyncJob(place_holder, string_seq, m_socket_handler, callback, service_type);
    }
    
    /**
     * @brief Async Wait Callback for IOService Work
     * @param BufferSequence - populates will data retrieved
     * @param Callback - returns error code
     */
    template <typename Callback>
    void asyncWait(int expires_on, const Callback &callback)
    {        
        
    
        m_log.write<Logging::DEBUG_LOG>("Async Read Job Added To Queue");
        //std::time_t start_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        
        time_point_type start = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
        
        m_io_service.addTimerJob(start, expires_on, callback);
    }

};

typedef std::shared_ptr<AsyncIO> async_io_ptr;
typedef std::weak_ptr<AsyncIO> async_io_wptr;

#endif // ASYNC_CONNECTION_HPP
