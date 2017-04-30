#ifndef PROCESS_HPP
#define PROCESS_HPP

// turn off the specific warning. Boost 1_64_0
#pragma GCC diagnostic ignored "-Wpedantic"

#include <boost/process/detail/config.hpp>
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


class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

class Process
    : public boost::enable_shared_from_this<Process>
{

public:
    Process(session_data_ptr session, ba::io_service& io_service, std::string cmdline)
        : m_session(session)
        , m_pipe(io_service)
        , m_child(cmdline, bp::std_out > m_pipe)
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
        return m_child.running() || m_child.exit_code() == 0;
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
        boost::asio::async_read_some(m_pipe, boost::asio::buffer(buf),
                                     [] (boost::bind(&Process::handleRead, shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

    /**
     * @brief Asyc Process Read Handler
     * @param error
     */
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if(!error)
        {
            std::cout << "async_read process error: " << error.message() << std::endl;
            return;
        }

        // TODO testing, we need to do pipe/ cp437/utf-8 conversions before sending!
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
        boost::asio::async_write(m_pipe,
                                 boost::asio::buffer(msg, msg.size()),
                                 boost::bind(&Process::handleWrite, shared_from_this(),
                                             boost::asio::placeholders::error));
    }
    
    /**
     * @brief Callback after Writing Data, If error/hangup notifies
     * everything this person has left.
     * @param error
     */
    void handleWrite(const boost::system::error_code& error)
    {
        if(error)
        {
            std::cout << "async_write process error: " << error.message() << std::endl;
        }        
    }
    
    session_data_ptr  m_session;
    bp::async_pipe    m_pipe;
    bp::child         m_child;
    std::vector<char> m_read_buffer;

};

typedef boost::shared_ptr<Process> process_ptr;

#endif // PROCESS_HPP
