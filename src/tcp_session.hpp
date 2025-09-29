#ifndef TCP_SESSION_HPP
#define TCP_SESSION_HPP

#include <utility>

#include "session.hpp"
#include "telnet_session.hpp"

#include "sdl2_net/SDL_net.hpp"

class TCPSession {
private:
    Session m_baseSession;
    TelnetSession m_telnetSession;
    Users m_userRec;

public:
    // Default constructor
    TCPSession(TCPsocket socket, const int nodeNumber, Config &config)
        : m_baseSession(socket, nodeNumber, config),
          m_telnetSession(m_baseSession) {
    }

    // Move constructor
    TCPSession(TCPSession &&other) noexcept
        : m_baseSession(std::move(other.m_baseSession)),
          m_telnetSession(std::move(other.m_telnetSession)) {
    }

    // Move assignment operator
    TCPSession &operator=(TCPSession &&other) noexcept {
        if (this != &other) {
            m_baseSession = std::move(other.m_baseSession);
            m_telnetSession = std::move(other.m_telnetSession);
        }
        return *this;
    }

    // Delete copy constructor and copy assignment operator
    TCPSession(const TCPSession &) = delete;

    TCPSession &operator=(const TCPSession &) = delete;

    // Accessors for the underlying sessions
    Session &getSession() { return m_baseSession; }
    TelnetSession &getTelnet() { return m_telnetSession; }

    // Telnet Session Accessors
    int getTermRows() const {
        return m_telnetSession.getTermRows();
    }
    int getTermCols() const {
        return m_telnetSession.getTermCols();
    }
    bool getUseAnsi() const {
        return m_telnetSession.getUseAnsi();
    }


    // User Accessors
    Users &getUserRec() {
        return m_userRec;
    }

    // Configuration Accessors
    Config &getConfig() {
        return m_baseSession.getConfig();
    }

    // Session Socket Accessors
    int getNodeNumber() const {
        return m_baseSession.getNodeNumber();
    }

    void hangup() {
        m_baseSession.hangup();
    }

    TCPsocket getSocket() const {
       return m_baseSession.getSocket();
    }

    void send(const std::string &value) {
        m_baseSession.send(value);
    }

    std::string receive() {
        return m_baseSession.receive();
    }

    void close() {
        return m_baseSession.close();
    }

    bool isActive() const {
        return m_baseSession.isActive();
    }
};

#endif
