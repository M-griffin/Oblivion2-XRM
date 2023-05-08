#ifndef ASYNC_ACCEPTOR_HPP
#define ASYNC_ACCEPTOR_HPP

#include "async_base.hpp"
#include "io_service.hpp"
#include "socket_handler.hpp"

#include <iostream>
#include <memory>

/**
 * @class AsyncAcceptor
 * @author Michael Griffin
 * @date 18/02/2018
 * @file async_acceptor.hpp
 * @brief Handles Async Socket Listener Jobs to IOService.
 */
class AsyncAcceptor
    : public AsyncBase
{
public:

    AsyncAcceptor(IOService& io_service, const socket_handler_ptr &socket_handler)
        : AsyncBase(io_service, socket_handler)
    {
    }

    ~AsyncAcceptor()
    {
        std::cout << "~AsyncAcceptor()" << std::endl;
        //m_socket_handler.reset();
    }


    /**
     * @brief Async Listener Callback for IOService Work
     * @param StringSequence - Host:Port
     * @param Callback - return error code and handles to new session
     */
    template <typename Protocol, typename Callback>
    void asyncAccept(const Protocol &protocol, const Callback &callback)
    {
        // Place Holder is used for template parameters, string_seq is used in writes
        // Where the Buffer Place Holder in the above method is used for reads.
        // nullptr can't be passed as reference for vector
        std::vector<unsigned char> place_holder;
        std::string string_place_holder;

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

        m_io_service.addAsyncJob(place_holder, string_place_holder, m_socket_handler, callback, service_type);
    }

};

typedef std::shared_ptr<AsyncAcceptor> acceptor_ptr;


#endif // ASYNC_ACCEPTOR_HPP
