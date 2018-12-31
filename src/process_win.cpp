#include "process_win.hpp"
#include "session_data.hpp"
#include "model-sys/structures.hpp"
#include "logging.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

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


ProcessWin::ProcessWin(session_data_ptr session, std::string cmdline)
    : ProcessBase(session, cmdline)
{
    // Startup External Process
    createProcess();
}

ProcessWin::~ProcessWin()
{ }


/**
 * @brief Test if where using Windows NT
 * @return
 */
bool ProcessWin::isWinNT()
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
DWORD WINAPI ProcessWin::terminateApp(DWORD pid, DWORD timeout)
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

/**
 * @brief Process Loop Thread
 */
void ProcessWin::executeProcessLoop()
{
    unsigned long exit   = 0;  // process exit code
    unsigned long bread  = 0;  // bytes read
    unsigned long avail  = 0;  // bytes available

    const int RCVBUFSIZE = 4096;
    unsigned char buf[RCVBUFSIZE];
    unsigned char tmpbuf[RCVBUFSIZE];

    while(1)
    {
        GetExitCodeProcess(m_process_info.hProcess, &exit);      //while the process is running

        if(exit != STILL_ACTIVE)
        {
            Logging *log = Logging::instance();
            log->xrmLog<Logging::CONSOLE_LOG>("exiting process");

            CloseHandle(m_process_info.hThread);
            CloseHandle(m_process_info.hProcess);

            if(m_session)
            {
                // Reset the socket and read waiting data refresh.
                m_session->m_is_process_running = false;
            }

            break;
        }


        // Handle Naped Pipes for STDIO Output of protocols.
        PeekNamedPipe(m_read_stdout, buf, RCVBUFSIZE, &bread, &avail, NULL);

        if(bread > 0)
        {
            memset(&buf, 0, sizeof(buf));
            memset(&tmpbuf, 0, sizeof(tmpbuf));
            ReadFile(m_read_stdout, tmpbuf, RCVBUFSIZE, &bread, NULL);  //read the stdout pipe

            if(strlen((const char *)tmpbuf) > 0)
            {
                if(m_session)
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
}

/**
 * @brief Startup a Windows Specific External Process
 */
bool ProcessWin::createProcess()
{
    STARTUPINFO         startup_info;
    SECURITY_ATTRIBUTES security_attrib;
    SECURITY_DESCRIPTOR secutiry_descrip;

    if(isWinNT())         //initialize security descriptor (Windows NT)
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
    Logging *log = Logging::instance();

    if(!CreatePipe(&m_new_stdin, &m_write_stdin, &security_attrib, 0))
    {
        log->xrmLog<Logging::ERROR_LOG>("Error Creating STDIN Pipe", __LINE__, __FILE__);
        return false;
    }

    if(!CreatePipe(&m_read_stdout, &m_new_stdout, &security_attrib, 0))
    {
        log->xrmLog<Logging::ERROR_LOG>("Error Creating STDOUT Pipe", __LINE__, __FILE__);
        CloseHandle(m_new_stdin);
        CloseHandle(m_write_stdin);
        return false;
    }

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

    char app[2048] = {0};
    strcpy((char *)app, m_command_line.c_str());
    log->xrmLog<Logging::DEBUG_LOG>("cmdline=", app, __LINE__, __FILE__);

    //spawn the child process
    if(!CreateProcess(app, NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
                      NULL, NULL, &startup_info, &m_process_info))
    {
        log->xrmLog<Logging::ERROR_LOG>("Error Creating Child Process", __LINE__, __FILE__);
        CloseHandle(m_new_stdin);
        CloseHandle(m_new_stdout);
        CloseHandle(m_read_stdout);
        CloseHandle(m_write_stdin);
        return false;
    }

    // Clear Screen on Process Start and show cursor.
    m_session->deliver("\x1b[?25h\x1b[1;1H\x1b[2J");

    // Execute Thread for File Transfer
    std::thread t([=] { executeProcessLoop(); });
    t.detach();

    return true;
}

/**
 * @brief Checks if the process is still running
 * @return
 */
bool ProcessWin::isRunning()
{
    unsigned long exit_code;
    GetExitCodeProcess(m_process_info.hProcess, &exit_code);

    return (exit_code == STILL_ACTIVE);
}

/**
 * @brief Pulls Input Data from User Session and Delivers to (Child Process)
 */
void ProcessWin::update()
{
    if(m_session)
    {
        unsigned char buf[50] = {0};
        unsigned long bread = 0;
        std::string session_data = std::move(m_session->m_parsed_data);

        strcat((char *)buf, session_data.c_str());

        if(buf[0] == 13)
            sprintf((char *)buf,"\r\n");

        if(session_data.size() > 0)
        {
            WriteFile(m_write_stdin, buf, strlen((char *)buf),&bread,NULL);
            m_session->deliver(session_data);
        }
    }
    else
    {
        Logging *log = Logging::instance();
        log->xrmLog<Logging::ERROR_LOG>("Process Update: Session is no longer active.", __LINE__, __FILE__);
    }
}

/**
 * @brief Kill Process
 */
void ProcessWin::terminate()
{

}
