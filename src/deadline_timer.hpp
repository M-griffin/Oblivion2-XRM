#ifndef DEADLINE_TIMER_HPP
#define DEADLINE_TIMER_HPP


#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <memory>
#include <functional>


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
        , m_cancellation_token(false)
    {
    }

    ~DeadlineTimer()
    {
        std::vector<std::future<void>>().swap(m_future_list);
    }

    typedef std::function<void(int)> StdFuncationCallBack;

    int m_expires_from_now;
    std::atomic_bool m_cancellation_token;
    std::vector<std::future<void>> m_future_list;

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
     * @param callback
     */
    template <typename Callback>
    void asyncWait(const Callback &callback)
    {
        int i = m_expires_from_now;
        //std::atomic_bool cancel = &m_cancellation_token;
        // If one exist, remove and re-create it.
        checkAsyncClean();

        // Add to container to keep in an active scope
        m_future_list.emplace_back(
            std::async(std::launch::async, [i, callback, this]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(i));
            StdFuncationCallBack fun_callback(callback);

            // If not Canceled, Execute CallBack with a default status, update lateron
            // Cancel seems to still execute, but i'm thinking we still need to execute
            // On Double ESC's HITS.
            if(!m_cancellation_token)
            {
                fun_callback(0);
            }
        })
        );
    }

    /**
     * @brief Clean any unfinished, or cancels on overwrites with new.
     */
    void checkAsyncClean()
    {
        if(m_future_list.size() > 0)
        {
            m_cancellation_token = true;
            m_future_list.pop_back();
            m_cancellation_token = false;
        }
        else
        {
            m_cancellation_token = false;
        }
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

#endif // DEADLINE_TIMER_HPP
