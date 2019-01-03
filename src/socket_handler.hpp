#ifndef __SOCKET_HANDLER__
#define __SOCKET_HANDLER__

#include "socket_state.hpp"
#include "io_service.hpp"

#include <iostream>
#include <vector>
#include <memory>

/**
 * @class SocketHandler
 * @author Michael Griffin
 * @date 11/12/2017
 * @file socket_handler.hpp
 * @brief Handles Socket Calls per individual Session
 */
class SocketHandler
{
public:

    SocketHandler()
        : m_socket()
        , m_socket_type("")
        , m_is_active(false)
    {
    }

    ~SocketHandler()
    {
        std::vector<socket_state_ptr>().swap(m_socket);
    }

    std::string const SOCKET_TYPE_TELNET = "TELNET";
    std::string const SOCKET_TYPE_SSH = "SSH";
    std::string const SOCKET_TYPE_FTP = "FTP";

    // Socket Events, True if Data Available.
    int sendSocket(unsigned char *buf, Uint32 len);
    int recvSocket(char *message);
    int poll();

    // Telnet
    bool connectTelnetSocket(std::string host, int port);
    bool createTelnetAcceptor(std::string host, int port);
    socket_handler_ptr acceptTelnetConnection();

    // SSH
    bool connectSshSocket(std::string host, int port, std::string username, std::string password);

    // IRC
    bool connectIrcSocket(std::string host, int port);

    std::string getSocketType() const;
    bool isActive() const;
    void setInactive();

    void close();

    void setSocketType(std::string type);
    void setSocketState(socket_state_ptr state);

private:

    std::vector<socket_state_ptr>   m_socket;
    std::string                     m_socket_type;
    bool                            m_is_active;

};

typedef std::shared_ptr<SocketHandler> socket_handler_ptr;

#endif // __SOCKET_HANDLER__
