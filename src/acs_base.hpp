#ifndef ACCESS_CONDITION_HPP
#define ACCESS_CONDITION_HPP

#include <string>

#include "io_session.hpp"

struct AcsCodeMapType;
struct AcsNode;
class Users;
class AccessLevel;
class UtilLog;


/**
 * @class AcsBase
 * @author Michael Griffin
 * @date 17/03/2017
 * @file acs_base.hpp
 * @brief Access Condition System handles Security Level and Flags
 */
class AcsBase {
public:
    explicit AcsBase();
    ~AcsBase();

    /**
     * @brief Toggle Bit Flag
     * @param flag
     * @param first_set
     * @param user
     */
    void setFlagToggle(unsigned char flag, bool first_set, Users &user);

    /**
     * @brief Toggle Bit Flag
     * @param flag
     * @param first_set
     * @param level
     */
    void setFlagLevelToggle(unsigned char flag, bool first_set, AccessLevel &level);

    /**
     * @brief Set Bit Flag on
     * @param flag
     * @param first_set
     * @param user
     */
    void setFlagOn(unsigned char flag, bool first_set, Users &user);

    /**
     * @brief Set Bit Flag off
     * @param flag
     * @param first_set
     * @param user
     */
    void setFlagOff(unsigned char flag, bool first_set, Users &user);

    /**
     * @brief Test If Bit Flag is set
     * @param flag
     * @param first_set
     * @param user
     * @return
     */
    bool checkAcsFlag(unsigned char flag, bool first_set, Users &user);

    /**
     * @brief Sets a Default String of Bit flags On
     * @param bitString
     * @param first_set
     * @param user
     */
    void setAcsFlagsOn(const std::string& bitString, bool first_set, Users &user);

    /**
     * @brief Sets a Default String of Bit flags Off
     * @param bitString
     * @param first_set
     * @param user
     */
    void setAcsFlagsOff(const std::string& bitString, bool first_set, Users &user);

    /**
     * @brief Parses and Validates code map
     * @param acs_string
     * @param user
     * @return
     */
    bool validateAcsString(const std::string &acs_string, Users &user);

    /**
     * @brief Bit String to Printable String
     * @param bits
     * @return
     */
    std::string getAcsFlagsFromBits(int bits);

    bool evalAcs(const AcsNode &node, Users &user);

    int parseNumber(const std::string &s, size_t start = 1);

    bool evalCondition(const AcsCodeMapType &cond, Users &user);

    // Using Session IO for Code Mapping
    UtilLog &m_log;

};

#endif
