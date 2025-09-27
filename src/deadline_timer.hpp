#ifndef DEADLINE_TIMER_HPP
#define DEADLINE_TIMER_HPP

#include <chrono>
#include <functional>

class DeadlineTimer {
public:
    DeadlineTimer(std::chrono::milliseconds delay, std::function<void()> callbackMethod)
        : delay(delay), callbackMethod(callbackMethod), start(std::chrono::steady_clock::now()), triggered(false) {}

    void update() {
        if (!triggered && std::chrono::steady_clock::now() - start >= delay) {
            callbackMethod();
            triggered = true;
        }
    }

private:
    std::chrono::milliseconds delay;
    std::function<void()> callbackMethod;
    std::chrono::steady_clock::time_point start;
    bool triggered;
};

#endif // DEADLINE_TIMER_HPP
