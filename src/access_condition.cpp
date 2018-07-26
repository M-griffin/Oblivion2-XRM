#include "access_condition.hpp"
#include "model-sys/users.hpp"

#include <algorithm>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>

/**
 * @brief Set Bit Flag on
 * @param flag
 * @param first_set
 * @param user
 */
void AccessCondition::setFlagOn(unsigned char flag, bool first_set, user_ptr user)
{
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z
    
    if (bit < 0 || bit > 25)
    {
        std::cout << "Error, Invalid bit flag: " << bit << std::endl;
        return;        
    }
    
    if (first_set)
    {
        user->iControlFlags1 |= 1 << bit;
    }
    else
    {
        user->iControlFlags2 |= 1 << bit;
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
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z
    
    if (bit < 0 || bit > 25)
    {
        std::cout << "Error, Invalid bit flag: " << bit << std::endl;
        return;        
    }
        
    if (first_set)
    {
        user->iControlFlags1 &= ~(1 << bit);
    }
    else
    {
        user->iControlFlags2 &= ~(1 << bit);
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
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z
    
    if (bit < 0 || bit > 25)
    {
        std::cout << "Error, Invalid bit flag: " << bit << std::endl;
        return false;        
    }

    if (first_set)
    {
        return (user->iControlFlags1 >> bit) & 1;
    }

    return (user->iControlFlags2 >> bit) & 1;
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

        switch(my_matches.m_match)
        {
            case 1: // SL NOT
            {
                std::stringstream ss;
                ss << my_matches.m_code.substr(2);
                int not_level = 0;
                if (!(ss >> not_level))
                {
                    condition = false;
                }
                else if (not_level != user->iLevel)
                {
                    condition = true;
                }
                else
                {
                    condition = false;
                }
                break;
            }
            case 2: // SL
            {
                std::stringstream ss;
                ss << my_matches.m_code.substr(1);
                int level = 0;
                if (!(ss >> level))
                {
                    condition = false;
                }
                else if (level <= user->iLevel)
                {
                    condition = true;
                }
                else
                {
                    condition = false;
                }
                break;
            }
            case 3: // AR NOT
            {                
                std::string not_flag = my_matches.m_code.substr(2);
                if (!checkAccessConditionFlag(not_flag[0], true, user))
                {
                    condition = true;
                }
                else
                {
                    condition = false;
                }                
                break;
            }            
            case 4: // AR
            {
                std::string flag = my_matches.m_code.substr(1);
                if (checkAccessConditionFlag(flag[0], true, user))
                {
                    condition = true;
                }
                else
                {
                    condition = false;
                }                
                break;
            }               
            case 5: // AR2 NOT
            {                
                std::string not_flag = my_matches.m_code.substr(2);
                if (!checkAccessConditionFlag(not_flag[0], false, user))
                {
                    condition = true;
                }
                else
                {
                    condition = false;
                }                
                break;
            }            
            case 6: // AR2
            {
                std::string flag = my_matches.m_code.substr(1);
                if (checkAccessConditionFlag(flag[0], false, user))
                {
                    condition = true;
                }
                else
                {
                    condition = false;
                }
                
                break;
            }   
            
            default:
                break;
        }
        
        // Check if any conditions fail
        if (!condition)
        {
            break;
        }
            
    }  
    
    return condition;
}

/**
 * @brief String Token Parser.
 * @param token
 * @param seperator
 */
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Parse ASC Strings then test User Flags
 * @param acs_string
 * @param user
 * @return 
 */
std::vector<MapType> AccessCondition::parseAcsString(const std::string &acs_string)
{
    std::vector<MapType> code_map;

    // First split any OR statements.
    std::vector<std::string> tokens;
    
    tokens = split(acs_string, '|');
    if (tokens.size() > 1)
    {
        // Then we have multiple 'OR' statements.
        // Only handles single STMT | STMT at this point.
        // Multiple OR statement should be surrounded with ( ) 
        // NOTE, at this time OR is not handled, just breaking up statements.
        // Or would need to be handled in the code mapping
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
    
    return code_map;
}

/**
 * @brief Parses and Validates codemap
 * @param expression
 * @return 
 */
bool AccessCondition::validateAcsString(const std::string &acs_string, user_ptr user)
{
    std::vector<MapType> code_map;
    code_map = parseAcsString(acs_string);
    
    // Allow Access on empty string, meaning no security.
    if (code_map.size() == 0) 
    {
        return true;
    }
    return parseCodeMap(code_map, user);
}

/**
 * @brief Bit String to Printable String
 * @param bits
 * @return 
 */
std::string AccessCondition::getAccessConditionFlagStringFromBits(int bits)
{    
    std::string bit_string = "";
    for (int i = 0; i < 26; i++)
    {
        if ((bits >> i) & 1)
        {
            bit_string += static_cast<char>(i + 65);
        }
        else {
            bit_string += '-';
        }
    }
    
    return bit_string;
}
