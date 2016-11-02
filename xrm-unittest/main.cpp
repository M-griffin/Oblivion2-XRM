/**
 *   Oblivion/2 XRM - UNIT TESTS
 *   Copyright (C) 2015 by Michael Griffin
 */

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif


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
    return UnitTest::RunAllTests();
}
