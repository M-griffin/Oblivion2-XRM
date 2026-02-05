#ifndef ACS_PARSER_HPP
#define ACS_PARSER_HPP

#include <vector>

#include "acs_lexer.hpp"

/*
Highest:   ()
           ~
           AND (implicit)
Lowest:    |
*/
enum class AcsConditionKind {
    SecurityLevel,
    UserFlag,
    AltFlag,
    NumericCompare
};

enum class NumericField {
    Age,
    TimeLeft,
    Calls,
    CallsToday,
    UploadKB,
    DownloadKB,
    Node,
    Menu
};

enum class AcsNodeType {
    And,
    Or,
    Not,
    Condition
};

struct AcsCodeMapType {
    AcsConditionKind kind;
    NumericField field;
    int value;
    bool negated;
    std::string code; // raw token text (ex: "S50", "FA", "OZ")

    char op; // 'S', 'F', 'O', 'Q', etc
    int number = 0; // numeric suffix (if any)
    char flag = 0; // A–Z if applicable
    std::string raw; // original token
};

struct AcsNode {
    AcsNodeType type;
    AcsCodeMapType condition;
    std::vector<AcsNode> children;
};


class AcsParser {
public:
    explicit AcsParser(const std::string &s)
        : lexer(s), current(lexer.next()) {
    }

    AcsNode parse() {
        return parseExpression();
    }

private:
    AcsLexer lexer;
    Token current;

    void advance() { current = lexer.next(); }

    AcsNode parseExpression() {
        AcsNode node = parseTerm();
        while (current.type == TokenType::Or) {
            advance();
            AcsNode rhs = parseTerm();
            node = AcsNode{AcsNodeType::Or, {}, {node, rhs}};
        }
        return node;
    }

    AcsNode parseTerm() {
        std::vector<AcsNode> nodes;
        while (current.type == TokenType::Identifier ||
               current.type == TokenType::Not ||
               current.type == TokenType::LParen) {
            nodes.push_back(parseFactor());
        }

        if (nodes.size() == 1)
            return nodes[0];

        return AcsNode{AcsNodeType::And, {}, nodes};
    }

    AcsNode parseFactor() {
        if (current.type == TokenType::Not) {
            advance();
            return AcsNode{AcsNodeType::Not, {}, {parseFactor()}};
        }

        if (current.type == TokenType::LParen) {
            advance();
            AcsNode node = parseExpression();
            if (current.type == TokenType::RParen)
                advance();
            return node;
        }

        // Condition
        AcsCodeMapType cond = parseCondition(current.text);
        advance();
        return AcsNode{AcsNodeType::Condition, cond, {}};
    }

    AcsCodeMapType parseCondition(const std::string &text) {
        AcsCodeMapType cond;
        cond.raw = text;

        if (text.empty())
            return cond;

        cond.op = std::toupper(text[0]);

        // -------------------------
        // Flag-based operators
        // -------------------------
        if ((cond.op == 'F' || cond.op == 'O') && text.size() >= 2) {
            cond.flag = std::toupper(text[1]);
            return cond;
        }

        // -------------------------
        // Numeric operators
        // -------------------------
        if (text.size() > 1 && std::isdigit(text[1])) {
            try {
                cond.number = std::stoi(text.substr(1));
            } catch (...) {
                cond.number = 0;
            }
            return cond;
        }

        // -------------------------
        // Single-char operators (* @ # etc)
        // -------------------------
        return cond;
    }
};


#endif
