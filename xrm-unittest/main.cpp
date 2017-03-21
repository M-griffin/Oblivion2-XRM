/**
 *   Oblivion/2 XRM - UNIT TESTS
 *   Copyright (C) 2015 by Michael Griffin
 */

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <boost/smart_ptr/shared_ptr.hpp>

#include "model/users.hpp"
#include "access_condition.hpp"

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


/**
 * @brief Test Suit for CommonIO Class.
 * @return
 */
SUITE(XRMAccessCondition)
{    
    TEST(SetFlagOn1)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = 'A';
        bool first_set = true;       

        // First Test, Set the Flag
        acs.setFlagOn(flag, first_set, user);
        
        // Second Test Check the Flag
        CHECK(acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(SetFlagOn2)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = 'B';
        bool first_set = false;       

        // First Test, Set the Flag
        acs.setFlagOn(flag, first_set, user);
        
        // Second Test Check the Flag
        CHECK(acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(SetFlagOn3_Failure)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = '1';
        bool first_set = true;       

        // First Test, Set the Flag
        acs.setFlagOn(flag, first_set, user);
        
        // Second Test Check the Flag returns false
        CHECK(!acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(SetFlagOff)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = 'B';
        bool first_set = true;       

        // Set the Flag on for 'B'
        user->iControlFlags1 |= 1 << 1;
    
        // First Test, Set the Flag
        acs.setFlagOff(flag, first_set, user);
        
        // Second Test Check the Flag
        CHECK(!acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(SetFlagOff2)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = 'C';
        bool first_set = false;       

        // Set the Flag on for 'B'
        user->iControlFlags2 |= 1 << 2;
    
        // First Test, Set the Flag
        acs.setFlagOff(flag, first_set, user);
        
        // Second Test Check the Flag
        CHECK(!acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(SetFlagOff3_Failure)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = '1';
        bool first_set = true;       

        // First Test, Set the Flag
        acs.setFlagOff(flag, first_set, user);
        
        // Second Test Check the Flag returns false
        CHECK(!acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(ACSStringToCodeMapTestSecurityLevels)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;
        
        std::string test_expression = "~s10S244s244~S20";
        code_map = acs.parseAcsString(test_expression);
        
        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
        }
        
        // Test Results of code map.
        CHECK(code_map[0].m_match == 1);
        CHECK(code_map[0].m_code == "~s10");
        CHECK(code_map[1].m_match == 2);
        CHECK(code_map[1].m_code == "S244");
        CHECK(code_map[2].m_match == 2);
        CHECK(code_map[2].m_code == "s244");
        CHECK(code_map[3].m_match == 1);
        CHECK(code_map[3].m_code == "~S20");
    }
        
        
    TEST(ACSStringToCodeMapTestARFlags)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;
        
        std::string test_expression = "~fAFBfC~FR";
        code_map = acs.parseAcsString(test_expression);
        
        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
        }

        // Test Results of code map.
        CHECK(code_map[0].m_match == 3);
        CHECK(code_map[0].m_code == "~fA");
        CHECK(code_map[1].m_match == 4);
        CHECK(code_map[1].m_code == "FB");
        CHECK(code_map[2].m_match == 4);
        CHECK(code_map[2].m_code == "fC");
        CHECK(code_map[3].m_match == 3);
        CHECK(code_map[3].m_code == "~FR");
    }
    
}


// run all unit tests
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
