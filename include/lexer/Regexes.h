#pragma once

#include "Token.h"

#include <vector>
#include <utility>
#include <string>
#include <regex>


namespace cool {


namespace lexer {


static inline std::vector<std::pair<std::regex, TokenType>> token_type_regexes {
    {std::regex(R"(([cC][lL][aA][sS][sS])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([eE][lL][sS][eE])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"((f[aA][lL][sS][eE])($|\W.*))"), TokenType::BoolConst},
    {std::regex(R"(([fF][iI])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([iI][fF])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([iI][nN])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([iI][nN][hH][eE][rR][iI][tT][sS])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([iI][sS][vV][oO][iI][dD])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([lL][eE][tT])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([lL][oO][oO][pP])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([pP][oO][oO][lL])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([tT][hH][eE][nN])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([wW][hH][iI][lL][eE])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([cC][aA][sS][eE])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([eE][sS][aA][cC])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([nN][eE][wW])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([oO][fF])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"(([nN][oO][tT])($|\W.*))"), TokenType::Keyword},
    {std::regex(R"((t[rR][uU][eE])($|\W.*))"), TokenType::BoolConst},
    {std::regex(R"((\d+)(.*))"), TokenType::Integer},
    {std::regex(R"(([a-z]\w*)(.*))"), TokenType::ObjectIdentifier},
    {std::regex(R"(([A-Z]\w*)(.*))"), TokenType::TypeIdentifier},
    {std::regex(R"((\{)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\})(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\()(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\))(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\:)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\<\-)(.*))"), TokenType::Assign},
    {std::regex(R"((\<\=)(.*))"), TokenType::LE},
    {std::regex(R"((\=\>)(.*))"), TokenType::DArrow},
    {std::regex(R"((\=)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\<)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\>)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\;)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\-)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\+)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\*)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\/)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\.)(.*))"), TokenType::SpecialNotation},
    {std::regex(R"((\,)(.*))"), TokenType::SpecialNotation},
};


} // namespace lexer


} // namespace cool
