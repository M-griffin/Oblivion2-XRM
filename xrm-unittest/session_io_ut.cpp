
/**
 * @brief Oblivion/2 XRM Unit Tests for SessionIO.
 * @return
 */

//#include "session_io_ut.hpp"
#include "session_io.hpp"

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

// C Standard
//#include <cstdio>
//#include <cstring>
// C++ Standard
#include <iostream>
#include <string>
//#include <vector>
#include <stdexcept>


SUITE(EnthralSessionIO)
{
    // We Can only test some of the inital setup here.
    // The Rest of the functions all rely on STDIO
    // So this is more integration Testing.
    // Can be overide functions and pass basic input!  Might do this!!

    // Set Term to not echo and coan
    /*
    TEST_FIXTURE(StandardIO, InitStdio)
    {
        bool result = InitConsoleTTY();
        CHECK_EQUAL(result, true);
    }

    // Reset Term to Default
    TEST_FIXTURE(StandardIO, Reset)
    {
        bool result = ResetConsoleTTY();
        CHECK_EQUAL(result, true);
    }

    // Test Bad Attribute
    TEST_FIXTURE(StandardIO, SetTerminalAttributes)
    {
        bool result = SetTerminalAttributes(4);
        CHECK_EQUAL(result, false);
    }*/

    // Rest of the Functions hit STDIO, so they are Integration Tests.
}
