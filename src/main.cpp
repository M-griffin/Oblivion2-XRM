/**
 * Oblivion/2 XRM (c) 2015-2018 Michael Griffin
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
 * lunux
 *   uuid
 * windows
 *   Rpcrt4
 *
 * List All Exit Error Codes here.
 * Error Exit Codes (1) Unable to Load Configuration File.
 * Error Exit Codes (2) Unable to use Fallback IPv4 Acceptor (Accept Connections).
 */


#include "data-sys/text_prompts_dao.hpp"
#include "model-sys/config.hpp"
#include "data-sys/config_dao.hpp"
#include "data-sys/db_startup.hpp"

#include "interface.hpp"
#include "communicator.hpp"
#include "common_io.hpp"

#include <memory>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <exception>

std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_MENU_PROMPT_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string GLOBAL_SCRIPT_PATH = "";
std::string USERS_DATABASE = "";


/**
 * @brief Main Program Entrance.
 *        Not using Parameters at this time.  Enable lateron.
 * @return
 */
// auto main(int argc, char* argv[]) -> int
auto main() -> int
{
    std::cout << "Oblivion/2 XRM Server (c) 2015-2018 Michael Griffin."
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

    // Loading and saving default Configuration file to XML
    {
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

        // Load Config and lets do some validation
        cfg.loadConfig();
        if (!cfg.validation())
        {
            return 0;
        }
    }

    // Database Startup in it's own context.
    {
        db_startup_ptr db(new DbStartup());
        db->initDatabaseTables();
    }

    // Initial Config File Read, and Start ASIO Server.
    {
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
        if (!cfg.loadConfig())
        {
            // TODO Throws exception right now, need to work in
            // better shutdown on from this point! just assert for now.
            exit(1);
        }

        // TODO, from rework, right now single asio server is setup,
        // One we have SSH server setup we can split this up again.
        if (cfg.m_config->use_service_telnet)
        {
            std::cout << "Setting up telnet connections on port "
            << cfg.m_config->port_telnet << std::endl;
        }

        // Isolate to code block for smart pointer deallocation.
        {
            // Create Handles to Services, and starts up connection listener and ASIO Thread Worker
            IOService io_service;
            interface_ptr setupAndRunAsioServer(new Interface(io_service, "TELNET", cfg.m_config->port_telnet));

            while(TheCommunicator::instance()->isActive())
            {
                // Main Thread - While system is active loop,  This will be external event processor
                // Or notifications, etc.. lets see what else we want to do here.

                // Timer, for cpu useage
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
    }

    // Release Communicator Instance
    TheCommunicator::releaseInstance();
    return 0;
}
