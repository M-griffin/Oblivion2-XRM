/*
 * Oblivion/2 XRM (c) 2015-2018 Michael Griffin
 * A Telnet Server and BBS system modeled after Oblivion/2 bbs software.
 *
 * XRM = Extreme Remake!
 * Compiles under MingW32/64 5.1.0 g++
 *
 * LIBS:
 * Sqlite3
 * SqliteWrapped
 * YamlC++
 * Unittest++
 *
 * List All Exit Error Codes here.
 * Error Exit Codes (1) Unable to Load Configuration File.
 * Error Exit Codes (2) Unable to use Fallback IPv4 Acceptor (Accept Connections).
 *
 */


#include "data-sys/text_prompts_dao.hpp"
#include "model-sys/config.hpp"
#include "data-sys/config_dao.hpp"
#include "data-sys/db_startup.hpp"

#include "server.hpp"
#include "communicator.hpp"
#include "common_io.hpp"

#include <memory>
#include <cstdlib>
#include <iostream>

//#include <thread>
#include <pthread.h>

#include <map>
#include <exception>

// temp
#include <fstream>


// Temp!
std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_MENU_PROMPT_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string GLOBAL_SCRIPT_PATH = "";
std::string USERS_DATABASE = "";

// Setup Handles to Services.
typedef std::shared_ptr<Server> server_ptr;

/**
 * @brief Main Program io_service loop
 * @param io_service
 */
//void run(boost::asio::io_service& io_service)
extern "C" void *run(void *)
{
    //static_cast<boost::asio::io_service*>(io)->run();
    //boost::asio::io_service io_service;  //= static_cast<boost::asio::io_service*>(io);

    // Create Handles to Services
    server_ptr serverTelnet;

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
    if (!cfg.loadConfig())
    {
        // TODO Throws exception right now, need to work in
        // better shutdown on from this point! just assert for now.
        return nullptr;
    }

    // Startup Telnet Server
    if (cfg.m_config->use_service_telnet)
    {
        std::cout << "Listening for telnet connections on port "
                  << cfg.m_config->port_telnet << std::endl;
        serverTelnet.reset(new Server(io_service, cfg.m_config->port_telnet));
    }

    // Initial Testing of SSL Server.
    /*
    if (cfg.m_config->use_service_ssl)
    {
        std::cout << "Listening for service connections on port "
                  << cfg.m_config->port_ssl << std::endl;
        serverSSL.reset(new ServerSSL(io_service, cfg.m_config->port_ssl));
    }*/

    
    // Loop IO Service Execution Thread
    // We need a break for shutdowns!
    while(TheCommunicator::instance()->isActive())
    {
        try
        {
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
            std::cerr << e.what() << std::endl;
        }
    }

    io_service.stop();
    pthread_exit(nullptr);

    return nullptr;
}

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

    // Start System Services and Main Loop.
    //boost::asio::io_service io_service;

    // Load BBS Configuration here into Global Singleton.
    //TheCommunicator::instance()->attachConfig(config);

    // start io_service.run( ) in separate thread
    //auto t = std::thread(&run, std::ref(io_service));

    // TODO Replace std::thread in pi2 casue of gcc 4.9.1 issues!
    pthread_t t;
    pthread_create(&t, nullptr, &run, nullptr);


    // Main Thread, Get Server Input from System Operator
    // We'll handle Node and System setup here.  Need to write interface.
    while(TheCommunicator::instance()->isActive())
    {

#ifdef _WIN32
        Sleep(1);
#else
        sleep(1);
#endif

        // Just testing with text input, change this to state for commands
        // Or communication with a session or all sessions!
        //std::string line == "";
        //std::getline(std::cin, line);

        /*
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
        }*/
    }

    // Proper Thread Shutdown
    void *status;
    pthread_join(t, &status);

    pthread_exit(nullptr);

    // Release Communicator Instance
    TheCommunicator::releaseInstance();
    return 0;
}
