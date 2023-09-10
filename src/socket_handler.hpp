#ifndef __SOCKET_HANDLER__
#define __SOCKET_HANDLER__

#include "socket_state.hpp"
#include "io_service.hpp"

#include <iostream>
#include <vector>
#include <memory>

class Logging;

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

    explicit SocketHandler();
    ~SocketHandler();    

    std::string const SOCKET_TYPE_TELNET = "TELNET";

    // Socket Events, True if Data Available.
    int sendSocket(unsigned char *buf, Uint32 len);
    int recvSocket(char *message);
    int poll();
    std::string getIpAddress();

    // Telnet
    bool createTelnetAcceptor(const std::string &host, const int &port);
    socket_handler_ptr acceptTelnetConnection();

    std::string getSocketType() const;
    bool isActive() const;
    void setInactive();

    void close();

    void setSocketType(std::string type);
    void setSocketState(socket_state_ptr state);
    
    void disconnectUser();

private:

    Logging                        &m_log;

    std::vector<socket_state_ptr>   m_socket;
    std::string                     m_socket_type;
    bool                            m_is_active;

};

typedef std::shared_ptr<SocketHandler> socket_handler_ptr;

#endif // __SOCKET_HANDLER__
