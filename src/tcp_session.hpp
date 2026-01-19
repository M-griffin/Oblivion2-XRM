#ifndef TCP_SESSION_HPP
#define TCP_SESSION_HPP

#include <iostream>
#include <utility>
#include <functional>
#include <string>
#include <chrono>
#include <experimental/optional>
#include <utf8.h>

#include "model-sys/structures.hpp"
#include "common_io.hpp"
#include "deadline_timer.hpp"
#include "libSqliteWrapped.h"
#include "processor_ansi.hpp"
#include "socket_service.hpp"
#include "session_io.hpp"
#include "telnet.hpp"
#include "telnet_session.hpp"
#include "state_manager.hpp"
#include "logging.hpp"
#include "session_writer.hpp"
#include "model-sys/users.hpp"
#include "model-sys/context.hpp"

#include "sdl2_net/SDL_net.hpp"

class TCPSession {

    Logging &m_log;
    SocketService m_socketService;
    SessionWriter m_session_writer;
    TelnetSession m_telnetSession;
    Users m_userRec;
    ProcessorAnsi m_ansi_process;
    CommonIO m_common_io;
    SessionIO m_session_io;
    SQLW::Database &m_coreDatabase;

    Context m_context;
    std::experimental::optional<StateManager> m_state_manager;

    // ESC handling
    std::string m_escBuffer;
    bool m_escPending = false;
    DeadlineTimer m_escTimer;

