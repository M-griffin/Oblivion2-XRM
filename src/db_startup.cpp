#include "db_startup.hpp"

#include <cassert>

#include "model-sys/structures.hpp"
#include "model-sys/protocol.hpp"
#include "model-sys/access_level.hpp"
#include "model-app/oneliners.hpp"
#include "data-app/dao_oneliners.hpp"

// Needed for Initializing and checking users data is set up
// On startup.
#include "data-sys/dao_session_stats.hpp"
#include "data-sys/dao_security.hpp"
#include "data-sys/dao_users.hpp"
#include "data-sys/yml_protocol.hpp"
#include "data-sys/dao_access_level.hpp"
#include "util_log.hpp"

#include "libSqliteWrapped.h"

namespace {

    std::string getCurrentISO8601Time() {
        // Get current time
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        // Use string stream to format the time
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
}
/**
 * @brief Setup for Database and Tables
 */
bool DBStartUp::initDatabaseTables() {
    // Setup Users Database name and path
    CORE_DATABASE = GLOBAL_DATA_PATH;

#ifdef _WIN32
    CORE_DATABASE.append("\\");
#else
    CORE_DATABASE.append("/");
#endif

    CORE_DATABASE.append("xrm_users.sqlite3");

    UtilLog &log = UtilLog::getInstance();

    // Review SQLite Path and Threading Feature Setup.
    {
        std::cout << "SQLiteConnection m_path=" << CORE_DATABASE << std::endl;

        int ts = sqlite3_threadsafe();
        if (ts) {
            log.log(UtilLog::LogLevel::Console, "SQLite is compiled with THREADSAFE=1 (threadsafe enabled)");
        } else {
            log.log(UtilLog::LogLevel::Console, "SQLite is compiled with THREADSAFE=0 (threadsafe disabled)");
        }
    }

    // Setup isolated scope for smart pointers and clean up.
    {
        log.log(UtilLog::LogLevel::Console, "user_database(CORE_DATABASE).");
        // Check and Setup users database if tables are not setup
        SQLW::StderrLog databaseLog;
        SQLW::Database user_database(CORE_DATABASE, &databaseLog);

        log.log(UtilLog::LogLevel::Console, "user_dao(CORE_DATABASE).");
        // Link to users dao for data access object
        UsersDao user_dao(user_database);

        log.log(UtilLog::LogLevel::Console, "security_dao(CORE_DATABASE).");
        // Link to security dao for data access object
        SecurityDao security_dao(user_database);

        // Verify if the security table exists.
        // Security must be present before user because of foreign key.
        log.log(UtilLog::LogLevel::Console, "security_dao.doesTableExist.");
        if (!security_dao.doesTableExist()) {
            log.log(UtilLog::LogLevel::Console, "doesn't exist (security table).");

            // Setup database Param, cache sizes etc..
            log.log(UtilLog::LogLevel::Console, "security_dao.firstTimeSetupParams.");
            if (!security_dao.firstTimeSetupParams()) {
                log.log(UtilLog::LogLevel::Info, "unable to execute firstTimeSetupParams (security table).");
                return (false);
            }

            // Setup create users table and indexes.
            log.log(UtilLog::LogLevel::Console, "security_dao.createTable.");
            if (!security_dao.createTable()) {
                log.log(UtilLog::LogLevel::Info, "unable to create (security table).");
                return (false);
            }

            log.log(UtilLog::LogLevel::Console, "security table created successfully.");
        }

        // Verify if the user table exists.
        if (!user_dao.doesTableExist()) {
            log.log(UtilLog::LogLevel::Console, "doesn't exist (user table).");

            // Setup database Param, cache sizes etc..
            if (!user_dao.firstTimeSetupParams()) {
                log.log(UtilLog::LogLevel::Info, "unable to execute firstTimeSetupParams (user table).");
                return (false);
            }

            // Setup create users table and indexes.
            if (!user_dao.createTable()) {
                log.log(UtilLog::LogLevel::Info, "unable to create (user table).");
                return (false);
            }

            log.log(UtilLog::LogLevel::Console, "user table created successfully.");
        }

        // Check Table setup for Session Stats
        SessionStatsDao session_stat_dao(user_database);

        // Verify if the user table exists.
        if (!session_stat_dao.doesTableExist()) {
            log.log(UtilLog::LogLevel::Console, "doesn't exist (session stats table).");

            // Setup database Param, cache sies etc..
            if (!session_stat_dao.firstTimeSetupParams()) {
                log.log(UtilLog::LogLevel::Info, "unable to execute firstTimeSetupParams (session stats table).");
                return (false);
            }

            // Setup create users table and indexes.
            if (!session_stat_dao.createTable()) {
                log.log(UtilLog::LogLevel::Info, "unable to create (session stats table).");
                return (false);
            }

            log.log(UtilLog::LogLevel::Console, "session stats table created successfully.");
        }

        // Link to Access Level dao for data access object
        AccessLevelDao access_dao(user_database);

        // Verify if the access_level table exists.
        if (!access_dao.doesTableExist()) {
            log.log(UtilLog::LogLevel::Console, "doesn't exist (access_level table).");

            // Setup database Param, cache sizes etc..
            if (!access_dao.firstTimeSetupParams()) {
                log.log(UtilLog::LogLevel::Info, "unable to execute firstTimeSetupParams (access_level table).");
                return (false);
            }

            // Setup create users table and indexes.
            if (!access_dao.createTable()) {
                log.log(UtilLog::LogLevel::Info, "unable to create (access_level table).");
                return (false);
            }

            log.log(UtilLog::LogLevel::Console, "access_level table created successfully.");

            // Check and Setup default Access Levels.
            AccessLevel level;

            // Set Initial Defaults for Not Validated Level
            // then reset are populated on Class Defaults.
            level.sName = "Not Validated";
            level.sStartMenu = "top";
            level.iLevel = 10;
            level.iTimeLimit = 120;
            level.bTimeLimit = true;

            access_dao.insertRecord(level);

            // Validated User
            AccessLevel level2;
            level2.sName = "Validated User";
            level2.sStartMenu = "top";
            level2.iLevel = 20;
            level2.iTimeLimit = 1440;
            level2.bTimeLimit = true;

            access_dao.insertRecord(level2);

            // Administrator (time Limit false by default)
            AccessLevel level3;
            level3.sName = "Sysop";
            level3.sStartMenu = "top";
            level3.iLevel = 255;
            level3.iTimeLimit = 1440;

            access_dao.insertRecord(level3);
        }


        Protocols prots;
        ProtocolDao protdb(prots, GLOBAL_DATA_PATH);

        if (!protdb.fileExists()) {
            log.log(UtilLog::LogLevel::Console, "Protocol configuration doesn't exist.");

            // Create Genric Protocol Entry to Test File Creation (not yet tested.)
            Protocol p1("Sexyz", "D", "Z", "C:\\TESTPATH\\", "--Test", false, false);

            prots.protocols.push_back(p1);
            protdb.saveConfig(prots);

            log.log(UtilLog::LogLevel::Console, "Protocol configuration created successfully");
        }


        OnelinerDao oneLineDao(user_database);

        if (!oneLineDao.doesTableExist()) {
            log.log(UtilLog::LogLevel::Console, "doesn't exist (oneliner table).");

            // Setup database Param, cache sizes etc..
            if (!oneLineDao.firstTimeSetupParams()) {
                log.log(UtilLog::LogLevel::Info, "unable to execute firstTimeSetupParams (oneliner table).");
                return (false);
            }

            // Setup create users table and indexes.
            if (!oneLineDao.createTable()) {
                log.log(UtilLog::LogLevel::Info, "unable to create (oneliner table).");
                return (false);
            }

            log.log(UtilLog::LogLevel::Console, "oneliner table created successfully.");

            // Insert a default record the first time the table
            // is created only.
            Oneliners one;
            one.iUserId = 1;
            one.sText = "Welcome to a new system running Oblivion/2 XRM";
            one.sUserInitials = "MF";
            one.sUserName = "Mercyful Fate";
            one.dtDatePosted = std::time(nullptr); // = getCurrentISO8601Time();

            oneLineDao.insertRecord(one);
        }
    }

    log.log(UtilLog::LogLevel::Console, "Database Startup Check completed.");

    return true;
}
