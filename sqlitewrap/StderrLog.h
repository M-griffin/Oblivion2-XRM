/**
 *	StderrLog.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2018  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _STDERRLOG_H_SQLITE
#define _STDERRLOG_H_SQLITE

#include "IError.h"
#include <string>

namespace SQLW
{
    class Database;

    /** Log class writing to standard error. */
    class StderrLog : public IError
    {
    public:
        void databaseError(Database&, const std::string&);
        void databaseError(Database&, Query&, const std::string&);
    };

} // namespace SQLW {


#endif // _STDERRLOG_H
