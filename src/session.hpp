#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>

#include "model-sys/config.hpp"
#include "sdl2_net/SDL_net.hpp"

class Session {
public:
    Session(TCPsocket socket, int nodeNumber, Config &config)
        : m_socket{socket}, m_nodeNumber{nodeNumber}, m_active{true}, m_config(config) {
    }

    // Non-copyable
    Session(const Session &) = delete;
    Session &operator=(const Session &) = delete;

    // Movable
    Session(Session &&other) noexcept
        : m_socket{other.m_socket},
          m_nodeNumber{other.m_nodeNumber},
          m_active{other.m_active},
          m_config{other.m_config}{
        other.m_socket = nullptr;
        other.m_nodeNumber = -1;
        other.m_active = false;
        //other.m_config =  nullptr;
    }

    Session &operator=(Session &&other) noexcept {
        if (this != &other) {
            close();
            m_socket = other.m_socket;
            m_nodeNumber = other.m_nodeNumber;
            m_active = other.m_active;
            m_config = other.getConfig();

            other.m_socket = nullptr;
            other.m_nodeNumber = -1;
            other.m_active = false;
            //other.m_config = nullptr;
        }
        return *this;
    }

    ~Session() {
        close();
    }

    bool isActive() const noexcept {
        return m_active;
    }

    TCPsocket getSocket() const noexcept {
        return m_socket;
    }

    int getNodeNumber() const noexcept {
        return m_nodeNumber;
    }

    Config &getConfig() const noexcept {
        return m_config;
    }

    void setConfig(const Config &config) const noexcept {
        m_config = config;
    }

    // Receives a message from the socket. Returns empty string if failed.
    std::string receive() {
        constexpr std::size_t BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        int received = SDLNet_TCP_Recv(m_socket, buffer, BUFFER_SIZE - 1);
        if (received <= 0) {
            m_active = false;
            return {};
        }
        buffer[received] = '\0';
        return std::string(buffer);
    }

    // Sends a message. If send fails, marks session inactive.
    void send(const std::string &message) {
        if (!m_active) return;
        int sent = SDLNet_TCP_Send(m_socket, message.c_str(), message.length());
        if (sent < static_cast<int>(message.length())) {
            m_active = false;
        }
    }

    void close() noexcept {
        if (m_socket) {
            SDLNet_TCP_Close(m_socket);
            m_socket = nullptr;
        }
        m_active = false;
    }

private:
    TCPsocket m_socket;
    int m_nodeNumber;
    bool m_active;

    Config &m_config;
};

#endif // SESSION_HPP
