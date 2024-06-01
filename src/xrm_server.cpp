/**
 * Oblivion/2 XRM rev.3 (c) 2015-2024 Michael Griffin
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

#include <map>
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

#include <boost/asio.hpp>

#include "model-sys/structures.hpp"
#include "model-sys/config.hpp"
#include "data-sys/config_dao.hpp"
#include "data-sys/db_startup.hpp"

#include "communicator.hpp"
#include "server.hpp"
#include "common_io.hpp"
#include "logging.hpp"

std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_MENU_PROMPT_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string GLOBAL_SCRIPT_PATH = "";
std::string GLOBAL_LOG_PATH = "";
std::string USERS_DATABASE = "";


std::string BUILD_INFO = "Oblivion/2 XRM-Server rev.3 build [00.00.154] Alpha Preview";

typedef std::shared_ptr<Server> server_telnet_ptr;

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
    m_log.write<Logging::CONSOLE_LOG>(BUILD_INFO);

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
    
    {
        
        config_ptr config = std::make_shared<Config>();
        
        if(!config)
        {
            m_log.write<Logging::ERROR_LOG>("Unable to allocate config object");
            exit(1);
        }
        
        ConfigDao cfg(config, GLOBAL_BBS_PATH);

        if(!cfg.fileExists())
        {
            m_log.write<Logging::ERROR_LOG>("Config File doesn't exist");
        }

        // Load Config and lets do some validation
        cfg.loadConfig();
                        
        // Startup Telnet Server
        server_telnet_ptr serverTelnet;
        boost::asio::io_service io_service;
        
        if (cfg.m_config->use_service_telnet)
        {
            std::cout << "Listening for telnet connections on port "
                      << cfg.m_config->port_telnet << std::endl;
                      
            server_telnet_ptr serverTelnet = std::make_shared<Server>(io_service, cfg.m_config->port_telnet);
            
            while(1)
            {
                try 
                {
                    io_service.run();
                    // Normal Exit of Loop, only loops when an Encaught Exception Occurs from possible Async Operations.
                    break;
                }
                catch(std::exception &e)
                {
                    m_log.write<Logging::ERROR_LOG>("Uncaught Exception - Restarting io_service.run()", e.what());
                }                
            }
        }
        
        std::cout << "Exting..." << std::endl;
        io_service.stop();
    }
    

    return 0;
}
