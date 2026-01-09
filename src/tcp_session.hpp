#ifndef TCP_SESSION_HPP
#define TCP_SESSION_HPP

#include <iostream>
#include <utility>
#include <functional>
#include <string>
#include <chrono>
#include <experimental/optional>

#include "common_io.hpp"
#include "deadline_timer.hpp"
#include "processor_ansi.hpp"
#include "session.hpp"
#include "session_io.hpp"
#include "telnet.hpp"
#include "telnet_session.hpp"
#include "state_manager.hpp"
#include "logging.hpp"
#include "model-sys/users.hpp"
#include "model-sys/context.hpp"

#include "sdl2_net/SDL_net.hpp"

class TCPSession {
    Logging &m_log;
    Session m_baseSession;
    TelnetSession m_telnetSession;
    Users m_userRec;
    ProcessorAnsi m_ansi_process;
    CommonIO m_common_io;
    SessionIO m_session_io;

    Context m_context;
    std::experimental::optional<StateManager> m_state_manager;

    // ESC handling
    std::string m_escBuffer;      // current ESC sequence
    bool m_escPending = false;    // one ESC waiting for resolution
    DeadlineTimer m_escTimer;

public:
    TCPSession(TCPsocket socket, const int nodeNumber, Config &config)
    : m_log(Logging::getInstance())
    , m_baseSession(socket, nodeNumber, config)
    , m_telnetSession(m_baseSession)
    , m_userRec()
    , m_ansi_process()
    , m_common_io()
    , m_session_io(*this, m_common_io)
    , m_context() {

        // Bind all necessary components
        m_context.bind(
            m_baseSession,
            m_telnetSession,
            m_userRec,
            m_ansi_process,
            m_common_io,
            m_session_io,
            m_baseSession.getConfig()
        );

        // Clear screen on connection
        const std::string clear_screen = "\x1b[1;1H\x1b[2J\x1b[0m";
        m_baseSession.send(clear_screen);

        // ===============================
        // Telnet Option Negotiation
        // ===============================

        // Tell client we won't use OLD_ENVIRON
        m_telnetSession.sendIACSequences(DONT, TELOPT_OLD_ENVIRON);

        // Enable SGA (Suppress Go Ahead)
        m_telnetSession.sendIACSequences(DO, TELOPT_SGA);
        m_telnetSession.addReply(TELOPT_SGA);

        // ECHO negotiation
        m_telnetSession.sendIACSequences(WILL, TELOPT_ECHO);
        m_telnetSession.addReply(TELOPT_ECHO);

        // SGA, BINARY support
        m_telnetSession.sendIACSequences(WILL, TELOPT_SGA);
        m_telnetSession.addReply(TELOPT_SGA);

        m_telnetSession.sendIACSequences(WILL, TELOPT_BINARY);
        m_telnetSession.addReply(TELOPT_BINARY);

        m_telnetSession.sendIACSequences(DO, TELOPT_BINARY);
        m_telnetSession.addReply(TELOPT_BINARY);

        // Terminal Type (TTYPE)
        m_telnetSession.sendIACSequences(DO, TELOPT_TTYPE);
        m_telnetSession.addReply(TELOPT_TTYPE);

        // Negotiate NAWS (window size)
        m_telnetSession.sendIACSequences(DO, TELOPT_NAWS);
        m_telnetSession.addReply(TELOPT_NAWS);

        // Log constructor call
        m_log.log(Logging::LogLevel::Console, "TCPSession() initialized with Telnet options negotiated");
    }

    ~TCPSession() {
        m_log.log(Logging::LogLevel::Console, "~TCPSession()");
        m_escTimer.cancel();
        m_state_manager = std::experimental::nullopt;
    }

    TCPSession(TCPSession &&) = delete;
    TCPSession &operator=(TCPSession &&) = delete;
    TCPSession(const TCPSession &) = delete;
    TCPSession &operator=(const TCPSession &) = delete;

    Session &getSession() { return m_baseSession; }
    TelnetSession &getTelnet() { return m_telnetSession; }
    Users &getUserRec() { return m_userRec; }
    Config &getConfig() { return m_baseSession.getConfig(); }
    int getNodeNumber() const { return m_baseSession.getNodeNumber(); }
    TCPsocket getSocket() const { return m_baseSession.getSocket(); }
    bool isActive() const { return m_baseSession.isActive(); }

    void hangup() { m_baseSession.hangup(); }
    void send(const std::string &value) { m_baseSession.send(value); }
    ByteBuffer receive() { return m_baseSession.receive(); }
    void close() { return m_baseSession.close(); }

    int getTermRows() const { return m_telnetSession.getTermRows(); }
    int getTermCols() const { return m_telnetSession.getTermCols(); }
    bool getUseAnsi() const { return m_telnetSession.getUseAnsi(); }

    void startSession() {
        m_state_manager.emplace(m_context);
        m_state_manager->createPreLogon();
    }

    void handleIncomingData(const ByteBuffer &msg) {
        for (Byte incoming : msg) {
            auto appBytes = m_telnetSession.telnetOptionParse(incoming);
            for (Byte b : appBytes) {
                handleIncomingByte(b);
            }
        }
    }

    void pollTimers() {
        if (m_escPending) {
            m_escTimer.isTriggered();
        }
        m_state_manager->pollTimers();
    }

private:
    void handleIncomingByte(uint8_t byte) {

        // =============================
        // ESC received
        // =============================
        if (byte == 0x1b) {

            // Resolve previous ESC if still pending
            if (m_escPending) {
                m_escTimer.cancel();
                m_state_manager->handleInput(std::string{ char(0x1b), '\0' });
                m_escBuffer.clear();
                m_escPending = false;
            }

            // Start new ESC decision
            m_escPending = true;
            m_escBuffer.clear();
            m_escBuffer.push_back(0x1b);

            m_escTimer.start(std::chrono::milliseconds(25), [this]() {
                m_state_manager->handleInput(std::string{ char(0x1b), '\0' });
                m_escBuffer.clear();
                m_escPending = false;
            });

            return;
        }

        // =============================
        // ESC sequence continuation
        // =============================
        if (m_escPending) {
            m_escBuffer.push_back(byte);

            if (isEscSequenceComplete(m_escBuffer)) {
                m_escTimer.cancel();
                m_state_manager->handleInput(m_escBuffer);
                m_escBuffer.clear();
                m_escPending = false;
            }
            return;
        }

        // =============================
        // Normal input
        // =============================
        if (byte == '\n') {
            m_state_manager->handleInput("\n");
        } else if (byte != '\r') {
            m_state_manager->handleInput(std::string(1, byte));
        }
    }

    bool isEscSequenceComplete(const std::string &seq) {
        if (seq.empty() || seq[0] != 0x1b) return true;
        if (seq.size() == 1) return false;

        const uint8_t second = seq[1];
        if (second == '[' || second == 'O') {
            const char last = seq.back();
            return (last >= '@' && last <= '~');
        }
        return true;
    }
};

#endif
