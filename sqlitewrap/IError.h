/**
 *	IError.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2018  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _IERROR_H_SQLITE
#define _IERROR_H_SQLITE

#include <string>

namespace SQLW
{
    class Database;
    class Query;

    /** Log class interface. */
    class IError
    {
    public:
        virtual void databaseError(Database&, const std::string&) = 0;
        virtual void databaseError(Database&, Query&, const std::string&) = 0;
    };

} // namespace SQLW {

#endif // _IERROR_H
