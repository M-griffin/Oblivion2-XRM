#ifndef SAFE_QUEUE_HPP
#define SAFE_QUEUE_HPP

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

/**
 * @class safe_queue
 * @author Michael Griffin
 * @date 15/08/2015
 * @file safe_queue.hpp
 * @brief Multi-Thread Safe Message Queue
 */
template <class T>
class SafeQueue
{
public:
    explicit SafeQueue(void) :
        q(),
        m(),
        c()
    {}

    // Copy Constructor (Overridden with MOVE)
    SafeQueue& operator=(SafeQueue& other)
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
    SafeQueue& operator=(SafeQueue&& other)
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

    ~SafeQueue(void)
    {
        std::queue<T>().swap(q);
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
        if (q.empty()) 
            return nullptr;
            
        std::unique_lock<std::mutex> lock(m);

        T val = q.front();
        q.pop();
        return val;
    }

    // Clear out the Entire Queue.
    void clear(void)
    {
        std::lock_guard<std::mutex> lock(m);
        std::queue<T>().swap(q);
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
    
    SafeQueue& copy(SafeQueue&& other)
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

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

#endif // SAFE_QUEUE_HPP
