#include "session_manager.hpp"
#include "session.hpp"

#include <functional>
#include <algorithm>


SessionManager::~SessionManager()
{
    std::cout << "~SessionManager" << std::endl;
}

/**
 * @brief Notifies that a user has joined the room
 * @param participant
 */
void SessionManager::join(session_ptr session)
{
    std::cout << "joined SessionManager" << std::endl;
    m_sessions.insert(session);
}

/**
 * @brief Notifies that a user has left the room
 * @param participant
 */
void SessionManager::leave(int node_number)
{
    std::cout << "disconnecting Node Session: " << node_number << std::endl;
    for(auto it = m_sessions.begin(); it != m_sessions.end(); it++)
    {
        if((*it)->m_session_data->m_node_number == node_number)
        {
            m_sessions.erase(it);
            std::cout << "disconnecting Session completed." << std::endl;
            break;
        }
    }
}

/**
 * @brief Sends message to all users in the current room.
 * @param participant
 */
void SessionManager::deliver(std::string msg)
{
    if(msg.size() == 0)
        return;

    std::cout << "deliver SessionManager notices: " << msg << std::endl;
    std::for_each(m_sessions.begin(), m_sessions.end(),
                  std::bind(&Session::deliver, std::placeholders::_1, std::ref(msg)));
}

/**
 * @brief Retrieve Number of users connected
 * Also helpful for determinging next node number.
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
 * @brief Broacaster Anchors the Sessions, we loop to shutdown all connections.
 * @return
 */
void SessionManager::shutdown()
{
    for(auto it = begin(m_sessions); it != end(m_sessions); ++it)
    {
        (*it)->m_connection->shutdown();
        m_sessions.erase(it);
    }
}
