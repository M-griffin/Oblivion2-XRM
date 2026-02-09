/**
 * Refactoring / author: 2026-02-06 / mrmisticismo@hotmail.com
 * Rewritten   / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published   / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2026  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include "../include/StderrLog.h"

#include <Database.h>
#include <Query.h>

namespace SQLW {
    void StderrLog::databaseError(Database &db, const std::string &msg) {
        auto t = std::time(nullptr);
        std::tm tp{};
#ifdef _WIN32
        localtime_s(&tp, &t);
#else
        localtime_r(&t, &tp);
#endif

        fprintf(stderr, "%d-%02d-%02d %02d:%02d:%02d :: Database [%s]: %s\n",
                tp.tm_year + 1900, tp.tm_mon + 1, tp.tm_mday,
                tp.tm_hour, tp.tm_min, tp.tm_sec,
                db.getName().c_str(),
                msg.c_str());

        fflush(stderr);
    }

    void StderrLog::databaseError(Database &db, Query &q, const std::string &msg) {
        auto t = std::time(nullptr);
        std::tm tp{};
#ifdef _WIN32
        localtime_s(&tp, &t);
#else
        localtime_r(&t, &tp);
#endif

        fprintf(stderr, "%d-%02d-%02d %02d:%02d:%02d :: Query Error in [%s]: %s (SQLite: %s [%d])\n",
                tp.tm_year + 1900, tp.tm_mon + 1, tp.tm_mday,
                tp.tm_hour, tp.tm_min, tp.tm_sec,
                db.getName().c_str(),
                msg.c_str(),
                q.getError().c_str(),
                q.getErrorCode());

        fprintf(stderr, "  (QUERY: \"%s\")\n", q.getLastQuery().c_str());
        fflush(stderr);
    }
} // namespace SQLW
