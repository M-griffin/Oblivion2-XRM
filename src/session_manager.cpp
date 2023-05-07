#include "session_manager.hpp"
#include "session.hpp"
#include "logging.hpp"

#include <functional>
#include <algorithm>


SessionManager::~SessionManager()
{
    std::cout << "~SessionManager()" << std::endl;
}

/**
 * @brief Notifies that a user has joined the room
 * @param participant
 */
void SessionManager::join(session_ptr session)
{
    m_sessions.insert(session);
}

/**
 * @brief Notifies that a user has left the room
 * @param participant
 */
void SessionManager::leave(session_ptr session)
{
    Logging *log = Logging::instance();
    int node_number = session->m_node_number;
    log->write<Logging::CONSOLE_LOG>("disconnecting Node Session=", node_number);
    
    async_io_ptr async_io = session->m_async_io.lock();

    if (async_io && async_io->isActive())
    {
        log->write<Logging::CONSOLE_LOG>("Shutdown ASIO=", node_number);
        async_io->shutdown();                             
    }
    
    m_sessions.erase(session);
    log->write<Logging::CONSOLE_LOG>("disconnecting Node Session completed=", node_number);
}

/**
 * @brief Sends message to all users in the current room.
 * @param participant
 */
void SessionManager::deliver(std::string msg)
{
    if(msg.size() == 0)
        return;

    Logging *log = Logging::instance();
    log->write<Logging::DEBUG_LOG>("deliver SessionManager notices=", msg);
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
    Logging *log = Logging::instance();
    for(auto it = begin(m_sessions); it != end(m_sessions); ++it)
    {
        log->write<Logging::DEBUG_LOG>("shutting Down Nodes=", (*it)->m_node_number );
        {
            async_io_ptr async_io = (*it)->m_async_io.lock();
            async_io->shutdown();            
        }
        m_sessions.erase(it);
    }
}
