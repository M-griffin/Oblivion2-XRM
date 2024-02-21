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
#include "data-sys/db_startup.hpp"

#include "communicator.hpp"
#include "interface.hpp"
#include "common_io.hpp"

std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_MENU_PROMPT_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string GLOBAL_SCRIPT_PATH = "";
std::string GLOBAL_LOG_PATH = "";
std::string USERS_DATABASE = "";

std::map<std::string, std::string> INPUT_SEQUENCE_MAP;

std::string BUILD_INFO = "Oblivion/2 XRM-Server rev.2 build [00.00.153] Alpha Preview";

/**
 * @brief Gracefull Shutdown Method.
 */
void atExitFunction()
{    
    std::cout << std::endl << "XRM SHUTDOWN COMPLETED!" << std::endl;
}


/**
 * @brief Setup a Static GLobal Map for Key Input that can be resued.
 */
void popuateInputSequenceMap() 
{
    // Arrow Keys, Hardware OA are translated to [A on the fly
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[A",    "up_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[B",    "dn_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[C",    "rt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[D",    "lt_arrow"));

    // Hardware Keys, or Num pad.
    INPUT_SEQUENCE_MAP.insert(std::make_pair("OA",    "up_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("OB",    "dn_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("OC",    "rt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("OD",    "lt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("OE",    "clear"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("OF",    "end"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("OH",    "home"));

    // Shift Arrow Keys
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[1;2A", "shift_up_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[1;2B", "shift_dn_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[1;2C", "shift_rt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[1;2D", "shift_lt_arrow"));

    // Shift TAB
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[Z",    "shift_tab"));

    // Function Keys ANSI
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[@",    "insert"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[H",    "home"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[K",    "end"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[F",    "end")); // = 0F
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[V",    "pg_up"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[U",    "pg_dn"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OP",   "f1"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OQ",   "f2"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OR",   "f3"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OS",   "f4"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OT",   "f5"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[17~", "f6"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[18~", "f7"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[19~", "f8"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[20~", "f9"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[21~", "f10"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[23~", "f11"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[24~", "f12"));

    // VT-100 Putty
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[1~",   "home"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[2~",   "insert"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[3~",   "del"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[4~",   "end"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[5~",   "pg_up"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[6~",   "pg_dn"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OU",   "f6"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OV",   "f7"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OW",   "f8"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OX",   "f9"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OY",   "f10"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[OZ",   "f11"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[O[",   "f12"));

    // Linux Console
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[A",   "f1"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[B",   "f2"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[C",   "f3"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[D",   "f4"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[E",   "f5"));

    // SCO
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[L",    "insert"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[I",    "pg_up"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[G",    "pg_dn"));

    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[M",   "f1"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[N",   "f2"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[O",   "f3"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[P",   "f4"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[Q",   "f5"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[R",   "f6"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[S",   "f7"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[T",   "f8"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[U",   "f9"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[V",   "f10"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[W",   "f11"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[[X",   "f12"));

    // rxvt
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[7~",   "home"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[8~",   "end"));

    // Shift Arrow Keys
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[a",    "shift_up_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[b",    "shift_dn_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[c",    "shift_rt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[d",    "shift_lt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[e",    "shift_clear"));

    // Shift Function
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[2$",   "insert"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[3$",   "del"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[5$",   "pg_up"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[6$",   "pg_dn"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[7$",   "home"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[8$",   "end"));

    // Ctrl
    INPUT_SEQUENCE_MAP.insert(std::make_pair("Oa",    "ctrl_up_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("Ob",    "ctrl_dn_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("Oc",    "ctrl_rt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("Od",    "ctrl_lt_arrow"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("Oe",    "ctrl_clear"));

    // Shift Function
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[2^",   "ctrl_insert"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[3^",   "ctrl_del"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[5^",   "ctrl_pg_up"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[6^",   "ctrl_pg_dn"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[7^",   "ctrl_home"));
    INPUT_SEQUENCE_MAP.insert(std::make_pair("[8^",   "ctrl_end"));
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

    // Isolate to code block for smart pointer deallocation.
    {
        // Setup Input Sequence for CommonIO
        popuateInputSequenceMap();
        
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
