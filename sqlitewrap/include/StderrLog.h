#ifndef STDERRLOG_H_SQLITE
#define STDERRLOG_H_SQLITE

/**
 * Refactoring / author: 2026-02-06 / mrmisticismo@hotmail.com
 * Rewritten   / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published   / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2026  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include "IError.h"
#include <string>

namespace SQLW {
    class Database;

    /** Log class writing to standard error. */
    class StderrLog final : public IError {
    public:
        void databaseError(Database &, const std::string &) override;

        void databaseError(Database &, Query &, const std::string &) override;
    };
} // namespace SQLW {


#endif
