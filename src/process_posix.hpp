#ifndef PROCESS_POSIX_HPP
#define PROCESS_POSIX_HPP

#include "process_base.hpp"

#include <termios.h>

#include <vector>
#include <string>

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;


/**
 * @struct ThreadArguments
 * @date 28/05/2017
 * @brief pthread (Posix) Arguments passer
 */
typedef struct ThreadArguments {

    session_data_ptr m_session;
    int              m_pty_file_desc;

} ThreadArguments;

typedef boost::shared_ptr<ThreadArguments> args_ptr;



/**
 * @class ProcessPosix
 * @author Michael Griffin
 * @date 30/04/2017
 * @file process.hpp
 * @brief Process Posix class for piping data between user sessions and external programs.
 */
class ProcessPosix
    : public ProcessBase
{

public:
    ProcessPosix(session_data_ptr session, std::string cmdline);
    ~ProcessPosix();

    // pty control structure
    struct termios m_termbuf_original;
    struct termios m_termbuf;

    /**
     * @brief Startup the inital Terminal buffer
     */
    void initTerminalOptions();

    /**
     * @brief Setup the inital Terminal buffer
     */
    void setTerminalOptions();

    /**
     * @brief Setup the inital Terminal buffer
     */
    void setTerminalBuffer();

    /**
     * @brief Process Loop Thread (std::thread)
     */
    //void executeProcessLoop();

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
     * @brief Pulls Input Data from User Session and Delivers to (Child Process)
     */
    void update();

    /**
     * @brief Kill Process
     */
    void terminate();

    int      m_pty_file_desc;
    int      m_proc_id;

    args_ptr m_args;

};


#endif // PROCESS_HPP
