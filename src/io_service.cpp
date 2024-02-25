#include "io_service.hpp"

#include <cstring>
#include <string>
#include <thread>
#include <chrono>
#include <system_error>
#include <memory>
//#include <malloc.h>

#include "common_io.hpp"
#include "socket_handler.hpp"
#include "logging.hpp"

IOService::IOService()
    : m_log(Logging::getInstance())
    , m_listener(nullptr)
    , m_is_active(true)
{
}

IOService::~IOService()
{
    m_log.write<Logging::DEBUG_LOG>("~IOService()");
    m_listener.reset();
    m_service_list.clear();    
    m_timer_list.clear();
}

/**
 * @brief Async Listener, check for incoming connections
 */
void IOService::checkAsyncListenersForConnections()
{        
    if(!m_listener || !m_listener->getSocketHandle()->isActive())
    {
        return;
    }

    socket_handler_ptr handler = m_listener->getSocketHandle()->acceptTelnetConnection();

    if(handler != nullptr)
    {
        m_log.write<Logging::DEBUG_LOG>("ioservice async accept - connection created.");
        std::error_code success_code(0, std::generic_category());

        try
        {                
            m_log.write<Logging::DEBUG_LOG>("ioservice Async-Accept - Execute CallBack", __FILE__, __LINE__);
            
            // Check for max nodes here, if we like can limit, send a message and drop
            // connection on handler by not passing it through the callback.
            // Creates a Session on New Incoming Connections
            m_listener->executeCallback(success_code, handler);
                            
            m_log.write<Logging::DEBUG_LOG>("ioservice Async-Accept - Returned CallBack", __FILE__, __LINE__);
        }
        catch(std::exception &ex)
        {
            m_log.write<Logging::WARN_LOG>("ioservice Exception Async-Accept", ex.what(), __FILE__, __LINE__);
        }
    }
}

/**
 * @brief Checks Timers for Execution of Callbacks.
 */
void IOService::checkAsyncTimers()
{
    if (m_timer_list.size() > 0) 
    {
        SafeQueue<timer_base_ptr> pulled_timer_list; 
        pulled_timer_list.copy(std::move(m_timer_list));
        
        while(!pulled_timer_list.isEmpty())
        {
            timer_base_ptr timer_work = pulled_timer_list.dequeue();
            
            if (!timer_work)
            {
                continue;
            }
            
            if (timer_work->isExpired())
            {
                timer_work->executeCallback(0);
            }
            else
            {
                // Throw it back on the Queue at the Back for next iteration
                m_timer_list.enqueue(timer_work);
            }
        }
    }
}


/**
 * @brief Main looping method
 */
