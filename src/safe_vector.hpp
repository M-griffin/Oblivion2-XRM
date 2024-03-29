#ifndef SAFE_VECTOR_HPP
#define SAFE_VECTOR_HPP

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <algorithm>

/**
 * @class SafeVector
 * @author Michael Griffin
 * @date 12/06/2017
 * @file safe_vector.hpp
 * @brief A simple Thread Safe Vector Template
 */
template <class T>
class SafeVector
{
public:
    explicit SafeVector(void)
        : v() // Queue
        , m() // Mutex
        , c() // Control
    {}

    // Default copy Constructor.
    SafeVector(const SafeVector&) = delete;

    // Copy Constructor (Overridden with MOVE)
    SafeVector& operator=(SafeVector& other)
    {
        if(this != &other)
        {
            std::lock_guard<std::mutex> lock(m);

            if(!&other.isEmpty())
            {
                v = other.v;
                m = other.m;
                c = other.c;
            }
        }

        return *this;
    }

    // Move Constructor.
    SafeVector& operator=(SafeVector&& other)
    {
        if(this != &other)
        {
            std::lock_guard<std::mutex> lock(m);

            if(!other.is_empty())
            {
                v.swap(other.v);
            }
        }

        return *this;
    }

    ~SafeVector(void)
    {
        std::vector<T>().swap(v);
    }

    // Add Item to Queue.
    void push_back(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        v.push_back(t);
        c.notify_one();
    }

    // Clear out the Entire Queue.
    void clear(void)
    {
        std::lock_guard<std::mutex> lock(m);

        while(v.size() > 0)
        {
            v.pop_back();
        }

        std::vector<T>().swap(v);
        c.notify_one();
    }

    // Check for Non-Blocking Wait.
    bool is_empty(void) const
    {
        return v.empty();
    }

    // Check for Non-Blocking Wait.
    unsigned long size(void) const
    {
        return v.size();
    }

    // Remove item quickly, swap with back, then pop back.
    // If only item in list, then pop it.
    void remove(std::string::size_type index)
    {
        if(v.size() > 0)
        {
            std::unique_lock<std::mutex> lock(m);

            // Create new Vector to hold all items.
            std::vector<T> temp;

            for(std::string::size_type i = 0; i < v.size(); i++)
            {
                if(i != index)
                {
                    temp.push_back(v.at(i));
                }
            }

            v.swap(temp);
            std::vector<T>().swap(temp);
        }
    }
    
    bool exists(T t)
    {
        if (is_empty()) 
        {
            return false;
            
        }
        
        std::unique_lock<std::mutex> lock(m);
        if (std::find(v.begin(), v.end(), t) != v.end())
        {
            return true;
        }
        return false;
    }

    T get(int index)
    {
        std::unique_lock<std::mutex> lock(m);
        return v.at(index);

    }

private:
    std::vector<T> v;
    mutable std::mutex m;
    std::condition_variable c;
};


#endif // SAFE_VECTOR_HPP
