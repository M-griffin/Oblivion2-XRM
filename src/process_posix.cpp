#include "process_posix.hpp"
#include "session_data.hpp"
#include "model-sys/structures.hpp"

#include <memory>

#include <unistd.h>

#ifdef TARGET_OS_MAC
#include <util.h>
#else
#include <pty.h>
#endif

#include <termios.h>
#include <csignal>
#include <cstring>

#include <iostream>
#include <vector>
#include <string>

//#include <thread>
#include <pthread.h>
#include <unistd.h>


ProcessPosix::ProcessPosix(session_data_ptr session, std::string cmdline)
    : ProcessBase(session, cmdline)
    , m_pty_file_desc(0)
    , m_proc_id(0)
    , m_args(new ThreadArguments())
{
    // Startup External Process
    createProcess();
}

ProcessPosix::~ProcessPosix()
{
    std::cout << "ProcessPosix" << std::endl;
}


/**
  * Startup the inital Terminal buffer
 */
void ProcessPosix::initTerminalOptions()
{
#ifdef STREAMSPTY
    if (really_stream)
        tcgetattr(ttyfd, &m_termbuf);
    else
# endif
        tcgetattr(m_pty_file_desc, &m_termbuf);

    m_termbuf_original = m_termbuf;
}

/**
 * @brief Setup the inital Terminal buffer
 */
void ProcessPosix::setTerminalBuffer()
{
    /*
     * Only make the necessary changes.
     */
    if (memcmp(&m_termbuf, &m_termbuf_original, sizeof(m_termbuf)))
# ifdef  STREAMSPTY
        if (really_stream)
            tcsetattr(ttyfd, TCSANOW, &m_termbuf);
        else
# endif
            tcsetattr(m_pty_file_desc, TCSANOW, &m_termbuf);
}

/**
 * @brief Setup the inital Terminal buffer
 */
void ProcessPosix::setTerminalOptions()
{

    if (memcmp(&m_termbuf, &m_termbuf_original, sizeof(m_termbuf)))
# ifdef  STREAMSPTY
        if (really_stream)
            tcsetattr(ttyfd, TCSANOW, &m_termbuf);
        else
# endif
            tcsetattr(m_pty_file_desc, TCSANOW, &m_termbuf);
}

/**
 * @brief Process Loop Thread (Pthread)
 */
extern "C" void *executeProcessLoop(void *args)
{
    int selret;
    fd_set rdfdset;

    // Setup Thread Arguments Passed.
    args_ptr thread_args = * reinterpret_cast<std::shared_ptr<ThreadArguments>*>(args);

    char character_buffer[2014] = {'\0'};

    do
    {
        FD_ZERO(&rdfdset);
        FD_SET(thread_args->m_pty_file_desc, &rdfdset);

        selret = select(thread_args->m_pty_file_desc + 1, &rdfdset, NULL, NULL, NULL);

        // Error / Lost Connection on
        if (selret <= 0)
        {
            thread_args->m_session->m_is_process_running = false;
            break;
        }


        if (FD_ISSET(thread_args->m_pty_file_desc, &rdfdset))
        {
            memset(&character_buffer, 0, 1024);
            selret = read(thread_args->m_pty_file_desc, character_buffer, 1023);

            if (selret <= 0)
            {
                thread_args->m_session->m_is_process_running = false;
                break;
            }

            if (thread_args->m_session)
            {
                std::string buffer(reinterpret_cast<char*>(character_buffer));
                thread_args->m_session->deliver(buffer);
            }
        }

        // Free CPU Usage from thread io/loop.
        //std::this_thread::sleep_for(std::chrono::milliseconds(20));
        usleep(20);

    }
    while (thread_args->m_session->m_is_process_running);

    thread_args->m_session->m_is_process_running = false;
    pthread_exit(nullptr);

    return nullptr;
}

/**
 * @brief Startup a Windows Specific External Process
 */
bool ProcessPosix::createProcess()
{
    char app[1024] = {0};
    strcpy((char *)app, m_command_line.c_str());
    std::cout << "cmdline: " << app << std::endl;

    char *argv_init[] = {NULL, NULL, NULL, NULL, NULL, NULL};
    argv_init[0] = strdup(m_command_line.c_str());

    // Start Fork
    m_proc_id = forkpty(&m_pty_file_desc, NULL, NULL, NULL);

    // Pid 0 Start Child Process.
    if (m_proc_id == 0)
    {
        std::cout << "Fork() Started Child Exit after Execution" << std::endl;

        setsid();
        tcsetpgrp(0, getpid());

        // exec shell, with correct argv and env
        execv(m_command_line.c_str(), argv_init);
        exit(1);
    }
    // Pid -1 is an error
    else if(m_proc_id == -1)
    {
        std::cout << "Fork() Error" << std::endl;
        m_session->m_is_process_running = false;
    }
    else if(m_proc_id > 0)
    {
        // Parent process.
        std::cout << "Fork() Started Parent Return" << std::endl;
    }

    // Setup Detected Screen Size.
    struct winsize ws;

    // Setup Term
    initTerminalOptions();

#ifdef TIOCSWINSZ
    memset(&ws, 0, sizeof(ws));
    ws.ws_col = m_session->m_telnet_state->getTermCols();
    ws.ws_row = m_session->m_telnet_state->getTermRows();
#ifndef TARGET_OS_MAC
    ioctl(m_pty_file_desc, TIOCSWINSZ, (char *)&ws);     // NOTE MAC not seeing this for PTY
#endif	
#endif

    // Clear Screen on Process Start and show cursor.
    m_session->deliver("\x1b[?25h\x1b[1;1H\x1b[2J");

    // Execute Thread for File Transfer
    //std::thread t([=] { executeProcessLoop(); });
    //t.detach();

    // Setup Arguments for Passing to Thread

    m_args->m_session = m_session;
    m_args->m_pty_file_desc = m_pty_file_desc;

    pthread_t t;
    pthread_create(&t, nullptr, &executeProcessLoop, reinterpret_cast<void *>(&m_args));

    return true;
}

/**
 * @brief Checks if the process is still running
 * @return
 */
bool ProcessPosix::isRunning()
{
    /*
    unsigned long exit_code;
    GetExitCodeProcess(m_process_info.hProcess, &exit_code);

    return (exit_code == STILL_ACTIVE);
    */
    return true;
}

/**
 * @brief Pulls Input Data from User Session and Delivers to (Child Process)
 */
void ProcessPosix::update()
{
    if (m_session)
    {
        std::string session_data = std::move(m_session->m_parsed_data);
        std::cout << "Process Update(): " << session_data << std::endl;

        if (session_data.size() > 0)
        {
            if(write(m_pty_file_desc, (char *)session_data.c_str(), session_data.size()) == -1)
            {
                // Error
            }
            //m_session->deliver(session_data);
        }
    }
    else
    {
        std::cout << "Process Update: Session is no longer active." << std::endl;
    }
}

/**
 * @brief Kill Process
 */
void ProcessPosix::terminate()
{

}
