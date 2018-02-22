#ifndef DEADLINE_TIMER_HPP
#define DEADLINE_TIMER_HPP

#include "async_base.hpp"
#include "io_service.hpp"
#include "socket_handler.hpp"

#include <chrono>
#include <thread>
#include <memory>
#include <iostream>

/**
 * @class DeadlineTimer
 * @author Michael Griffin
 * @date 17/02/2018
 * @file deadline_timer.hpp
 * @brief Timer class for Wait and AsyncWait.
 */
class DeadlineTimer
    : AsyncBase
{
public:
    DeadlineTimer(IOService &io_service, socket_handler_ptr socket_handler) 
        : AsyncBase(io_service, socket_handler)
    { 
        std::cout << "DeadlineTimer Created" << std::endl;
    }
    
    ~DeadlineTimer()
    { 
        std::cout << "~DeadlineTimer" << std::endl;
    }

    // Non-Blocking Will push job to io_service which will check if expires time has passed.
    // Then run the handler, or ignore and exit if asyncCancel
    template <typename StringSequence, typename Callback>
    void asyncWait(StringSequence string_seq, const Callback &callback)
    {
       std::vector<unsigned char> place_holder;
       m_io_service.addAsyncJob(place_holder, string_seq, m_socket_handler, callback, SERVICE_TYPE_ASYNC_TIMER);
    }
       
    // Blocking Will push job to io_service which will set a flag to ignore poll.
    template <typename StringSequence, typename Callback>
    void wait(StringSequence string_seq, const Callback &callback)
    {
       std::vector<unsigned char> place_holder;
       m_io_service.addAsyncJob(place_holder, string_seq, m_socket_handler, callback, SERVICE_TYPE_ASYNC_TIMER);
    }
    
    // Will mark the existing job as expired so cancle further waiting
    void asyncCancel()
    {
        
    }
    
    
};

typedef std::shared_ptr<DeadlineTimer> deadline_timer_ptr;

#endif // DEADLINE_TIMER_HPP
