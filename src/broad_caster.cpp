#include "broad_caster.hpp"
#include "session.hpp"
#include "session_data.hpp"
#include "server.hpp"

#include <algorithm>
#include <boost/bind.hpp>

BroadCaster::~BroadCaster()
{
    std::cout << "~BroadCaster" << std::endl;
}

/**
 * @brief Notifies that a user has joined the room
 * @param participant
 */
void BroadCaster::join(session_ptr session) //, session_data_ptr participant)
{
    std::cout << "join room" << std::endl;
    m_sessions.insert(session);
    //deliver("\r\nConnected to the room");
}

/**
 * @brief Notifies that a user has left the room
 * @param participant
 */
void BroadCaster::leave(int node_number)
{
    std::cout << "nleft room" << std::endl;
    deliver("\r\nleft the room");

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
void BroadCaster::deliver(std::string msg)
{
    if(msg.size() == 0)
        return;

    std::cout << "deliver room notices: " << msg << std::endl;
    std::for_each(m_sessions.begin(), m_sessions.end(),
                  boost::bind(&Session::deliver, _1, boost::ref(msg)));
}

/**
 * @brief Retrieve Number of users connected
 * Also helpful for determinging next node number.
 * @return
 */
int BroadCaster::connections()
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
void BroadCaster::shutdown()
{
    for(auto it = begin(m_sessions); it != end(m_sessions); ++it)
    {
        (*it)->m_tcp_connection->m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        (*it)->m_tcp_connection->m_socket.close();
        m_sessions.erase(it);
    }
}
