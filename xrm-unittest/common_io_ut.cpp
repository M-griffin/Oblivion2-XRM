
/**
 * @brief Oblivion/2 XRM Unit Tests for CommonIO.
 * @return
 */

#include "common_io.hpp"

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
SUITE(EnthralCommonIO)
{
    // Test the Program Path is being read correctly.
    TEST(ProgramPath)
    {
        CommonIO common;
        std::string myPath = common.getProgramPath();
        CHECK(!myPath.empty());
    }

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

    TEST(EraseStringEmpty)
    {
        CommonIO common;
        std::string temp = "";
        std::string trim_temp = common.eraseString(temp, 6);
        CHECK_EQUAL(trim_temp,"");
    }

    TEST(EraseStringRange)
    {
        CommonIO common;
        std::string temp = "testing";
        std::string trim_temp = common.eraseString(temp, 0, 4);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"ing");
    }

    TEST(EraseStringRangePast)
    {
        CommonIO common;
        std::string temp = "testing";
        std::string trim_temp = common.eraseString(temp, 2, 20);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"te");
    }

    TEST(EraseStringRangeUTF8)
    {
        CommonIO common;
        std::string temp = "あにま! Lin";
        std::string trim_temp = common.eraseString(temp, 0, 2);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"ま! Lin");
    }

    TEST(EraseStringRangeUTF8_2)
    {
        CommonIO common;
        std::string temp = "あにま! Lin";
        std::string trim_temp = common.eraseString(temp, 1, 4);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"あLin");
    }

    TEST(RightPadding)
    {
        CommonIO common;
        std::string temp = "---";
        std::string padd_temp = common.rightPadding(temp, 6);
        CHECK_EQUAL(padd_temp,"---   ");
    }

    TEST(RightPaddingTruncate)
    {
        CommonIO common;
        std::string temp = "------";
        std::string padd_temp = common.rightPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"---");
    }

    TEST(RightPaddingEmpty)
    {
        CommonIO common;
        std::string temp = "";
        std::string padd_temp = common.rightPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"");
    }

    TEST(RightPadding0)
    {
        CommonIO common;
        std::string temp = "---";
        std::string padd_temp = common.rightPadding(temp, 0);
        CHECK_EQUAL(padd_temp,"---");
    }

    TEST(LeftPadding)
    {
        CommonIO common;
        std::string temp = "---";
        std::string padd_temp = common.leftPadding(temp, 6);
        CHECK_EQUAL(padd_temp,"   ---");
    }

    TEST(LeftPaddingTruncate)
    {
        CommonIO common;
        std::string temp = "------ ";
        std::string padd_temp = common.leftPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"-- ");
    }

    TEST(LeftPaddingEmpty)
    {
        CommonIO common;
        std::string temp = "";
        std::string padd_temp = common.leftPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"");
    }

    TEST(LeftPadding0)
    {
        CommonIO common;
        std::string temp = "---";
        std::string padd_temp = common.leftPadding(temp, 0);
        CHECK_EQUAL(padd_temp,"---");
    }

    TEST(MaskStringEmpty)
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

    TEST(IsDigitBadPreceeding)
    {
        CommonIO common;
        std::string temp = "a1";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, false);
    }

    TEST(IsDigitBadAfter)
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

    TEST(IsDigitMultiple)
    {
        CommonIO common;
        std::string temp = "125";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, true);
    }

    TEST(IsDigitUnicodeBadAfter)
    {
        CommonIO common;
        std::string temp = "६a";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, false);
    }

    TEST(isDigitUnicodeMultiple)
    {
        CommonIO common;
        std::string temp = "६೬";
        bool result = common.isDigit(temp);
        CHECK_EQUAL(result, true);
    }

    TEST(isDigitUnicodeBadPreceeding)
    {
        CommonIO common;
        std::string temp = "a६ ೬";
        bool result = common.isDigit(temp);
        //std::cout << "[" << padd_temp << "]" << std::endl;
        CHECK_EQUAL(result, false);
    }
}
