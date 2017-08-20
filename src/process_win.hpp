#ifndef PROCESS_WIN_HPP
#define PROCESS_WIN_HPP

#include "process_base.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <vector>
#include <string>


class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

/**
 * @class ProcessWin
 * @author Michael Griffin
 * @date 30/04/2017
 * @file process.hpp
 * @brief Process Windows class for piping data between user sessions and external programs.
 */
class ProcessWin
    : public ProcessBase
{

public:
    ProcessWin(session_data_ptr session, std::string cmdline);
    ~ProcessWin();

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

    /**
     * @brief Process Loop Thread
     */
    void executeProcessLoop();

    /**
     * @brief Startup a Windows Specific External Process
     */
    virtual bool createProcess() override; 

    /**
     * @brief Checks if the process is still running
     * @return
     */
    virtual bool isRunning() override;

    /**
     * @brief Pulls Input Data from User Session and Delivers to (Child Process)
     */
    virtual void update() override;

    /**
     * @brief Kill Process
     */
    virtual void terminate() override;

    PROCESS_INFORMATION m_process_info;
    HANDLE              m_new_stdin;
    HANDLE              m_new_stdout;
    HANDLE              m_read_stdout;
    HANDLE              m_write_stdin;

};


#endif // PROCESS_HPP
