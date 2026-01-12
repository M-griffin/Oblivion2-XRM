#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cassert>

#include "../common_io.hpp"
#include "../processor_ansi.hpp"
#include "../session_writer.hpp"
#include "../session_io.hpp"
#include "../telnet_session.hpp"

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

    SessionWriter *sessionWriter = nullptr;
    TelnetSession *telnetSession = nullptr;
    Users *userRec = nullptr;
    ProcessorAnsi *ansiProcess = nullptr;
    CommonIO *commonIO = nullptr;
    SessionIO *sessionIO = nullptr;
    Config *config = nullptr;
    SQLW::Database *database = nullptr;

    void bind(
        SessionWriter &sw,
        TelnetSession &ts,
        Users &ur,
        ProcessorAnsi &ap,
        CommonIO &cio,
        SessionIO &sio,
        Config &cfg,
        SQLW::Database &db
    ) {
        sessionWriter = &sw;
        telnetSession = &ts;
        userRec = &ur;
        ansiProcess = &ap;
        commonIO = &cio;
        sessionIO = &sio;
        config = &cfg;
        database = &db;
    }

    SessionWriter &getSessionWrite() const {
        assert(sessionWriter);
        return *sessionWriter;
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

    SQLW::Database &getDatabase() const {
        assert(database);
        return *database;
    }

};

#endif
