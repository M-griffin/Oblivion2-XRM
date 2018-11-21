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
        CHECK_EQUAL(result, 3);
    }

    TEST(numberOfCharsWithHighAsciiAndUtf8BeforeAndAfter)
    {
        // 21 Bytes, should be 9 seperate characters
        CommonIO common;
        std::string temp = "あにま"; // 3 + 3 + 3 = 9 bytes
        temp += std::string(1, static_cast<unsigned char>(148)); // 1 byte
        temp += std::string(1, static_cast<unsigned char>(126)); // 1 byte
        temp += std::string(1, static_cast<unsigned char>(155)); // 1 byte
        temp += "あにま"; // 3 + 3 + 3 = 9 bytes
        int result = common.numberOfChars(temp);
        CHECK_EQUAL(result, 9);
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
