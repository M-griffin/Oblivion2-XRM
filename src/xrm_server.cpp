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
    
    Logging &m_log = Logging::getInstance();
    m_log.write<Logging::CONSOLE_LOG>("Oblivion/2 XRM-Server rev.2(c) 2015-2023 Michael Griffin.");

    {
        CommonIO common;
        GLOBAL_BBS_PATH = common.getProgramPath("xrm-server");
    }
    m_log.write<Logging::CONSOLE_LOG>("BBS HOME Directory Registered=", GLOBAL_BBS_PATH);

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
        m_log.write<Logging::WARN_LOG>("Unable to create LOG folder=", GLOBAL_LOG_PATH);
    }

#else

    // Create LOG Directory if it doesn't exist.
    if(mkdir(GLOBAL_LOG_PATH.c_str(), 0770) == -1 && errno != EEXIST)
    {
        m_log.write<Logging::WARN_LOG>("Unable to create LOG folder=", GLOBAL_LOG_PATH);
    }

#endif  

    // Loading and saving default Configuration file to XML
    {
        config_ptr config = std::make_shared<Config>();
        
        if(!config)
        {
            m_log.write<Logging::ERROR_LOG>("Unable to allocate config object");
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
            m_log.write<Logging::ERROR_LOG>("Config Object validation failed!");
            exit(1);
        }

        // All Good, Attached to Global Communicator Instance.
        Communicator::getInstance().attachConfiguration(config);
        m_log.write<Logging::CONSOLE_LOG>("Starting up Oblivion/2 XRM-Server");
    }
    
    // Database Startup in it's own context.
    {
        db_startup_ptr db = std::make_shared<DbStartup>();
        bool db_startup = db->initDatabaseTables();

        // Write all error logs and exit.
        if(!db_startup)
        {
            m_log.write<Logging::ERROR_LOG>("Database Startup failed, exiting...");
            return 0;
        }
    }

    // Isolate to code block for smart pointer deallocation.
    {
        // Create Handles to Services, and starts up connection listener and ASIO Thread Worker
        IOService io_service;
        int port = Communicator::getInstance().getConfiguration()->port_telnet;
        std::string logging_level = Communicator::getInstance().getConfiguration()->logging_level;
        Logging::getInstance().setLoggingLevel(logging_level);
        
        // Testing Main Loop without returning.
        auto setupAndRunAsioServer = std::make_shared<Interface>(io_service, "TELNET", port);

        /*
        while(io_service.isActive()) 
        {            
            std::string line;
            std::getline(std::cin, line);
            
            
             * Clean Shutdown, (2) Steps at this time from Console Commands.
             * 
             * 1. Kill - All Connections, so all sessions cleanly shutown, should wait at least 10 seconds.
             * 2. Quit - Stops IO Worker Service and All Async Jobs and listeners
             * 
             * If we try to do both of these Kill isn't finished then we get leaks on quit.
             * Which has to be looked into more in a single smpoother process.
             * 
             * Works for Now.
             *
            if (line == "kill") {
                m_log.write<Logging::INFO_LOG>("Killing All Connections before exiting...");
                setupAndRunAsioServer->shutdown();
            }
            
            if (line == "quit") {
                m_log.write<Logging::INFO_LOG>("Shutting down IOservice, exiting...");
                io_service.stop();
            }
          
            // Timer, for cpu usage
            std::this_thread::sleep_for(std::chrono::milliseconds(40));            
        }*/

    }

    return 0;
}
