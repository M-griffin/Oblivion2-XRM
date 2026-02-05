#ifndef ACCESS_CONDITION_HPP
#define ACCESS_CONDITION_HPP

#include <string>
#include <vector>

#include "io_session.hpp"
#include "model-sys/structures.hpp"

struct AcsCodeMapType;
struct AcsNode;
class Users;
class AccessLevel;
class Logging;


/**
 * @class AcsBase
 * @author Michael Griffin
 * @date 17/03/2017
 * @file access_condition.hpp
 * @brief Access Condition System handles Security Level and Flags
 */
class AcsBase {
public:
    explicit AcsBase(IoSession &io);

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
    bool checkAccessConditionFlag(unsigned char flag, bool first_set, Users &user);

    /**
     * @brief Sets a Default String of Bit flags On
     * @param bitString
     * @param first_set
     * @param user
     */
    void setAccessConditionsFlagsOn(std::string bitString, bool first_set, Users &user);

    /**
     * @brief Sets a Default String of Bit flags Off
     * @param bitString
     * @param first_set
     * @param user
     */
    void setAccessConditionsFlagsOff(std::string bitString, bool first_set, Users &user);

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
    std::string getAccessConditionFlagStringFromBits(int bits);

    bool evalAcs(const AcsNode &node, Users &user);

    int parseNumber(const std::string &s, size_t start = 1);

    bool evalCondition(const AcsCodeMapType &cond, Users &user);

    // Using Session IO for Code Mapping
    Logging &m_log;
    IoSession &m_session_io;

};

#endif
