#ifndef ASYNC_BASE_HPP
#define ASYNC_BASE_HPP

#include "io_service.hpp"
#include "socket_handler.hpp"
#include "logging.hpp"

#include <iostream>

/**
 * @class AsyncBase
 * @author Michael Griffin
 * @date 17/02/2018
 * @file async_base.hpp
 * @brief Async Base Class Socket Class for Interfacing with IOService
 */
class AsyncBase
{
public:
    AsyncBase(IOService& io_service, socket_handler_ptr socket_handler)
        : m_log(Logging::getInstance())
        , m_io_service(io_service)
        , m_socket_handler(socket_handler)
    { }
    
    ~AsyncBase()
    { 
        m_log.write<Logging::DEBUG_LOG>("~AsyncBase()");
        m_socket_handler.reset();
    }

    /**
     * @brief Is the Socket Created and Open
     * @return
     */
    bool isActive()
    {
        return (m_socket_handler) ? m_socket_handler->isActive() : false;
    }

    /**
     * @brief Handle to Socket
     * @return
     */
    socket_handler_ptr getSocketHandle()
    {
        return m_socket_handler;
    }

    /**
     * @brief Shutdowns an Open Connection
     */
    void shutdown()
    {
        try
        {
            if(m_socket_handler->isActive())
            {
                m_socket_handler->close();
            }
        }
        catch(std::exception &ex)
        {
            std::cout << "AsyncBase shutdown() - Caught exception=" << ex.what() << std::endl;
        }
    }

    Logging            &m_log;
    IOService          &m_io_service;
    socket_handler_ptr  m_socket_handler;
};

#endif // ASYNC_BASE_HPP
