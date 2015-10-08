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

#include "StandardIO_Test.hpp"
#include "StandardIO.hpp"

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
    using namespace stdio;

    // We Can only test some of the inital setup here.
    // The Rest of the functions all rely on STDIO
    // So this is more integration Testing.
    // Can be overide functions and pass basic input!  Might do this!!

    // Set Term to not echo and coan
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
    }

    // Rest of the Functions hit STDIO, so they are Integration Tests.
}
