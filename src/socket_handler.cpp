
#include "socket_handler.hpp"

#include "socket_state.hpp"
#include "logging.hpp"

#include <iostream>
#include <exception>


SocketHandler::SocketHandler()
    : m_log(Logging::getInstance())
    , m_socket()
    , m_socket_type("")
    , m_is_active(false)
{
}

SocketHandler::~SocketHandler()
{
    m_log.write<Logging::DEBUG_LOG>("~SocketHandler()");
    
    // Clean up the socket left overs.
    if (m_socket.size() > 0 && m_socket.back() != NULL) {
        m_socket.back()->onExit();
    }
    std::vector<socket_state_ptr>().swap(m_socket);
}

/**
 * @brief Send Socket Data
 * @param buffer
 * @param length
 * @return
 */
int SocketHandler::sendSocket(unsigned char *buffer, Uint32 length)
{
    return m_socket.back()->sendSocket(buffer, length);
}

/**
 * @brief Receive Waiting Socket Data
 * @param message
 * @return
 */
int SocketHandler::recvSocket(char *message)
{
    return m_socket.back()->recvSocket(message);
}

/**
 * @brief Poll if Socket has any data to retrieve
 * @return
 */
int SocketHandler::SocketHandler::poll()
{
    int ret = 0;

    if(m_is_active)
    {
        ret = m_socket.back()->pollSocket();

        if(ret == -1)
        {
            m_log.write<Logging::WARN_LOG>("Socket Closed by host, disconnecting.", __FILE__, __LINE__);

            // Shutdown Socket.
            m_socket.back()->onExit();
            m_is_active = false;
        }
    }
    else
    {
        // Inactive Connection
        m_log.write<Logging::WARN_LOG>("Shutdown received, Socket Closed.", __FILE__, __LINE__);
        ret = -1;
    }

    return ret;
}

/**
 * @brief Retrieve Ip Address of Remote Socket.
 * @return 
 */
std::string SocketHandler::getIpAddress()
{
    return m_socket.back()->getIPAddress();
}

/**
 * @brief Telnet Socket Listener
 * @param host
 * @param port
 * @return
 */
bool SocketHandler::createTelnetAcceptor(const std::string &host, const int &port)
{
    if(!m_is_active)
    {
        try
        {
            m_socket_type = SOCKET_TYPE_TELNET;            
            socket_state_ptr sdl_socket = std::make_shared<SDL_Socket>(host, port);
            m_socket.push_back(sdl_socket);

            if(m_socket.back()->onListen())
            {
                m_is_active = true;
            }
            else
            {
                m_log.write<Logging::ERROR_LOG>("Unable to initialize Telnet Socket.", __FILE__, __LINE__);
                close();
                return false;
            }
        }
        catch(std::exception& e)
        {
            close();
            m_log.write<Logging::ERROR_LOG>("Exception creating new SDL_Socket", e.what(), __FILE__, __LINE__);
            return false;
        }
    }
    else
    {
        m_log.write<Logging::ERROR_LOG>("Telnet Socket already Active", __FILE__, __LINE__);
        return false;
    }

    return true;
}

/**
 * @brief Check and return handler on Connections
 * @return
 */
socket_handler_ptr SocketHandler::acceptTelnetConnection()
{
    if (m_socket.size() > 0 && m_socket.back()->m_is_socket_active) {
        return m_socket.back()->pollSocketAccepts();
    }
    
    return nullptr;
}

/**
 * @brief Type of Socket
 * @return
 */
std::string SocketHandler::getSocketType() const
{
    return m_socket_type;
}

/**
 * @brief Check if Socket is Active
 * @return
 */
bool SocketHandler::isActive() const
{
    return m_is_active;
}

/**
 * @brief On Errors, set Inactive
 */
void SocketHandler::setInactive()
{
    m_is_active = false;
}

/**
 * @brief Socket Reset
 */
void SocketHandler::close()
{
    m_log.write<Logging::DEBUG_LOG>("SocketHandler closed()");
    try
    {
        // Deactivate Socket, then Clean it.
        if(m_socket.size() > 0 && m_socket.back())
        {
            m_socket.back()->onExit();
        }

        m_is_active = false;
        m_socket_type.erase();
        m_socket.pop_back();
        std::vector<socket_state_ptr>().swap(m_socket);
    }
    catch(std::exception& e)
    {
        m_log.write<Logging::ERROR_LOG>("Exception=", e.what(), __FILE__, __LINE__);
    }
}

/**
 * @brief Set Socket Type, eg. Telnet on Connection Accepts
 * @param state
 */
void SocketHandler::setSocketType(std::string type)
{
    m_socket_type = type;
}

/**
 * @brief Set Socket State, For creating unique standalone socket sessions
 * @param state
 */
void SocketHandler::setSocketState(socket_state_ptr state)
{
    m_socket.push_back(state);
    m_is_active = true;
}

/**
 * @brief Set Socket State, For creating unique standalone socket sessions
 * @param state
 */
void SocketHandler::disconnectUser()
{
    m_socket.back()->disconnectUser();
}

