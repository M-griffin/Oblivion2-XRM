#include "acs_base.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include "acs_parser.hpp"
#include "model-sys/users.hpp"
#include "model-sys/access_level.hpp"

#include "io_session.hpp"
#include "util_log.hpp"

/**
 * @brief Class Constructor And Initialization
 * @return 
 */
AcsBase::AcsBase()
    : m_log(UtilLog::getInstance()) {
}

AcsBase::~AcsBase() {
    m_log.log(UtilLog::LogLevel::Debug, "~AcsBase()");
}

/**
 * @brief Toggle Bit Flag
 * @param flag
 * @param firstSet
 * @param user
 */
void AcsBase::setFlagToggle(unsigned char flag, bool firstSet, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(UtilLog::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (firstSet) {
        user.iControlFlags1 ^= 1 << bit;
    } else {
        user.iControlFlags2 ^= 1 << bit;
    }
}

/**
 * @brief Toggle Bit Flag
 * @param flag
 * @param firstSet
 * @param level
 */
void AcsBase::setFlagLevelToggle(unsigned char flag, bool firstSet, AccessLevel &level) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(UtilLog::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (firstSet) {
        level.iARFlags1 ^= 1 << bit;
    } else {
        level.iARFlags2 ^= 1 << bit;
    }
}

/**
 * @brief Set Bit Flag on
 * @param flag
 * @param firstSet
 * @param user
 */
void AcsBase::setFlagOn(unsigned char flag, bool firstSet, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(UtilLog::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (firstSet) {
        user.iControlFlags1 |= 1 << bit;
    } else {
        user.iControlFlags2 |= 1 << bit;
    }
}

/**
 * @brief Set Bit Flag off
 * @param flag
 * @param firstSet
 * @param user
 */
void AcsBase::setFlagOff(unsigned char flag, bool firstSet, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(UtilLog::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (firstSet) {
        user.iControlFlags1 &= ~(1 << bit);
    } else {
        user.iControlFlags2 &= ~(1 << bit);
    }
}

/**
 * @brief Test If Bit Flag is set
 * @param flag
 * @param firstSet
 * @param user
 * @return
 */
bool AcsBase::checkAcsFlag(unsigned char flag, bool firstSet, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(UtilLog::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return false;
    }

    if (firstSet) {
        return (user.iControlFlags1 >> bit) & 1;
    }

    return (user.iControlFlags2 >> bit) & 1;
}

/**
 * @brief Sets a Default String of Bit flags On
 * @param bitString
 * @param firstSet
 * @param user
 */
void AcsBase::setAcsFlagsOn(const std::string &bitString, bool firstSet, Users &user) {
    for (char flag: bitString) {
        setFlagOn(static_cast<unsigned char>(flag), firstSet, user);
    }
}

/**
 * @brief Sets a Default String of Bit flags Off
 * @param bitString
 * @param firstSet
 * @param user
 */
void AcsBase::setAcsFlagsOff(const std::string &bitString, bool firstSet, Users &user) {
    for (char flag: bitString) {
        setFlagOff(static_cast<unsigned char>(flag), firstSet, user);
    }
}

/**
 * @brief String Token Parser.
 * @param s
 * @param delimiter
 */
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

/**
 * @brief Parses and Validates code map
 * @param acsString
 * @param user
 * @return
 */
bool AcsBase::validateAcsString(const std::string &acsString, Users &user) {
    if (acsString.empty()) {
        return true;
    }

    AcsParser parser(acsString);
    AcsNode tree = parser.parse();
    return evalAcs(tree, user);
}

/**
 * @brief Bit String to Printable String
 * @param bits
 * @return
 */
std::string AcsBase::getAcsFlagsFromBits(int bits) {
    std::string bitString;

    for (int i = 0; i < 26; i++) {
        if ((bits >> i) & 1) {
            bitString += static_cast<char>(i + 65);
        } else {
            bitString += '-';
        }
    }

    return bitString;
}

int AcsBase::parseNumber(const std::string &s, size_t start) {
    if (start >= s.size()) {
        return 0;
    }

    try {
        return std::stoi(s.substr(start));
    } catch (...) {
        return 0;
    }
}

bool AcsBase::evalCondition(const AcsCodeMapType &c, Users &user) {
    switch (c.op) {

        // --- Shorthands ---
        case '=': // The "Is Sysop" shorthand
            return user.iLevel == 255;

        case '\'': // The "Is Co-Sysop" shorthand
            // Often check for Level 255 OR a specific bit flag like 'A' on set 1
            return user.iLevel >= 250 || ((user.iControlFlags1 >> 0) & 1);

        case '!': // Not Equals (if applicable)
            return user.iLevel != c.number;

        // --- Standard Opcodes ---
        case 'S':
            return user.iLevel >= c.number;

        case 'F': {
            int bit = c.flag - 'A';
            return bit >= 0 && bit < 26 && ((user.iControlFlags1 >> bit) & 1);
        }

        case 'O': {
            int bit = c.flag - 'A';
            return bit >= 0 && bit < 26 &&
                   ((user.iControlFlags2 >> bit) & 1);
        }

        // User Specific Condition Checks.
        case 'A':
            return false; // TODO Implement
        //return user.age >= c.number;

        case 'Q':
            return false; // TODO Implement
        //return user.calls >= c.number;

        case 'E':
            return false; // TODO Implement
        //return user.callsToday >= c.number;

        case 'U':
            return false; // TODO Implement
        //return user.id == c.number;

        case 'P':
            return false; // TODO Implement
        //return user.filePoints >= c.number;

        case '*':
            return false; // TODO Implement
        //if (user.calls == 0) return true;
        //return (double)user.posts / user.calls >= user.curPostCallRatio;

        case '@':
            return false; // TODO Implement
        //if (user.downloads == 0) return true;
        //return (double)user.uploads / user.downloads >= user.curUpDownRatio;

        default:
            return false;
    }
}

bool AcsBase::evalAcs(const AcsNode &node, Users &user) {
    switch (node.type) {
        case AcsNodeType::Condition:
            return evalCondition(node.condition, user);

        case AcsNodeType::Not:
            return !evalAcs(node.children[0], user);

        case AcsNodeType::And:
            for (const auto &c: node.children)
                if (!evalAcs(c, user)) return false;
            return true;

        case AcsNodeType::Or:
            for (const auto &c: node.children)
                if (evalAcs(c, user)) return true;
            return false;
    }
    return false;
}
