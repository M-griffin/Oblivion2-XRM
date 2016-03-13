/**
 *	StderrLog.cpp
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include <stdio.h>
#ifdef _WIN32
//#pragma warning(disable:4786)
#endif

#include <string>
#include <map>
#include <time.h>
#include <sqlite3.h>

#include "Database.h"
#include "Query.h"
#include "IError.h"
#include "StderrLog.h"


namespace SQLW
{
    void StderrLog::databaseError(Database& db, const std::string& str)
    {
        UNUSED(db);
        time_t t = time(nullptr);
        struct tm *tp = localtime(&t);
        fprintf(stderr,"%d-%02d-%02d %02d:%02d:%02d :: Database: %s\n",
                tp -> tm_year + 1900,tp -> tm_mon + 1,tp -> tm_mday,
                tp -> tm_hour,tp -> tm_min, tp -> tm_sec,
                str.c_str());
    }

    void StderrLog::databaseError(Database& db, Query& q, const std::string& str)
    {
        UNUSED(db);
        time_t t = time(nullptr);
        struct tm *tp = localtime(&t);
        fprintf(stderr,"%d-%02d-%02d %02d:%02d:%02d :: Query: %s: %s(%d)\n",
                tp -> tm_year + 1900,tp -> tm_mon + 1,tp -> tm_mday,
                tp -> tm_hour,tp -> tm_min, tp -> tm_sec,
                str.c_str(),q.GetError().c_str(),q.GetErrno());
        fprintf(stderr," (QUERY: \"%s\")\n",q.getLastQuery().c_str());
    }

} // namespace SQLW {
