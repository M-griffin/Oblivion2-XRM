/*
 * Oblivion/2 XRM (c) 2015 Michael Griffin
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

#include "compat/prompts_dao.hpp"  // testing
#include "model/config.hpp"
#include "data/config_dao.hpp"

#include "server.hpp"
#include "server_ssl.hpp"
#include "communicator.hpp"
#include "common_io.hpp"

//#include "libSqliteWrapped.h"
//#include "menu_system.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>

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
std::string BBS_PATH = "";
std::string DATA_PATH = "";
std::string MENU_PATH = "";
std::string TEXTFILE_PATH = "";

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

    // Grab Lock and Create Instance of accessable configuration.
    config_ptr cfg(TheCommunicator::Instance()->getConfig().lock());
    if (!cfg)
    {
        std::cout << "Error: getConfig.lock()" << std::endl;
        assert(false);
    }
    
    // Service Startup Here.
    try
    {        
        // Startup Telnet Server
        if (cfg->use_service_telnet)
        {
            std::cout << "Listening for telnet connections on port "
                      << cfg->port_telnet << std::endl;
            serverTelnet.reset(new Server(io_service, cfg->port_telnet));
        }

        // Initial Testing of SSL Server.
        if (cfg->use_service_ssl)
        {
            std::cout << "Listening for service connections on port "
                      << cfg->port_ssl << std::endl;
            serverSSL.reset(new ServerSSL(io_service, cfg->port_ssl));
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
    std::cout << "Oblivion/2 XRM Server (c) 2015 Michael Griffin." << std::endl;

    CommonIO common;
    BBS_PATH = common.getProgramPath();
    std::cout << "BBS HOME Directory Registered: " << BBS_PATH << std::endl;

    // Have to match with Config record, or ignore in config!
    DATA_PATH = BBS_PATH + "DATA";
    MENU_PATH = BBS_PATH + "MENU";
    TEXTFILE_PATH = BBS_PATH + "TEXTFILE";

    // Start System Services and Main Loop.
    boost::asio::io_service io_service;

    // NEW Loading and saving default Configuration file to XML
    config_ptr config(new Config());
    if (!config)
    {
        std::cout << "Unable to allocate config structure" << std::endl;
        assert(false);
    }


    // Handle to Data Access Object.
    ConfigDao cfg(config, BBS_PATH);

    // Try to read configuration file, if it doesn't exist create with defaults.
    if (!cfg.loadConfig())
    {
        // If config file doesn't exist. then save a default, and reopen.
        std::cout << "Unable to Load, or Recreate missing xrm-config.xml file." << std::endl;
        assert(false);
    }

    // Load BBS Configuration here into Global Singleton.
    TheCommunicator::Instance()->attachConfig(config);

    // Testing!
    TextPrompt tp;
    PromptDao  prompt;
    CommonIO   common_io;

    //m_session_data->deliver(m_session_io.pipe2ansi("|CS|07"));
    std::string filename = "PROMPTS.DAT";


    // Loop each Option after Reading the Menu.
    int u = 0;

    std::string path = DATA_PATH;
    path.append("\\");
    path.append("prompts.xml");
    std::ofstream ofs(path);

    while(prompt.recordRead(&tp, filename, u++))
    {
        // Convert Pascal to C Strings.
        common_io.PascalToCString(tp.Desc);
        common_io.PascalToCString(tp.Prompt);

        ofs << "<prompt>" << std::endl;
        ofs << "<id>" << u << "</id>" << std::endl;
        ofs << "<description>" << tp.Desc << "</description>" << std::endl;
        ofs << "<text>" << tp.Prompt << "</text>" << std::endl;
        ofs << "</prompt>" << std::endl;
        //if (u == 25) break;
    }
    ofs.close();
/*
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
            TheCommunicator::Instance()->shutdown();
            io_service.stop();
            t.join();
            break;
        }
        else
        {
            // Sends Global Text to All Users connected.
            // Need to update this to pick 'either'
            // All or Individual Sessions.
            TheCommunicator::Instance()->addMessageQueue(line);
            TheCommunicator::Instance()->sendGlobalMessage();
        }
    }*/

    // Release Communicator Instance
    TheCommunicator::ReleaseInstance();
    return 0;
}
