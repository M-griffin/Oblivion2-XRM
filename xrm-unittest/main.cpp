/**
 *   Oblivion/2 XRM - UNIT / INTEGRATION TESTS
 *   Copyright (C) 2015 - 2017 by Michael Griffin
 */

#include <UnitTest++.h>
#include <boost/smart_ptr/shared_ptr.hpp>


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




/*
* These Unit Tests are To test the core logic of classes and methods.
* Any method that hit IO, Database, File, Network, is considered an Integration Test
* and will be setup in seperate Tests Suite or done manaully through the interface.
*/


// run all unit tests
int main(int argc, char **argv)
{
    int result = UnitTest::RunAllTests();

    // Cleanup Integration Test Databases.
    remove("xrm_itConferenceTest.sqlite3");
    remove("xrm_itOnelinersTest.sqlite3");
    remove("xrm_itFileAreaTest.sqlite3");
    remove("xrm_itMessageAreaTest.sqlite3");

    return result;
}
