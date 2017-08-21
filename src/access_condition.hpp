#ifndef ACCESS_CONDITION_HPP
#define ACCESS_CONDITION_HPP

#include "session_io.hpp"
#include "model-sys/structures.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <map>

class Users;
typedef boost::shared_ptr<Users> user_ptr;

/**
 * @class AccessCondition
 * @author Michael Griffin
 * @date 17/03/2017
 * @file access_condition.hpp
 * @brief Access Condition System handles Security Level and Flags
 */
class AccessCondition
{
public:
    explicit AccessCondition()  { };
    ~AccessCondition() { };
    
    /**
     * @brief Set Bit Flag on
     * @param flag
     * @param first_set
     * @param user
     */
    void setFlagOn(unsigned char flag, bool first_set, user_ptr user);

    /**
     * @brief Set Bit Flag off
     * @param flag
     * @param first_set
     * @param user
     */
    void setFlagOff(unsigned char flag, bool first_set, user_ptr user);

    /**
     * @brief Test If Bit Flag is set
     * @param flag
     * @param first_set
     * @param user
     * @return 
     */
    bool checkAccessConditionFlag(unsigned char flag, bool first_set, user_ptr user);
    
    /**
     * @brief Sets a Default String of Bitflags On
     * @param bitString
     */
    void setAccessConditionsFlagsOn(std::string bitString, bool first_set, user_ptr user);

    /**
     * @brief Sets a Default String of Bitflags Off
     * @param bitString
     */
    void setAccessConditionsFlagsOff(std::string bitString, bool first_set, user_ptr user);
        
    /**
     * @brief Parse Code Map and Test Secutiry and AR Flags.
     * @param code_map
     * @param user
     * @return 
     */
    bool parseCodeMap(const std::vector<MapType> &code_map, user_ptr user);

    /**
     * @brief Parse ASC Strings then test User Flags
     * @param acs_string
     * @param user
     * @return 
     */
    std::vector<MapType> parseAcsString(const std::string &acs_string);

    /**
     * @brief Parses and Validates codemap
     * @param expression
     * @return 
     */
    bool validateAcsString(const std::string &acs_string, user_ptr user);

    // Using Session IO for Code Mapping
    SessionIO  m_session_io;

    // note update to secutiry expressions
    // start with NOT s255 then test for s255
    // start with all not, but all normal will get caught and never pass through.
    // seperate expressons with | or. and ( )
    const std::string ACS_EXPRESSION = {
        "([~]{1}[sS]{1}\\d{1,3})|([sS]{1}\\d{1,3})|"
        "([~]{1}[fF]{1}[A-Z]{1})|([fF]{1}[A-Z]{1})|"
        "([~]{1}[oO]{1}[A-Z]{1})|([oO]{1}[A-Z]{1})"};
        

};

#endif // ACCESS_CONDITION_HPP
