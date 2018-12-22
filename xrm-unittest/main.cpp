/**
 *   Oblivion/2 XRM - UNIT / INTEGRATION TESTS
 *   Copyright (C) 2015 - 2017 by Michael Griffin
 */

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

// Mocking Paths
std::string GLOBAL_BBS_PATH      = "";
std::string GLOBAL_DATA_PATH     = "";
std::string GLOBAL_MENU_PATH     = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string USERS_DATABASE       = "";

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
    int result = UnitTest::RunAllTests();

    // Cleanup Integration Test Databases.
    remove("xrm_itConferenceTest.sqlite3");
    remove("xrm_itOnelinersTest.sqlite3");
    remove("xrm_itFileAreaTest.sqlite3");
    remove("xrm_itMessageAreaTest.sqlite3");

    return result;
}
