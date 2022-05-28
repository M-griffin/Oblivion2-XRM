/**
 * Oblivion/2 XRM (c) 2015-2019 Michael Griffin
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
#include "data-sys/text_prompts_dao.hpp"
#include "data-sys/config_dao.hpp"
#include "data-sys/db_startup.hpp"

#include "interface.hpp"
#include "communicator.hpp"
#include "common_io.hpp"
#include "encoding.hpp"
#include "logging.hpp"

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

Logging* Logging::m_global_logging_instance = nullptr;


/**
 * @brief Gracefull Shutdown Method.
 */
void atExitFunction()
{
    // Check for any remaining LOG Writes, then exit gracefully.
    if(Logging::isActive())
    {
        int log_entries = Logging::instance()->getNumberOfLogEntries();

        for(int i = 0; i < log_entries; i++)
        {
            log_entry_ptr entry = Logging::instance()->getLogQueueEntry();

            if(entry != nullptr)
            {
                Logging::instance()->writeOutYamlFile(entry);
            }
        }
    }

    TheCommunicator::releaseInstance();
    Encoding::releaseInstance();
    Logging::releaseInstance();
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

    std::cout << "Oblivion/2 XRM-Server (c) 2015-2019 Michael Griffin."
              << std::endl
              << std::endl;

    CommonIO common;
    GLOBAL_BBS_PATH = common.getProgramPath("xrm-server");
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
    // Startup the Encoding Instance and Char Mappings.
    {
        Encoding::instance();
    }

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
        // This also controls logging, need to start this prior to any
        // any logging objects being used.
        TheCommunicator::instance()->attachConfiguration(config);
        Logging::instance()->xrmLog<Logging::CONSOLE_LOG>("Starting up Oblivion/2 XRM-Server");
    }

    // Database Startup in it's own context.
    {
        db_startup_ptr db(new DbStartup());
        bool db_startup = db->initDatabaseTables();

        // Write all error logs and exit.
        if(!db_startup)
        {
            // DataBase Startup Failed
            std::cout << "Database Startup failed, writting system logs" << std::endl;
            int log_entries = Logging::instance()->getNumberOfLogEntries();

            for(int i = 0; i < log_entries; i++)
            {
                log_entry_ptr entry = Logging::instance()->getLogQueueEntry();

                if(entry != nullptr)
                {
                    Logging::instance()->writeOutYamlFile(entry);
                }
            }
        }
    }

    // Isolate to code block for smart pointer deallocation.
    {
        // Create Handles to Services, and starts up connection listener and ASIO Thread Worker
        config_ptr config = TheCommunicator::instance()->getConfiguration();

        IOService io_service;
        interface_ptr setupAndRunAsioServer(new Interface(io_service, "TELNET", config->port_telnet));

        while(TheCommunicator::instance()->isActive())
        {
            // Main Thread - While system is active loop,  This will be external event processor
            // Or notifications, etc.. lets see what else we want to do here.
            int log_entries = Logging::instance()->getNumberOfLogEntries();

            for(int i = 0; i < log_entries; i++)
            {
                log_entry_ptr entry = Logging::instance()->getLogQueueEntry();

                if(entry != nullptr)
                {
                    Logging::instance()->writeOutYamlFile(entry);
                }
            }

            // Timer, for cpu usage
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
    }

    return 0;
}
