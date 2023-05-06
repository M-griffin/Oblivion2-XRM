#include "io_service.hpp"
#include "common_io.hpp"
#include "socket_handler.hpp"
#include "logging.hpp"

#include <cstring>
#include <string>
#include <thread>
#include <chrono>
#include <system_error>

IOService::IOService()
    : m_is_active(true)
{
}

IOService::~IOService()
{
    std::cout << "~IOService()" << std::endl;
    m_service_list.clear();
    m_listener_list.clear();
    m_timer_list.clear();
}

/**
 * @brief Async Listener, check for incoming connections
 */
void IOService::checkAsyncListenersForConnections()
{
    Logging *log = Logging::instance();

    // Timers are not removed each iteration
    // Async stay active until expired or canceled
    // And wait, will block socket polling for (x) amount of time
    for(unsigned int i = 0; i < m_listener_list.size(); i++)
    {
        service_base_ptr listener_work = m_listener_list.get(i);

        if(!listener_work || !listener_work->getSocketHandle()->isActive())
        {
            std::cout << "Removing Async Listener Job" << std::endl;
            listener_work.reset();
            m_listener_list.remove(i);
            --i; // Compensate for item removed.
            continue;
        }

        socket_handler_ptr handler = listener_work->getSocketHandle()->acceptTelnetConnection();

        if(handler != nullptr)
        {
            log->write<Logging::DEBUG_LOG>("async accept - connection created.");
            std::error_code success_code(0, std::generic_category());

            try
            {        
                // Check for max nodes here, if we like can limit, send a message and drop
                // connection on handler by not passing it through the callback.
                listener_work->executeCallback(success_code, handler);
                
                handler.reset();
                listener_work.reset();
                m_listener_list.remove(i);
                --i; // Compensate for item removed.
            }
            catch(std::exception &ex)
            {
                log->write<Logging::ERROR_LOG>("Exception Async-Accept", ex.what(), __FILE__, __LINE__);
            }
        }
    }
}


/**
 * @brief Main looping method
 */
void IOService::run()
{
    Logging *log = Logging::instance();

    char msg_buffer[MAX_BUFFER_SIZE];

    while(m_is_active)
    {
        // Check for incoming connections
        checkAsyncListenersForConnections();

        // This will wait for another job to be inserted on next call
        // Do we want to insert the job back, if poll is empty or
        // move to vector then look polls..  i think #2.
        for(unsigned int i = 0; i < m_service_list.size(); i++)
        {
            service_base_ptr job_work = m_service_list.get(i);

            // Remove Any jobs when the socket has been disconnected.
            if(!job_work || !job_work->getSocketHandle() || !job_work->getSocketHandle()->isActive())
            {
                std::cout << "Removing Async Job - Socket Inactive" << std::endl;
                job_work.reset();
                m_service_list.remove(i);
                --i; // Compensate for item removed.
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
                        log->write<Logging::ERROR_LOG>("async_read - lost connection!: ", length);
                        job_work->getSocketHandle()->setInactive();
                        std::error_code lost_connect_error_code(1, std::system_category());
                        job_work->executeCallback(lost_connect_error_code, nullptr);
                    }
                    else
                    {
                        job_work->setBuffer((unsigned char *)msg_buffer);
                        std::error_code success_code(0, std::generic_category());
                        job_work->executeCallback(success_code, nullptr);
                    }
                    
                    // Clear Job, then Pop it off the list.
                    job_work.reset();
                    m_service_list.remove(i);
                    --i; // Compensate for item removed.
                }
                else if(result == -1)
                {
                    log->write<Logging::ERROR_LOG>("async_poll - lost connection!");
                    std::error_code lost_connect_error_code(1, std::system_category());
                    job_work->executeCallback(lost_connect_error_code, nullptr);
                    
                    // Clear Job, then Pop it off the list.
                    job_work.reset();
                    m_service_list.remove(i);
                    --i; // Compensate for item removed.
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
                    log->write<Logging::ERROR_LOG>("async_write - lost connection!");
                    job_work->getSocketHandle()->setInactive();
                    std::error_code lost_connect_error_code(1, std::system_category());
                    job_work->executeCallback(lost_connect_error_code, nullptr);
                    
                    // Clear Job then Pop it off the list.
                    job_work.reset();
                    m_service_list.remove(i);
                    --i; // Compensate for item removed.
                    
                }
                else if (result > 0) // This was else, for 
                {
                    std::error_code success_code(0, std::generic_category());
                    job_work->executeCallback(success_code, nullptr);
                    
                    // Clear Job then Pop it off the list.
                    job_work.reset();
                    m_service_list.remove(i);
                    --i; // Compensate for item removed.
                } 
            }
        }

        // Temp timer, change to 10/20 miliseconds for cpu usage
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

}

/**
 * @brief Shutdown Async Polling Service
 */
void IOService::stop()
{
    m_is_active = false;
    
    std::cout << "shutting down IOService" << std::endl;
    // Clear All Lists and attached handles.
    m_service_list.clear();
    m_timer_list.clear();
    m_listener_list.clear();
}

bool IOService::isActive()
{
    return m_is_active;
}