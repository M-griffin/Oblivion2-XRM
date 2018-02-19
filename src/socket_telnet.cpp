#include "socket_state.hpp"
#include "socket_handler.hpp"

#include <SDL2_net/SDL_net.hpp>

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <time.h>


/**
 * @brief Send Data Over the Socket
 * @param buffer
 * @param length
 * @return 
 */
int SDL_Socket::sendSocket(unsigned char *buffer, Uint32 length)
{
    int result = 0;
    if (m_is_socket_active)
    {
        result = SDLNet_TCP_Send(m_tcp_socket, buffer, length);
        if(result < (signed)strlen((char *)buffer))
        {
            if(SDLNet_GetError() && strlen(SDLNet_GetError()))
            {
                std::cout << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
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
    if (m_is_socket_active)
    {
        result = SDLNet_TCP_Recv(m_tcp_socket, message, 8192);
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
    if (m_is_socket_active)
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
TCPsocket SDL_Socket::pollTCPSocketAccepts()
{
    TCPsocket socket = nullptr;
    if (m_is_socket_active)
    {        
        if(SDLNet_SocketReady(m_tcp_socket)) 
        {
            socket = SDLNet_TCP_Accept(m_tcp_socket);       
        }
    }

    return socket;
}

/**
 * @brief (Server) On Socket Accepts, new Session Socket Setup.
 * @param socket
 */
void SDL_Socket::spawnSocket(TCPsocket socket)
{
    
    m_socket_set = SDLNet_AllocSocketSet(1);
    if(!m_socket_set)
    {
        std::cout << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << std::endl;
        onExit();
        return;
    }
    
    // Attached New Socket from Accept to it's own session instance.
    if(SDLNet_TCP_AddSocket(m_socket_set, socket) == -1)
    {
        std::cout << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << std::endl;
        onExit();
        return;
    }

    // Successful Startup
    m_is_socket_active = true;
    std::cout << "Connection Spawn Successful" << std::endl;
}

/**
 * @brief (Client) Connects out to Servers
 * @return 
 */
bool SDL_Socket::onConnect()
{
    IPaddress ip;

    m_socket_set = SDLNet_AllocSocketSet(1);
    if(!m_socket_set)
    {
        std::cout << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << std::endl;
        onExit();
        return false;
    }

    /* Resolve the argument into an IP address type */
    std::cout << "Connecting to " << m_host.c_str() << " on port " << m_port << std::endl;
    if(SDLNet_ResolveHost(&ip, m_host.c_str(), m_port) == -1)
    {
        std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        m_tcp_socket = nullptr;
        onExit();
        return false;
    }

    m_tcp_socket = SDLNet_TCP_Open(&ip);
    if(!m_tcp_socket)
    {
        std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        onExit();
        return false;
    }

    if(SDLNet_TCP_AddSocket(m_socket_set, m_tcp_socket) == -1)
    {
        std::cout << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << std::endl;
        onExit();
        return false;
    }

    // Successful Startup
    m_is_socket_active = true;
    std::cout << "Connection Successful" << std::endl;

    return true;
}

/**
 * @brief (Server) Sets up the Listening Socket for New Conenction Polling.
 * @return 
 */
bool SDL_Socket::onListen()
{
    IPaddress ip;

    m_socket_set = SDLNet_AllocSocketSet(1);
    if(!m_socket_set)
    {
        std::cout << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << std::endl;
        onExit();
        return false;
    }

    /* Resolve the argument into an IP address type */
    std::cout << "Listening for Telnet on port " << m_port << std::endl;
    if(SDLNet_ResolveHost(&ip, nullptr, m_port) == -1)
    {
        std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        m_tcp_socket = nullptr;
        onExit();
        return false;
    }

    m_tcp_socket = SDLNet_TCP_Open(&ip);
    if(!m_tcp_socket)
    {
        std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        onExit();
        return false;
    }

    if(SDLNet_TCP_AddSocket(m_socket_set, m_tcp_socket) == -1)
    {
        std::cout << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << std::endl;
        onExit();
        return false;
    }

    // Successful Startup
    m_is_socket_active = true;
    std::cout << "Handshake Setup Successful" << std::endl;    
    return true;
}


/**
 * @brief Shutdown Socket.
 * @return 
 */
bool SDL_Socket::onExit()
{
    std::cout << "SDL_Socket::onExit()" << std::endl;
    /*  double check global shutdown?!?
    if(TheInputHandler::Instance()->isGlobalShutdown())
     */
    {
        if (m_tcp_socket)
        {
            SDLNet_TCP_Close(m_tcp_socket);
        }
        m_tcp_socket = nullptr;
    }

    if (m_socket_set)
    {
        SDLNet_FreeSocketSet(m_socket_set);
    }

    m_socket_set = nullptr;
    return true;
}
