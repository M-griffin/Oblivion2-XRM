#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>
#include <vector>

#include "model-sys/config.hpp"
#include "sdl2_net/SDL_net.hpp"
#include "util_log.hpp"

using Byte = uint8_t;
using ByteBuffer = std::vector<Byte>;

class TcpSocketBase {
    UtilLog &m_log;
    TCPsocket m_socket;
    int m_nodeNumber;
    bool m_active;
    Config &m_config;

    ByteBuffer m_pendingBytes;

public:
    TcpSocketBase(TCPsocket socket, const int nodeNumber, Config &config)
        : m_log(UtilLog::getInstance())
          , m_socket(socket)
          , m_nodeNumber(nodeNumber)
          , m_active(true)
          , m_config(config) {
    }

    TcpSocketBase(const TcpSocketBase &) = delete;
    TcpSocketBase &operator=(const TcpSocketBase &) = delete;
    TcpSocketBase(TcpSocketBase &&other) = delete;
    TcpSocketBase &operator=(TcpSocketBase &&other) = delete;

    ~TcpSocketBase() {
        m_log.log(UtilLog::LogLevel::Console, "~Session()");
        close();
    }

    bool isActive() const noexcept {
        return m_active;
    }

    // Mark inactive only (NO socket close here)
    void hangup() {
        if (!m_active) {
            return;
        }
        m_log.log(UtilLog::LogLevel::Console, "Session hangup requested");
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

    // --------------------------------------------------
    // I/O

    ByteBuffer receive() {
        if (!m_active || !m_socket) {
            return {};
        }

        constexpr std::size_t BUFFER_SIZE = 512;
        ByteBuffer buffer(BUFFER_SIZE);

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

    void send(const std::string &message, bool doDisconnection = false) {
        if (!m_active || !m_socket) {
            return;
        }

        const int length = static_cast<int>(message.size());
        size_t totalSent = 0;
        while (totalSent < length) {
            const int sent = SDLNet_TCP_Send(m_socket, message.data(), length);
            if (sent <= 0) {
                m_active = false;
                break;
            }
            totalSent += sent;
        }

        // ex. logoff w/ ansi, display then disconnect user
        if (doDisconnection) {
            m_active = false;
        }
    }

    void send(const ByteBuffer &bytes, bool doDisconnection = false) {
        if (!m_active || !m_socket) {
            return;
        }

        const int length = static_cast<int>(bytes.size());
        size_t totalSent = 0;
        while (totalSent < length) {
            const int sent = SDLNet_TCP_Send(m_socket, bytes.data(), length);
            if (sent <= 0) {
                m_active = false;
                break;
            }
            totalSent += sent;
        }

        // ex. logoff w/ ansi, display then disconnect user
        if (doDisconnection) {
            m_active = false;
        }
    }

    // --------------------------------------------------
    // Final close (ONLY server should call this)
    // --------------------------------------------------

    void close() noexcept {
        if (m_socket) {
            SDLNet_TCP_Close(m_socket);
            m_socket = nullptr;
        }
        m_active = false;
    }
};

#endif
