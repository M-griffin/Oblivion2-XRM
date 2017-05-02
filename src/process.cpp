#include "process.hpp"
#include "session_data.hpp"
#include "model-sys/structures.hpp"

// Windows only for Testing!
#include <windows.h>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>


#include <iostream>
#include <vector>
#include <string>
#include <thread>

namespace ba = boost::asio;
namespace bs = boost::system;

// Processes
#define TA_FAILED        0
#define TA_SUCCESS_CLEAN 1
#define TA_SUCCESS_KILL  2
#define TA_SUCCESS_16    3

#define OUTFD            1
#define INFD             0

#define FALSE            0
#define TRUE             1

BOOL CALLBACK terminateAppEnum(HWND hwnd, LPARAM lParam);

/**
 * @brief Terminate Application
 * @param hwnd
 * @param lParam
 */
BOOL CALLBACK terminateAppEnum(HWND hwnd, LPARAM lParam)
{
    DWORD dwID;

    GetWindowThreadProcessId(hwnd, &dwID) ;

    if(dwID == (DWORD)lParam)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0) ;
    }

    return TRUE;
}


Process::Process(session_data_ptr session, 
                 ba::io_service&  io_service, 
                 std::string      cmdline)
        : m_session(session)
        , m_input_handle(io_service)
        , m_output_handle(io_service)
        , m_command_line(cmdline)
{ 
    // Startup External Process
    createProcess();
}        

Process::~Process()
{ }


/**
 * @brief Test if where using Windows NT
 * @return 
 */
