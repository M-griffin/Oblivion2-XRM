#ifndef SESSION_WRITER_HPP
#define SESSION_WRITER_HPP

#include <string>

#include "tcp_socket_base.hpp"
#include "io_encoding.hpp"

class TcpSessionWrapper {

    TcpSocketBase &m_socketBase;

    bool m_is_authorized;
    bool m_use_ansi;
    IoEncoding::TextEncoding m_encoding = IoEncoding::TextEncoding::CP437;

public:
    explicit TcpSessionWrapper(TcpSocketBase &s)
        : m_socketBase(s)
        , m_is_authorized(false)
        , m_use_ansi(false) {
    }

    void send(const std::string &v, const bool isDisconnected = false) {
        if (m_encoding == IoEncoding::TextEncoding::CP437) {
            const IoEncoding encode;
            const std::string result = encode.utf8Decode(v);
            m_socketBase.send(result, isDisconnected);
            return;
        }

        m_socketBase.send(v, isDisconnected);
    }

    void send(const ByteBuffer &v, const bool isDisconnected = false) {
        if (m_encoding == IoEncoding::TextEncoding::CP437) {
            const IoEncoding encode;
            std::string buffer = std::string(buffer.begin(), buffer.end());;
            const std::string result = encode.utf8Decode(buffer);
            m_socketBase.send(result, isDisconnected);
            return;
        }
        m_socketBase.send(v, isDisconnected);
    }

    void sendRaw(const std::string &v, const bool isDisconnected = false) {
        m_socketBase.send(v, isDisconnected);
    }

    void sendRaw(const ByteBuffer &v, const bool isDisconnected = false) {
        m_socketBase.send(v, isDisconnected);
    }

    [[nodiscard]]
    bool isAnsi() const {
        return m_use_ansi;
    }

    void setAnsi(const bool use_ansi) {
        m_use_ansi = use_ansi;
    }

    [[nodiscard]]
    IoEncoding::TextEncoding getEncoding() const {
        return m_encoding;
    }

    void setEncoding(const IoEncoding::TextEncoding value) {
        m_encoding = value;
    }

    [[nodiscard]]
    int getNodeNumber() const {
        return m_socketBase.getNodeNumber();
    }

    void hangup() {
        m_socketBase.hangup();
    }

    [[nodiscard]]
    bool isAuthorized() const {
        return m_is_authorized;
    }

    void setAuthorized(const bool isAuthorized) {
        m_is_authorized = isAuthorized;
    }

    bool isActive() {
        return m_socketBase.isActive();
    }

};

#endif
