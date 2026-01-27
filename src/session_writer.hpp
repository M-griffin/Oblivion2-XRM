#ifndef SESSION_WRITER_HPP
#define SESSION_WRITER_HPP

#include <string>

#include "socket_service.hpp"
#include "io_encoding.hpp"

class SessionWriter {

    SocketService &m_socketService;

    bool m_is_authorized;
    bool m_use_ansi;
    IoEncoding::TextEncoding m_encoding = IoEncoding::TextEncoding::CP437;

public:
    explicit SessionWriter(SocketService &s)
        : m_socketService(s)
        , m_is_authorized(false)
        , m_use_ansi(false) {
    }

    void send(const std::string &v, const bool isDisconnected = false) {
        if (m_encoding == IoEncoding::TextEncoding::CP437) {
            const IoEncoding encode;
            const std::string result = encode.utf8Decode(v);
            m_socketService.send(result, isDisconnected);
            return;
        }

        m_socketService.send(v, isDisconnected);
    }

    void send(const ByteBuffer &v, const bool isDisconnected = false) {
        if (m_encoding == IoEncoding::TextEncoding::CP437) {
            const IoEncoding encode;
            std::string buffer = std::string(buffer.begin(), buffer.end());;
            const std::string result = encode.utf8Decode(buffer);
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

    IoEncoding::TextEncoding getEncoding() const {
        return m_encoding;
    }

    void setEncoding(const IoEncoding::TextEncoding value) {
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

};

#endif
