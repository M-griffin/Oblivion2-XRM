/**
 *	SysLog.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2018  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */
#ifndef _SYSLOG_H_SQLITE
#define _SYSLOG_H_SQLITE
#ifndef _WIN32

#include <syslog.h>


namespace SQLW
{
    /** Log class writing to syslog. */
    class SysLog : public IError
    {
    public:
        SysLog(const std::string& = "database", int = LOG_PID, int = LOG_USER);
        virtual ~SysLog();

        void databaseError(Database&, const std::string&);
        void databaseError(Database&, Query&, const std::string&);

    };

} // namespace SQLW {

#endif // WIN32
#endif // _SYSLOG_H
