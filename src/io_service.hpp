
#ifndef IO_SERVICE_HPP
#define IO_SERVICE_HPP

#include <functional>
#include <sstream>
#include <vector>
#include <memory>
#include <typeinfo>
#include <chrono>

#include "safe_vector.hpp"
#include "safe_queue.hpp"

class Logging;

class SocketHandler;
typedef std::shared_ptr<SocketHandler> socket_handler_ptr;

const int SERVICE_TYPE_NONE              = 0;
const int SERVICE_TYPE_READ              = 1;
const int SERVICE_TYPE_WRITE             = 2;
const int SERVICE_TYPE_CONNECT_TELNET    = 3;
const int SERVICE_TYPE_CONNECT_SSH       = 4;
const int SERVICE_TYPE_CONNECT_IRC       = 5;
const int SERVICE_TYPE_LISTENER_TELNET   = 6;
const int SERVICE_TYPE_LISTENER_SSH      = 7;
const int SERVICE_TYPE_LISTENER_IRC      = 8;
const int SERVICE_TYPE_ASYNC_TIMER       = 9;
const int SERVICE_TYPE_BLOCK_TIMER       = 10;

#define SERVICE_TIMER(x) ((int)(x) <= SERVICE_TYPE_BLOCK_TIMER \
                          && (int)(x) >= SERVICE_TYPE_ASYNC_TIMER)

#define SERVICE_LISTENER(x) ((int)(x) <= SERVICE_TYPE_LISTENER_IRC \
                             && (int)(x) >= SERVICE_TYPE_LISTENER_TELNET)

/**
 * @class IOService
 * @author Michael Griffin
 * @date 06/12/2017
 * @file io_service.hpp
 * @brief Handle Async Socket Read/Write and function Callback on results.
 */
class IOService
{

public:
    IOService();
    ~IOService();
    
    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point<clock, std::chrono::milliseconds>;

    /**
     * Handles Call Back Functions, execute at the end of Asyn Job.
     */
    typedef std::function<void(const std::error_code&, socket_handler_ptr)> StdFunctionCallbackHandler;
    typedef std::function<void(int)> StdTimerCallBack;

    static const int MAX_BUFFER_SIZE = 16384;

    /**
     * @class ServiceBase
     * @author Michael Griffin
     * @date 06/12/2017
     * @file io_service.hpp
     * @brief IO Service Job Base Template, needed to access virtual methods on ServiceJob
     */
    class ServiceBase
    {
    public:
        virtual ~ServiceBase()
        {
            //std::cout << "ServiceBase Destructor called\n";
        }
        virtual void setBuffer(unsigned char *buffer) = 0;
        virtual std::vector<unsigned char> &getBuffer() = 0;
        virtual std::string getStringSequence() = 0;
        virtual socket_handler_ptr getSocketHandle() = 0;
        virtual void executeCallback(const std::error_code &er, socket_handler_ptr conn) = 0;
        virtual int getServiceType() = 0;
    };
    typedef std::shared_ptr<ServiceBase> service_base_ptr;


    /**
     * @class ServiceJob
     * @author Michael Griffin
     * @date 06/12/2017
     * @file io_service.hpp
     * @brief IO Service Job Template
     */
    template <class MutableBufferSequence, class StringSequence, class SocketHandle, class Callback, class ServiceType>
    class ServiceJob : public ServiceBase
    {
    public:
    
        virtual ~ServiceJob()
        {
            //std::cout << "ServiceJob Destructor called\n";
            m_socket_handle.reset();
        }
        
        virtual void setBuffer(unsigned char *buffer) override
        {
            for(int i = 0; i < MAX_BUFFER_SIZE; i++)
            {
                m_buffer.push_back(buffer[i]);
            }
        }

        virtual std::vector<unsigned char> &getBuffer() override
        {
            return m_buffer;
        }

        virtual std::string getStringSequence() override
        {
            return m_string_sequence;
        }

        virtual socket_handler_ptr getSocketHandle() override
        {
            return m_socket_handle;
        }

        virtual void executeCallback(const std::error_code &err, socket_handler_ptr handle) override
        {
            StdFunctionCallbackHandler callback_method(m_callback);
            callback_method(err, handle);
        }

        virtual int getServiceType() override
        {
            return m_service_type;
        }


        ServiceJob(MutableBufferSequence &buffer, StringSequence string_sequence, SocketHandle socket_handle, 
                Callback &callback, ServiceType service_type)
            : m_buffer(buffer)
            , m_string_sequence(string_sequence)
            , m_socket_handle(socket_handle)
            , m_callback(callback)
            , m_service_type(service_type)
        { }        

        MutableBufferSequence &m_buffer;
        StringSequence         m_string_sequence;
        SocketHandle           m_socket_handle;
        Callback               m_callback;
        ServiceType            m_service_type;
    };    

