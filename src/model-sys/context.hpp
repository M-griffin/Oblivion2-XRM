#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cassert>

#include "../common_io.hpp"
#include "../processor_ansi.hpp"
#include "../session.hpp"
#include "../session_io.hpp"
#include "../telnet_session.hpp"

#include "config.hpp"
#include "users.hpp"

/**
 * @class Context
 * @author Michael Griffin
 * @date 12/29/2025
 * @file context.hpp
 * @brief Session Context For Passing Main Objects
 */
struct Context {
    Session *baseSession = nullptr;
    TelnetSession *telnetSession = nullptr;
    Users *userRec = nullptr;
    ProcessorAnsi *ansiProcess = nullptr;
    CommonIO *commonIO = nullptr;
    SessionIO *sessionIO = nullptr;
    Config *config = nullptr;

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

    Session &base() const {
        assert(baseSession);
        return *baseSession;
    }

    TelnetSession &telnet() const {
        assert(telnetSession);
        return *telnetSession;
    }

    Users &user() const {
        assert(userRec);
        return *userRec;
    }

    ProcessorAnsi &ansi() const {
        assert(ansiProcess);
        return *ansiProcess;
    }

    CommonIO &common() const {
        assert(commonIO);
        return *commonIO;
    }

    SessionIO &session() const {
        assert(sessionIO);
        return *sessionIO;
    }

    Config &cfg() const {
        assert(config);
        return *config;
    }
};

#endif
