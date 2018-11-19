/**
 * @brief Oblivion/2 XRM Unit Tests for CommonIO.
 * @return
 */

#include "common_io.hpp"

#include <UnitTest++.h>

// C++ Standard
#include <iostream>
#include <string>
#include <stdexcept>

/**
 * @brief Test Suit for CommonIO Class.
 * @return
 */
SUITE(XRM_MockUTTestClass)
{

    TEST(numberOfCharsWithHighAscii)
    {
        CommonIO common;
        std::string temp = std::string(1, static_cast<unsigned char>(148));
        temp += std::string(1, static_cast<unsigned char>(126));
        temp += std::string(1, static_cast<unsigned char>(155));
        int result = common.numberOfChars(temp);
        std::cout << "result: " << result << std::endl;
    }

    TEST(parseAnsiScreenTestHighAscii)
    {
        CommonIO common;
        std::string temp = std::string(1, static_cast<unsigned char>(148));
        temp += std::string(1, static_cast<unsigned char>(126));
        temp += std::string(1, static_cast<unsigned char>(155));
        common.testUnicode(temp);
    }

}
