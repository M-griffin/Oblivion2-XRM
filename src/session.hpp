#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>
#include <vector>

#include "model-sys/config.hpp"
#include "sdl2_net/SDL_net.hpp"
#include "logging.hpp"

using Byte = uint8_t;
using ByteBuffer = std::vector<Byte>;

class Session {
public:
    Session(TCPsocket socket, const int nodeNumber, Config &config)
        : m_log(Logging::getInstance())
          , m_socket{socket}
          , m_nodeNumber{nodeNumber}
          , m_active{true}
          , m_config{config} {
    }

    // Non-copyable
    Session(const Session &) = delete;

    Session &operator=(const Session &) = delete;

    // Movable
    Session(Session &&other) noexcept
        : m_log(Logging::getInstance()),
          m_socket{other.m_socket},
          m_nodeNumber{other.m_nodeNumber},
          m_active{other.m_active},
          m_config{other.m_config} {
        other.m_socket = nullptr;
        other.m_nodeNumber = -1;
        other.m_active = false;
        other.m_config = getConfig();
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
            other.m_config = getConfig();
        }
        return *this;
    }

    ~Session() {
        m_log.log(Logging::LogLevel::Console, "~Session()");
        close();
    }

    bool isActive() const noexcept {
        return m_active;
    }

    void setInActive() {
        m_active = false;
    }

    TCPsocket getSocket() const noexcept {
        return m_socket;
    }

    int getNodeNumber() const noexcept {
        return m_nodeNumber;
    }

    Config &getConfig() {
        return m_config;
    }

    void setConfig(const Config &config) {
        m_config = config;
    }

    // Receives a message from the socket. Returns empty string if failed.
    /*
    std::string receive() {
        constexpr std::size_t BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        const int received = SDLNet_TCP_Recv(m_socket, buffer, BUFFER_SIZE - 1);
        if (received <= 0) {
            m_active = false;
            return std::string{};
        }
        buffer[received] = '\0';
        return std::string{buffer};
    }*/

    // Receives a message from the socket. Returns empty string if failed.
    ByteBuffer receive() {
        constexpr std::size_t BUFFER_SIZE = 512;

        ByteBuffer buffer;
        buffer.resize(BUFFER_SIZE);

        const int received = SDLNet_TCP_Recv(
            m_socket,
            buffer.data(),
            static_cast<int>(buffer.size())
        );

        if (received <= 0) {
            m_active = false;
            return {};
        }

        buffer.resize(received);
        return buffer;
    }


    // Sends a message. If send fails, marks session inactive.
    void send(const std::string &message, const bool isDisconnection = false) {
        if (!m_active) {
            return;
        }

        /*
        std::string outputBuffer = "";

        // On Output, We have internal UTF8 now, translate to CP437
        if(m_encoding == Encoding::ENCODE_CP437)
        {
            outputBuffer = Encoding::getInstance().utf8Decode(msg);
        }
        else
        {
            outputBuffer = msg;
        }*/

        const int length = static_cast<int>(message.length());
        const int sent = SDLNet_TCP_Send(m_socket, message.c_str(), length);
        if (sent < length || isDisconnection) {
            hangup();
        }
    }

    void send(const ByteBuffer &bytes) {
        if (!m_active) {
            return;
        }

        const int length = static_cast<int>(bytes.size());
        const int sent = SDLNet_TCP_Send(m_socket, bytes.data(), length);
        if (sent < length) {
            hangup();
        }
    }

    void close() noexcept {
        if (m_socket) {
            SDLNet_TCP_Close(m_socket);
            m_socket = nullptr;
        }
        m_active = false;
    }

    void hangup() {
        m_active = false;
    }

private:
    Logging &m_log;
    TCPsocket m_socket;
    int m_nodeNumber;
    bool m_active;
    Config &m_config;
};

#endif
