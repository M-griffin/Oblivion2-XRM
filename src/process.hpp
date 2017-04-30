#ifndef PROCESS_HPP
#define PROCESS_HPP

// turn off the specific warning. Boost 1_64_0
#pragma GCC diagnostic ignored "-Wpedantic"

#include <boost/process/detail/config.hpp>
#include <boost/process/asnyc.hpp>
#include <boost/process.hpp>

// turn the warnings back on
#pragma GCC diagnostic pop

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>
#include <string>

namespace ba = boost::asio;
namespace bp = boost::process;
namespace bs = boost::system;


class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

/**
 * @class Process
 * @author Michael Griffin
 * @date 30/04/2017
 * @file process.hpp
 * @brief Asynchronous Process class for piping data between user sessions and external programs.
 */
class Process
    : public boost::enable_shared_from_this<Process>
{

public:
    Process(session_data_ptr session, ba::io_service& io_service, std::string cmdline)
        : m_session(session)
        , m_output_pipe(io_service)
        , m_input_pipe(io_service)
        , m_child(cmdline, bp::std_out > m_output_pipe, bp::std_in < m_input_pipe, bp::std_err > nullptr,
                    bp::windows::show,
                    // Process Handlers
                    bp::on_setup([](auto &e)
                    { 
                        std::cout << "Process on_setup" << std::endl;
                        e.startup_info.dwFlags = STARTF_RUNFULLSCREEN; 
                    }),
                    bp::on_error([](auto&, const std::error_code & error)
                    { 
                        std::cout << "Process on_error" << std::endl;
                        std::cerr << error.message() << std::endl; 
                    }),                    
                    bp::on_exit([&](int exit, const std::error_code& error)
                    {                      
                        std::cout << "Process on_exit: " << exit << " : " << error.message << std::endl;
                        m_input_pipe.async_close();
                        m_output_pipe.async_close();
                    }))
    {
        waitingForData();
    }

    ~Process()
    { }

    
    /**
     * @brief Checks if the process is still running
     * @return
     */
    bool isRunning()
    {
        return m_child.running() && m_child.exit_code() == 0;
    }

    /**
     * @brief Get Child Buffer Data, then reset buffer for next call back.
     * @return
     */
    std::string getBuffData()
    {
        std::string data(m_read_buffer.begin(), m_read_buffer.end());
        data.append("\0");
        std::vector<char>().swap(m_read_buffer);
        return data;
    }

    /**
     * @brief Data Handler for incoming Data (From Child Process)
     */
    void waitingForData()
    {
        // Reads from pipe (From Child Process) need to setup call back to method!
        ba::async_read_some(m_output_pipe, ba::buffer(buf),
                                     [] (boost::bind(&Process::handleRead, shared_from_this(),
                                             ba::placeholders::error,
                                             ba::placeholders::bytes_transferred));
    }

    /**
     * @brief Asyc Process Read Handler
     * @param error
     */
    void handleRead(const bs::error_code& error, std::size_t bytes_transferred)
    {
        if(!error)
        {
            std::cout << "async_read process error: " << error.message() << std::endl;
            return;
        }

        // Deliver Data Back to the User Session.
        std::string pipe_data = std::move(getBuffData());
        m_session->deliver(pipe_data);

        // Restart Next Call Back
        if (isRunning())
        {
            waitingForData();
            return;
        }

        std::cout << "Process is no longer running. " << std::endl;
    }
    
    /**
     * @brief Deliver Message (Child Process)
     * @param msg
     */
    void deliver(const std::string &msg)
    {
        ba::async_write(m_input_pipe, ba::buffer(msg, msg.size()),
                                [] (boost::bind(&Process::handleWrite, shared_from_this(),
                                    ba::placeholders::error));
    }
    
    /**
     * @brief Callback after Writing Data, If error/hangup notifies
     * everything this person has left.
     * @param error
     */
    void handleWrite(const bs::error_code& error)
    {
        if(error)
        {
            std::cout << "async_write process error: " << error.message() << std::endl;
        }        
    }
    
    session_data_ptr  m_session;
    bp::async_pipe    m_output_pipe;
    bp::async_pipe    m_input_pipe;
    bp::child         m_child;
    std::vector<char> m_read_buffer;

};

typedef boost::shared_ptr<Process> process_ptr;

#endif // PROCESS_HPP
