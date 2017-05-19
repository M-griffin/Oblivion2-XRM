#include "db_startup.hpp"

#include "../model-sys/structures.hpp"
#include "../model-sys/protocol.hpp"
#include "../model-app/oneliners.hpp"

#include "base_dao.hpp"
#include "protocol_dao.hpp"
#include "../data-app/oneliners_dao.hpp"

// Needed for Initializing and checking users data is setup
// On startup.
#include "session_stats_dao.hpp"
#include "security_dao.hpp"
#include "users_dao.hpp"

#include "libSqliteWrapped.h"

DbStartup::DbStartup()
{
}

DbStartup::~DbStartup()
{
}

/**
 * @brief Setup for Database and Tables
 */
void DbStartup::initDatabaseTables()
{

    // Setup Users Database name and path
    std::string USERS_DATABASE = GLOBAL_DATA_PATH;

#ifdef _WIN32
    USERS_DATABASE.append("\\");
#else
    USERS_DATABASE.append("/");
#endif

    USERS_DATABASE.append("xrm_users.sqlite3");

    // Setup isolated scope for smart pointers and clean up.
    {
        // Check and Setup users database if tables are not setup
        SQLW::Database user_database(USERS_DATABASE);

        // Link to users dao for data access object
        UsersDao user_dao(user_database);

        // Link to security dao for data access object
        SecurityDao security_dao(user_database);

        // Verify if the security table exists.
        // Security must be present before user becasue of foreign key.
        if (!security_dao.doesTableExist())
        {
            std::cout << "doesn't exist (security table)." << std::endl;

            // Setup database Param, cache sies etc..
            if (!security_dao.firstTimeSetupParams())
            {
                std::cout << "unable to execute firstTimeSetupParams (security table)." << std::endl;
                assert(false);
            }

            // Setup create users table and indexes.
            if (!security_dao.createTable())
            {
                std::cout << "unable to create (security table)." << std::endl;
                assert(false);
            }

            std::cout << "security table created successfully." << std::endl;
        }

        // Verify if the user table exists.
        if (!user_dao.doesTableExist())
        {
            std::cout << "doesn't exist (user table)." << std::endl;

            // Setup database Param, cache sies etc..
            if (!user_dao.firstTimeSetupParams())
            {
                std::cout << "unable to execute firstTimeSetupParams (user table)." << std::endl;
                assert(false);
            }

            // Setup create users table and indexes.
            if (!user_dao.createTable())
            {
                std::cout << "unable to create (user table)." << std::endl;
                assert(false);
            }

            std::cout << "user table created successfully." << std::endl;
        }

        // Check Table setup for Session Stats
        SessionStatsDao session_stat_dao(user_database);
        // Verify if the user table exists.
        if (!session_stat_dao.doesTableExist())
        {
            std::cout << "doesn't exist (sessionstats table)." << std::endl;

            // Setup database Param, cache sies etc..
            if (!session_stat_dao.firstTimeSetupParams())
            {
                std::cout << "unable to execute firstTimeSetupParams (sessionstats table)." << std::endl;
                assert(false);
            }

            // Setup create users table and indexes.
            if (!session_stat_dao.createTable())
            {
                std::cout << "unable to create (sessionstats table)." << std::endl;
                assert(false);
            }

            std::cout << "sessionstats table created successfully." << std::endl;
        }

        

        protocols_ptr prots(new Protocols());
        ProtocolDao protdb(prots, GLOBAL_DATA_PATH);

        if (!protdb.fileExists())
        {
            // Create Genric Protocol Entry to Test File Creation
            Protocol p1("Sexyz", "D", "Z", "C:\\TESTPATH\\", "--Test", false, false);

            prots->protocols.push_back(p1);
            protdb.saveConfig(prots);
        }
        
        
        OnelinerDao onedb(user_database);
        
        if (!onedb.doesTableExist())
        {
            std::cout << "doesn't exist (oneliner table)." << std::endl;

            // Setup database Param, cache sies etc..
            if (!onedb.firstTimeSetupParams())
            {
                std::cout << "unable to execute firstTimeSetupParams (oneliner table)." << std::endl;
                assert(false);
            }

            // Setup create users table and indexes.
            if (!onedb.createTable())
            {
                std::cout << "unable to create (oneliner table)." << std::endl;
                assert(false);
            }

            std::cout << "oneliner table created successfully." << std::endl;
        }

        oneliner_ptr one(new Oneliners());
        one->iUserId = 1;
        one->sText = "Testing";
        one->sUserInitials = "MF";
        one->sUserName = "Mercyful Fate";
        //one->dtDatePosted
        onedb.insertRecord(one);
        
        
        /*
        // Check and Setup default Access Levels.
        access_level_ptr levels(new AccessLevels());
        AccessLevelDao acl(levels, GLOBAL_DATA_PATH);

        if (!acl.fileExists())
        {
            // Create Genric levels for First Time Startup
            Level al1(10,  "unauthorized");
            Level al2(20,  "validated");
            Level al3(255, "sysop");

            levels->access_levels.push_back(al1);
            levels->access_levels.push_back(al2);
            levels->access_levels.push_back(al3);

            acl.saveConfig(levels);
        }
        */
    }


    
}
