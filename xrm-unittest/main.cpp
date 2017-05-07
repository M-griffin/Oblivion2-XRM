/**
 *   Oblivion/2 XRM - UNIT TESTS
 *   Copyright (C) 2015 by Michael Griffin
 */

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <boost/smart_ptr/shared_ptr.hpp>

#include "data-sys/conference_dao.hpp"
#include "model-sys/conference.hpp"

#include "libSqliteWrapped.h"


// C Standard
#include <cstdio>
#include <cstring>
// C++ Standard
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// Mocking Paths
std::string GLOBAL_BBS_PATH      = "";
std::string GLOBAL_DATA_PATH     = "";
std::string GLOBAL_MENU_PATH     = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string USERS_DATABASE       = "";


/**
 * Mock the ConferenceDao and test everything with BaseDao
 * to Make sure everything is solid.
 */
class MyFixture
{
public:
   MyFixture() { /* setup goes here */ }
   ~MyFixture() { /* teardown goes here */ }
};


// USERS_DATABASE = "xrm_itTest_users.sqlite3";

/**
 * @brief Unit Testing for Initial Sqlite Database DAO and BaseDao.
 * @return 
 */
SUITE(XRMConferenceDao)
{
    
    TEST_FIXTURE(MyFixture, MyFixtureTest)
    {
        // checks go here
        CHECK(1);
    }
    
    
/*    
    // Tests Node Type Casting from String to String
    TEST(SystemConfigOutputTest_ThenStringReassignment)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
        
        // run the form on enter.
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
        
        // 106 Config Options generated.
        CHECK(opts.size() == MAX_OPTIONS);
        
        // Setup Yaml Mapping, and assign the config to it.
        YAML::Node node;
        node = config;
        
        // Test Value.
        CHECK(opts[BBS_NAME_SYSOP].form_value == node[opts[BBS_NAME_SYSOP].name].as<std::string>());
        
        // Update Value on Option.. 
        opts[BBS_NAME_SYSOP].form_value = "new value";
        
        // Assign new value by lookup
        node[opts[BBS_NAME_SYSOP].name] = opts[BBS_NAME_SYSOP].form_value;
        
        // Move from Node back to Config and Translations String to String
        Config c = node.as<Config>();
        
        // Now test new value is populated in config class.
        CHECK(opts[BBS_NAME_SYSOP].form_value == c.bbs_name_sysop);
    }
*/


}


/*
* These Unit Tests are To test the core logic of classes and methods.
* Any method that hit IO, Database, File, Network, is considered an Integration Test
* and will be setup in seperate Tests Suite or done manaully through the interface.
*/


// run all unit tests
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
