#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cassert>

#include "../common_io.hpp"
#include "../processor_ansi.hpp"
#include "../session.hpp"
#include "../session_io.hpp"
#include "../telnet_session.hpp"
#include "../state_manager.hpp"

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
    StateManager *state = nullptr;

    void bind(
        Session &bs,
        TelnetSession &ts,
        Users &ur,
        ProcessorAnsi &ap,
        CommonIO &cio,
        SessionIO &sio,
        Config &cfg,
        StateManager &st
    ) {
        baseSession = &bs;
        telnetSession = &ts;
        userRec = &ur;
        ansiProcess = &ap;
        commonIO = &cio;
        sessionIO = &sio;
        config = &cfg;
        state = &st;
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

    StateManager &getState() const {
        assert(state);
        return *state;
    }
};

#endif