    // UTF8 Handling
    std::string m_utf8Buffer;

public:
    TCPSession(TCPsocket socket, const int nodeNumber, Config &config, SQLW::Database &coreDatabase)
        : m_log(Logging::getInstance())
          , m_socketService(socket, nodeNumber, config)
          , m_session_writer(m_socketService)
          , m_telnetSession(m_session_writer)
          , m_userRec()
          , m_ansi_process()
          , m_common_io()
          , m_session_io(m_session_writer, m_common_io)
          , m_coreDatabase(coreDatabase)
          , m_context() {
        // Bind all necessary components
        m_context.bind(
            m_session_writer,
            m_telnetSession,
            m_userRec,
            m_ansi_process,
            m_common_io,
            m_session_io,
            m_socketService.getConfig(),
            m_coreDatabase
        );

        // Clear screen on connection
        const std::string clear_screen = "\x1b[1;1H\x1b[2J\x1b[0m";
        m_socketService.send(clear_screen);

        // Telnet Option Negotiation Start

        // Tell client we won't use OLD_ENVIRON
        m_telnetSession.sendIACSequences(DONT, TELOPT_OLD_ENVIRON);
        m_telnetSession.sendIACSequences(DO, TELOPT_NEW_ENVIRON);

        // Enable SGA (Suppress Go Ahead)
        m_telnetSession.sendIACSequences(DO, TELOPT_SGA);
        m_telnetSession.addReply(TELOPT_SGA);

        // ECHO negotiation
        m_telnetSession.sendIACSequences(DONT, TELOPT_ECHO);
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

    SocketService &getSession() { return m_socketService; }
    TelnetSession &getTelnet() { return m_telnetSession; }
    Users &getUserRec() { return m_userRec; }
    Config &getConfig() { return m_socketService.getConfig(); }
    int getNodeNumber() const { return m_socketService.getNodeNumber(); }
    TCPsocket getSocket() const { return m_socketService.getSocket(); }
    bool isActive() const { return m_socketService.isActive(); }

    void hangup() { m_socketService.hangup(); }
    void send(const std::string &value) { m_socketService.send(value); }
    ByteBuffer receive() { return m_socketService.receive(); }
    void close() { return m_socketService.close(); }

    int getTermRows() const { return m_telnetSession.getTermRows(); }
    int getTermCols() const { return m_telnetSession.getTermCols(); }

    void startSession() {
        m_state_manager.emplace(m_context);
        m_state_manager->createPreLogon();
    }

    void handleIncomingData(const ByteBuffer &msg) {
        for (Byte incoming: msg) {
            auto appBytes = m_telnetSession.telnetOptionParse(incoming);
            for (Byte b: appBytes) {
                handleIncomingByte(b);
            }
        }
    }

    void pollTimers() {
        if (!m_socketService.isActive()) {
            return;
        }

        if (m_escPending) {
            m_escTimer.isTriggered();
        }

        m_state_manager->pollTimers();
    }

private:
    void handleIncomingByte(uint8_t byte) {
        // ESC received
        if (byte == 0x1b) {
            // Resolve previous ESC if still pending
            if (m_escPending) {
                m_escTimer.cancel();
                m_state_manager->handleInput(std::string{static_cast<char>(0x1b), '\0'});
                m_escBuffer.clear();
                m_escPending = false;
            }

            // Start new ESC decision
            m_escPending = true;
            m_escBuffer.clear();
            m_escBuffer.push_back(0x1b);

            m_escTimer.start(std::chrono::milliseconds(25), [this]() {
                m_state_manager->handleInput(std::string{static_cast<char>(0x1b), '\0'});
                m_escBuffer.clear();
                m_escPending = false;
            });

            return;
        }

        if (m_escPending) {
            m_escBuffer.push_back(byte);

            if (isEscSequenceComplete(m_escBuffer)) {
                m_escTimer.cancel();

                // CSI handling (ESC [ ...)
                if (m_escBuffer.size() >= 2 && m_escBuffer[1] == '[') {
                    handleCSISequence(m_escBuffer);
                    m_state_manager->handleInput(m_escBuffer);
                }

                m_escBuffer.clear();
                m_escPending = false;
            }

            return;
        }

        // After telnet + ANSI handling
        m_utf8Buffer.push_back(static_cast<char>(byte));

        auto it = m_utf8Buffer.begin();
        auto end = m_utf8Buffer.end();
        auto last_good = it;

        try {
            while (it != end) {
                last_good = it;
                utf8::next(it, end);
            }

            // All valid UTF-8
            m_state_manager->handleInput(m_utf8Buffer);
            m_utf8Buffer.clear();
        }
        catch (utf8::not_enough_room &) {
            if (last_good != m_utf8Buffer.begin()) {
                m_state_manager->handleInput(
                    std::string(m_utf8Buffer.begin(), last_good)
                );
                m_utf8Buffer.erase(m_utf8Buffer.begin(), last_good);
            }
        }
        catch (utf8::exception &) {
            m_utf8Buffer.erase(m_utf8Buffer.begin());
        }
    }

    bool isEscSequenceComplete(const std::string &seq) {
        if (seq.empty() || seq[0] != 0x1b)
            return true;

        if (seq.size() == 1)
            return false;

        const Byte second = seq[1];

        // CSI
        if (second == '[' || second == 'O') {
            if (seq.size() < 3) {
                return false;
            }

            Byte last = seq.back();
            return last >= 0x40 && last <= 0x7E;
        }

        // OSC / DCS / APC / PM
        if (second == ']' || second == 'P' ||
            second == '^' || second == '_') {
            // BEL terminator
            if (seq.back() == 0x07) {
                return true;
            }

            // ESC
            if (seq.size() >= 2 && seq[seq.size() - 2] == 0x1b && seq.back() == '\\') {
                return true;
            }

            return false;
        }

        // 2-byte ESC sequence
        return true;
    }

    void handleCSISequence(const std::string &seq) {
        // Cursor Position Report: ESC [ row ; col R
        if (!seq.empty() && seq.back() == 'R') {
            int row = 0;
            int col = 0;
            if (std::sscanf(seq.c_str(), "\x1b[%d;%dR", &row, &col) == 2) {
                m_log.log(
                    Logging::LogLevel::Info,
                    "Received CPR: row=" + std::to_string(row) +
                    " col=" + std::to_string(col)
                );
            }
        }
    }
};

#endif
