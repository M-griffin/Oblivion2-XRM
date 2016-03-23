#include "session_manager.hpp"
#include "session.hpp"
#include "server_ssl.hpp"
#include "session_data.hpp"
#include "server.hpp"

#include <algorithm>
#include <boost/bind.hpp>

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
    //deliver("\r\nConnected to the BroadCaster");
}


/**
 * @brief Notifies that a user has left the room
 * @param participant
 */
void SessionManager::leave(int node_number)
{
    std::cout << "nleft SessionManager" << std::endl;
    deliver("\r\nleft the SessionManager");

    // Clear Session
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
                  boost::bind(&Session::deliver, _1, boost::ref(msg)));
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
        if ((*it)->m_connection->m_is_secure)
        {
            (*it)->m_connection->m_secure_socket.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            (*it)->m_connection->m_secure_socket.lowest_layer().close();
        }
        else
        {
            (*it)->m_connection->m_normal_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            (*it)->m_connection->m_normal_socket.close();
        }

        m_sessions.erase(it);
    }
}
