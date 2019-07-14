/**
 * @brief Oblivion/2 XRM Unit Tests for AccessCondition.
 * @return
 */

#include "access_condition.hpp"

#include <UnitTest++.h>

#include "model-sys/users.hpp"

// C Standard
#include <cstdio>
#include <cstring>
// C++ Standard
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

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
        acs.setFlagOff(flag, first_set, user);

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
            return;
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
            return;
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

    TEST(ACSStringToCodeMapTestARFlagsSecond)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~oAOBoC~OR";
        code_map = acs.parseAcsString(test_expression);

        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
            return;
        }

        // Test Results of code map.
        // Not's are Match 5
        // Normal Match 6
        CHECK(code_map[0].m_match == 5);
        CHECK(code_map[0].m_code == "~oA");
        CHECK(code_map[1].m_match == 6);
        CHECK(code_map[1].m_code == "OB");
        CHECK(code_map[2].m_match == 6);
        CHECK(code_map[2].m_code == "oC");
        CHECK(code_map[3].m_match == 5);
        CHECK(code_map[3].m_code == "~OR");
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
            return;
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

    TEST(ACSStringToCodeMapTestSecurityAndARFlagsSecond)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~oA~s10S244OBoC~ORs244~S20";
        code_map = acs.parseAcsString(test_expression);

        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
            return;
        }

        // Test Results of code map.
        // Not's are Match 1  SL
        // Normal Match 2     SL
        // Not's are Match 5  AR2
        // Normal Match 6     AR2
        CHECK(code_map[0].m_match == 5);
        CHECK(code_map[0].m_code == "~oA");
        CHECK(code_map[1].m_match == 1);
        CHECK(code_map[1].m_code == "~s10");
        CHECK(code_map[2].m_match == 2);
        CHECK(code_map[2].m_code == "S244");
        CHECK(code_map[3].m_match == 6);
        CHECK(code_map[3].m_code == "OB");
        CHECK(code_map[4].m_match == 6);
        CHECK(code_map[4].m_code == "oC");
        CHECK(code_map[5].m_match == 5);
        CHECK(code_map[5].m_code == "~OR");
        CHECK(code_map[6].m_match == 2);
        CHECK(code_map[6].m_code == "s244");
        CHECK(code_map[7].m_match == 1);
        CHECK(code_map[7].m_code == "~S20");
    }

    TEST(ACSStringToCodeMapTestSecurityAndARFlags2)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "ASDCXZ(~fA~s10S244FBfC~FRs244~S20)sfsdf";
        code_map = acs.parseAcsString(test_expression);

        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
            return;
        }

        // Test Results of code map.
        // Not's are Match 1  SL
        // Normal Match 2     SL
        // Not's are Match 3  AR
        // Normal Match 4     AR
        CHECK(code_map[0].m_match == 3);
        CHECK(code_map[0].m_code == "~fA");
        //CHECK(code_map[0].m_or == false);
        CHECK(code_map[1].m_match == 1);
        CHECK(code_map[1].m_code == "~s10");
        //CHECK(code_map[1].m_or == false);
        CHECK(code_map[2].m_match == 2);
        CHECK(code_map[2].m_code == "S244");
        //CHECK(code_map[2].m_or == false);
        CHECK(code_map[3].m_match == 4);
        CHECK(code_map[3].m_code == "FB");
        //CHECK(code_map[3].m_or == false);
        CHECK(code_map[4].m_match == 4);
        CHECK(code_map[4].m_code == "fC");
        //CHECK(code_map[4].m_or == false);
        CHECK(code_map[5].m_match == 3);
        CHECK(code_map[5].m_code == "~FR");
        //CHECK(code_map[5].m_or == false);
        CHECK(code_map[6].m_match == 2);
        CHECK(code_map[6].m_code == "s244");
        //CHECK(code_map[6].m_or == false);
        CHECK(code_map[7].m_match == 1);
        CHECK(code_map[7].m_code == "~S20");
        //CHECK(code_map[7].m_or == false);
    }

    TEST(ACSStringToCodeMapTestSecurityAndARFlags2Second)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "ASDCXZ(~oA~s10S244OBoC~ORs244~S20)sfsdf";
        code_map = acs.parseAcsString(test_expression);

        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
            return;
        }

        // Test Results of code map.
        // Not's are Match 1  SL
        // Normal Match 2     SL
        // Not's are Match 5  AR2
        // Normal Match 6     AR2
        CHECK(code_map[0].m_match == 5);
        CHECK(code_map[0].m_code == "~oA");
        //CHECK(code_map[0].m_or == false);
        CHECK(code_map[1].m_match == 1);
        CHECK(code_map[1].m_code == "~s10");
        //CHECK(code_map[1].m_or == false);
        CHECK(code_map[2].m_match == 2);
        CHECK(code_map[2].m_code == "S244");
        //CHECK(code_map[2].m_or == false);
        CHECK(code_map[3].m_match == 6);
        CHECK(code_map[3].m_code == "OB");
        //CHECK(code_map[3].m_or == false);
        CHECK(code_map[4].m_match == 6);
        CHECK(code_map[4].m_code == "oC");
        //CHECK(code_map[4].m_or == false);
        CHECK(code_map[5].m_match == 5);
        CHECK(code_map[5].m_code == "~OR");
        //CHECK(code_map[5].m_or == false);
        CHECK(code_map[6].m_match == 2);
        CHECK(code_map[6].m_code == "s244");
        //CHECK(code_map[6].m_or == false);
        CHECK(code_map[7].m_match == 1);
        CHECK(code_map[7].m_code == "~S20");
        //CHECK(code_map[7].m_or == false);
    }

    /**
     * Need to Impliment better way to split OR statements.
     * Most likely need a state to keep track and reset.
     * This would have to be in the Parse CodeMap.
     *
    TEST(ACSStringToCodeMapTestSecurityAndARFlagsOrStatements)
    {
        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "(~fA|~s10)S244FBfC~FRs244~S20sfsdf";
        code_map = acs.parseAcsString(test_expression);

        // Just a test note, make sure size > 0!
        if (code_map.size() < 1)
        {
            CHECK(false);
            return;
        }

        // Test Results of code map.
        // Not's are Match 1  SL
        // Normal Match 2     SL
        // Not's are Match 3  AR
        // Normal Match 4     AR
        CHECK(code_map[0].m_match == 3);
        CHECK(code_map[0].m_code == "~fA");
        CHECK(code_map[0].m_or == true);
        CHECK(code_map[1].m_match == 1);
        CHECK(code_map[1].m_code == "~s10");
        CHECK(code_map[1].m_or == true);
        CHECK(code_map[2].m_match == 2);
        CHECK(code_map[2].m_code == "S244");
        CHECK(code_map[2].m_or == false);
        CHECK(code_map[3].m_match == 4);
        CHECK(code_map[3].m_code == "FB");
        CHECK(code_map[3].m_or == false);
        CHECK(code_map[4].m_match == 4);
        CHECK(code_map[4].m_code == "fC");
        CHECK(code_map[4].m_or == false);
        CHECK(code_map[5].m_match == 3);
        CHECK(code_map[5].m_code == "~FR");
        CHECK(code_map[5].m_or == false);
        CHECK(code_map[6].m_match == 2);
        CHECK(code_map[6].m_code == "s244");
        CHECK(code_map[6].m_or == false);
        CHECK(code_map[7].m_match == 1);
        CHECK(code_map[7].m_code == "~S20");
        CHECK(code_map[7].m_or == false);
    }
    */

    TEST(ParseCodeMapTestSecurityLevelGood)
    {
        user_ptr user(new Users);
        user->iLevel = 20;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "s20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelGood2)
    {
        user_ptr user(new Users);
        user->iLevel = 30;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "s20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelBad1)
    {
        user_ptr user(new Users);
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "s20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTLevelGood)
    {
        user_ptr user(new Users);
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~s20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTLevelGood2)
    {
        user_ptr user(new Users);
        user->iLevel = 25;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~s20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTLevelBad)
    {
        user_ptr user(new Users);
        user->iLevel = 20;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~s20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    /////////////////////////////

    TEST(ParseCodeMapTestSecurityLevelGoodCaps)
    {
        user_ptr user(new Users);
        user->iLevel = 20;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "S20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelGood2Caps)
    {
        user_ptr user(new Users);
        user->iLevel = 30;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "S20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelBad1Caps)
    {
        user_ptr user(new Users);
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "S20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTLevelGoodCaps)
    {
        user_ptr user(new Users);
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~S20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTLevelGood2Caps)
    {
        user_ptr user(new Users);
        user->iLevel = 25;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~S20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTLevelBadCaps)
    {
        user_ptr user(new Users);
        user->iLevel = 20;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~S20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    /////////////////////


    TEST(ParseCodeMapTestSecurityARFlagsTrueSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "oB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityARFlagsFalseSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "oA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityARFlagsTrueCapsSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "OB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityARFlagsFalseCapsSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "OA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    //////////////////////////////////////

    TEST(ParseCodeMapTestSecurityARFlagsTrue)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "fB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityARFlagsFalse)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "fA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityARFlagsTrueCaps)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "FB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityARFlagsFalseCaps)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "FA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    //////////////////////////////////////

    TEST(ParseCodeMapTestSecurityNOTARFlagsTrueSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~oA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTARFlagsFalseSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~oB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTARFlagsTrueCapsSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~OA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTARFlagsFalseCapsSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~OB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityANDARFlagsTrueCapsSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~OA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    //////////////////////////////////////

    TEST(ParseCodeMapTestSecurityNOTARFlagsTrue)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~fA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTARFlagsFalse)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~fB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTARFlagsTrueCaps)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~FA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityNOTARFlagsFalseCaps)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~FB";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityANDARFlagsTrueCaps)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~FA";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    //////////////////////////////////////

    TEST(ParseCodeMapTestSecurityLevelAndNotARFlagsTrueCaps)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~FAs15";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelAndNotARFlagsTrueCapsSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~OAs15";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }


    TEST(ParseCodeMapTestSecurityLevelAndARFlagsTrueCaps)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "FBs10";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelAndARFlagsTrueCapsSecond)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "OBs10";
        code_map = acs.parseAcsString(test_expression);
        CHECK(acs.parseCodeMap(code_map, user));
    }

    //////////////////////////////////////

    TEST(ParseCodeMapTestSecurityLevelAndNotARFlagsTrueCapsFailSecurity)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~FAs20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelAndNotARFlagsTrueCapsSecondFailSecurity)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~OAs20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }


    TEST(ParseCodeMapTestSecurityLevelAndARFlagsTrueCapsFailSecurity)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "FBs20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelAndARFlagsTrueCapsSecondFailSecurity)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "OBs20";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    //////////////////////////////////////

    TEST(ParseCodeMapTestSecurityLevelAndNotARFlagsTrueCapsFailFlag)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~FBs15";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelAndNotARFlagsTrueCapsSecondFailFlag)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "~OBs15";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }


    TEST(ParseCodeMapTestSecurityLevelAndARFlagsTrueCapsFailFlag)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "FAs15";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(ParseCodeMapTestSecurityLevelAndARFlagsTrueCapsSecondFailFlag)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        std::vector<MapType> code_map;
        AccessCondition acs;

        std::string test_expression = "OAs15";
        code_map = acs.parseAcsString(test_expression);
        CHECK(!acs.parseCodeMap(code_map, user));
    }

    TEST(validateAcsStringTestFail)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        AccessCondition acs;

        std::string test_expression = "OAs15";
        bool result = acs.validateAcsString(test_expression, user);

        CHECK(!result);
    }

    TEST(validateAcsStringTest2Fail)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        AccessCondition acs;

        std::string test_expression = "FAs15";
        bool result = acs.validateAcsString(test_expression, user);

        CHECK(!result);
    }

    TEST(validateAcsStringTestSuccess)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        AccessCondition acs;

        std::string test_expression = "OBs15";
        bool result = acs.validateAcsString(test_expression, user);

        CHECK(result);
    }

    TEST(validateAcsStringTest2Success)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        AccessCondition acs;

        std::string test_expression = "FBs15";
        bool result = acs.validateAcsString(test_expression, user);

        CHECK(result);
    }

    TEST(validateAcsStringTestFailSL)
    {
        user_ptr user(new Users);
        user->iControlFlags2 |= 1 << 1; // B
        user->iLevel = 15;

        AccessCondition acs;

        std::string test_expression = "OBs25";
        bool result = acs.validateAcsString(test_expression, user);

        CHECK(!result);
    }

    TEST(validateAcsStringTest2FailSL)
    {
        user_ptr user(new Users);
        user->iControlFlags1 |= 1 << 1; // B
        user->iLevel = 15;

        AccessCondition acs;

        std::string test_expression = "FBs25";
        bool result = acs.validateAcsString(test_expression, user);

        CHECK(!result);
    }

    // No Restrictions should pass through True.
    TEST(validateAcsStringTestEmptyStringPass)
    {
        user_ptr user(new Users);
        AccessCondition acs;

        std::string test_expression = "";
        bool result = acs.validateAcsString(test_expression, user);

        CHECK(result);
    }

}
