#include "access_condition.hpp"
#include "model/users.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>


/**
 * @brief Set Bit Flag on
 * @param flag
 * @param first_set
 * @param user
 */
void AccessCondition::setFlagOn(unsigned char flag, bool first_set, user_ptr user)
{
    int bit = flag -= 65; // Handles A - Z
    std::cout << "Bit Flag: " << bit << std::endl;
    if (first_set)
    {
        user->iControlFlags1 |= bit;
    }
    else
    {
        user->iControlFlags2 |= bit;
    }    
}

/**
 * @brief Set Bit Flag off
 * @param flag
 * @param first_set
 * @param user
 */
void AccessCondition::setFlagOff(unsigned char flag, bool first_set, user_ptr user)
{
    int bit = flag -= 65; // Handles A - Z
    std::cout << "Bit Flag: " << bit << std::endl;
    if (first_set)
    {
        user->iControlFlags1 &= ~bit;
    }
    else
    {
        user->iControlFlags2 &= ~bit;
    }    
}

/**
 * @brief Test If Bit Flag is set
 * @param flag
 * @param first_set
 * @param user
 * @return 
 */
bool AccessCondition::checkAccessConditionFlag(unsigned char flag, bool first_set, user_ptr user)
{
    int bit = flag -= 65; // Handles A - Z
    std::cout << "Bit Flag: " << bit << std::endl;
    if (first_set)
    {
        return user->iControlFlags1 & bit;
    }

    return user->iControlFlags2 & bit;
}

/**
 * @brief Sets a Default String of Bitflags On
 * @param bitString
 */
void AccessCondition::setAccessConditionsFlagsOn(std::string bitString, bool first_set, user_ptr user)
{
    for (char flag : bitString)
    {
        setFlagOn((unsigned char)flag, first_set, user);
    }    
}

/**
 * @brief Sets a Default String of Bitflags Off
 * @param bitString
 */
void AccessCondition::setAccessConditionsFlagsOff(std::string bitString, bool first_set, user_ptr user)
{
    for (char flag : bitString)
    {
        setFlagOff((unsigned char)flag, first_set, user);
    }    
}

/**
 * @brief Parse Code Map and Test Secutiry and AR Flags.
 * @param code_map
 * @param user
 * @return 
 */
bool AccessCondition::parseCodeMap(const std::vector<MapType> &code_map, user_ptr user)
{
    bool condition = false;
      
    MapType my_matches;

    // Make a copy so the original is not modified.
    std::vector<MapType> code_mapping;
    code_mapping.assign(code_map.begin(), code_map.end());

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.
    while(code_mapping.size() > 0)
    {
        // Loop Backwards to perserve string offsets on replacement.
        my_matches = code_mapping.back();
        code_mapping.pop_back();

        // Check for Custom Screen Translation Mappings
        // If these exist, they take presidence over standard codes
        //if (m_mapped_codes.size() > 0)
        //{
            
            /*
            std::map<std::string, std::string>::iterator it;
            it = m_mapped_codes.find(my_matches.m_code);
            if (it != m_mapped_codes.end())
            {
                //std::cout << "gen found: " << my_matches.m_code << " : " << it->second << std::endl;
                // If found, replace mci sequence with text
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, it->second);
            }
            else
            {
                //std::cout << "gen not found: " << my_matches.m_code << std::endl;
                std::string remove_code = "";
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, remove_code);
            }*/
        //}
    }  
    
    return condition;
}

/**
 * @brief Parse ASC Strings then test User Flags
 * @param acs_string
 * @param user
 * @return 
 */
bool AccessCondition::parseAcsString(std::string &acs_string, user_ptr user)
{
    bool condition = false;
    std::vector<MapType> code_map;
    
    // First split string 
    if (user)
    {
        // First split any OR statements.
        std::vector<std::string> tokens;
        boost::split(tokens, acs_string, boost::is_any_of("|"));

        if (tokens.size() > 1)
        {
            // Then we have multiple statements.
            for (std::string t : tokens)
            {
                std::vector<MapType> tmp = m_session_io.parseToCodeMap(t, ACS_EXPRESSION);          
                code_map.insert( code_map.end(), tmp.begin(), tmp.end() );
            }
        }
        else 
        {
            code_map = m_session_io.parseToCodeMap(acs_string, ACS_EXPRESSION);
        }
        
        // Parse code map to test Secutiry and AR Flags.
        condition = parseCodeMap(code_map, user);        
    }
    
    return condition;
}
