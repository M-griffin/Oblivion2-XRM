/**
 * @brief Oblivion/2 XRM Unit Tests for AccessCondition.
 * @return
 */

#include "access_condition.hpp"

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <boost/smart_ptr/shared_ptr.hpp>

#include "model/users.hpp"

// C Standard
#include <cstdio>
#include <cstring>
// C++ Standard
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>


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
    
    TEST(SetFlagOn4_Failure_Underflow)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = '@';
        bool first_set = true;       

        // First Test, Set the Flag
        acs.setFlagOn(flag, first_set, user);
        
        // Second Test Check the Flag returns false
        CHECK(!acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(SetFlagOn5_Failure_Overflow)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = '[';
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
    
    TEST(SetFlagOff4_Failure_Underflow)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = '@';
        bool first_set = false;       

        // First Test, Set the Flag
        acs.setFlagOn(flag, first_set, user);
        
        // Second Test Check the Flag returns false
        CHECK(!acs.checkAccessConditionFlag(flag, first_set, user));
    }
    
    TEST(SetFlagOff5_Failure_Overflow)
    {
        AccessCondition acs;
        user_ptr user(new Users);
        
        unsigned char flag = '[';
        bool first_set = false;       

        // First Test, Set the Flag
        acs.setFlagOn(flag, first_set, user);
        
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
        // Not's are Match 1
        // Normal Match 2
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
        // Not's are Match 3
        // Normal Match 4
        CHECK(code_map[0].m_match == 3);
        CHECK(code_map[0].m_code == "~fA");
        CHECK(code_map[1].m_match == 4);
        CHECK(code_map[1].m_code == "FB");
        CHECK(code_map[2].m_match == 4);
        CHECK(code_map[2].m_code == "fC");
        CHECK(code_map[3].m_match == 3);
        CHECK(code_map[3].m_code == "~FR");
    }
    
    TEST(ACSStringToCodeMapTestSecurityAndARFlags)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;
        
        std::string test_expression = "~fA~s10S244FBfC~FRs244~S20";
        code_map = acs.parseAcsString(test_expression);
        
        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
        }

        // Test Results of code map.
        // Not's are Match 1  SL
        // Normal Match 2     SL
        // Not's are Match 3  AR
        // Normal Match 4     AR
        CHECK(code_map[0].m_match == 3);
        CHECK(code_map[0].m_code == "~fA");
        CHECK(code_map[1].m_match == 1);
        CHECK(code_map[1].m_code == "~s10");
        CHECK(code_map[2].m_match == 2);
        CHECK(code_map[2].m_code == "S244");
        CHECK(code_map[3].m_match == 4);
        CHECK(code_map[3].m_code == "FB");
        CHECK(code_map[4].m_match == 4);
        CHECK(code_map[4].m_code == "fC");
        CHECK(code_map[5].m_match == 3);
        CHECK(code_map[5].m_code == "~FR");
        CHECK(code_map[6].m_match == 2);
        CHECK(code_map[6].m_code == "s244");
        CHECK(code_map[7].m_match == 1);
        CHECK(code_map[7].m_code == "~S20");
    }
    
}

