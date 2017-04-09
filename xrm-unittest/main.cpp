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
#include <boost/smart_ptr/make_shared.hpp>


#include "model/config.hpp"
#include "model/menu.hpp"
#include "forms/form_system_config.hpp"


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



// Definitions for XRMFormSystemConfig Unit Tests.
#define MAX_OPTIONS 106
#define BBS_NAME_SYSOP 0
#define USE_SSL_SERVICE 9
#define PORT_TELNET 6


/**
 * @brief Unit Testing for Initial Form Configuration Implimentation
 * @return 
 */
SUITE(XRMFormSystemConfig)
{
    /**
     * Just testing output of onEnter .. And generate mapping translation mock up.
     * In generate config value are places in menu options form_value as strings
     * They are then edited and updated.  YAML::Node then translates and maps
     * Back to their original types changing Strings back to String, Bool, Int etc..
     * These tests confirm the mapping and updates are working correctly for the form interface.
     */
    
    // Tests Node Type Casting from String to String
    TEST(SystemConfigOutputTest_ThenStringReassignment)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
        
        // run the form on enter.
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
        std::cout << "opts size: " << opts.size() << std::endl;
        
        // 106 Config Options generated.
        CHECK(opts.size() == MAX_OPTIONS);
        
        // Setup Yaml Mapping, and assign the config to it.
        YAML::Node node;
        node = config;
        
        // Test we can Read Each Value and the mappings match
        std::cout << "Name[0]: " << opts[BBS_NAME_SYSOP].name << std::endl;        
        std::cout << "Node Value: " << node[opts[BBS_NAME_SYSOP].name] << std::endl;
        
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
    
    // Tests Node Type Casting from String to Boolean
    TEST(SystemConfigOutputTest_ThenBoolReassignment)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
        
        // run the form on enter.
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
        std::cout << "opts size: " << opts.size() << std::endl;
        
        // 106 Config Options generated.
        CHECK(opts.size() == MAX_OPTIONS);
        
        // Setup Yaml Mapping, and assign the config to it.
        YAML::Node node;
        node = config;
        
        // Test we can Read Each Value and the mappings match
        std::cout << "Name[0]: " << opts[USE_SSL_SERVICE].name << std::endl;        
        std::cout << "Node Value: " << node[opts[USE_SSL_SERVICE].name] << std::endl;
        
        // Test Value.
        CHECK(!node[opts[USE_SSL_SERVICE].name].as<bool>());
        
        // Update Value on Option.. 
        opts[USE_SSL_SERVICE].form_value = "true";
        
        // Assign new value by lookup
        node[opts[USE_SSL_SERVICE].name] = opts[USE_SSL_SERVICE].form_value;
        
        // Move from Node back to Config and Translations String to Bool
        Config c = node.as<Config>();
        
        // Now test new value is populated in config class.
        CHECK(c.use_service_ssl == true);
    }
    
    // Tests Node Type Casting from String to Integer
    TEST(SystemConfigOutputTest_ThenIntegerReassignment)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
        
        // run the form on enter.
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
        std::cout << "opts size: " << opts.size() << std::endl;
        
        // 106 Config Options generated.
        CHECK(opts.size() == MAX_OPTIONS);
        
        // Setup Yaml Mapping, and assign the config to it.
        YAML::Node node;
        node = config;
        
        // Test we can Read Each Value and the mappings match
        std::cout << "Name[0]: " << opts[PORT_TELNET].name << std::endl;        
        std::cout << "Node Value: " << node[opts[PORT_TELNET].name] << std::endl;
        
        // Test Value.
        CHECK(node[opts[PORT_TELNET].name].as<int>() == 6023);
        
        // Update Value on Option.. 
        opts[PORT_TELNET].form_value = "2323";
        
        // Assign new value by lookup
        node[opts[PORT_TELNET].name] = opts[PORT_TELNET].form_value;
        
        // Move from Node back to Config and Translations String to Int
        // Do Extra Test of Conversion to Smart Pointer!
        Config conf = node.as<Config>();
        config_ptr c = boost::make_shared<Config>(conf);
                
        // Now test new value is populated in config class.
        CHECK(c->port_telnet == 2323);
    }


    // User FormSystemConfig Methods
    // Tests Node Type Casting from String to String
    TEST(SystemConfigOutputTest_ThenStringReassignment_FormMethods)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
        
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
        
        // Test we can Read Each Value and the mappings match
        std::cout << "Name[0]: " << opts[BBS_NAME_SYSOP].name << std::endl;        
        
        std::string newValue = "newValue";
        form->updateNodeMapping(opts[BBS_NAME_SYSOP], newValue);
                
        // Move from Node back to Config and Translations String to String
        config_ptr c = form->retrieveNodeMapping();
        
        // Now test new value is populated in config class.
        CHECK(newValue == c->bbs_name_sysop);
    }
    
    // Tests Node Type Casting from String to Boolean
    TEST(SystemConfigOutputTest_ThenBoolReassignment_FormMethods)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
                
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
                
        // Test we can Read Each Value and the mappings match
        std::cout << "Name[0]: " << opts[USE_SSL_SERVICE].name << std::endl;        
        
        std::string newValue = "true";        
        form->updateNodeMapping(opts[USE_SSL_SERVICE], newValue);
                
        // Move from Node back to Config and Translations String to String
        config_ptr c = form->retrieveNodeMapping();
                        
        // Now test new value is populated in config class.
        CHECK(c->use_service_ssl == true);
    }
    
    // Tests Node Type Casting from String to Integer
    TEST(SystemConfigOutputTest_ThenIntegerReassignment_FormMethods)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
        
        // run the form on enter.
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
        
        // Test we can Read Each Value and the mappings match
        std::cout << "Name[0]: " << opts[PORT_TELNET].name << std::endl;        

        std::string newValue = "2323";        
        form->updateNodeMapping(opts[PORT_TELNET], newValue);
                
        // Move from Node back to Config and Translations String to String
        config_ptr c = form->retrieveNodeMapping();
                
        // Now test new value is populated in config class.
        CHECK(c->port_telnet == 2323);
    }

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
