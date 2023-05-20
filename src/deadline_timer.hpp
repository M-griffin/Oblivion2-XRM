#ifndef DEADLINE_TIMER_HPP
#define DEADLINE_TIMER_HPP


#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <functional>
#include <atomic>

/**
 * @class DeadlineTimer
 * @author Michael Griffin
 * @date 17/02/2018
 * @file deadline_timer.hpp
 * @brief Timer class for Wait and AsyncWait.
 */
class DeadlineTimer
{
public:
    explicit DeadlineTimer()
        : m_expires_from_now(0)
        , m_num_instances (0)
    {
    }

    ~DeadlineTimer()
    {
        std::cout << "~DeadlineTimer()" << std::endl;
    }

    typedef std::function<void(int)> StdFuncationCallBack;

    int              m_expires_from_now;
    std::atomic_int  m_num_instances;

    /**
     * @brief Sets the waiting period for the timer.
     * @param time
     */
    void setWaitInMilliseconds(int time)
    {
        m_expires_from_now = time;
    }

    /**
     * @brief Async Non-Block Wait (MilliSeconds) with Callback
     * @param callback_method
     */
    template <typename Callback>
    void asyncWait(const Callback &callback_method)
    {
        // Values Passed to the Thread that don't change.
        int i = m_expires_from_now;      
        int c = m_num_instances +1;

        // Add to container to keep in an active scope        
        std::thread time_thread([i, c, callback_method, this]()
        {
            m_num_instances += 1;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(i));
            StdFuncationCallBack fun_callback(callback_method);

            // If Current Instance is the 
            // If overwrritten we'll have (2) instances, ignore the first.
            if(m_num_instances == 1)
            {
                std::cout << "DeadlineTimer() Executing CallBack, NumInstances=" << m_num_instances << " CurrInstance=" << c << std::endl;
                fun_callback(0);
            }
            else
            {
                std::cout << "DeadlineTimer() Cancled, Num Instances=" << m_num_instances << " CurrInstance=" << c << std::endl;
            }
            
            m_num_instances -= 1;
        });
        
        // Detach so there is no hold on the thread.
        time_thread.detach();
    }

    /**
     * @brief Blocking Wait (MilliSeconds), Sleeps then returns
     */
    void wait()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_expires_from_now));
    }

};

typedef std::shared_ptr<DeadlineTimer> deadline_timer_ptr;
typedef std::unique_ptr<DeadlineTimer> deadline_timer_uptr;

#endif // DEADLINE_TIMER_HPP
