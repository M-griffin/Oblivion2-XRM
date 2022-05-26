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
    : m_is_active(false)
{
}

IOService::~IOService()
{
    m_service_list.clear();
    m_listener_list.clear();
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
            m_listener_list.remove(i);
            --i; // Compensate for item removed.
            continue;
        }

        socket_handler_ptr handler = listener_work->getSocketHandle()->acceptTelnetConnection();

        if(handler != nullptr)
        {
            log->xrmLog<Logging::DEBUG_LOG>("async accept - connection created.");
            std::error_code success_code(0, std::generic_category());

            try
            {
                // Check for max nodes here, if we like can limit, send a message and drop
                // connection on handler by not passing it through the callback.
                listener_work->executeCallback(success_code, handler);
            }
            catch(std::exception &ex)
            {
                log->xrmLog<Logging::ERROR_LOG>("Exception Async-Accept", ex.what(), __FILE__, __LINE__);
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
    m_is_active = true;

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

            if(!job_work || !job_work->getSocketHandle()->isActive())
            {
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
                        log->xrmLog<Logging::ERROR_LOG>("async_read - lost connection!: ", length);
                        job_work->getSocketHandle()->setInactive();
                        std::error_code lost_connect_error_code(1, std::system_category());
                        job_work->executeCallback(lost_connect_error_code, nullptr);
                        m_service_list.remove(i);
                        --i; // Compensate for item removed.
                    }
                    else
                    {
                        job_work->setBuffer((unsigned char *)msg_buffer);
                        std::error_code success_code(0, std::generic_category());
                        job_work->executeCallback(success_code, nullptr);
                        m_service_list.remove(i);
                        --i; // Compensate for item removed.
                    }
                }
                else if(result == -1)
                {
                    log->xrmLog<Logging::ERROR_LOG>("async_poll - lost connection!");
                    std::error_code lost_connect_error_code(1, std::system_category());
                    job_work->executeCallback(lost_connect_error_code, nullptr);
                    m_service_list.remove(i);
                    --i; // Compensate for item removed.
                }
            }

            /**
             * Handle Write Service if Data is Available.
             */
            else if(job_work->getServiceType() == SERVICE_TYPE_WRITE)
            {
                int result = job_work->getSocketHandle()->sendSocket(
                                 (unsigned char*)job_work->getStringSequence().c_str(),
                                 job_work->getStringSequence().size());

                if(result <= 0)
                {
                    // Error - Lost Connection
                    log->xrmLog<Logging::ERROR_LOG>("async_write - lost connection!");
                    job_work->getSocketHandle()->setInactive();
                    std::error_code lost_connect_error_code(1, std::system_category());
                    job_work->executeCallback(lost_connect_error_code, nullptr);
                    m_service_list.remove(i);
                    --i; // Compensate for item removed.
                }
                else
                {
                    std::error_code success_code(0, std::generic_category());
                    job_work->executeCallback(success_code, nullptr);
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
    // Clear All Lists and attached handles.
    m_service_list.clear();
    m_timer_list.clear();
    m_listener_list.clear();
}
