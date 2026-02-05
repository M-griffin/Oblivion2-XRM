#ifndef ACS_LEXER_HPP
#define ACS_LEXER_HPP

#include <string>

enum class TokenType {
    Identifier, // S50, FA, ~, etc.
    Or, // |
    Not, // ~
    LParen, // (
    RParen, // )
    End
};

struct Token {
    TokenType type;
    std::string text;
};

class AcsLexer {
public:
    explicit AcsLexer(const std::string &s) : input(s), pos(0) {
    }

    Token next() {
        skipWhitespace();

        if (pos >= input.size())
            return {TokenType::End, ""};

        char c = input[pos];

        if (c == '|') {
            pos++;
            return {TokenType::Or, "|"};
        }
        if (c == '~') {
            pos++;
            return {TokenType::Not, "~"};
        }
        if (c == '(') {
            pos++;
            return {TokenType::LParen, "("};
        }
        if (c == ')') {
            pos++;
            return {TokenType::RParen, ")"};
        }

        // Identifier: S50, FA, Q100, etc.
        if (std::isalpha(c)) {
            size_t start = pos++;
            while (pos < input.size() &&
                   (std::isalnum(input[pos])))
                pos++;
            return {TokenType::Identifier, input.substr(start, pos - start)};
        }

        // Skip unknown characters
        pos++;
        return next();
    }

private:
    void skipWhitespace() {
        while (pos < input.size() && std::isspace(input[pos]))
            pos++;
    }

    std::string input;
    size_t pos;
};

#endif
