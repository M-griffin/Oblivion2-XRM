/**
 * Oblivion/2 XRM (c) 2015-2025 Michael Griffin
 * A Telnet Server and BBS system modeled after Oblivion/2 bbs software.
 *
 * XRM = Extreme Remake!
 * Compiles under MingW32/64 g++ >= 5.1.0
 * Now using CMAKE
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
 * Error Exit Codes (2) Unable to Startup Networking
 */

#include <string>
#include <cstdlib>
#include <iostream>

#ifndef _WIN32
#include <sys/stat.h>
#else
#include <direct.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <csignal>
#include <functional>
#include <unordered_map>
#endif

#include "model-sys/structures.hpp"
#include "model-sys/config.hpp"
#include "data-sys/config_dao.hpp"

#include "common_io.hpp"
#include "logging.hpp"
#include "tcp_server.hpp"
#include "data-sys/db_startup.hpp"

std::string GLOBAL_BBS_PATH;
std::string GLOBAL_DATA_PATH;
std::string GLOBAL_MENU_PATH;
std::string GLOBAL_MENU_PROMPT_PATH;
std::string GLOBAL_TEXTFILE_PATH;
std::string GLOBAL_SCRIPT_PATH;
std::string GLOBAL_LOG_PATH;
std::string CORE_DATABASE;

std::string BUILD_INFO = "Oblivion/2 XRM-Server Rev.3 build [00.03.01] Alpha Preview";

/**
 * @brief Gracefully Shutdown Method.
 */
void atExitFunction() {
    std::cout << std::endl << "XRM SHUTDOWN COMPLETED!" << std::endl;
}

#ifdef _WIN32
// Windows requires a static callback
static BOOL WINAPI CtrlHandler(DWORD ctrlType) {
    std::cout << "\nSignal received: " << ctrlType << std::endl;

    if (ctrlType == CTRL_C_EVENT) {
        std::cout << "\n[CTRL+C] Graceful shutdown requested" << std::endl;
        TCPServer::getInstance().stop();
        std::exit(0);
    }
    if (ctrlType == CTRL_BREAK_EVENT) {
        std::cout << "\n[CTRL+BREAK] Graceful shutdown requested" << std::endl;
        TCPServer::getInstance().stop();
        std::exit(0);
    }
    if (ctrlType == CTRL_CLOSE_EVENT) {
        std::cout << "\n[CTRL+CLOSE] Graceful shutdown requested" << std::endl;
        TCPServer::getInstance().stop();
        std::exit(0);
    }
    if (ctrlType == CTRL_LOGOFF_EVENT) {
        std::cout << "\n[CTRL+LOGOFF] Graceful shutdown requested" << std::endl;
        TCPServer::getInstance().stop();
        std::exit(0);
    }
    if (ctrlType == CTRL_SHUTDOWN_EVENT) {
        std::cout << "\n[CTRL+SHUTDOWN] Graceful shutdown requested" << std::endl;
        TCPServer::getInstance().stop();
        std::exit(0);
    }

    return FALSE;
}
#endif

bool setupSignalHandlers() {
#ifdef _WIN32
    if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        std::cerr << "[Server] Failed to set Windows CTRL handler" << std::endl;
        return false;
    }
#else
    // Linux/Unix: Use sigaction for reliable signal handling
    struct sigaction sa {};
    sa.sa_handler = [](int signal) {
        if (signal == SIGINT || signal == SIGTERM) {
            // Use a global pointer or static registry if truly needed
            std::cout << "\n[SIGINT] Graceful shutdown requested" << std::endl;
            TCPServer::getInstance().stop();
            std::exit(0);
        }
    };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, nullptr) < 0) {
        std::cerr << "[Server] Failed to set SIGINT handler\n";
        return false;
    }

    if (sigaction(SIGTERM, &sa, nullptr) < 0) {
        std::cerr << "[Server] Failed to set SIGTERM handler\n";
        return false;
    }
#endif
    return true;
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
    m_log.log(Logging::LogLevel::Console, BUILD_INFO); {
        CommonIO common;
        GLOBAL_BBS_PATH = common.getProgramPath("xrm-server");
    }

    m_log.log(Logging::LogLevel::Console, "BBS HOME Directory Registered=", GLOBAL_BBS_PATH);

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
        m_log.log(Logging::LogLevel::Warn, "Unable to create LOG folder=", GLOBAL_LOG_PATH);
    }

#else

    // Create LOG Directory if it doesn't exist.
    if(mkdir(GLOBAL_LOG_PATH.c_str(), 0770) == -1 && errno != EEXIST)
    {
        m_log.log(Logging::LogLevel::Warn, "Unable to create LOG folder=", GLOBAL_LOG_PATH);
    }

#endif

    m_log.log(Logging::LogLevel::Console, "Checking Database SQLite");

    // Database Startup in its own context.
    {
        DBStartUp db;
        bool db_startup = db.initDatabaseTables();

        // Write all error logs and exit.
        if (!db_startup) {
            m_log.log(Logging::LogLevel::Error, "Database Startup failed, exiting...");
            exit(1);
        }
    }


    // Isolate to code block for smart pointer deallocation.
    {
        Config config;
        ConfigDao cfg(config, GLOBAL_BBS_PATH);

        if (!cfg.fileExists()) {
            cfg.saveConfig(config);
        }

        // Load Config and let's do some validation
        cfg.loadConfig();

        if (!cfg.validation()) {
            m_log.log(Logging::LogLevel::Error, "Config Object validation failed!");
            exit(1);
        }

        // TODO Setup from Config File!
        const Uint16 maxSessions = 255;
        Logging::getInstance().setLogLevelFromString(config.logging_level);

        m_log.log(Logging::LogLevel::Console, "Starting up XRM-Server", "port", config.port_telnet,
                                          "max_sessions", maxSessions);

        if (!setupSignalHandlers()) {
            m_log.log(Logging::LogLevel::Error, "XRM-Server Startup failed setting signal handlers, exiting...");
            exit(2);
        }

        TCPServer &server = TCPServer::getInstance();
        if (server.start(config.port_telnet, maxSessions)) {
            server.run(config, maxSessions);
        } else {
            m_log.log(Logging::LogLevel::Error, "XRM-Server Startup failed, exiting...");
            exit(2);
        }
    }

    exit(0);
}
