/**
 * Oblivion/2 XRM rev.2 (c) 2015-2023 Michael Griffin
 * A Telnet Server and BBS system modeled after Oblivion/2 bbs software.
 *
 * XRM = Extreme Remake!
 * Compiles under MingW32/64 g++ >= 5.1.0
 *
 * LIBS:
 * Sqlite3
 * YamlC++
 * Unittest++
 * OpenSSL
 * linux
 *   uuid
 * windows
 *   Rpcrt4
 *
 * List All Exit Error Codes here.
 * Error Exit Codes (1) Unable to Load Configuration File.
 * Error Exit Codes (2) Unable to use Fallback IPv4 Acceptor (Accept Connections).
 */

#include "model-sys/structures.hpp"
#include "model-sys/config.hpp"
#include "data-sys/config_dao.hpp"
#include "data-sys/db_startup.hpp"

#include "communicator.hpp"
#include "interface.hpp"
#include "common_io.hpp"

#include <string>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <exception>

#ifndef _WIN32
#include <sys/stat.h>
#else
#include <direct.h>
#endif

std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_MENU_PROMPT_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string GLOBAL_SCRIPT_PATH = "";
std::string GLOBAL_LOG_PATH = "";
std::string USERS_DATABASE = "";


/**
 * @brief Gracefull Shutdown Method.
 */
void atExitFunction()
{    
    Communicator::getInstance().releaseConfiguration();
    std::cout << std::endl << "XRM SHUTDOWN COMPLETED!" << std::endl;
}

/**
 * @brief Main Program Entrance.
 *        Not using Parameters at this time.  Enable lateron.
 * @return
 */
auto main() -> int
{
    // Setup Cleanup method when program exits.
    std::atexit(atExitFunction);

    std::cout << "Oblivion/2 XRM-Server rev.2(c) 2015-2023 Michael Griffin."
              << std::endl
              << std::endl;

    {
        CommonIO common;
        GLOBAL_BBS_PATH = common.getProgramPath("xrm-server");
    }
    std::cout << "BBS HOME Directory Registered: " << std::endl;
    std::cout << GLOBAL_BBS_PATH << std::endl;

    // Setup System Folder Paths off main BBS Path.
    GLOBAL_DATA_PATH = GLOBAL_BBS_PATH + "DATA";
    GLOBAL_MENU_PATH = GLOBAL_BBS_PATH + "MENU";
    GLOBAL_MENU_PROMPT_PATH = GLOBAL_BBS_PATH + "MPROMPT";
    GLOBAL_TEXTFILE_PATH = GLOBAL_BBS_PATH + "TEXTFILE";
    GLOBAL_SCRIPT_PATH = GLOBAL_BBS_PATH + "SCRIPTS";
    GLOBAL_LOG_PATH = GLOBAL_BBS_PATH + "LOGS";

#ifdef _WIN32

    // Create LOG Directory if it doesn't exist.
    if(_mkdir(GLOBAL_LOG_PATH.c_str()) != 0 && errno != EEXIST)
    {
        std::cout << "Unable to create LOG folder: " << GLOBAL_LOG_PATH << std::endl;
    }

#else

    // Create LOG Directory if it doesn't exist.
    if(mkdir(GLOBAL_LOG_PATH.c_str(), 0770) == -1 && errno != EEXIST)
    {
        std::cout << "Unable to create LOG folder: " << GLOBAL_LOG_PATH << std::endl;
    }

#endif  

    // Loading and saving default Configuration file to XML
    {
        config_ptr config(new Config());

        if(!config)
        {
            std::cout << "Unable to allocate config structure" << std::endl;
            exit(1);
        }

        // Handle to Data Access Object,  at the moment were not using directories
        // Setup in the config, everything is branched from the main path.
        // Later on we'll check config for overrides only.
        ConfigDao cfg(config, GLOBAL_BBS_PATH);

        if(!cfg.fileExists())
        {
            cfg.saveConfig(config);
        }

        // Load Config and lets do some validation
        cfg.loadConfig();

        if(!cfg.validation())
        {
            std::cout << "Unable to validate config structure" << std::endl;
            exit(1);
        }

        // All Good, Attached to Global Communicator Instance.
        Communicator::getInstance().attachConfiguration(config);
        std::cout << "Starting up Oblivion/2 XRM-Server" << std::endl;
    }
    
    // Database Startup in it's own context.
    {
        db_startup_ptr db(new DbStartup());
        bool db_startup = db->initDatabaseTables();

        // Write all error logs and exit.
        if(!db_startup)
        {
            std::cout << "Database Startup failed, exiting..." << std::endl;
            return 0;
        }
    }

    // Isolate to code block for smart pointer deallocation.
    {
        // Create Handles to Services, and starts up connection listener and ASIO Thread Worker
        IOService io_service;        
        int port = Communicator::getInstance().getConfiguration()->port_telnet;
        std::cout << "Reading Configuration Telnet Port=" << port << std::endl;            
        interface_ptr setupAndRunAsioServer(new Interface(io_service, "TELNET", port));
        
        
        while(io_service.isActive()) 
        {            
            std::string line;
            std::getline(std::cin, line);
            std::cout << "line read: [" << line << "]" << std::endl;
            
            /**
             * Clean Shutdown, (2) Steps at this time from Console Commands.
             * 
             * 1. Kill - All Connections, so all sessions cleanly shutown, should wait at least 10 seconds.
             * 2. Quit - Stops IO Worker Service and All Async Jobs and listeners
             * 
             * If we try to do both of these Kill isn't finished then we get leaks on quit.
             * Which has to be looked into more in a single smpoother process.
             * 
             * Works for Now.
             */
            if (line == "kill") {
                std::cout << "Killing All Connections before exiting..." << std::endl;
                setupAndRunAsioServer->shutdown();
            }
            
            if (line == "quit") {
                std::cout << "Shutting down IOservice, exiting..." << std::endl;
                io_service.stop();
            }
          
            // Timer, for cpu usage
            std::this_thread::sleep_for(std::chrono::milliseconds(40));            
        }

    }

    return 0;
}
