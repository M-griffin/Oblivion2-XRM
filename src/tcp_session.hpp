#ifndef TCP_SESSION_HPP
#define TCP_SESSION_HPP

#include <iostream>
#include <utility>

#include "common_io.hpp"
#include "processor_ansi.hpp"
#include "session.hpp"
#include "session_io.hpp"
#include "telnet.hpp"
#include "telnet_session.hpp"
#include "model-sys/users.hpp"
#include "model-sys/context.hpp"

#include "mods/mod_prelogon.hpp"

#include "sdl2_net/SDL_net.hpp"


class TCPSession {
    Session m_baseSession;
    TelnetSession m_telnetSession;
    Users m_userRec;
    ProcessorAnsi m_ansi_process;
    CommonIO m_common_io;
    SessionIO m_session_io;

    // Context Of Shared Components Per Session.
    Context m_context;

    std::vector<ModPreLogon> m_prelogon;

public:
    std::string m_encoding_text;
    int m_encoding;
    bool m_is_use_ansi;

    // Default constructor
    TCPSession(TCPsocket socket, const int nodeNumber, Config &config)
        : m_baseSession(socket, nodeNumber, config)
          , m_telnetSession(m_baseSession)
          , m_userRec()
          , m_ansi_process()
          , m_common_io()
          , m_session_io(*this, m_common_io)
          , m_encoding(0)
          , m_is_use_ansi(false) {
        m_context.bind(
            m_baseSession,
            m_telnetSession,
            m_userRec,
            m_ansi_process,
            m_common_io,
            m_session_io,
            m_baseSession.getConfig()
        );

        std::cout << "Sending Telnet Default Sequences on New Connection" << std::endl;

        // On initial Session Connection,  setup and send TELNET Options to
        // start the negotiation of client features.
        // On initial connection, clear and home cursor
        const std::string clear_screen = "\x1b[1;1H\x1b[2J\x1b[0m";
        m_baseSession.send(clear_screen);

        m_telnetSession.sendIACSequences(DONT, TELOPT_OLD_ENVIRON);

        m_telnetSession.sendIACSequences(DO, TELOPT_SGA);
        m_telnetSession.addReply(TELOPT_SGA);

        m_telnetSession.sendIACSequences(WILL, TELOPT_ECHO);
        m_telnetSession.addReply(TELOPT_ECHO);

        m_telnetSession.sendIACSequences(WILL, TELOPT_SGA);
        m_telnetSession.addReply(TELOPT_SGA);

        m_telnetSession.sendIACSequences(WILL, TELOPT_BINARY);
        m_telnetSession.addReply(TELOPT_BINARY);

        m_telnetSession.sendIACSequences(DO, TELOPT_BINARY);
        m_telnetSession.addReply(TELOPT_BINARY);

        m_telnetSession.sendIACSequences(DO, TELOPT_TTYPE);
        m_telnetSession.addReply(TELOPT_TTYPE);

        m_telnetSession.sendIACSequences(DO, TELOPT_NAWS);
        m_telnetSession.addReply(TELOPT_NAWS);

        std::cout << "Ending Telnet Default Sequences on New Connection" << std::endl;
    }

    // Move constructor
    TCPSession(TCPSession &&other) noexcept
        : m_baseSession(std::move(other.m_baseSession))
          , m_telnetSession(std::move(other.m_telnetSession))
          , m_userRec(std::move(other.m_userRec))
          , m_ansi_process(std::move(other.m_ansi_process))
          , m_common_io(std::move(other.m_common_io))
          , m_session_io(*this, m_common_io)
          , m_prelogon(std::move(other.m_prelogon))
          , m_encoding_text(std::move(other.m_encoding_text))
          , m_encoding(other.m_encoding)
          , m_is_use_ansi(other.m_is_use_ansi) {
        m_context.bind(
            m_baseSession,
            m_telnetSession,
            m_userRec,
            m_ansi_process,
            m_common_io,
            m_session_io,
            m_baseSession.getConfig()
        );
    }


    // Move assignment operator
    TCPSession &operator=(TCPSession &&other) noexcept {
        if (this != &other) {
            m_baseSession = std::move(other.m_baseSession);
            m_userRec = std::move(other.m_userRec);
            m_ansi_process = std::move(other.m_ansi_process);
            m_common_io = std::move(other.m_common_io);
            m_prelogon = std::move(other.m_prelogon);

            m_telnetSession = std::move(TelnetSession(m_baseSession));
            m_session_io = SessionIO(*this, m_common_io);

            m_context.bind(
                m_baseSession,
                m_telnetSession,
                m_userRec,
                m_ansi_process,
                m_common_io,
                m_session_io,
                m_baseSession.getConfig()
            );

            m_encoding_text = std::move(other.m_encoding_text);
            m_encoding = other.m_encoding;
            m_is_use_ansi = other.m_is_use_ansi;
        }
        return *this;
    }

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

    Users &getUserRec() {
        return m_userRec;
    }

    Config &getConfig() {
        return m_baseSession.getConfig();
    }

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

    void handleIncomingData(const std::string &msg) {
        for (const unsigned char incoming: msg) {
            unsigned char c = m_telnetSession.telnetOptionParse(incoming);
        }
    }
};

#endif
