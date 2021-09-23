#pragma once

#include <string>

namespace cool {

namespace lexer {

enum class TokenType {
    Integer,
    Keyword,
    TypeIdentifier,
    ObjectIdentifier,
    String,
    SpecialNotation,
    BoolConst,
    Assign,
    DArrow,
    LE,
    Error
};

struct Token {
    unsigned line_number;
    TokenType token_type;
    std::string lexeme;

    [[nodiscard]] int operator==(const Token& other) const noexcept {
        return token_type == other.token_type && lexeme == other.lexeme;
    }
};

} // namespace lexer

} // namespace cool
