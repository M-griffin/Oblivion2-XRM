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
SUITE(XRMCommonIO)
{
    TEST(ProgramPath)
    {
        CommonIO common;
        std::string myPath = common.getProgramPath("xrm-server");
        CHECK(!myPath.empty());
    }

    /**
     * @brief General Test Method for debugging.
     * @return
     *
    TEST(parseAnsiScreenTestHighAscii)
    {
        CommonIO common;
        std::string temp = std::string(1, static_cast<unsigned char>(148));
        temp += std::string(1, static_cast<unsigned char>(126));
        temp += std::string(1, static_cast<unsigned char>(155));
        common.testUnicode(temp);
        CHECK_EQUAL(1, 1);
    }*/

    TEST(NumberOfCharacters)
    {
        CommonIO common;
        std::string temp = "testing";
        int length = common.numberOfChars(temp);
        CHECK_EQUAL(length,7);
    }

    TEST(NumberOfCharactersEmpty)
    {
        CommonIO common;
        std::string temp = "";
        int length = common.numberOfChars(temp);
        CHECK_EQUAL(length,0);
    }

    TEST(NumberOfCharactersUTF8)
    {
        CommonIO common;
        std::string temp = "あにま! Linux";
        int length = common.numberOfChars(temp);
        CHECK_EQUAL(length,10);
    }

    /**
     * @brief Test Number of Characters with High ASCII mix-ins.
     * @return
     *
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
        // 21 Bytes, should be 9 separate characters
        CommonIO common;
        std::string temp = "あにま"; // 3 + 3 + 3 = 9 bytes
        temp += std::string(1, static_cast<unsigned char>(148)); // 1 byte
        temp += std::string(1, static_cast<unsigned char>(126)); // 1 byte
        temp += std::string(1, static_cast<unsigned char>(155)); // 1 byte
        temp += "あにま"; // 3 + 3 + 3 = 9 bytes
        int result = common.numberOfChars(temp);
        CHECK_EQUAL(result, 9);
    }*/

    /**
     * @brief Left of string, remove spaces.
     * @return
     */
    TEST(LeftTrim)
    {
        CommonIO common;
        std::string temp = "   Linux   ";
        std::string left_temp = common.leftTrim(temp);
        CHECK_EQUAL(left_temp,"Linux   ");
    }

    TEST(LeftTrimEmpty)
    {
        CommonIO common;
        std::string temp = "";
        std::string left_temp = common.leftTrim(temp);
        CHECK_EQUAL(left_temp,"");
    }

    /**
     * @brief Right of String Remove spaces
     * @return
     */
    TEST(RightTrim)
    {
        CommonIO common;
        std::string temp = "   Linux   ";
        std::string right_temp = common.rightTrim(temp);
        CHECK_EQUAL(right_temp,"   Linux");
    }

    TEST(RightTrimEmpty)
    {
        CommonIO common;
        std::string temp = "";
        std::string right_temp = common.rightTrim(temp);
        CHECK_EQUAL(right_temp,"");
    }

    /**
     * @brief Remove all spaces left or right of text.
     * @return
     */
    TEST(Trim)
    {
        CommonIO common;
        std::string temp = "   Linux   ";
        std::string trim_temp = common.trim(temp);
        CHECK_EQUAL(trim_temp,"Linux");
    }

    TEST(TrimEmpty)
    {
        CommonIO common;
        std::string temp = "";
        std::string trim_temp = common.trim(temp);
        CHECK_EQUAL(trim_temp,"");
    }

    /**
     * @brief Erase Data in a String w/ start, end range.
     * @return
     */
    TEST(EraseString)
    {
        CommonIO common;
        std::string temp = "   Linux----";
        std::string trim_temp = common.eraseString(temp, 6);
        CHECK_EQUAL(trim_temp,"   Lin");
    }

    TEST(EraseString0)
    {
        CommonIO common;
        std::string temp = "   Linux----";
        std::string trim_temp = common.eraseString(temp, 0);
        CHECK_EQUAL(trim_temp,"");
    }

    TEST(EraseString_Empty)
    {
        CommonIO common;
        std::string temp = "";
        std::string trim_temp = common.eraseString(temp, 6);
        CHECK_EQUAL(trim_temp,"");
    }

    TEST(EraseString_Range)
    {
        CommonIO common;
        std::string temp = "testing";
        std::string trim_temp = common.eraseString(temp, 0, 4);
        CHECK_EQUAL(trim_temp,"ing");
    }

    TEST(EraseString_Range_Past)
    {
        CommonIO common;
        std::string temp = "testing";
        std::string trim_temp = common.eraseString(temp, 2, 20);
        CHECK_EQUAL(trim_temp,"te");
    }

    TEST(EraseString_Range_UTF8)
    {
        CommonIO common;
        std::string temp = "あにま! Lin";
        std::string trim_temp = common.eraseString(temp, 0, 2);
        CHECK_EQUAL(trim_temp,"ま! Lin");
    }

    TEST(EraseString_Range_UTF8_2)
    {
        CommonIO common;
        std::string temp = "あにま! Lin";
        std::string trim_temp = common.eraseString(temp, 1, 4);
        CHECK_EQUAL(trim_temp,"あLin");
    }

    /**
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

    /**
     * @brief Pad Spaces on the right of the string, also truncates for absolute field lengths.
     * @return
     */
    TEST(RightPadding)
    {
        CommonIO common;
        std::string temp = "---";
        std::string pad_temp = common.rightPadding(temp, 6);
        CHECK_EQUAL(pad_temp,"---   ");
    }

    TEST(RightPadding_Truncate)
    {
        CommonIO common;
        std::string temp = "------";
        std::string pad_temp = common.rightPadding(temp, 3);
        CHECK_EQUAL(pad_temp,"---");
    }

    TEST(RightPadding_Empty)
    {
        CommonIO common;
        std::string temp = "";
        std::string pad_temp = common.rightPadding(temp, 3);
        CHECK_EQUAL(pad_temp,"   ");
    }

    TEST(RightPadding0)
    {
        CommonIO common;
        std::string temp = "---";
        std::string pad_temp = common.rightPadding(temp, 0);
        CHECK_EQUAL(pad_temp,"---");
    }

    /**
     * @brief Pad Spaces on the Left of the string, also truncates for absolute field lengths.
     * @return
     */
    TEST(LeftPadding)
    {
        CommonIO common;
        std::string temp = "---";
        std::string pad_temp = common.leftPadding(temp, 6);
        CHECK_EQUAL(pad_temp,"   ---");
    }

    TEST(LeftPadding_Truncate)
    {
        CommonIO common;
        std::string temp = "------ ";
        std::string pad_temp = common.leftPadding(temp, 3);
        CHECK_EQUAL(pad_temp,"-- ");
    }

    TEST(LeftPadding_Empty)
    {
        CommonIO common;
        std::string temp = "";
        std::string pad_temp = common.leftPadding(temp, 3);
        CHECK_EQUAL(pad_temp,"   ");
    }

    TEST(LeftPadding0)
    {
        CommonIO common;
        std::string temp = "---";
        std::string pad_temp = common.leftPadding(temp, 0);
        CHECK_EQUAL(pad_temp,"---");
    }

    /**
     * @brief Masks Field data returned, for passwords etc..
     * @return
     */
    TEST(MaskString_Empty)
    {
        CommonIO common;
        std::string temp = "";
        std::string mask_temp = common.maskString(temp);
        CHECK_EQUAL(mask_temp,"");
    }

    TEST(MaskString)
    {
        CommonIO common;
        std::string temp = "testing";
        std::string mask_temp = common.maskString(temp);
        CHECK_EQUAL(mask_temp,"*******");
    }

    /**
     * @brief Tests if string is a digit or number value.
     * @return
     */
    TEST(IsDigit_Bad_Preceding)
    {
        CommonIO common;
        std::string temp = "a1";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, false);
    }

    TEST(IsDigit_Bad_After)
    {
        CommonIO common;
        std::string temp = "1a";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, false);
    }

    TEST(IsDigit)
    {
        CommonIO common;
        std::string temp = "1";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, true);
    }

    TEST(IsDigit_Multiple)
    {
        CommonIO common;
        std::string temp = "125";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, true);
    }

    TEST(IsDigit_Unicode_Bad_After)
    {
        CommonIO common;
        std::string temp = "६a";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, false);
    }

    /**
     * @brief At this time, unicode digits are no longer
     *        Functional after removing boost locale,
     *        Alternatives will be worked on later.
     * @return
     *
    TEST(isDigit_Unicode_Multiple)
    {
        CommonIO common;
        std::string temp = "६೬";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, true);
    }*/

    TEST(isDigit_Unicode_Bad_Preceding)
    {
        CommonIO common;
        std::string temp = "a६ ೬";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, false);
    }

    /**
    * @brief Parses Input char at a time.
    * @return
    */

    TEST(parseInput_Normal_Character_Should_Return_Same)
    {
        CommonIO common;
        std::string temp = "T";
        std::string result = common.parseInput(temp);
        CHECK_EQUAL(result, "T");
    }

    TEST(parseInput_ESC_Character_Should_Return_Empty)
    {
        CommonIO common;
        std::string temp = "\x1b";
        std::string result = common.parseInput(temp);
        CHECK_EQUAL(result, "");
    }

    TEST(parseInput_ESC_Character_Should_Return_Empty_Null_Returns_ESC)
    {
        CommonIO common;
        std::string temp = "\x1b";
        std::string result = common.parseInput(temp);
        CHECK_EQUAL(result, "");

        temp = "\0";
        result = common.parseInput(temp);
        CHECK_EQUAL(result, "\x1b");
    }

    TEST(parseInput_Multiple_Characters_Returns_Empty)
    {
        CommonIO common;
        std::string temp = "AB";
        std::string result = common.parseInput(temp);
        CHECK_EQUAL(result, "");
    }

    TEST(parseInput_Unicode_Character_Returns_Unicode_Character)
    {
        CommonIO common;
        std::string temp = "あ";
        std::string result = common.parseInput(temp);
        CHECK_EQUAL(result, "あ");
    }

    TEST(parseInput_Multiple_Unicode_Character_Returns_Empty)
    {
        CommonIO common;
        std::string temp = "ああ";
        std::string result = common.parseInput(temp);
        CHECK_EQUAL(result, "");
    }

    TEST(parseInput_ESC_With_Termination_Null_Returns_ESC)
    {
        CommonIO common;
        std::string temp = "\x1b";
        std::string result = common.parseInput(temp);
        CHECK_EQUAL(result, "");

        temp = "\0";
        result = common.parseInput(temp);
        CHECK_EQUAL(result, "\x1b");
    }

    /*
    TEST(parseAnsiScreenTest)
    {
        CommonIO common;
        std::string temp = std::string(1, static_cast<unsigned char>(148));
        common.testUnicode(temp);
        CHECK_EQUAL(1, 1);
    }*/

}
