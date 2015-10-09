/**
 * @brief Oblivion/2 XRM Unit Tests for CommonIO.
 * @return
 */

#include "ansi_processor.hpp"

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

// C++ Standard
#include <iostream>
#include <string>
#include <stdexcept>

/**
 * @brief Test Suit for CommonIO Class.
 * @return
 */
SUITE(XRMAnsiProcessor)
{
    // Test the Program Path is being read correctly.
    TEST(parseAnsiNormalText)
    {
        AnsiProcessor ansi(25,80);
        std::string myAnsi = ansi.parseAnsiScreen("1234");
        CHECK(myAnsi.compare("1234"));
    }    
}
