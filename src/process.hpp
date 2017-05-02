#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <windows.h>

#include <vector>
#include <string>

namespace ba = boost::asio;
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
     * @brief Test if where using Windows NT
     * @return 
     */
    bool isWinNT();
        
    /**
     * @brief Kill Process once threads and sockets are killed.
     * @param dwPID
     * @param dwTimeout
     * @return 
     */
    DWORD WINAPI terminateApp(DWORD pid, DWORD timeout);

    void executeProcessLoop();

    /**
     * @brief Startup a Windows Specific External Process
     */    
    bool createProcess();
        
    /**
     * @brief Checks if the process is still running
     * @return
     */
    bool isRunning();
    
    
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
     * @brief Pulls Input Data from User Session and Delivers to (Child Process)
     */
    void update();
    
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
        
    }
    
    session_data_ptr    m_session;    
    boost::asio::windows::stream_handle m_input_handle;
    boost::asio::windows::stream_handle m_output_handle;
    std::string         m_command_line;
        
    PROCESS_INFORMATION m_process_info;
    HANDLE              m_new_stdin;
    HANDLE              m_new_stdout;
    HANDLE              m_read_stdout;
    HANDLE              m_write_stdin;
    std::vector<char>   m_read_buffer;     

};

typedef boost::shared_ptr<Process> process_ptr;

#endif // PROCESS_HPP
