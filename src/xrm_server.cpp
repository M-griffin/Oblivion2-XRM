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

#include "model-sys/structures.hpp"
#include "model-sys/config.hpp"
#include "data-sys/config_dao.hpp"
//#include "data-sys/db_startup.hpp"

//#include "communicator.hpp"
#include "common_io.hpp"
#include "logging.hpp"
#include "tcp_server.hpp"

std::string GLOBAL_BBS_PATH;
std::string GLOBAL_DATA_PATH;
std::string GLOBAL_MENU_PATH;
std::string GLOBAL_MENU_PROMPT_PATH;
std::string GLOBAL_TEXTFILE_PATH;
std::string GLOBAL_SCRIPT_PATH;
std::string GLOBAL_LOG_PATH;
std::string USERS_DATABASE;

std::string BUILD_INFO = "Oblivion/2 XRM-Server rev.3 build [00.00.00] Alpha Preview";

/**
 * @brief Gracefully Shutdown Method.
 */
void atExitFunction() {
    std::cout << std::endl << "XRM SHUTDOWN COMPLETED!" << std::endl;
}

/**
 * @brief Main Program Entrance.
 *        Not using Parameters at this time.  Enable lateron.
 * @return
 */
auto main() -> int {
    // Setup Cleanup method when program exits.
    std::atexit(atExitFunction);

    Logging &m_log = Logging::getInstance();
    m_log.write<Logging::CONSOLE_LOG>(BUILD_INFO); {
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
    if (_mkdir(GLOBAL_LOG_PATH.c_str()) != 0 && errno != EEXIST) {
        m_log.write<Logging::WARN_LOG>("Unable to create LOG folder=", GLOBAL_LOG_PATH);
    }

#else

    // Create LOG Directory if it doesn't exist.
    if(mkdir(GLOBAL_LOG_PATH.c_str(), 0770) == -1 && errno != EEXIST)
    {
        m_log.write<Logging::WARN_LOG>("Unable to create LOG folder=", GLOBAL_LOG_PATH);
    }

#endif

    // Database Startup in it's own context.
    {
        /*
                DbStartup db;
                bool db_startup = db.initDatabaseTables();

                // Write all error logs and exit.
                if (!db_startup) {
                    m_log.write<Logging::ERROR_LOG>("Database Startup failed, exiting...");
                    exit(1);
                }
            }*/

        // Isolate to code block for smart pointer deallocation.
        {
            Config config;
            ConfigDao cfg(config, GLOBAL_BBS_PATH);

            if (!cfg.fileExists()) {
                cfg.saveConfig(config);
            }

            // Load Config and lets do some validation
            cfg.loadConfig();

            if (!cfg.validation()) {
                m_log.write<Logging::ERROR_LOG>("Config Object validation failed!");
                exit(1);
            }

            Uint16 maxClients = 10;
            Logging::getInstance().setLoggingLevel(config.logging_level);

            m_log.write<Logging::CONSOLE_LOG>("Starting up Oblivion/2 XRM-Server", "port", config.port_telnet,
                "max_sessions", maxClients);

            TcpServer server(config.port_telnet, maxClients);
            if (server.start()) {
                server.run();
            } else {
                m_log.write<Logging::ERROR_LOG>("TCP Startup failed, exiting...");
            }
        }

        exit(0);
    }
}
