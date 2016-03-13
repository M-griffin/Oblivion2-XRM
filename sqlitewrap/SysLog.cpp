/**
 *	SysLog.cpp
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */
#ifndef _WIN32

#include <sqlite3.h>
#include <syslog.h>

#include "Database.h"
#include "Query.h"
#include "IError.h"
#include "SysLog.h"

namespace SQLW
{
    SysLog::SysLog(const std::string& appname,int option,int facility)
    {
        openlog(appname.c_str(), option, facility);
    }

    SysLog::~SysLog()
    {
        closelog();
    }

    void SysLog::databaseError(Database& db,const std::string& str)
    {
        syslog(LOG_ERR, "%s", str.c_str());
    }


    void SysLog::databaseError(Database& db,Query& q,const std::string& str)
    {
        syslog(LOG_ERR, "%s: %s(%d)", str.c_str(),q.GetError().c_str(),q.GetErrno());
        syslog(LOG_ERR, "QUERY: \"%s\"", q.GetLastQuery().c_str());
    }

} // namespace SQLW


#endif // WIN32
