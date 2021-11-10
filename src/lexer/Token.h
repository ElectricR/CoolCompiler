#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

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

static inline std::unordered_map<TokenType, std::string_view> token_name = {
    {TokenType::Integer, "INT_CONST"},
    {TokenType::Keyword, ""},
    {TokenType::TypeIdentifier, "TYPEID"},
    {TokenType::ObjectIdentifier, "OBJECTID"},
    {TokenType::String, "STR_CONST"},
    {TokenType::SpecialNotation, ""},
    {TokenType::BoolConst, "BOOL_CONST"},
    {TokenType::Assign, "ASSIGN"},
    {TokenType::DArrow, "DARROW"},
    {TokenType::LE, "LE"},
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
