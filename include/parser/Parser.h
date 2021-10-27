#pragma once

#include "AST/AST.h"
#include "lexer/Lexer.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

namespace cool::parser {

namespace detail {

class ItWrapper {
public:
    explicit ItWrapper(const std::vector<lexer::Token>& tokens)
        : current_iterator(tokens.cbegin()), end(tokens.cend()) {}

    bool is_exhausted() const noexcept {
        return current_iterator == end;
    }

    void advance() noexcept {
        ++current_iterator;
    }

    const lexer::Token& current() noexcept {
        return *current_iterator;
    }

    lexer::Token next() const noexcept {
        return *(current_iterator + 1);
    }

    bool contains_next() const noexcept {
        return !is_exhausted() && ((current_iterator + 1) != end);
    }

private:
    std::vector<lexer::Token>::const_iterator current_iterator;
    std::vector<lexer::Token>::const_iterator end;
};

bool string_view_to_bool(std::string_view str_view) noexcept; 

} // namespace detail

class Parser {
public:
    explicit Parser(const lexer::Lexer& lexer) : token_it(lexer.get_result()) {
        result = parse_program();
    }

    [[nodiscard]] const std::optional<AST::Program>&
    get_result() const noexcept {
        return result;
    }

    [[nodiscard]] const std::stringstream& get_error() const noexcept {
        return error;
    }

private:
    [[nodiscard]] std::optional<AST::Program> parse_program() noexcept;

    [[nodiscard]] std::optional<AST::Class> parse_class() noexcept; 

    [[nodiscard]] std::optional<AST::Feature> parse_feature() noexcept; 

    [[nodiscard]] std::optional<AST::MethodFeature> parse_method_feature(
        auto& object_id) noexcept; 

    [[nodiscard]] std::optional<AST::Formal> parse_formal() noexcept; 

    [[nodiscard]] std::optional<AST::FieldFeature> parse_field_feature(
        auto& object_id) noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_8() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_7() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_6() noexcept; 
    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_5() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_4() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_2() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_3() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_1() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>>
    parse_expression_lvl_0() noexcept; 

    [[nodiscard]] std::optional<std::shared_ptr<AST::Expression>> parse_dot(
        std::shared_ptr<AST::Expression> expression_lvl_1, bool with_type,
        auto dot_token) noexcept; 

    [[nodiscard]] std::optional<AST::FunctionExpression> parse_function(
        auto object_id) noexcept; 

    [[nodiscard]] std::optional<AST::IfExpression> parse_if() noexcept; 

    [[nodiscard]] std::optional<AST::WhileExpression> parse_while() noexcept; 

    [[nodiscard]] std::optional<AST::CompoundExpression>
    parse_compound_expression() noexcept; 

    [[nodiscard]] std::optional<AST::CaseExpression> parse_case() noexcept; 

    [[nodiscard]] std::optional<AST::LetExpression> parse_let() noexcept; 

    [[nodiscard]] std::optional<AST::NewExpression> parse_new() noexcept; 

    [[nodiscard]] std::optional<lexer::Token> extract_token(
        lexer::TokenType required_token_type,
        std::string_view required_lexeme = "",
        bool raise_error = true) noexcept; 

    detail::ItWrapper token_it;
    std::optional<AST::Program> result;
    std::stringstream error;
};

} // namespace cool::parser
