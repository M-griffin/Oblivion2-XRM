
#ifndef __MY_SOCKET__H
#define __MY_SOCKET__H

#ifdef _WIN32
#include <winsock2.h>
#endif

#include <sdl2_net/SDL_net.hpp>
#include <libssh/libssh.h>

#include <iostream>
#include <memory>
#include <string>

class SocketHandler;
typedef std::shared_ptr<SocketHandler> socket_handler_ptr;

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
        , m_is_socket_active(false)
    { }

    virtual ~SocketState()
    {
        std::cout << "~SocketState()" << std::endl;
    }

    virtual int sendSocket(unsigned char *message, Uint32 len) = 0;
    virtual int recvSocket(char *message) = 0;
    virtual socket_handler_ptr pollSocketAccepts() = 0;
    virtual void spawnSocket(TCPsocket socket) = 0;
    virtual int pollSocket() = 0;
    //virtual bool onConnect() = 0;
    virtual bool onListen() = 0;
    //virtual void closeSocket() =0;
    virtual bool onExit() = 0;

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

    SDL_Socket(const std::string &host, const int &port)
        : SocketState(host, port)
        , m_tcp_socket(nullptr)
        , m_socket_set(nullptr)
    { }

    virtual ~SDL_Socket()
    {
        std::cout << "~SDL_Socket()" << std::endl;
    }

    virtual int sendSocket(unsigned char *message, Uint32 len) override;
    virtual int recvSocket(char *message) override;
    virtual socket_handler_ptr pollSocketAccepts() override; 
    virtual void spawnSocket(TCPsocket socket) override;
    virtual int pollSocket() override;
    //virtual bool onConnect() override;
    virtual bool onListen() override;
    //virtual void closeSocket() override;
    virtual bool onExit() override;

private:

    TCPsocket        m_tcp_socket;
    SDLNet_SocketSet m_socket_set;

};

/**
 * @class SSH_Socket
 * @author Michael Griffin
 * @date 11/12/2017
 * @file socket_state.hpp
 * @brief SSH Socket Template
 */
class SSH_Socket : public SocketState
{
public:

    SSH_Socket(const std::string &host, const int &port,
               const std::string &user, const std::string &pass)
        : SocketState(host, port)
        , m_user_id(user)
        , m_password(pass)
        , m_ssh_channel(nullptr)
        , m_ssh_session(nullptr)
    { }

    virtual  ~SSH_Socket()
    {
        std::cout << "~SSH_Socket()" << std::endl;
    }

    virtual int sendSocket(unsigned char *message, Uint32 len) override;
    virtual int recvSocket(char *message) override;
    virtual socket_handler_ptr pollSocketAccepts() override;
    virtual void spawnSocket(TCPsocket socket) override;
    virtual int pollSocket() override;
    //virtual bool onConnect() override;
    virtual bool onListen() override;
    //virtual void closeSocket() override;
    virtual bool onExit() override;

    // Specific Functions for SSH
    int verify_knownhost();
    int authenticate_console();
    int authenticate_kbdint();

private:

    std::string m_user_id;
    std::string m_password;

    ssh_channel m_ssh_channel;
    ssh_session m_ssh_session;

};


#endif