bool Process::isWinNT()  
{
    OSVERSIONINFO osv;
    osv.dwOSVersionInfoSize = sizeof(osv);
    GetVersionEx(&osv);
    return (osv.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

/**
 * @brief Kill Process once threads and sockets are killed.
 * @param dwPID
 * @param dwTimeout
 * @return 
 */
DWORD WINAPI Process::terminateApp(DWORD pid, DWORD timeout)
{
    HANDLE  proc;
    DWORD   result;

    // If we can't open the process with PROCESS_TERMINATE rights,
    // then we give up immediately.
    proc = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, FALSE, pid);
    if(!proc)
    {
        return TA_FAILED;
    }

    // TerminateAppEnum() posts WM_CLOSE to all windows whose PID
    // matches your process's.
    ::EnumWindows((WNDENUMPROC)terminateAppEnum, (LPARAM)pid);

    // Wait on the handle. If it signals, great. If it times out,
    // then you kill it.
    if(WaitForSingleObject(proc, timeout) != WAIT_OBJECT_0)
    {
        result = (TerminateProcess(proc, 0) ? TA_SUCCESS_KILL : TA_FAILED);    
    }
    else
    {
        result = TA_SUCCESS_CLEAN;
    }

    CloseHandle(proc);

    return result;
}


void Process::executeProcessLoop() 
{    
    unsigned long exit   = 0;  // process exit code
    unsigned long bread  = 0;  // bytes read
    unsigned long avail  = 0;  // bytes available

    const int RCVBUFSIZE = 4096;
    unsigned char buf[RCVBUFSIZE];
    unsigned char tmpbuf[RCVBUFSIZE];

    // While process is active!!
    while(1)
    {
        GetExitCodeProcess(m_process_info.hProcess, &exit);      //while the process is running
        if(exit != STILL_ACTIVE)
        {
            std::cout << "Exiting Process" << std::endl;

            CloseHandle(m_process_info.hThread);
            CloseHandle(m_process_info.hProcess);

            if (m_session)
            {
                // Reset the socket and read waiting data refresh.
                m_session->m_is_process_running = false;
            }
            break;
        }


        // Handle Naped Pipes for STDIO Output of protocols.
        PeekNamedPipe(m_read_stdout, buf, RCVBUFSIZE, &bread, &avail, NULL);        

        if (bread > 0)
        {
            memset(&buf, 0, sizeof(buf));
            memset(&tmpbuf, 0, sizeof(tmpbuf));

            ReadFile(m_read_stdout, tmpbuf, RCVBUFSIZE, &bread, NULL);  //read the stdout pipe

            if (strlen((const char *)tmpbuf) > 0)
            {
                std::cout << "buffer [" << tmpbuf << "] " <<std::endl;

                if (m_session)
                {
                    std::string buffer(reinterpret_cast<char*>(tmpbuf));
                    m_session->deliver(buffer);
                }
            }

            memset(&buf, 0, sizeof(buf));
            memset(&tmpbuf, 0, sizeof(tmpbuf));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    CloseHandle(m_new_stdin);
    CloseHandle(m_new_stdout);
    CloseHandle(m_read_stdout);
    CloseHandle(m_write_stdin);

    std::cout << "File Transfer Completed!" << std::endl;
}

/**
 * @brief Startup a Windows Specific External Process
 */    
bool Process::createProcess()    
{
    STARTUPINFO         startup_info;
    SECURITY_ATTRIBUTES security_attrib;
    SECURITY_DESCRIPTOR secutiry_descrip;
        
    if (isWinNT())        //initialize security descriptor (Windows NT)
    {
        InitializeSecurityDescriptor(&secutiry_descrip, SECURITY_DESCRIPTOR_REVISION);
        SetSecurityDescriptorDacl(&secutiry_descrip, true, NULL, false);
        security_attrib.lpSecurityDescriptor = &secutiry_descrip;
    }
    else
    {
        security_attrib.lpSecurityDescriptor = NULL;        
    }

    // Secutiry Attributes
    security_attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
    security_attrib.bInheritHandle = true;         


    // Create stdin pipe
    if (!CreatePipe(&m_new_stdin, &m_write_stdin, &security_attrib, 0))   
    {
        std::cout << "Error Creating STDIN Pipe" << std::endl;
        return false;
    }
    
    /*
    // Ensure the read handle to the pipe for stdout is not inherited.
    if (!SetHandleInformation(m_new_stdin, HANDLE_FLAG_INHERIT, 0)) {
        std::cout << "Error: m_new_stdin CreatePipe.\n";
        return -1;
    }*/
    
    if (!CreatePipe(&m_read_stdout, &m_new_stdout, &security_attrib, 0))
    {
        std::cout << "Error Creating STDOUT Pipe" << std::endl;
        CloseHandle(m_new_stdin);
        CloseHandle(m_write_stdin);
        return false;
    }
    
    /*
    // Ensure the read handle to the pipe for stdout is not inherited.
    if (!SetHandleInformation(m_read_stdout, HANDLE_FLAG_INHERIT, 0)) {
        std::cout << "Error: m_new_stdin CreatePipe.\n";
        return -1;
    }*/

    // Startupinfo for the process
    GetStartupInfo(&startup_info);    

    //The dwFlags member tells CreateProcess how to make the process.
    //STARTF_USESTDHANDLES validates the hStd* members. STARTF_USESHOWWINDOW
    //validates the wShowWindow member.
    //
    startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    startup_info.wShowWindow = SW_SHOW;
    startup_info.hStdOutput = m_new_stdout;
    startup_info.hStdError = m_new_stdout;
    startup_info.hStdInput = m_new_stdin;

    char app[] = "C:\\Windows\\System32\\cmd.exe"; // Program to execute on command line.

    //spawn the child process
    if (!CreateProcess(app, NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, 
        NULL, NULL, &startup_info, &m_process_info))
    {
        std::cout << "Error Creating Child Process" << std::endl;
        CloseHandle(m_new_stdin);
        CloseHandle(m_new_stdout);
        CloseHandle(m_read_stdout);
        CloseHandle(m_write_stdin);
        return false;
    }
    
    // Clear Screen on Process Start and show cursor.
    m_session->deliver("\x1b[?25h\x1b[1;1H\x1b[2J");
    
    // Assign Boost to Window Handles.
    //m_output_handle.assign(m_read_stdout);
    //m_input_handle.assign(m_write_stdin);
    
    // Execute Thread for File Transfer
    std::thread t([=] { executeProcessLoop(); });
    t.detach();
    
    return true;
}
    
/**
 * @brief Checks if the process is still running
 * @return
 */
bool Process::isRunning()
{
    unsigned long exit_code;
    GetExitCodeProcess(m_process_info.hProcess, &exit_code);
    
    return (exit_code == STILL_ACTIVE);
}

/**
 * @brief Data Handler for incoming Data (From Child Process)
 */
void Process::waitingForData()
{  
    if (!m_session->m_is_process_running)
        return;       

    // Reads from pipe (From Child Process) need to setup call back to method!                                        
    m_output_handle.async_read_some(ba::buffer(m_read_buffer),
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
    std::string pipe_data = std::move(m_read_buffer.data());
            
    // Deliver Pipe (Process Output back to Users Socket)
    if (m_session && pipe_data.size() > 0)
    {
        std::cout << "data: " << pipe_data << std::endl;
        m_session->deliver(pipe_data);
    }    

    // Restart Next Call Back
    if (isRunning() && m_session->m_is_process_running)
    {
        waitingForData();
        return;
    }
}

/**
 * @brief Deliver Message (Child Process)
 * @param msg
 */
void Process::deliver(const std::string &msg)
{
    std::cout << "Process deliver()" << std::endl;
    ba::async_write(m_input_handle, 
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
                  << error.value()
                  << std::endl;
    }        
}

/**
 * @brief Pulls Input Data from User Session and Delivers to (Child Process)
 */
void Process::update()
{
    if (m_session)
    {
        unsigned char buf[50] = {0};
        unsigned long bread = 0;
        std::string session_data = std::move(m_session->m_parsed_data);
        std::cout << "Process Update(): " << session_data << std::endl;
        
        strcat((char *)buf, session_data.c_str());
        
        if (buf[0] == 13)
            sprintf((char *)buf,"\r\n");
        
        if (session_data.size() > 0)
        {
            WriteFile(m_write_stdin, buf, strlen((char *)buf),&bread,NULL);
            m_session->deliver(session_data);    
        }
    }    
    else
    {
        std::cout << "Process Update: Session is no longer active." << std::endl;
    }
}

