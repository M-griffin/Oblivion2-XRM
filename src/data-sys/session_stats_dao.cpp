#include "session_stats_dao.hpp"

#include "../model-sys/session_stats.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>

SessionStatsDao::SessionStatsDao(SQLW::Database &database)
    : m_stats_database(database)
{
}

SessionStatsDao::~SessionStatsDao()
{
}

