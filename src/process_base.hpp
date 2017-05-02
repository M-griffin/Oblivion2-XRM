#ifndef PROCESS_BASE_HPP
#define PROCESS_BASE_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <iostream>

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

class ProcessBase
{
public:
    
    virtual ~ProcessBase()
    {
        std::cout << "~ProcessBase." << std::endl;
    }
    
    virtual void executeProcessLoop() = 0;
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

typedef boost::shared_ptr<ProcessBase> process_ptr;

#endif // PROCESS_BASE_HPP


