/***************************************************************************
 *   Enthral BBS Alpha 2 - UNIT TESTS                                      *
 *   Copyright (C) 2004-2015 by Michael Griffin                            *
 *   mrmisticismo@hotmail.com                                              *
 *                                                                         *
 *   Purpose:                                                              *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "CommonIO_Test.hpp"
#include "CommonIO.hpp"

#include <unittest++/UnitTest++.h>

// C Standard
//#include <cstdio>
//#include <cstring>
// C++ Standard
#include <iostream>
#include <string>
//#include <vector>
#include <stdexcept>

SUITE(EnthralCommonIO)
{
    // Test the Program Path is being read correctly.
    TEST(ProgramPath)
    {
        std::string myPath = common::GetProgramPath();
        CHECK(!myPath.empty());
    }

    TEST(NumberOfCharacters)
    {
        std::string temp = "testing";
        int length = common::NumberOfChars(temp);
        CHECK_EQUAL(length,7);
    }

    TEST(NumberOfCharactersEmpty)
    {
        std::string temp = "";
        int length = common::NumberOfChars(temp);
        CHECK_EQUAL(length,0);
    }

    TEST(NumberOfCharactersUTF8)
    {
        std::string temp = "あにま! Linux";
        int length = common::NumberOfChars(temp);
        CHECK_EQUAL(length,10);
    }

    TEST(LeftTrim)
    {
        std::string temp = "   Linux   ";
        std::string left_temp = common::LeftTrim(temp);
        CHECK_EQUAL(left_temp,"Linux   ");
    }

    TEST(LeftTrimEmpty)
    {
        std::string temp = "";
        std::string left_temp = common::LeftTrim(temp);
        CHECK_EQUAL(left_temp,"");
    }

    TEST(RightTrim)
    {
        std::string temp = "   Linux   ";
        std::string right_temp = common::RightTrim(temp);
        CHECK_EQUAL(right_temp,"   Linux");
    }

    TEST(RightTrimEmpty)
    {
        std::string temp = "";
        std::string right_temp = common::RightTrim(temp);
        CHECK_EQUAL(right_temp,"");
    }

    TEST(Trim)
    {
        std::string temp = "   Linux   ";
        std::string trim_temp = common::Trim(temp);
        CHECK_EQUAL(trim_temp,"Linux");
    }

    TEST(TrimEmpty)
    {
        std::string temp = "";
        std::string trim_temp = common::Trim(temp);
        CHECK_EQUAL(trim_temp,"");
    }

    TEST(EraseString)
    {
        std::string temp = "   Linux----";
        std::string trim_temp = common::EraseString(temp, 6);
        CHECK_EQUAL(trim_temp,"   Lin");
    }

    TEST(EraseString0)
    {
        std::string temp = "   Linux----";
        std::string trim_temp = common::EraseString(temp, 0);
        CHECK_EQUAL(trim_temp,"");
    }

    TEST(EraseStringEmpty)
    {
        std::string temp = "";
        std::string trim_temp = common::EraseString(temp, 6);
        CHECK_EQUAL(trim_temp,"");
    }

    TEST(EraseStringRange)
    {
        std::string temp = "testing";
        std::string trim_temp = common::EraseString(temp, 0, 4);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"ing");
    }

    TEST(EraseStringRangePast)
    {
        std::string temp = "testing";
        std::string trim_temp = common::EraseString(temp, 2, 20);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"te");
    }

    TEST(EraseStringRangeUTF8)
    {
        std::string temp = "あにま! Lin";
        std::string trim_temp = common::EraseString(temp, 0, 2);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"ま! Lin");
    }

    TEST(EraseStringRangeUTF8_2)
    {
        std::string temp = "あにま! Lin";
        std::string trim_temp = common::EraseString(temp, 1, 4);
        std::cout << "[" << trim_temp << "]" << std::endl;
        CHECK_EQUAL(trim_temp,"あLin");
    }

    TEST(RightPadding)
    {
        std::string temp = "---";
        std::string padd_temp = common::RightPadding(temp, 6);
        CHECK_EQUAL(padd_temp,"---   ");
    }

    TEST(RightPaddingTruncate)
    {
        std::string temp = "------";
        std::string padd_temp = common::RightPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"---");
    }

    TEST(RightPaddingEmpty)
    {
        std::string temp = "";
        std::string padd_temp = common::RightPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"");
    }

    TEST(RightPadding0)
    {
        std::string temp = "---";
        std::string padd_temp = common::RightPadding(temp, 0);
        CHECK_EQUAL(padd_temp,"---");
    }

    TEST(LeftPadding)
    {
        std::string temp = "---";
        std::string padd_temp = common::LeftPadding(temp, 6);
        CHECK_EQUAL(padd_temp,"   ---");
    }

    TEST(LeftPaddingTruncate)
    {
        std::string temp = "------ ";
        std::string padd_temp = common::LeftPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"-- ");
    }

    TEST(LeftPaddingEmpty)
    {
        std::string temp = "";
        std::string padd_temp = common::LeftPadding(temp, 3);
        CHECK_EQUAL(padd_temp,"");
    }

    TEST(LeftPadding0)
    {
        std::string temp = "---";
        std::string padd_temp = common::LeftPadding(temp, 0);
        CHECK_EQUAL(padd_temp,"---");
    }

    TEST(MaskStringEmpty)
    {
        std::string temp = "";
        std::string mask_temp = common::MaskString(temp);
        CHECK_EQUAL(mask_temp,"");
    }

    TEST(MaskString)
    {
        std::string temp = "testing";
        std::string mask_temp = common::MaskString(temp);
        CHECK_EQUAL(mask_temp,"*******");
    }

    TEST(IsDigitBadPreceeding)
    {
        std::string temp = "a1";
        bool result = common::IsDigit(temp);
        CHECK_EQUAL(result, false);
    }

    TEST(IsDigitBadAfter)
    {
        std::string temp = "1a";
        bool result = common::IsDigit(temp);
        CHECK_EQUAL(result, false);
    }

    TEST(IsDigit)
    {
        std::string temp = "1";
        bool result = common::IsDigit(temp);
        CHECK_EQUAL(result, true);
    }

    TEST(IsDigitMultiple)
    {
        std::string temp = "125";
        bool result = common::IsDigit(temp);
        CHECK_EQUAL(result, true);
    }

    TEST(IsDigitUnicodeBadAfter)
    {
        std::string temp = "६a";
        bool result = common::IsDigit(temp);
        CHECK_EQUAL(result, false);
    }

    TEST(isDigitUnicodeMultiple)
    {
        std::string temp = "६೬";
        bool result = common::IsDigit(temp);
        CHECK_EQUAL(result, true);
    }

    TEST(isDigitUnicodeBadPreceeding)
    {
        std::string temp = "a६ ೬";
        bool result = common::IsDigit(temp);
        //std::cout << "[" << padd_temp << "]" << std::endl;
        CHECK_EQUAL(result, false);
    }

}
