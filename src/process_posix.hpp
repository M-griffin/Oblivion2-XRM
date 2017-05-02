#ifndef PROCESS_POSIX_HPP
#define PROCESS_POSIX_HPP

#include "process_base.hpp"

#include <vector>
#include <string>

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

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

    /**
     * @brief Process Loop Thread
     */
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
     * @brief Pulls Input Data from User Session and Delivers to (Child Process)
     */
    void update();

    /**
     * @brief Kill Process
     */
    void terminate();


};


#endif // PROCESS_HPP
