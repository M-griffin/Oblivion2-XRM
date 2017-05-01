#include "process.hpp"
#include "session_data.hpp"
#include "model-sys/structures.hpp"

// turn off the specific warning. Boost 1_64_0
#pragma GCC diagnostic ignored "-Wpedantic"

#include <boost/process/detail/config.hpp>
#include <boost/process/extend.hpp>
#include <boost/process/async.hpp>
#include <boost/process.hpp>

// Windows only for Testing!
#include <boost/process/windows.hpp>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>


// turn the warnings back on
#pragma GCC diagnostic pop

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

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
            // io_service
            
            
            //bp::std_err > m_output_pipe,
            
            //io_service,
            //bp::windows::show, // Windows Only for Testing.
            // Process Handlers
            
            bp::extend::on_setup([](auto &)
            { 
                std::cout << "Process on_setup" << std::endl;
                //e.startup_info.dwFlags = STARTF_USESHOWWINDOW;
                //e.startup_info.dwCreationFlags = CREATE_NEW_CONSOLE; 
                //e.startup_info.hStdOutput = m_output_pipe;
                //e.hStdError = newstdout;
                //e.startup_info.hStdInput = m_input_pipe;
            }),
            bp::extend::on_success([](auto&)
            { 
                std::cout << "Process on_success" << std::endl;
            }),            
            bp::extend::on_error([](auto&, const std::error_code & error)
            { 
                std::cout << "Process on_error" << std::endl;
                std::cerr << error.message() << std::endl; 
            }),
            bp::on_exit([&](int exit, const std::error_code&)
            {                      
                std::cout << "Process on_exit: " << exit << std::endl;
                m_input_pipe.async_close();
                m_output_pipe.async_close();
                m_session->m_is_process_running = false;
            }))
{   
    // Start up the CallBack connection on Pipe Data
    std::cout << "Process Startup Ready()" << std::endl;    
    
}

Process::~Process()
{ }

    
/**
 * @brief Checks if the process is still running
 * @return
 */
bool Process::isRunning()
{
    //std::cout << "m_child.running(): " << m_child.running() << std::endl;
    //std::cout << "Process m_child.exit_code(): " << m_child.exit_code() << std::endl;
    // Verify if running, what the exit code actually is.
    return m_child.running(); // && m_child.exit_code() == 0;
}

/**
 * @brief Get Child Buffer Data, then reset buffer for next call back.
 * @return
 */
std::string Process::getBufferData()
{
    std::string data = "";
    if (m_read_buffer.size() > 0)
    {
        data.append(m_read_buffer.data());
    }
    return data;
}

/**
 * @brief Data Handler for incoming Data (From Child Process)
 */
void Process::waitingForData()
{  
    if (!m_session->m_is_process_running)
        return;       

    if (m_output_pipe.is_open())
    {
        // Reads from pipe (From Child Process) need to setup call back to method!                                           
        m_output_pipe.async_read_some(ba::buffer(m_read_buffer),
                                        boost::bind(&Process::handleRead, shared_from_this(),
                                            ba::placeholders::error,
                                            ba::placeholders::bytes_transferred));
    }
    else
    {
        std::cout << "Process waitingForData pipe is closed." << std::endl;
    }
}

/**
 * @brief Asyc Process Read Handler when data is received.
 * @param error
 */
void Process::handleRead(const bs::error_code& error, std::size_t bytes_transferred)
{
    if(error)
    {
        std::cout << "async_read process error: " 
                  << error.message() 
                  << error.value()
                  << " : " 
                  << bytes_transferred 
                  << std::endl;
        return;
    }
    

    // Deliver Data Back to the User Session.
    std::string pipe_data = std::move(getBufferData());
            
    // Deliver Pipe (Process Output back to Users Socket)
    if (m_session && pipe_data.size() > 0)
    {
        std::cout << "data: " << pipe_data << std::endl;
        m_session->deliver(pipe_data);
    }    

    // Restart Next Call Back
    if (isRunning() && m_session->m_is_process_running)
    {
        //waitingForData();
        return;
    }

    std::cout << "Process is no longer running. " << std::endl;
}

/**
 * @brief Deliver Message (Child Process)
 * @param msg
 */
void Process::deliver(const std::string &msg)
{
    std::cout << "Process deliver()" << std::endl;
    if (m_input_pipe.is_open())
    {
        ba::async_write(m_input_pipe, 
                ba::buffer(msg, msg.size()),
                boost::bind(&Process::handleWrite, 
                    shared_from_this(),
                    ba::placeholders::error));
    }    
    else
    {
        std::cout << "Process deliver pipe is closed." << std::endl;
    }
}

/**
 * @brief Pulls Input Data from User Session and Delivers to (Child Process)
 */
void Process::update()
{
    if (m_session)
    {
        //session_data_wptr
        std::cout << "Process Update()" << std::endl;
        std::string session_data = std::move(m_session->m_parsed_data);
        if (session_data.size() > 0)
        {
            deliver(session_data);    
        }
    }    
    else
    {
        std::cout << "Process Update: Session is no longer active." << std::endl;
    }
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
                  << error.value()
                  << std::endl;
    }        
}
