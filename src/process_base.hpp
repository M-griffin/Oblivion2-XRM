#ifndef PROCESS_BASE_HPP
#define PROCESS_BASE_HPP

#include <memory>
#include <iostream>

class SessionData;
typedef std::shared_ptr<SessionData> session_data_ptr;

class ProcessBase
{
public:

    virtual ~ProcessBase()
    {
    }

    // Note this is called within the class, no need to make it virtual.
    //virtual void executeProcessLoop() = 0;

    virtual bool createProcess() = 0;
    virtual bool isRunning() = 0;
    virtual void update() = 0;
    virtual void terminate() = 0;

    ProcessBase(session_data_ptr session, std::string cmdline)
        : m_session(session)
        , m_command_line(cmdline)
    { }

    session_data_ptr    m_session;
    std::string         m_command_line;

};

typedef std::shared_ptr<ProcessBase> process_ptr;

#endif // PROCESS_BASE_HPP
