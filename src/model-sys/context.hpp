#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cassert>

#include "../io_common.hpp"
#include "../screen_ansi_proc.hpp"
#include "../tcp_session_wrapper.hpp"
#include "../io_session.hpp"
#include "../tcp_telnet.hpp"

#include "config.hpp"
#include "users.hpp"

#include "libSqliteWrapped.h"

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

    TcpSessionWrapper *sessionWriter = nullptr;
    TcpTelnet *telnetSession = nullptr;
    Users *userRec = nullptr;
    ScreenAnsiProc *screenAnsi = nullptr;
    IoCommon *ioCommon = nullptr;
    IoSession *ioSession = nullptr;
    Config *config = nullptr;
    SQLW::Database *database = nullptr;

    void bind(
        TcpSessionWrapper &sw,
        TcpTelnet &ts,
        Users &ur,
        ScreenAnsiProc &ap,
        IoCommon &cio,
        IoSession &sio,
        Config &cfg,
        SQLW::Database &db
    ) {
        sessionWriter = &sw;
        telnetSession = &ts;
        userRec = &ur;
        screenAnsi = &ap;
        ioCommon = &cio;
        ioSession = &sio;
        config = &cfg;
        database = &db;
    }

    TcpSessionWrapper &getSessionWrite() const {
        assert(sessionWriter);
        return *sessionWriter;
    }

    TcpTelnet &getTelnet() const {
        assert(telnetSession);
        return *telnetSession;
    }

    Users &getUser() const {
        assert(userRec);
        return *userRec;
    }

    ScreenAnsiProc &getScreenAnsi() const {
        assert(screenAnsi);
        return *screenAnsi;
    }

    IoCommon &getIoCommon() const {
        assert(ioCommon);
        return *ioCommon;
    }

    IoSession &getIoSession() const {
        assert(ioSession);
        return *ioSession;
    }

    Config &getCfg() const {
        assert(config);
        return *config;
    }

    SQLW::Database &getDatabase() const {
        assert(database);
        return *database;
    }

};

#endif
