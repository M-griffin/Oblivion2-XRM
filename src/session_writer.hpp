#ifndef SESSION_WRITER_HPP
#define SESSION_WRITER_HPP

#include <string>

#include "socket_service.hpp"
#include "encoding.hpp"

class SessionWriter {
public:
    explicit SessionWriter(SocketService &s)
        : m_socketService(s) {
    }

    void send(const std::string &v, const bool isDisconnected = false) {
        if (m_encoding == Encoding::TextEncoding::CP437) {
            const Encoding encode;
            std::string result = encode.utf8Decode(v);
            m_socketService.send(result, isDisconnected);
            return;
        }

        m_socketService.send(v, isDisconnected);
    }

    void send(const ByteBuffer &v, const bool isDisconnected = false) {
        if (m_encoding == Encoding::TextEncoding::CP437) {
            const Encoding encode;
            std::string buffer = std::string(buffer.begin(), buffer.end());;
            std::string result = encode.utf8Decode(buffer);
            m_socketService.send(result, isDisconnected);
            return;
        }
        m_socketService.send(v, isDisconnected);
    }

    void sendRaw(const std::string &v, const bool isDisconnected = false) {
        m_socketService.send(v, isDisconnected);
    }

    void sendRaw(const ByteBuffer &v, const bool isDisconnected = false) {
        m_socketService.send(v, isDisconnected);
    }

    bool isAnsi() const {
        return m_use_ansi;
    }

    void setAnsi(const bool use_ansi) {
        m_use_ansi = use_ansi;
    }

    Encoding::TextEncoding getEncoding() const {
        return m_encoding;
    }

    void setEncoding(const Encoding::TextEncoding value) {
        m_encoding = value;
    }

    int getNodeNumber() const {
        return m_socketService.getNodeNumber();
    }

    void hangup() {
        m_socketService.hangup();
    }

    bool isAuthorized() const {
        return m_is_authorized;
    }

    void setAuthorized(const bool isAuthorized) {
        m_is_authorized = isAuthorized;
    }

    bool isActive() {
        return m_socketService.isActive();
    }

private:
    SocketService &m_socketService;

    bool m_is_authorized = false;
    bool m_use_ansi = false;
    Encoding::TextEncoding m_encoding = Encoding::TextEncoding::CP437;
};

#endif
