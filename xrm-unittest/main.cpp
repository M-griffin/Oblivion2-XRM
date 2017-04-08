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


/**
 * @brief Unit Testing for Initial Form Configuration Implimentation
 * @return 
 */
SUITE(XRMFormSystemConfig)
{
    // Just testing output of onEnter .. 
    TEST(SystemConfigOutputTest)
    {        
        config_ptr config(new Config());
        form_ptr form(new FormSystemConfig(config));
        
        // run the form on enter.
        form->onEnter();
        std::vector<MenuOption> opts = form->baseGetFormOptions();
        std::cout << "opts size: " << opts.size() << std::endl;
        
        // 106 Config Options generated.
        CHECK(opts.size() == 106);        
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
