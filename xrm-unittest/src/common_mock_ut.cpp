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

    /**
     * @brief Test Number of Characters with High ASCII mix-ins.
     * @return
     */
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

    /* This was remove from the code.
    TEST(parseAnsiScreenTestHighAscii)
    {
        CommonIO common;
        std::string temp = std::string(1, static_cast<unsigned char>(148));
        temp += std::string(1, static_cast<unsigned char>(126));
        temp += std::string(1, static_cast<unsigned char>(155));
        common.testUnicode(temp);
    }*/

    /** No Longer a Valid Test.
     * @brief Erase Data in a String w/ start, end range. w/ High Ascii
     * @return
     *
    TEST(EraseStringWithHighAscii)
    {
        CommonIO common;
        std::string temp = std::string(1, static_cast<unsigned char>(155));
        temp += "   Linux----";
        std::string trim_temp = common.eraseString(temp, 6);

        std::string result = std::string(1, static_cast<unsigned char>(155));
        result += "   Li";
        CHECK_EQUAL(trim_temp, result);
    }*/

}
