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
    Process(session_data_ptr session, ba::io_service& io_service, std::string cmdline);        
    ~Process();

    
    /**
     * @brief Checks if the process is still running
     * @return
     */
    bool isRunning();
    
    /**
     * @brief Get Child Buffer Data, then reset buffer for next call back.
     * @return
     */
    std::string getBufferData();
    
    /**
     * @brief Data Handler for incoming Data (From Child Process)
     */
    void waitingForData();
    
    /**
     * @brief Asyc Process Read Handler when data is received.
     * @param error
     */
    void handleRead(const bs::error_code& error, std::size_t bytes_transferred);
    
    /**
     * @brief Deliver Message (Child Process)
     * @param msg
     */
    void deliver(const std::string &msg);
    
    /**
     * @brief Callback after Writing Data, Checks Errors
     * @param error
     */
    void handleWrite(const bs::error_code& error);
    
    /**
     * @brief Kill Process
     */
    void terminate()
    {
        m_child.terminate();
    }
    
    session_data_ptr  m_session;
    bp::async_pipe    m_output_pipe;
    bp::async_pipe    m_input_pipe;
    bp::child         m_child;
    std::vector<char> m_read_buffer;

};

typedef boost::shared_ptr<Process> process_ptr;

#endif // PROCESS_HPP
