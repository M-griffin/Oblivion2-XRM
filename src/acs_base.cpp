#include "acs_base.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include "acs_parser.hpp"
#include "model-sys/users.hpp"
#include "model-sys/access_level.hpp"

#include "io_session.hpp"
#include "logging.hpp"

/**
 * @brief Class Constructor And Initialization
 * @return 
 */
AcsBase::AcsBase(IoSession &io)
    : m_log(Logging::getInstance())
      , m_session_io(io) {
}

AcsBase::~AcsBase() {
    m_log.log(Logging::LogLevel::Debug, "~AcsBase()");
}

/**
 * @brief Toggle Bit Flag
 * @param flag
 * @param first_set
 * @param user
 */
void AcsBase::setFlagToggle(unsigned char flag, bool first_set, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(Logging::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (first_set) {
        user.iControlFlags1 ^= 1 << bit;
    } else {
        user.iControlFlags2 ^= 1 << bit;
    }
}

/**
 * @brief Toggle Bit Flag
 * @param flag
 * @param first_set
 * @param level
 */
void AcsBase::setFlagLevelToggle(unsigned char flag, bool first_set, AccessLevel &level) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(Logging::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (first_set) {
        level.iARFlags1 ^= 1 << bit;
    } else {
        level.iARFlags2 ^= 1 << bit;
    }
}

/**
 * @brief Set Bit Flag on
 * @param flag
 * @param first_set
 * @param user
 */
void AcsBase::setFlagOn(unsigned char flag, bool first_set, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(Logging::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (first_set) {
        user.iControlFlags1 |= 1 << bit;
    } else {
        user.iControlFlags2 |= 1 << bit;
    }
}

/**
 * @brief Set Bit Flag off
 * @param flag
 * @param first_set
 * @param user
 */
void AcsBase::setFlagOff(unsigned char flag, bool first_set, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(Logging::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return;
    }

    if (first_set) {
        user.iControlFlags1 &= ~(1 << bit);
    } else {
        user.iControlFlags2 &= ~(1 << bit);
    }
}

/**
 * @brief Test If Bit Flag is set
 * @param flag
 * @param first_set
 * @param user
 * @return
 */
bool AcsBase::checkAccessConditionFlag(unsigned char flag, bool first_set, Users &user) {
    int bit = toupper(flag);
    bit -= 65; // Handles A - Z

    if (bit < 0 || bit > 25) {
        m_log.log(Logging::LogLevel::Error, "Error, Invalid bit flag=", bit, __FILE__, __LINE__);
        return false;
    }

    if (first_set) {
        return (user.iControlFlags1 >> bit) & 1;
    }

    return (user.iControlFlags2 >> bit) & 1;
}

/**
 * @brief Sets a Default String of Bit flags On
 * @param bitString
 * @param first_set
 * @param user
 */
void AcsBase::setAccessConditionsFlagsOn(std::string bitString, bool first_set, Users &user) {
    for (char flag: bitString) {
        setFlagOn(static_cast<unsigned char>(flag), first_set, user);
    }
}

/**
 * @brief Sets a Default String of Bit flags Off
 * @param bitString
 * @param first_set
 * @param user
 */
void AcsBase::setAccessConditionsFlagsOff(std::string bitString, bool first_set, Users &user) {
    for (char flag: bitString) {
        setFlagOff(static_cast<unsigned char>(flag), first_set, user);
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
 * @param acs_string
 * @param user
 * @return
 */
bool AcsBase::validateAcsString(const std::string &acs_string, Users &user) {
    if (acs_string.empty()) {
        return true;
    }

    AcsParser parser(acs_string);
    AcsNode tree = parser.parse();
    return evalAcs(tree, user);
}

/**
 * @brief Bit String to Printable String
 * @param bits
 * @return
 */
std::string AcsBase::getAccessConditionFlagStringFromBits(int bits) {
    std::string bit_string;

    for (int i = 0; i < 26; i++) {
        if ((bits >> i) & 1) {
            bit_string += static_cast<char>(i + 65);
        } else {
            bit_string += '-';
        }
    }

    return bit_string;
}

int AcsBase::parseNumber(const std::string &s, size_t start) {
    if (start >= s.size())
        return 0;

    try {
        return std::stoi(s.substr(start));
    } catch (...) {
        return 0;
    }
}

bool AcsBase::evalCondition(const AcsCodeMapType &cond, Users &user) {
    const std::string &code = cond.code;
    char op = std::toupper(code[0]);

    switch (op) {
        // --------------------------
        // Security level
        // --------------------------
        case 'S': {
            int level = parseNumber(code);
            return user.iLevel >= level;
        }

        // --------------------------
        // AR flags (set 1)
        // --------------------------
        case 'F': {
            char flag = std::toupper(code[1]);
            int bit = flag - 'A';
            if (bit < 0 || bit > 25) return false;
            return (user.iControlFlags1 >> bit) & 1;
        }

        // --------------------------
        // AR flags (set 2)
        // --------------------------
        case 'O': {
            char flag = std::toupper(code[1]);
            int bit = flag - 'A';
            if (bit < 0 || bit > 25) return false;
            return (user.iControlFlags2 >> bit) & 1;
        }

        // --------------------------
        // Calls today
        // --------------------------
        case 'E': {
            int v = parseNumber(code);
            //return user.callsToday >= v;
            return false; // TODO Implement
        }

        // --------------------------
        // Total calls
        // --------------------------
        case 'Q': {
            int v = parseNumber(code);
            //return user.calls >= v;
            return false; // TODO Implement
        }

        // --------------------------
        // User number
        // --------------------------
        case 'U': {
            int v = parseNumber(code);
            //return user.id == v;
            return false; // TODO Implement
        }

        // --------------------------
        // Age
        // --------------------------
        case 'A': {
            int v = parseNumber(code);
            //return user.age >= v;
            return false; // TODO Implement
        }

        // --------------------------
        // File points
        // --------------------------
        case 'P': {
            int v = parseNumber(code);
            //return user.filePoints >= v;
            return false; // TODO Implement
        }

        // --------------------------
        // Posts/calls ratio
        // --------------------------
        case '*': {
            return false; // TODO Implement
            /*
            if (user.calls == 0) return true;
            double ratio = double(user.posts) / double(user.calls);
            return ratio >= user.curPostCallRatio;
            */
        }

        // --------------------------
        // Upload/download ratio
        // --------------------------
        case '@': {
            // return false; // TODO Implement
            /*
            if (user.downloads == 0) return true;
            double ratio = double(user.uploads) / double(user.downloads);
            return ratio >= user.curUpDownRatio;
            */
        }

        // --------------------------
        // Unknown token
        // --------------------------
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
