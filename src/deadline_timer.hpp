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
 * @brief Timer class for Wait and Async_Wait.
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


    // Blocking Socket Handler polling for the timer to expire, just call thread sleep
    void wait(int miliseconds_to_wait)
    {
        // this could pause the io_service, since this is calls in sessions
        // might need to stall socket_reads for miliseconds_to_wait
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds_to_wait));
    }

    // Set an expiry time relative to now.
    void expiresAt(int miliseconds_to_expire)
    {
        
        
    }  
    
    // Non-Blocking Will push job to io_service which will check if expires time has passed.
    // Then run the handler, or ignore and exit if asyncCancel
    void asyncWait()
    {
       // Rewrite the async_read below for this!
    }
    
    // Will mark the existing job as expired so cancle further waiting
    void asyncCancel()
    {
        
    }
    

    // TODO REWRITE for adding timer jobs.
    /**
     * @brief 
     * @param BufferSequence - populates will data retrieved
     * @param Callback - returns error code
     */
    template <typename BufferSequence, typename Callback>
    void async_read(BufferSequence &buffer, const Callback &callback)
    {
        m_io_service.addAsyncJob(buffer, nullptr, m_socket_handler, callback, SERVICE_TYPE_TIMER);
    }
    
};

typedef std::shared_ptr<DeadlineTimer> deadline_timer_ptr;

#endif // DEADLINE_TIMER_HPP
