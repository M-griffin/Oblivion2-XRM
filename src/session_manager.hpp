#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include <iostream>
#include <string>
#include <set>

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

class Session;
typedef boost::shared_ptr<Session> session_ptr;

class SessionSSL;
typedef boost::shared_ptr<SessionSSL> session_ssl_ptr;

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
    explicit SessionManager()
    {
        std::cout << "SessionManager" << std::endl;
    }
    ~SessionManager();

    /**
     * @brief OverRides for Conencting TCP and SSL Sessions
     * @param Session
     */
    void join(session_ptr session);

    void leave(int node_number);
    void deliver(std::string msg);

    int connections();
    void shutdown();

private:

    std::set<session_ptr> m_sessions;
    std::set<session_ssl_ptr> m_sessions_ssl;
};

typedef boost::shared_ptr<SessionManager> session_manager_ptr;
typedef boost::weak_ptr<SessionManager>	  session_manager_wptr;

#endif // CHAT_ROOM_HPP
