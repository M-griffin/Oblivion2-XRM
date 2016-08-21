/*
 * Oblivion/2 XRM (c) 2015-2016 Michael Griffin
 * A Telnet Server and BBS system modeled after Oblivion/2 bbs software.
 *
 * XRM = Extreme Remake!
 * Compiles under MingW32/64 5.1.0 g++
 *
 * LIBS:
 * Boost 58
 * ICU 4c-55_1
 * Sqlite3
 * SqliteWrapped
 *
 * List All Exit Error Codes here.
 * Error Exit Codes (1) Unable to Load Configuration File.
 * Error Exit Codes (2) Unable to use Fallback IPv4 Acceptor (Accept Connections).
 *
 */

#include "data/text_prompts_dao.hpp"

#include "model/config.hpp"
#include "data/config_dao.hpp"

#include "server.hpp"
#include "server_ssl.hpp"
#include "communicator.hpp"
#include "common_io.hpp"

// Needed for Initializing and checking users data is setup
// On startup.
#include "data/security_dao.hpp"
#include "data/users_dao.hpp"
#include "libSqliteWrapped.h"
//#include "menu_system.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <map>

// temp
#include <fstream>

//#include <cstdio>
//#include <sqlite3.h>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

// Temp!
std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string USERS_DATABASE = "";

using boost::asio::ip::tcp;

// Setup Handles to Services.
typedef boost::shared_ptr<Server>	 server_telnet_ptr;
typedef boost::shared_ptr<ServerSSL> server_ssl_ptr;

/* NOTES

    #Cap sockets to 512 on Windows because winsock can only process 512 at time
    if sys.platform == 'win32':
        MAX_CONNECTIONS = 500
    ## Cap sockets to 1000 on Linux because you can only have 1024 file descriptors
    else:
        MAX_CONNECTIONS = 1000
 */

/** NOT USED AT THIS TIME!
 * @brief Generic Timer, used for testing right now.
 * @param timer
 * @param count
 */

void handler(const boost::system::error_code& /*e*/,
             boost::asio::deadline_timer* timer, int* count)
{
    if(*count < 5)
    {
        std::cout << *count << "\n";
        ++(*count);

        // Reset next Timer.
        timer->expires_at(timer->expires_at() + boost::posix_time::milliseconds(250));
        timer->async_wait(
            boost::bind(handler, boost::asio::placeholders::error, timer, count));
    }
}

/**
 * @brief Main Program io_service loop
 * @param io_service
 */
