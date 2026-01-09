#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cassert>

#include "../common_io.hpp"
#include "../processor_ansi.hpp"
#include "../session.hpp"
#include "../session_io.hpp"
#include "../telnet_session.hpp"
#include "../encoding.hpp"

#include "config.hpp"
#include "users.hpp"

/**
 * @class Context
 * @author Michael Griffin
 * @date 12/29/2025
 * @file context.hpp
 * @brief Session Context For Passing Main Objects
 */
class Context {

public:

    explicit Context() = default;
    ~Context() = default;

    Session *baseSession = nullptr;
    TelnetSession *telnetSession = nullptr;
    Users *userRec = nullptr;
    ProcessorAnsi *ansiProcess = nullptr;
    CommonIO *commonIO = nullptr;
    SessionIO *sessionIO = nullptr;
    Config *config = nullptr;

    bool m_use_ansi = false;
    Encoding::TextEncoding m_encoding = Encoding::TextEncoding::CP437;

    void bind(
        Session &bs,
        TelnetSession &ts,
        Users &ur,
        ProcessorAnsi &ap,
        CommonIO &cio,
        SessionIO &sio,
        Config &cfg
    ) {
        baseSession = &bs;
        telnetSession = &ts;
        userRec = &ur;
        ansiProcess = &ap;
        commonIO = &cio;
        sessionIO = &sio;
        config = &cfg;
    }

    Session &getBase() const {
        assert(baseSession);
        return *baseSession;
    }

    TelnetSession &getTelnet() const {
        assert(telnetSession);
        return *telnetSession;
    }

    Users &getUser() const {
        assert(userRec);
        return *userRec;
    }

    ProcessorAnsi &getAnsi() const {
        assert(ansiProcess);
        return *ansiProcess;
    }

    CommonIO &getCommonIO() const {
        assert(commonIO);
        return *commonIO;
    }

    SessionIO &getSessionIO() const {
        assert(sessionIO);
        return *sessionIO;
    }

    Config &getCfg() const {
        assert(config);
        return *config;
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
};

#endif
