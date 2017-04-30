#include "process.hpp"
#include "session_data.hpp"
#include "model-sys/structures.hpp"

// turn off the specific warning. Boost 1_64_0
#pragma GCC diagnostic ignored "-Wpedantic"

#include <boost/process/detail/config.hpp>
#include <boost/process.hpp>
#include <boost/process/async.hpp>
#include <boost/process/windows.hpp>


// turn the warnings back on
#pragma GCC diagnostic pop

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <vector>
#include <string>


namespace ba = boost::asio;
namespace bp = boost::process;
namespace bs = boost::system;

Process::Process(session_data_ptr session, ba::io_service& io_service, std::string cmdline)
        : m_session(session)
        , m_output_pipe(io_service)
        , m_input_pipe(io_service)
        , m_child(cmdline, 
            bp::start_dir=GLOBAL_SCRIPT_PATH,
            bp::std_out > m_output_pipe, 
            bp::std_in < m_input_pipe, 
            bp::std_err > bp::null,
            bp::windows::show,
            // Process Handlers
            /*
            bp::on_setup([](auto &e)
            { 
                std::cout << "Process on_setup" << std::endl;
                e.startup_info.dwFlags = STARTF_RUNFULLSCREEN; 
            }),
            bp::on_error([](auto&, const std::error_code & error)
            { 
                std::cout << "Process on_error" << std::endl;
                std::cerr << error.message() << std::endl; 
            }),*/
            bp::on_exit([&](int exit, const std::error_code&)
            {                      
                std::cout << "Process on_exit" << exit << std::endl;
                m_input_pipe.async_close();
                m_output_pipe.async_close();
                m_session->m_is_process_running = false;
            }))
{
    // Start up the CallBack connection on Pipe Data
    waitingForData();
}

Process::~Process()
{ }

    
/**
 * @brief Checks if the process is still running
 * @return
 */
bool Process::isRunning()
{
    std::cout << "m_child.exit_code(): " << m_child.exit_code() << std::endl;
    // Verify if running, what the exit code actually is.
    return m_child.running() && m_child.exit_code() == 0;
}

/**
 * @brief Get Child Buffer Data, then reset buffer for next call back.
 * @return
 */
std::string Process::getBufferData()
{
    std::string data(m_read_buffer.begin(), m_read_buffer.end());
    data.append("\0");
    std::vector<char>().swap(m_read_buffer);
    return data;
}

/**
 * @brief Data Handler for incoming Data (From Child Process)
 */
void Process::waitingForData()
{
    // Reads from pipe (From Child Process) need to setup call back to method!
    m_output_pipe.async_read_some(ba::buffer(m_read_buffer),
                                    boost::bind(&Process::handleRead, shared_from_this(),
                                        ba::placeholders::error,
                                        ba::placeholders::bytes_transferred));
}

/**
 * @brief Asyc Process Read Handler when data is received.
 * @param error
 */
void Process::handleRead(const bs::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        std::cout << "async_read process error: " 
                  << error.message() 
                  << " : " 
                  << bytes_transferred 
                  << std::endl;
        return;
    }

    // Deliver Data Back to the User Session.
    std::string pipe_data = std::move(getBufferData());
    m_session->deliver(pipe_data);

    // Restart Next Call Back
    if (isRunning())
    {
        waitingForData();
        return;
    }

    std::cout << "Process is no longer running. " << std::endl;
    // Do we terminate here? hmm 
    //m_child.terminate();
}

/**
 * @brief Deliver Message (Child Process)
 * @param msg
 */
void Process::deliver(const std::string &msg)
{
    ba::async_write(m_input_pipe, 
                    ba::buffer(msg, msg.size()),
                    boost::bind(&Process::handleWrite, 
                        shared_from_this(),
                        ba::placeholders::error));
}

/**
 * @brief Callback after Writing Data, Checks Errors
 * @param error
 */
void Process::handleWrite(const bs::error_code& error)
{
    if(error)
    {
        std::cout << "async_write process error: " 
                  << error.message() 
                  << std::endl;
    }        
}
