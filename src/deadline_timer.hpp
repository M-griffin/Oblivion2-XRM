#ifndef DEADLINE_TIMER_HPP
#define DEADLINE_TIMER_HPP

#include <chrono>
#include <functional>

class DeadlineTimer {
public:
    DeadlineTimer(const std::chrono::milliseconds delay, const std::function<void()> &callbackMethod)
        : m_delay(delay), m_callbackMethod(callbackMethod), m_start(std::chrono::steady_clock::now()), m_triggered(false) {}

    void update() {
        if (!m_triggered && std::chrono::steady_clock::now() - m_start >= m_delay) {
            m_callbackMethod();
            m_triggered = true;
        }
    }

private:
    std::chrono::milliseconds m_delay;
    std::function<void()> m_callbackMethod;
    std::chrono::steady_clock::time_point m_start;
    bool m_triggered;
};

#endif
