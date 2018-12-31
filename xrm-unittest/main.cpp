/**
 *   Oblivion/2 XRM - UNIT / INTEGRATION TESTS
 *   Copyright (C) 2015 - 2017 by Michael Griffin
 */

#include "model-sys/structures.hpp"
#include "model-sys/config.hpp"
#include "data-sys/config_dao.hpp"

#include <UnitTest++.h>
#include <memory>


// C Standard
#include <cstdio>
#include <cstring>
// C++ Standard
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "encoding.hpp"
#include "logging.hpp"
#include "communicator.hpp"

// Mocking Paths
std::string GLOBAL_BBS_PATH         = "";
std::string GLOBAL_DATA_PATH        = "";
std::string GLOBAL_MENU_PATH        = "";
std::string GLOBAL_MENU_PROMPT_PATH = "";
std::string GLOBAL_SCRIPT_PATH      = "";
std::string GLOBAL_TEXTFILE_PATH    = "";
std::string GLOBAL_LOG_PATH         = "";
std::string USERS_DATABASE          = "";

Logging* Logging::m_global_logging_instance = nullptr;

/**
 * @brief Unit Testing for Initial Form Configuration Implimentation
 * @return
 *
SUITE(XRMEncoding)
{


    // Tests Node Type Casting from CP437 -> UTF8 -> CP437
    TEST(Encoding_Test_encode_decode)
    {
        std::string test = "\x1b[0m\x1b[1;37m\x1b[?25h\x1b[1;36m";

        std::string result = Encoding::instance()->utf8Encode(test);
		std::cout << "result " << result << std::endl;


        std::string original = Encoding::instance()->utf8Decode(result);
		std::cout << "original " << original << std::endl;

        std::cout << test << " : " << original << std::endl;

        // Test Value.
        CHECK_EQUAL(test, original);

    }
}*/

/*
* These Unit Tests are To test the core logic of classes and methods.
* Any method that hit IO, Database, File, Network, is considered an Integration Test
* and will be setup in seperate Tests Suite or done manaully through the interface.
*/


// run all unit tests
int main()
{
    // Startup OBV/2 Configurations for proper unit coverage.

    std::cout << "Oblivion/2 XRM-Server Unit Tests (c) 2015-2018 Michael Griffin."
              << std::endl
              << std::endl;

    CommonIO common;
    GLOBAL_BBS_PATH = common.getProgramPath("xrm-unittest");
    std::cout << "BBS HOME Directory Registered: " << std::endl;
    std::cout << GLOBAL_BBS_PATH << std::endl;

    // Setup System Folder Paths off main BBS Path.
    GLOBAL_DATA_PATH = GLOBAL_BBS_PATH;
    GLOBAL_MENU_PATH = GLOBAL_BBS_PATH;
    GLOBAL_MENU_PROMPT_PATH = GLOBAL_BBS_PATH;
    GLOBAL_TEXTFILE_PATH = GLOBAL_BBS_PATH;
    GLOBAL_SCRIPT_PATH = GLOBAL_BBS_PATH;
    GLOBAL_LOG_PATH = GLOBAL_BBS_PATH;

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
            Encoding::releaseInstance();
            return(1);
        }

        // Handle to Data Access Object,  at the moment were not using directories
        // Setup in the config, everything is branched from the main path.
        // Later on we'll check config for overides only.
        ConfigDao cfg(config, GLOBAL_BBS_PATH);

        if(!cfg.fileExists())
        {
            cfg.saveConfig(config);
        }

        // Load Config and lets do some validation
        cfg.loadConfig();

        if(!cfg.validation())
        {
            Encoding::releaseInstance();
            return 1;
        }

        // All Good, Attached to Global Communicator Instance.
        TheCommunicator::instance()->attachConfiguration(config);
    }


    int result = UnitTest::RunAllTests();

    // Cleanup Integration Test Databases.
    remove("xrm_itConferenceTest.sqlite3");
    remove("xrm_itOnelinersTest.sqlite3");
    remove("xrm_itFileAreaTest.sqlite3");
    remove("xrm_itMessageAreaTest.sqlite3");

    Logging::releaseInstance();
    Encoding::releaseInstance();
    TheCommunicator::releaseInstance();

    return result;
}
