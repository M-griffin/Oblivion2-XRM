#ifndef SAFE_SET_HPP
#define SAFE_SET_HPP

#include <iostream>
#include <set>
#include <mutex>
#include <condition_variable>

/**
 * @class safe_set
 * @author Michael Griffin
 * @date 18/10/2015
 * @file safe_set.hpp
 * @brief Multi-Thread Safe Set List
 */
template <class T>
class SafeSet
{
public:
    explicit SafeSet(void) :
        q(),
        m(),
        c()
    {}

    // Copy Constructor (Overridden with MOVE)
    SafeSet& operator=(SafeSet& other)
    {
        if(this != &other)
        {
            std::lock_guard<std::mutex> lock(m);

            if(!&other.isEmpty())
            {
                q = other.q;
                m = other.m;
                c = other.c;
            }
        }

        return *this;
    }

    // Move Constructor.
    SafeSet& operator=(SafeSet&& other)
    {
        if(this != &other)
        {
            std::lock_guard<std::mutex> lock(m);

            if(!other.isEmpty())
            {
                q.swap(other.q);
            }
        }

        return *this;
    }

    ~SafeSet(void)
    {
        //should be queue or vector!  most likely vector!
        std::set<T>().swap(q);
    }

    // Add Item to Queue.
    //void enqueue(const T& t)
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get first Item in Queue. FIFO
    T dequeue(void)
    {
        std::unique_lock<std::mutex> lock(m);

        while(q.empty())
        {
            c.wait(lock);
        }

        T val = q.front();
        q.pop();
        return val;
    }

    const T begin(void)
    {
        return q.front();
    }

    const T end(void)
    {
        return q.back();
    }

    // Clear out the Entire Queue.
    void clear(void)
    {
        std::lock_guard<std::mutex> lock(m);
        std::set<T>().swap(q);
    }

    // Check for Non-Blocking Wait.
    bool isEmpty(void) const
    {
        return q.empty();
    }

    // Check for Non-Blocking Wait.
    unsigned long size(void) const
    {
        return q.size();
    }

private:
    std::set<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

#endif // SAFE_QUEUE_HPP