void IOService::run()
{
    char msg_buffer[MAX_BUFFER_SIZE];
    
    // Timed Garbage Collection to free memory back to the OS
    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point<clock, std::chrono::milliseconds>;
    
    time_point_type start   = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    
    while(m_is_active)
    {        
        // Check for incoming connections
        checkAsyncListenersForConnections();
        
        checkAsyncTimers();
        
        // When there are no Connections Or Activity we are looping here.
        if (m_service_list.size() == 0)
        {            
            // Temp timer, change to 10/20 miliseconds for cpu usage
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            
            // Time > 10 Minutes, reset time when no activity no reason to garbage collect.
            time_point_type end = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
            long time_passed = (end - start).count();
            if (time_passed >= 600000)
            {                                    
                std::cout << "No Timer Activity, Reset() time_passed= " << time_passed << std::endl;        
                start = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
            }
            continue;
        }
        
        // Test, forgot what this 
        SafeQueue<service_base_ptr> pulled_list; 
        pulled_list.copy(std::move(m_service_list));       

        // This will wait for another job to be inserted on next call
        // Do we want to insert the job back, if poll is empty or
        // move to vector then look polls..  i think #2.
        while(!pulled_list.isEmpty())
        {
            service_base_ptr job_work = pulled_list.dequeue();
           

            // Remove Any jobs when the socket has been disconnected.
            if(!job_work || !job_work->getSocketHandle() || !job_work->getSocketHandle()->isActive())
            {
                m_log.write<Logging::DEBUG_LOG>("ioservice Removing Async Job - Socket Inactive");
                job_work.reset();
                
                // Temp timer, change to 10/20 miliseconds for cpu usage
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                continue;
            }

            /**
             * Handle Read Service if Data is Available.
             */
            if(job_work->getServiceType() == SERVICE_TYPE_READ)
            {
                // If Data Available, read, then populate buffer
                // Otherwise keep polling till data is available.
                int result = job_work->getSocketHandle()->poll();

                if(result > 0)
                {
                    memset(&msg_buffer, 0, MAX_BUFFER_SIZE);
                    int length = job_work->getSocketHandle()->recvSocket(msg_buffer);

                    if(length < 0)
                    {
                        // Error - Lost Connection
                        m_log.write<Logging::WARN_LOG>("ioservice async_read - lost connection!: ", length);
                        job_work->getSocketHandle()->setInactive();
                        std::error_code lost_connect_error_code(1, std::system_category());
                        job_work->executeCallback(lost_connect_error_code, nullptr);
                    }
                    else
                    {
                        // Success
                        job_work->setBuffer((unsigned char *)msg_buffer);
                        std::error_code success_code(0, std::generic_category());
                        job_work->executeCallback(success_code, nullptr);
                    }
                    
                    // Clear Job, then Pop it off the list.
                    job_work.reset();
                }
                else if(result == -1)
                {
                    m_log.write<Logging::WARN_LOG>("ioservice async_poll - lost connection!");
                    std::error_code lost_connect_error_code(1, std::system_category());
                    job_work->executeCallback(lost_connect_error_code, nullptr);
                    
                    // Clear Job, then Pop it off the list.
                    job_work.reset();
                }
                else
                {
                    // No Data Retrieved, Add Read Job back to end of Queue.
                    m_service_list.enqueue(job_work);
                }
            }

            /**
             * Handle Write Service if Data is Available.
             * Usuall on a Write it's only setup when data is available.
             */
            else if(job_work->getServiceType() == SERVICE_TYPE_WRITE)
            {
                int result = job_work->getSocketHandle()->sendSocket(
                                 (unsigned char*)job_work->getStringSequence().c_str(),
                                 job_work->getStringSequence().size());

                if(result <= 0)
                {
                    // Error - Lost Connection
                    m_log.write<Logging::WARN_LOG>("ioservice async_write - lost connection!");
                    job_work->getSocketHandle()->setInactive();
                    std::error_code lost_connect_error_code(1, std::system_category());
                    job_work->executeCallback(lost_connect_error_code, nullptr);
                    
                    // Clear Job then Pop it off the list.
                    job_work.reset();                    
                }
                else
                {
                    std::error_code success_code(0, std::generic_category());
                    job_work->executeCallback(success_code, nullptr);
                    
                    // Clear Job then Pop it off the list.
                    job_work.reset();
                } 
            }
        }

        // Temp timer, change to 10/20 miliseconds for cpu usage
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        
        time_point_type end = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
        long time_passed = (end - start).count();
        
        // Garbage collection Memory back to the OS Every 5 Minute (Testing, update this maybe hourly)
        if (time_passed >= 300000)            
        {            
            //std::cout << "malloc_trim() time_passed= " << time_passed << ", bytes freeed= " << malloc_trim(0) << std::endl;        
            
            
            // Reset Timer
            start = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
        }
    }
    
    

}

/**
 * @brief Shutdown Async Polling Service
 */
void IOService::stop()
{
    m_is_active = false;    
    m_log.write<Logging::DEBUG_LOG>("Shutting down IOService - Stop");
    
    // Clear All Lists and attached handles.
    m_service_list.clear();
    m_timer_list.clear();
    m_listener.reset();
}

bool IOService::isActive()
{
    return m_is_active;
}