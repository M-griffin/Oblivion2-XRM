/**
 * @brief Oblivion/2 XRM Unit Tests for Form_System_Config and Base.
 * @return
 */

#include <UnitTest++.h>

#include "model-sys/config.hpp"
#include "model-sys/menu.hpp"
#include "forms/form_system_config.hpp"


// C Standard
#include <cstdio>
#include <cstring>
// C++ Standard
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>


// Definitions for XRMFormSystemConfig Unit Tests.
// NOTE These are position in the config index of the methods,
// If we add new otpions, then these must be incremented!
#define MAX_OPTIONS 108
#define BBS_NAME_SYSOP 0
#define USE_SSL_SERVICE 10
#define PORT_TELNET 7


/**
 * @brief Unit Testing for Initial Form Configuration Implementation
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

    // Tests Node Type Casting from String to Boolean
    TEST(SystemConfigOutputTest_ThenBoolReassignment)
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

        // 106 Config Options generated.
        CHECK(opts.size() == MAX_OPTIONS);

        // Setup Yaml Mapping, and assign the config to it.
        YAML::Node node;
        node = config;

        // Test Value.
        CHECK(node[opts[PORT_TELNET].name].as<int>() == 6023);

        // Update Value on Option..
        opts[PORT_TELNET].form_value = "2323";

        // Assign new value by lookup
        node[opts[PORT_TELNET].name] = opts[PORT_TELNET].form_value;

        // Move from Node back to Config and Translations String to Int
        // Do Extra Test of Conversion to Smart Pointer!
        Config conf = node.as<Config>();
        config_ptr c = std::make_shared<Config>(conf);

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

        std::string newValue = "newValue";
        form->updateNodeMapping(opts[BBS_NAME_SYSOP], newValue);

        // Move from Node back to Config and Translations String to String
        config_ptr c = form->retrieveNodeMapping<Config>();

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

        std::string newValue = "true";
        form->updateNodeMapping(opts[USE_SSL_SERVICE], newValue);

        // Move from Node back to Config and Translations String to String
        config_ptr c = form->retrieveNodeMapping<Config>();

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

        std::string newValue = "2323";
        form->updateNodeMapping(opts[PORT_TELNET], newValue);

        // Move from Node back to Config and Translations String to String
        config_ptr c = form->retrieveNodeMapping<Config>();

        // Now test new value is populated in config class.
        CHECK(c->port_telnet == 2323);
    }

}
