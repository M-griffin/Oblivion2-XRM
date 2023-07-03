#include "session_manager.hpp"

#include "state_manager.hpp"
#include "session.hpp"
#include "logging.hpp"

#include <functional>
#include <algorithm>
#include <string>
#include <vector>

SessionManager::SessionManager()
    : m_log(Logging::getInstance())
{
}

/**
 * @brief Notifies that a user has joined the room
 * @param participant
 */
void SessionManager::join(const session_ptr &session)
{
    m_sessions.insert(session);
}

/**
 * @brief Notifies that a user has left the room
 * @param participant
 */
void SessionManager::leave(const session_ptr &session)
{
    int node_number = session->m_node_number;
    m_log.write<Logging::CONSOLE_LOG>("SessionManager - disconnecting Node Session=", node_number);
    
    try 
    {
        // Clean Update The State Manager Prior to Removing the session!        
        //session->shutdownAsoi();
        session->m_state_manager->clean();        
        m_sessions.erase(session);
        
        m_log.write<Logging::CONSOLE_LOG>("SessionManager - disconnecting Node Session completed=", node_number);
    }
    catch (std::exception &ex)
    {
        m_log.write<Logging::ERROR_LOG>("SessionManager - disconnecting Exception=", ex.what(), node_number);
    }    
}

/**
 * @brief Sends message to all users in the current room.
 * @param participant
 */
void SessionManager::deliver(const std::string &msg)
{
    if(msg.size() == 0)
    {
        return;        
    }

    m_log.write<Logging::DEBUG_LOG>("SessionManager - deliver SessionManager notices=", msg);
    std::for_each(m_sessions.begin(), m_sessions.end(),
                  std::bind(&Session::deliver, std::placeholders::_1, std::ref(msg)));
}

/**
 * @brief Retrieve Number of users connected
 * Also helpful for determining next node number.
 * @return
 */
int SessionManager::connections()
{
    int count = 0;

    for(auto it = begin(m_sessions); it != end(m_sessions); ++it)
    {
        ++count;
    }

    return count;
}

/**
 * @brief Broadcaster Anchors the Sessions, we loop to shutdown all connections.
 * @return
 */
void SessionManager::shutdown()
{
    for(auto it = begin(m_sessions); it != end(m_sessions); ++it)
    {
        m_log.write<Logging::DEBUG_LOG>("shutting Down Nodes=", (*it)->m_node_number );
        {
            (*it)->logoff();
        }
        m_sessions.erase(it);
    }
}
