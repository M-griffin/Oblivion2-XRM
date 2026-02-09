#ifndef DEADLINE_TIMER_HPP
#define DEADLINE_TIMER_HPP

#include <chrono>
#include <functional>

class StateTimer {
public:
    StateTimer()
        : m_delay(std::chrono::milliseconds(0))
          , m_callbackMethod(nullptr)
          , m_start(std::chrono::steady_clock::now())
          , m_triggered(false) {
    }

    StateTimer(const std::chrono::milliseconds delay, const std::function<void()> &callbackMethod)
        : m_delay(delay)
          , m_callbackMethod(callbackMethod)
          , m_start(std::chrono::steady_clock::now())
          , m_triggered(false) {
    }

    bool isTriggered() {
        if (m_triggered) {
            return m_triggered;
        }

        // timer not armed or setup for call back.
        if (!m_callbackMethod) {
            return false;
        }

        if (std::chrono::steady_clock::now() - m_start >= m_delay) {
            m_callbackMethod();
            m_triggered = true;
        }

        return m_triggered;
    }

    void start(const std::chrono::milliseconds delay, const std::function<void()> &callbackMethod) {
        m_delay = delay;
        m_callbackMethod = callbackMethod;
        m_start = std::chrono::steady_clock::now();
        m_triggered = false;
    }

    void cancel() {
        m_delay = std::chrono::milliseconds(-0);
        m_triggered = true;
        m_callbackMethod = nullptr;
    }

private:
    std::chrono::milliseconds m_delay;
    std::function<void()> m_callbackMethod;
    std::chrono::steady_clock::time_point m_start;
    bool m_triggered;
};

#endif
