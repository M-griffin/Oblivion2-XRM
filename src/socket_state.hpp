
#ifndef __MY_SOCKET__H
#define __MY_SOCKET__H

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "sdl2_net/SDL_net.hpp"

#include <iostream>
#include <memory>
#include <string>

class SocketHandler;
typedef std::shared_ptr<SocketHandler> socket_handler_ptr;

class Logging;

/**
 * @class SocketState
 * @author Michael Griffin
 * @date 11/12/2017
 * @file socket_state.hpp
 * @brief Base Socket State Template
 */
class SocketState
{
public:

    SocketState(const std::string &host, const int &port)
        : m_host(host)
        , m_port(port)
        , m_is_socket_active(true)
    { }

    virtual ~SocketState()
    {
        //std::cout << "SocketState Destructor" << std::endl;
    }

    virtual int sendSocket(unsigned char *message, Uint32 len) = 0;
    virtual int recvSocket(char *message) = 0;
    virtual socket_handler_ptr pollSocketAccepts() = 0;
    virtual void spawnSocket(TCPsocket socket) = 0;
    virtual int pollSocket() = 0;
    virtual bool onListen() = 0;
    virtual bool onExit() = 0;
    virtual bool disconnectUser() = 0;
    virtual std::string getIPAddress() =0;
    
    std::string m_host;
    int         m_port;
    bool        m_is_socket_active;
};

typedef std::shared_ptr<SocketState> socket_state_ptr;


/**
 * @class SDL_Socket
 * @author Blue
 * @date 11/12/2017
 * @file socket_state.hpp
 * @brief SDL / TCP Telnet Socket Template
 */
class SDL_Socket : public SocketState
{
public:

    SDL_Socket(const std::string &host, const int &port);

    virtual ~SDL_Socket()
    {
        //std::cout << "SDL_Socket Destructor" << std::endl;
    }

    virtual int sendSocket(unsigned char *message, Uint32 len) override;
    virtual int recvSocket(char *message) override;
    virtual socket_handler_ptr pollSocketAccepts() override; 
    virtual void spawnSocket(TCPsocket socket) override;
    virtual int pollSocket() override;
    virtual bool onListen() override;
    virtual bool onExit() override;
    virtual bool disconnectUser() override;
    virtual std::string getIPAddress() override;

private:

    Logging         &m_log;
    TCPsocket        m_tcp_socket;
    SDLNet_SocketSet m_socket_set;

};



#endif
