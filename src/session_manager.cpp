#include "session_manager.hpp"

#include <functional>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include "state_manager.hpp"
#include "async_io.hpp"
#include "session.hpp"
#include "logging.hpp"
#include "common_io.hpp"

SessionManager::SessionManager()
    : m_log(Logging::getInstance())
{
}

SessionManager::~SessionManager()
{
    m_log.write<Logging::DEBUG_LOG>("~SessionManager()");
    m_sessions.clear();
    std::set<session_ptr>().swap(m_sessions);
}
    

/**
 * @brief Retrieve next Free Node Number
 */
int SessionManager::getNodeNumber()
{
    // Lock for thread safety
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Start with node_number 1
    int node_check = 1;
    
    m_log.write<Logging::CONSOLE_LOG>("m_sessions.size()", m_sessions.size());
    
    // If there are sessions, find the first available node number
    if (m_sessions.size() > 0)
    {
        // Copy Node Numbers to a vector for sorting
        std::vector<int> node_array;
    
        for (session_ptr ptr : m_sessions)
        {
            node_array.push_back(ptr->m_node_number);
        }    
    
        // Sort node numbers in ascending order
        std::sort(node_array.begin(), node_array.end());
        
        // Find the first available node number
        for (int node_number : node_array) {
            if (node_check != node_number) {
                // Return the first unused node number
                return node_check;
            }
            node_check++;
        }
    }
    
    // If no sessions or no gaps in node numbers, return the next available node (node_check)
    return node_check;
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
    std::lock_guard<std::mutex> lock(m_mutex);
    int node_number = session->m_node_number;
    
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
                  std::bind(&Session::deliver, std::placeholders::_1, std::ref(msg), false));
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
    std::lock_guard<std::mutex> lock(m_mutex);
    m_log.write<Logging::CONSOLE_LOG>("SessionManager Shutdown! NumSessions=", m_sessions.size());
    
    // Loop and Disconnects Each Active Session
    std::for_each(m_sessions.begin(), m_sessions.end(), 
        [] (session_ptr p) 
        { 
            std::string msg = "\r\nService is shutting down, please try again later. \r\n";
            p->m_async_io->getSocketHandle()->sendSocket((unsigned char*)msg.c_str(), msg.size());
            p->disconnectUser();
        });
}
