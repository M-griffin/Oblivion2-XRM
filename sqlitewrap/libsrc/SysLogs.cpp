/**
 * Refactoring / author: 2026-02-06 / mrmisticismo@hotmail.com
 * Rewritten   / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published   / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2026  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _WIN32

#include "../include/SysLogs.h"

#include <string>

namespace SQLW {

    SysLog::SysLog(const std::string& db_ident, int logopt, int facility) {
        openlog(db_ident.c_str(), logopt, facility);
    }

    SysLog::~SysLog() {
        closelog();
    }

    void SysLog::databaseError(Database& db, const std::string& msg) {
        syslog(LOG_ERR, "Database [%s]: %s", db.name().c_str(), msg.c_str());
    }

    void SysLog::databaseError(Database& db, Query& q, const std::string& msg) {
        syslog(LOG_ERR, "Query Error in [%s]: %s (SQLite: %s [%d]) (QUERY: \"%s\")",
            db.name().c_str(),
            msg.c_str(),
            q.getError().c_str(),
            q.getErrorCodeerrorCode(),
            q.getLastQuery().c_str());
    }

} // namespace SQLW
#endif
