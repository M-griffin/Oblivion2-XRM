
#include "socket_handler.hpp"
#include "socket_state.hpp"
#include "logging.hpp"

#include <iostream>
#include <exception>

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
int SocketHandler::poll()
{
    Logging *log = Logging::instance();

    int ret = 0;

    if(m_is_active)
    {
        ret = m_socket.back()->pollSocket();

        if(ret == -1)
        {
            log->xrmLog<Logging::ERROR_LOG>("Socket Closed by host, disconnecting.", __FILE__, __LINE__);

            // Shutdown Socket.
            m_socket.back()->onExit();
            m_is_active = false;
        }
        else if(ret == 0)  // No Data!
        {

        }
        else
        {
        }
    }
    else
    {
        // Inactive Connection
        log->xrmLog<Logging::ERROR_LOG>("Showdown received, Socket Closed.", __FILE__, __LINE__);
        ret = -1;
    }

    return ret;
}

/**
 * @brief Connect Telnet Socket
 * @param host
 * @param port
 * @return
 */
bool SocketHandler::connectTelnetSocket(std::string host, int port)
{
    Logging *log = Logging::instance();

    if(!m_is_active)
    {
        try
        {
            m_socket_type = SOCKET_TYPE_TELNET;
            socket_state_ptr sdl_socket(new SDL_Socket(host, port));
            m_socket.push_back(sdl_socket);

            if(m_socket.back()->onConnect())
            {
                m_is_active = true;
            }
            else
            {
                log->xrmLog<Logging::ERROR_LOG>("Unable to initialize Telnet Socket.", __FILE__, __LINE__);
                close();
                return false;
            }
        }
        catch(std::exception& e)
        {
            log->xrmLog<Logging::ERROR_LOG>("Exception creating new SDL_Socket.", e.what(), __FILE__, __LINE__);
            close();
            return false;
        }
    }
    else
    {
        log->xrmLog<Logging::ERROR_LOG>("Telnet Socket already Active in Use!", __FILE__, __LINE__);
        return false;
    }

    return true;
}

/**
 * @brief Create SSH Socket
 * @param host
 * @param port
 * @param username
 * @param password
 * @return
 */
bool SocketHandler::connectSshSocket(std::string host, int port,
                                     std::string username, std::string password)
{
    Logging *log = Logging::instance();

    if(!m_is_active)
    {
        try
        {
            m_socket_type = SOCKET_TYPE_SSH;
            socket_state_ptr ssh_socket(new SSH_Socket(host, port, username, password));
            m_socket.push_back(ssh_socket);

            if(m_socket.back()->onConnect())
            {
                m_is_active = true;
            }
            else
            {
                log->xrmLog<Logging::ERROR_LOG>("Unable to initialize SSH Socket.", __FILE__, __LINE__);
                close();
                return false;
            }
        }
        catch(std::exception& e)
        {
            log->xrmLog<Logging::ERROR_LOG>("Exception creating SSH_Socket=", e.what(), __FILE__, __LINE__);
            m_is_active = false;
            close();
            return false;
        }
    }
    else
    {
        log->xrmLog<Logging::ERROR_LOG>("SSH Socket already Active!", __FILE__, __LINE__);
        return false;
    }

    return true;
}

/**
 * @brief Connect IRC Socket
 * @param host
 * @param port
 * @return
 *
 // TODO, this will need nick etc..
bool SocketHandler::connectIrcSocket(std::string host, int port)
{
    std::cout << "SocketHandler::initIRC" << std::endl;
    if(!m_is_active)
    {
        try
        {
            m_socket_type = SOCKET_TYPE_TELNET;
            socket_state_ptr sdl_socket(new IRC_Socket(host, port));
            m_socket.push_back(sdl_socket);
            if(m_socket.back()->onEnter())
            {
                m_is_active = true;
            }
            else
            {
                std::cout << "Unable to initialize IRC Socket." << std::endl;
                close();
                return false;
            }
        }
        catch(std::exception& e)
        {
            close();
            std::cerr << "exception creating new IRC_Socket: "
                      << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "IRC Socket already Active!" << std::endl;
        return false;
    }
    return true;
}
*/

/**
 * @brief Telnet Socket Listener
 * @param host
 * @param port
 * @return
 */
bool SocketHandler::createTelnetAcceptor(std::string host, int port)
{
    Logging *log = Logging::instance();

    if(!m_is_active)
    {
        try
        {
            m_socket_type = SOCKET_TYPE_TELNET;
            socket_state_ptr sdl_socket(new SDL_Socket(host, port));
            m_socket.push_back(sdl_socket);

            if(m_socket.back()->onListen())
            {
                m_is_active = true;
            }
            else
            {
                log->xrmLog<Logging::ERROR_LOG>("Unable to initialize Telnet Socket.", __FILE__, __LINE__);
                close();
                return false;
            }
        }
        catch(std::exception& e)
        {
            close();
            log->xrmLog<Logging::ERROR_LOG>("Exception creating new SDL_Socket", e.what(), __FILE__, __LINE__);
            return false;
        }
    }
    else
    {
        log->xrmLog<Logging::ERROR_LOG>("Telnet Socket already Active", __FILE__, __LINE__);
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
    return m_socket.back()->pollSocketAccepts();
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
        Logging *log = Logging::instance();
        log->xrmLog<Logging::ERROR_LOG>("Exception=", e.what(), __FILE__, __LINE__);
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