void run(boost::asio::io_service& io_service)
{    
    // Create Handles to Services
    server_telnet_ptr serverTelnet;
    server_ssl_ptr    serverSSL;

    // Default Config Instance
    config_ptr config(new Config());
    if (!config)
    {
        std::cout << "Unable to allocate config structure" << std::endl;
        assert(false);
    }

    // Setup the Data Access Object
    //config_dao_ptr cfg(config, GLOBAL_BBS_PATH);
    ConfigDao cfg(config, GLOBAL_BBS_PATH);

    // Loads the Config file into the Data Access Object.
    cfg.loadConfig();
    
    // Service Startup Here.
    try
    {        
        // Startup Telnet Server
        if (cfg.m_config->use_service_telnet)
        {
            std::cout << "Listening for telnet connections on port "
                      << cfg.m_config->port_telnet << std::endl;
            serverTelnet.reset(new Server(io_service, cfg.m_config->port_telnet));
        }

        // Initial Testing of SSL Server.
        if (cfg.m_config->use_service_ssl)
        {
            std::cout << "Listening for service connections on port "
                      << cfg.m_config->port_ssl << std::endl;
            serverSSL.reset(new ServerSSL(io_service, cfg.m_config->port_ssl));
        }

        // Setup first timer.
        //    int count = 0;
        //    boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(1));
        //    timer.async_wait(
        //        boost::bind(handler, boost::asio::placeholders::error, &timer, &count));

        io_service.run();

        /* If we want to created extra helper threads to Enable later, don't need now in development!
         std::thread thread1{[&io_service](){ io_service.run(); }};
         std::thread thread2{[&io_service](){ io_service.run(); }};
         thread1.join();
         thread2.join();
        */

        // here we process all network (or other) operations associated with io_service
    }
    catch(std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}

/**
 * @brief Main Program Entrance.
 *        Not using Parameters at this time.  Enable lateron.
 * @return
 */
// auto main(int argc, char* argv[]) -> int
auto main() -> int
{
    std::cout << "Oblivion/2 XRM Server (c) 2015-2016 Michael Griffin." << std::endl;

    CommonIO common;
    GLOBAL_BBS_PATH = common.getProgramPath();
    std::cout << "BBS HOME Directory Registered: " << GLOBAL_BBS_PATH << std::endl;

    // Setup System Folder Paths off main BBS Path.
    GLOBAL_DATA_PATH = GLOBAL_BBS_PATH + "DATA";
    GLOBAL_MENU_PATH = GLOBAL_BBS_PATH + "MENU";
    GLOBAL_TEXTFILE_PATH = GLOBAL_BBS_PATH + "TEXTFILE";

    // Setup Users Database name and path
    USERS_DATABASE = GLOBAL_DATA_PATH;

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
        users_dao_ptr user_dao(new UsersDao(user_database));

        // Link to security dao for data access object
        security_dao_ptr security_dao(new SecurityDao(user_database));

        // Verify if the security table exists.
        // Security must be present before user becasue of foreign key.
        if (!security_dao->isTableExists())
        {
            std::cout << "doesn't exist (security table)." << std::endl;

            // Setup database Param, cache sies etc..
            if (!security_dao->firstTimeSetupParams())
            {
                std::cout << "unable to execute firstTimeSetupParams (security table)." << std::endl;
                assert(false);
            }

            // Setup create users table and indexes.
            if (!security_dao->createTable())
            {
                std::cout << "unable to create (security table)." << std::endl;
                assert(false);
            }

            std::cout << "security table created successfully." << std::endl;
        }

        // Verify if the user table exists.
        if (!user_dao->isTableExists())
        {
            std::cout << "doesn't exist (user table)." << std::endl;

            // Setup database Param, cache sies etc..
            if (!user_dao->firstTimeSetupParams())
            {
                std::cout << "unable to execute firstTimeSetupParams (user table)." << std::endl;
                assert(false);
            }

            // Setup create users table and indexes.
            if (!user_dao->createTable())
            {
                std::cout << "unable to create (user table)." << std::endl;
                assert(false);
            }

            std::cout << "user table created successfully." << std::endl;
        }
       
        // NEW Loading and saving default Configuration file to XML

        config_ptr config(new Config());
        if (!config)
        {
            std::cout << "Unable to allocate config structure" << std::endl;
            assert(false);
        }

        // Handle to Data Access Object,  at the moment were not using directories
        // Setup in the config, everything is branched from the main path.
        // Later on we'll check config for overides only.
        ConfigDao cfg(config, GLOBAL_BBS_PATH);

        if (!cfg.fileExists())
        {
            cfg.saveConfig(config);
        }

    }

    // Start System Services and Main Loop.
    boost::asio::io_service io_service;

    // Load BBS Configuration here into Global Singleton.
    //TheCommunicator::instance()->attachConfig(config);

    // start io_service.run( ) in separate thread
    auto t = std::thread(&run, std::ref(io_service));

    // Main Thread, Get Server Input from System Operator
    // We'll handle Node and System setup here.  Need to write interface.
    while(true)
    {
        // Just testing with text input, change this to state for commands
        // Or communication with a session or all sessions!
        std::string line;
        std::getline(std::cin, line);

        // process input, stop from Sysop/Local User will shutdown the server.
        // Just a temp for testing.
        if(line == "stop")
        {
            std::cout << "Shutting Down the Server" << std::endl;
            // Shutdown the Thread and merge it back.
            TheCommunicator::instance()->shutdown();
            io_service.stop();
            t.join();
            break;
        }
        else
        {
            // Sends Global Text to All Users connected.
            // Need to update this to pick 'either'
            // All or Individual Sessions.
            TheCommunicator::instance()->addMessageQueue(line);
            TheCommunicator::instance()->sendGlobalMessage();
        }
    }

    // Release Communicator Instance
    TheCommunicator::releaseInstance();
    return 0;
}
