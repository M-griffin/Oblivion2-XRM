#include "socket_state.hpp"
#include "socket_handler.hpp"
#include "communicator.hpp"
#include "logging.hpp"

#include <sdl2_net/SDL_net.hpp>

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <time.h>

#define MAX_BUFFER 16384

/**
 * @brief Send Data Over the Socket
 * @param buffer
 * @param length
 * @return
 */
int SDL_Socket::sendSocket(unsigned char *buffer, Uint32 length)
{
    int result = 0;

    if(m_is_socket_active)
    {
        result = SDLNet_TCP_Send(m_tcp_socket, buffer, length);

        if(result < (signed)strlen((char *)buffer))
        {
            if(SDLNet_GetError() && strlen(SDLNet_GetError()))
            {
                Logging *log = Logging::instance();
                log->xrmLog<Logging::ERROR_LOG>("SDLNet_TCP_Send=", SDLNet_GetError, __FILE__, __LINE__);
                return(0);
            }
        }
    }

    return(result);
}

/**
 * @brief Receive Data from Socket
 * @param message
 * @return
 */
int SDL_Socket::recvSocket(char *message)
{
    int result = -1;

    if(m_is_socket_active)
    {
        result = SDLNet_TCP_Recv(m_tcp_socket, message, MAX_BUFFER);

        if(result <= 0)
        {
            // -1 is Error 0 is Server Closed Connection
            return -1;
        }

        message[result] = 0;
    }

    return result;
}

/**
 * @brief Polls For Data To Read from the socket.
 * @return
 */
int SDL_Socket::pollSocket()
{
    int num_ready = -1;

    if(m_is_socket_active)
    {
        num_ready = SDLNet_CheckSockets(m_socket_set, 0);

        if(num_ready == -1)
        {
            m_is_socket_active = false;
            return num_ready;
        }

        if(num_ready && SDLNet_SocketReady(m_tcp_socket) > 0)
        {
            num_ready = 1;
        }
        else
        {
            num_ready = 0;
        }
    }

    return num_ready;
}

/**
 * @brief Polls For Data To Read from the socket.
 * @return
 */
socket_handler_ptr SDL_Socket::pollSocketAccepts()
{
    Logging *log = Logging::instance();
    TCPsocket socket = nullptr;

    int numActiveSockets = SDLNet_CheckSockets(m_socket_set, 0);

    if(numActiveSockets != 0)
    {
        log->xrmLog<Logging::DEBUG_LOG>("pollSocketAccepts=", numActiveSockets, "with data to be processed");
    }

    if(numActiveSockets > 0 && m_is_socket_active)
    {
        if(SDLNet_SocketReady(m_tcp_socket) != 0)
        {
            socket = SDLNet_TCP_Accept(m_tcp_socket);

            // Setup the State, SDL_Socket
            config_ptr config = TheCommunicator::instance()->getConfiguration();
            socket_state_ptr state(new SDL_Socket("127.0.0.1", config->port_telnet));
            state->spawnSocket(socket);

            // Setup a Handle, which will link back to Async_Connection
            // For individual sessions and polling read/write from clients.
            socket_handler_ptr handler(new SocketHandler());
            handler->setSocketType("TELNET");
            handler->setSocketState(state);

            log->xrmLog<Logging::DEBUG_LOG>("Return new SocketHandler TELNET");
            return handler;
        }
    }

    return nullptr;
}

/**
 * @brief (Server) On Socket Accepts, new Session Socket Setup.
 * @param socket
 */
void SDL_Socket::spawnSocket(TCPsocket socket)
{
    Logging *log = Logging::instance();
    m_tcp_socket = socket;
    m_socket_set = SDLNet_AllocSocketSet(1);

    if(!m_socket_set)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_AllocSocketSet=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return;
    }

    // Attached New Socket from Accept to it's own session instance.
    if(SDLNet_TCP_AddSocket(m_socket_set, m_tcp_socket) == -1)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_TCP_AddSocket=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return;
    }

    // Successful Startup
    m_is_socket_active = true;
    log->xrmLog<Logging::DEBUG_LOG>("Connection Spawn Successful");
}

/**
 * @brief (Client) Connects out to Servers
 * @return
 */
bool SDL_Socket::onConnect()
{
    Logging *log = Logging::instance();
    IPaddress ip;
    m_socket_set = SDLNet_AllocSocketSet(1);

    if(!m_socket_set)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_AllocSocketSet=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return false;
    }

    /* Resolve the argument into an IP address type */
    log->xrmLog<Logging::CONSOLE_LOG>("Client Connecting=", m_host, "port=", m_port);

    if(SDLNet_ResolveHost(&ip, m_host.c_str(), m_port) == -1)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_ResolveHost=", SDLNet_GetError, __FILE__, __LINE__);
        m_tcp_socket = nullptr;
        onExit();
        return false;
    }

    m_tcp_socket = SDLNet_TCP_Open(&ip);

    if(!m_tcp_socket)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_TCP_Open=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return false;
    }

    if(SDLNet_TCP_AddSocket(m_socket_set, m_tcp_socket) == -1)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_TCP_AddSocket=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return false;
    }

    // Successful Startup
    m_is_socket_active = true;
    log->xrmLog<Logging::DEBUG_LOG>("Connection Successful");

    return true;
}

/**
 * @brief (Server) Sets up the Listening Socket for New Conenction Polling.
 * @return
 */
bool SDL_Socket::onListen()
{
    Logging *log = Logging::instance();
    IPaddress ip;
    m_socket_set = SDLNet_AllocSocketSet(1);

    if(!m_socket_set)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_AllocSocketSet=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return false;
    }

    /* Resolve the argument into an IP address type */
    log->xrmLog<Logging::CONSOLE_LOG>("Listening for Telnet on port=", m_port);

    if(SDLNet_ResolveHost(&ip, NULL, m_port) == -1)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_ResolveHost=", SDLNet_GetError, __FILE__, __LINE__);
        m_tcp_socket = nullptr;
        onExit();
        return false;
    }

    m_tcp_socket = SDLNet_TCP_Open(&ip);

    if(!m_tcp_socket)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_TCP_Open=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return false;
    }

    if(SDLNet_TCP_AddSocket(m_socket_set, m_tcp_socket) == -1)
    {
        log->xrmLog<Logging::ERROR_LOG>("SDLNet_TCP_AddSocket=", SDLNet_GetError, __FILE__, __LINE__);
        onExit();
        return false;
    }

    // Successful Startup
    m_is_socket_active = true;
    return true;
}

/**
 * @brief Shutdown Socket.
 * @return
 */
bool SDL_Socket::onExit()
{
    if(m_tcp_socket)
    {
        SDLNet_TCP_Close(m_tcp_socket);
    }

    m_tcp_socket = nullptr;

    if(m_socket_set)
    {
        SDLNet_FreeSocketSet(m_socket_set);
    }

    m_socket_set = nullptr;
    return true;
}
