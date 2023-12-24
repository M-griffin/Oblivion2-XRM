#include "db_startup.hpp"

#include "../model-sys/structures.hpp"
#include "../model-sys/protocol.hpp"
#include "../model-sys/access_level.hpp"
#include "../model-app/oneliners.hpp"

#include "base_dao.hpp"
#include "protocol_dao.hpp"
#include "access_level_dao.hpp"
#include "../data-app/oneliners_dao.hpp"
#include "../logging.hpp"

// Needed for Initializing and checking users data is setup
// On startup.
#include "session_stats_dao.hpp"
#include "security_dao.hpp"
#include "users_dao.hpp"

#include "libSqliteWrapped.h"

#include <cassert>

DbStartup::DbStartup()
{
}

DbStartup::~DbStartup()
{
}

/**
 * @brief Setup for Database and Tables
 */
bool DbStartup::initDatabaseTables()
{

    // Setup Users Database name and path
    USERS_DATABASE = GLOBAL_DATA_PATH;

#ifdef _WIN32
    USERS_DATABASE.append("\\");
#else
    USERS_DATABASE.append("/");
#endif

    USERS_DATABASE.append("xrm_users.sqlite3");

    Logging &log = Logging::getInstance();

    // Setup isolated scope for smart pointers and clean up.
    {
        // Check and Setup users database if tables are not setup
        SQLW::Database user_database(USERS_DATABASE);

        // Link to users dao for data access object
        UsersDao user_dao(user_database);

        // Link to security dao for data access object
        SecurityDao security_dao(user_database);

        // Verify if the security table exists.
        // Security must be present before user because of foreign key.
        if(!security_dao.doesTableExist())
        {
            log.write<Logging::CONSOLE_LOG>("doesn't exist (security table).");

            // Setup database Param, cache sizes etc..
            if(!security_dao.firstTimeSetupParams())
            {
                log.write<Logging::ERROR_LOG>("unable to execute firstTimeSetupParams (security table).");
                return(false);
            }

            // Setup create users table and indexes.
            if(!security_dao.createTable())
            {
                log.write<Logging::ERROR_LOG>("unable to create (security table).");
                return(false);
            }

            log.write<Logging::CONSOLE_LOG>("security table created successfully.");
        }

        // Verify if the user table exists.
        if(!user_dao.doesTableExist())
        {
            log.write<Logging::CONSOLE_LOG>("doesn't exist (user table).");

            // Setup database Param, cache sizes etc..
            if(!user_dao.firstTimeSetupParams())
            {
                log.write<Logging::ERROR_LOG>("unable to execute firstTimeSetupParams (user table).");
                return(false);
            }

            // Setup create users table and indexes.
            if(!user_dao.createTable())
            {
                log.write<Logging::ERROR_LOG>("unable to create (user table).");
                return(false);
            }

            log.write<Logging::CONSOLE_LOG>("user table created successfully.");
        }

        // Check Table setup for Session Stats
        SessionStatsDao session_stat_dao(user_database);

        // Verify if the user table exists.
        if(!session_stat_dao.doesTableExist())
        {
            log.write<Logging::CONSOLE_LOG>("doesn't exist (session stats table).");

            // Setup database Param, cache sies etc..
            if(!session_stat_dao.firstTimeSetupParams())
            {
                log.write<Logging::ERROR_LOG>("unable to execute firstTimeSetupParams (session stats table).");
                return(false);
            }

            // Setup create users table and indexes.
            if(!session_stat_dao.createTable())
            {
                log.write<Logging::ERROR_LOG>("unable to create (session stats table).");
                return(false);
            }

            log.write<Logging::CONSOLE_LOG>("session stats table created successfully.");
        }

        // Link to Access Level dao for data access object
        AccessLevelDao access_dao(user_database);

        // Verify if the access_level table exists.
        if(!access_dao.doesTableExist())
        {
            log.write<Logging::CONSOLE_LOG>("doesn't exist (access_level table).");

            // Setup database Param, cache sizes etc..
            if(!access_dao.firstTimeSetupParams())
            {
                log.write<Logging::ERROR_LOG>("unable to execute firstTimeSetupParams (access_level table).");
                return(false);
            }

            // Setup create users table and indexes.
            if(!access_dao.createTable())
            {
                log.write<Logging::ERROR_LOG>("unable to create (access_level table).");
                return(false);
            }

            log.write<Logging::CONSOLE_LOG>("access_level table created successfully.");

            // Check and Setup default Access Levels.
            access_level_ptr level = std::make_shared<AccessLevel>();

            // Set Initial Defaults for Not Validated Level
            // the reest are populated on Class Defaults.
            level->sName = "Not Validated";
            level->sStartMenu ="top";
            level->iLevel = 10;
            level->iTimeLimit = 120;
            level->bTimeLimit = true;

            access_dao.insertRecord(level);

            // Validated User
            level.reset();
            level = std::make_shared<AccessLevel>();
            level->sName = "Validated User";
            level->sStartMenu ="top";
            level->iLevel = 20;
            level->iTimeLimit = 1440;
            level->bTimeLimit = true;

            access_dao.insertRecord(level);

            // Administrator (time Limit false by default)
            level.reset();
            level = std::make_shared<AccessLevel>();
            level->sName = "Sysop";
            level->sStartMenu ="top";
            level->iLevel = 255;
            level->iTimeLimit = 1440;

            access_dao.insertRecord(level);
        }


        protocols_ptr prots = std::make_shared<Protocols>();
        ProtocolDao protdb(prots, GLOBAL_DATA_PATH);

        if(!protdb.fileExists())
        {
            log.write<Logging::CONSOLE_LOG>("Protocol configuration doesn't exist.");

            // Create Genric Protocol Entry to Test File Creation
            Protocol p1("Sexyz", "D", "Z", "C:\\TESTPATH\\", "--Test", false, false);

            prots->protocols.push_back(p1);
            protdb.saveConfig(prots);

            log.write<Logging::CONSOLE_LOG>("Protocol configuration created successfully");
        }


        OnelinerDao oneLineDao(user_database);

        if(!oneLineDao.doesTableExist())
        {
            log.write<Logging::CONSOLE_LOG>("doesn't exist (oneliner table).");

            // Setup database Param, cache sizes etc..
            if(!oneLineDao.firstTimeSetupParams())
            {
                log.write<Logging::ERROR_LOG>("unable to execute firstTimeSetupParams (oneliner table).");
                return(false);
            }

            // Setup create users table and indexes.
            if(!oneLineDao.createTable())
            {
                log.write<Logging::ERROR_LOG>("unable to create (oneliner table).");
                return(false);
            }

            log.write<Logging::CONSOLE_LOG>("oneliner table created successfully.");

            // Insert a default record the first time the table
            // is created only.
            oneliner_ptr one = std::make_shared<Oneliners>();
            one->iUserId = 1;
            one->sText = "Welcome to a new system running Oblivion/2 XRM";
            one->sUserInitials = "MF";
            one->sUserName = "Mercyful Fate";
            //one->dtDatePosted

            oneLineDao.insertRecord(one);
        }
    }

    return true;
}
