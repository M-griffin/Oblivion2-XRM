/*
 * Oblivion/2 XRM (c) 2015 Michael Griffin
 * A Telnet Server and BBS system modeled after Oblivion/2 bbs software.
 *
 * XRM = X-Treme Remake!  :)
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

#include "struct.hpp"
#include "server.hpp"
#include "communicator.hpp"
#include "common_io.hpp"

//#include "libsqlitewrapped.h" // testing
//#include "menu_system.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>

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
    try
    {        
        //tcp::endpoint endpoint(tcp::v4(), 6023);
        //tcp::endpoint endpoint(tcp::v6(), 6023);
        Server serv(io_service); //, endpoint);

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
    std::cout << "Listening for connections on port 6023." << std::endl;

    CommonIO common;
    BBS_PATH = common.getProgramPath();
    std::cout << "BBS HOME Directory Registered: " << BBS_PATH << std::endl;

    // Have to match with Config record, or ignore in config!
    DATA_PATH = BBS_PATH + "DATA";
    MENU_PATH = BBS_PATH + "MENU";
    TEXTFILE_PATH = BBS_PATH + "TEXTFILE";

    // Load System Configuration
    TheCommunicator::Instance()->loadSystemConfig();

    // Database is working!  just testing.
    /*
    Database::Mutex mutex; // not really necessary here at all
	StderrLog log;
	Database db(mutex, "test3.db", &log);
	Query q(db);

	// create a test3 table
	q.execute("create table test3 ( num integer, name string )");

	// fill test3 with some data
	q.execute("insert into test3 values(1, 'Anders')");
	q.execute("insert into test3 values(2, 'Grymse')");

	// retrieve data
	q.get_result("select * from test3");
	while (q.fetch_row())
	{
		long num = q.getval();
		std::string name = q.getstr();
		printf("#%ld: %s\n", num, name.c_str());
	}
	q.free_result();
    */

    boost::asio::io_service io_service;
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
    }

    // Release Communicator Instance
    TheCommunicator::ReleaseInstance();
    return 0;
}
