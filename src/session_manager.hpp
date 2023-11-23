#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <memory>
#include <iostream>
#include <string>
#include <set>
#include <mutex>

class Logging;

class CommonIO;
typedef std::shared_ptr<CommonIO> common_io_ptr;

class Session;
typedef std::shared_ptr<Session> session_ptr;

/**
 * @class SessionManager
 * @author Michael Griffin
 * @date 15/08/2015
 * @file session_manager.hpp
 * @brief Main Channel of Communications between Sessions
 * Also Very Import, keeps handle on Active Sessions and keeps smart pointers alive!
 */
class SessionManager
{
public:
    
    explicit SessionManager();
    
    ~SessionManager();    

    /**
     * @brief OverRides for Connecting TCP and SSL Sessions
     * @param Session
     */
    void join(session_ptr session);

    void leave(session_ptr session);
    void deliver(const std::string &msg);

    int connections();
    void shutdown();

private:

    Logging               &m_log;
    std::set<session_ptr>  m_sessions;
    std::mutex             m_mutex;
    
};

typedef std::shared_ptr<SessionManager> session_manager_ptr;
typedef std::weak_ptr<SessionManager> session_manager_wptr;

#endif // CHAT_ROOM_HPP