    /**
     * @brief Add Async Jobs to the Vector Queue
     * @param buffer
     * @param string_sequence
     * @param socket_handle
     * @param callback
     * @param service_type
     */
    template <typename MutableBufferSequence, typename StringSequence, typename SocketHandle, typename Callback, typename ServiceType>
    void addAsyncJob(MutableBufferSequence &buffer, StringSequence string_sequence, SocketHandle socket_handle,
                     Callback &callback, ServiceType service_type)
    {
        // We keep NEW here for Initial Pointer Creation due to Varidict Templates.
        // Made Sure Virtual Descructors are called properly when Jobs are released. 
        ServiceJob<MutableBufferSequence, StringSequence, SocketHandle, Callback, ServiceType> *job
            = new ServiceJob <MutableBufferSequence, StringSequence, SocketHandle, Callback, ServiceType>
                (buffer, string_sequence, socket_handle, callback, service_type);

        if(SERVICE_LISTENER(service_type))
        {
            // Server Connection Listener Job (1) for each Service.
            m_listener_list.push_back(std::shared_ptr<ServiceBase>(job));
        }
        else
        {
            // Standard Async Job
            m_service_list.enqueue(std::shared_ptr<ServiceBase>(job));
        }
    }
    
    /**
     * @class TimerBase
     * @author Mercyful
     * @date 16/12/23
     * @file io_service.hpp
     * @brief IO Service Timer Base Template, needed to access virtual methods on ServiceJob
     */
    class TimerBase
    {
    public:
        using clock             = std::chrono::system_clock;
        using time_point_type   = std::chrono::time_point<clock, std::chrono::milliseconds>;
    
        virtual ~TimerBase()
        {
            //std::cout << "TimerBase Destructor called\n";
        }
        virtual void executeCallback(int value) = 0;
        virtual time_point_type getStartTime() = 0;
        virtual int getExpiresFromNow() = 0;
        virtual long getTimePassed() = 0;
        virtual bool isExpired() = 0;
    };
    typedef std::shared_ptr<TimerBase> timer_base_ptr;
    
    /**
     * @class TimerJob
     * @author Michael Griffin
     * @date 12/12/2023
     * @file io_service.hpp
     * @brief IO Service Job Template
     */
    template <class StartTime, class MilliSeconds, class Callback>
    class TimerJob : public TimerBase
    {
    public:
        using clock             = std::chrono::system_clock;
        using time_point_type   = std::chrono::time_point<clock, std::chrono::milliseconds>;
    
        virtual ~TimerJob()
        {
            //std::cout << "TimerJob Destructor called\n";
        }
        
        virtual void executeCallback(int value) override
        {
            StdTimerCallBack callback_method(m_callback);
            callback_method(value);
        }

        virtual time_point_type getStartTime() override
        {
            return m_start_time;
        }
        
        virtual int getExpiresFromNow() override
        {
            return m_expires_from_now;
        }
        
        virtual long getTimePassed() override
        {
            time_point_type end = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
            return (end - m_start_time).count();
        }
        
        virtual bool isExpired() override
        {
            if (getTimePassed() > m_expires_from_now)
            {
                return true;
            }
            
            return false;
        }
        

        TimerJob(StartTime start_time, MilliSeconds milli_seconds, Callback &callback)
            : m_start_time(start_time)
            , m_expires_from_now(milli_seconds)
            , m_callback(callback)
        { }        

        StartTime              m_start_time;
        MilliSeconds           m_expires_from_now;
        Callback               m_callback;

    };
    
    
    /**
     * @brief Add Async Jobs to the Vector Queue
     * @param buffer
     * @param string_sequence
     * @param socket_handle
     * @param callback
     * @param service_type
     */
    template <typename StartTime, typename MilliSeconds, typename Callback>
    void addTimerJob(StartTime start_time, MilliSeconds expires_from_now, Callback &callback)
    {
        // We keep NEW here for Initial Pointer Creation due to Varidict Templates.
        // Made Sure Virtual Descructors are called properly when Jobs are released. 
        TimerJob<StartTime, MilliSeconds, Callback> *job = new TimerJob <StartTime, MilliSeconds, Callback>
                (start_time, expires_from_now, callback);

        // Timer (Priority List Job)
        m_timer_list.enqueue(std::shared_ptr<TimerBase>(job));
    }

    /**
     * @Brief Always check all timers (Priority each iteration)
     */
    void checkPriorityTimers();

    /**
     * @brief Async Listener, check for incoming connections
     */
    void checkAsyncListenersForConnections();
    
    /**
     * @brief Checks Timers for Execution of Callbacks.
     */
    void checkAsyncTimers();

    /**
     * @brief Main looping method
     */
    void run();

    /**
     * @brief Shutdown Async Polling Service
     */
    void stop();
    
    /**
     * @brief Check if the IO Service Is Active
     */
    bool isActive();
    
    SafeVector<service_base_ptr>  m_listener_list;
    SafeQueue<service_base_ptr>   m_service_list;    
    SafeQueue<timer_base_ptr>     m_timer_list;
    
    Logging                      &m_log;
    bool                          m_is_active;

};

#endif // IO_SERVICE_HPP
