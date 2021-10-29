#include "parser/Parser.h"

bool cool::parser::detail::string_view_to_bool(std::string_view str_view) noexcept {
    bool result = false;
    std::istringstream ss{std::string{str_view}};
    ss >> std::boolalpha >> result;
    return result;
}

std::optional<cool::AST::Program>
cool::parser::Parser::parse_program() noexcept {
    cool::AST::Program program;

    while (!token_it.is_exhausted()) {
        auto class_ = parse_class();
        auto semicolon = extract_token(lexer::TokenType::SpecialNotation, ";");
        if (semicolon && class_) {
            program.classes.push_back(std::move(class_.value()));
        } else {
            return {};
        }
    }
    if (program.classes.empty()) {
        return {};
    }
    return program;
}

[[nodiscard]] std::optional<cool::AST::Class>
cool::parser::Parser::parse_class() noexcept {
    auto class_token = extract_token(lexer::TokenType::Keyword, "CLASS");
    auto type_id = extract_token(lexer::TokenType::TypeIdentifier);
    auto inherits_keyword =
        extract_token(lexer::TokenType::Keyword, "INHERITS", false);
    auto inherits_type_id = extract_token(
        lexer::TokenType::TypeIdentifier, "", inherits_keyword.has_value());
    auto opening_parenthesis =
        extract_token(lexer::TokenType::SpecialNotation, "{");

    if (!class_token || !type_id ||
        (inherits_keyword.has_value() ^ inherits_type_id.has_value()) ||
        !opening_parenthesis) {
        return {};
    }

    std::vector<cool::AST::Feature> features;
    while (true) {
        auto closing_parenthesis =
            extract_token(lexer::TokenType::SpecialNotation, "}", false);
        if (closing_parenthesis) {
            cool::AST::Class class_ = {std::string{type_id.value().lexeme}, {},
                features, type_id.value().line_number, filepath};
            if (inherits_type_id.has_value()) {
                class_.inherits = inherits_type_id.value().lexeme;
            }
            return class_;
        }
        if (token_it.is_exhausted()) {
            error << "Expected \"}\"." << std::endl;
            return {};
        }
        auto feature = parse_feature();
        if (!feature) {
            return {};
        } else {
            features.push_back(std::move(feature.value()));
        }
    }
}

[[nodiscard]] std::optional<cool::AST::Feature>
cool::parser::Parser::parse_feature() noexcept {
    auto object_id = extract_token(lexer::TokenType::ObjectIdentifier);
    auto brace = extract_token(lexer::TokenType::SpecialNotation, "(", false);
    auto colon = extract_token(
        lexer::TokenType::SpecialNotation, ":", !brace.has_value());

    if (object_id && brace) {
        auto method_feature = parse_method_feature(object_id);
        auto semicolon = extract_token(lexer::TokenType::SpecialNotation, ";");
        if (method_feature && semicolon) {
            return {{method_feature.value()}};
        }
    } else if (object_id && colon) {
        auto field_feature = parse_field_feature(object_id);
        auto semicolon = extract_token(lexer::TokenType::SpecialNotation, ";");
        if (field_feature && semicolon) {
            return {{field_feature.value()}};
        }
    }
    return {};
}

[[nodiscard]] std::optional<cool::AST::MethodFeature>
cool::parser::Parser::parse_method_feature(auto& object_id) noexcept {
    bool found_formal = false;
    std::vector<cool::AST::Formal> formals;

    while (true) {
        auto brace =
            extract_token(lexer::TokenType::SpecialNotation, ")", false);
        if (brace) {
            break;
        }
        if (token_it.is_exhausted()) {
            error << "Expected \"}\"." << std::endl;
            return {};
        }
        if (found_formal) {
            auto comma = extract_token(lexer::TokenType::SpecialNotation, ",");
            if (!comma) {
                return {};
            }
        }
        auto formal = parse_formal();
        if (formal) {
            found_formal = true;
            formals.push_back(std::move(formal.value()));
        } else {
            return {};
        }
    }

    auto colon = extract_token(lexer::TokenType::SpecialNotation, ":");
    auto type_id = extract_token(lexer::TokenType::TypeIdentifier);
    auto opening_parenthesis =
        extract_token(lexer::TokenType::SpecialNotation, "{");
    auto expression = parse_expression_lvl_8();
    auto closing_parenthesis =
        extract_token(lexer::TokenType::SpecialNotation, "}");

    if (colon && type_id && opening_parenthesis && expression &&
        closing_parenthesis) {
        return {{std::string{object_id.value().lexeme}, std::move(formals),
            std::string{type_id.value().lexeme}, expression.value(),
            object_id.value().line_number}};
    }
    return {};
}

[[nodiscard]] std::optional<cool::AST::Formal>
cool::parser::Parser::parse_formal() noexcept {
    auto object_id = extract_token(lexer::TokenType::ObjectIdentifier);
    auto colon = extract_token(lexer::TokenType::SpecialNotation, ":");
    auto type_id = extract_token(lexer::TokenType::TypeIdentifier);

    if (object_id && colon && type_id) {
        return {{std::string{object_id.value().lexeme},
            std::string{type_id.value().lexeme},
            object_id.value().line_number}};
    }
    return {};
}

[[nodiscard]] std::optional<cool::AST::FieldFeature>
cool::parser::Parser::parse_field_feature(auto& object_id) noexcept {
    auto type_id = extract_token(lexer::TokenType::TypeIdentifier);
    if (type_id) {
        auto assign = extract_token(lexer::TokenType::Assign, "", false);
        if (!assign) {
            return {{std::string{object_id.value().lexeme},
                std::string{type_id.value().lexeme}, {},
                object_id.value().line_number}};
        }
        auto expression = parse_expression_lvl_8();
        if (expression) {
            return {{std::string{object_id.value().lexeme},
                std::string{type_id.value().lexeme}, {expression.value()},
                object_id.value().line_number}};
        }
    }
    return {};
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_8() noexcept {
    if (token_it.contains_next()) {
        auto current = token_it.current();
        auto next = token_it.next();
        if (current.token_type == lexer::TokenType::ObjectIdentifier &&
            next.token_type == lexer::TokenType::Assign) {
            token_it.advance();
            token_it.advance();
            auto expression = parse_expression_lvl_8();
            if (expression) {
                return std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{
                        cool::AST::AssignExpression{current.lexeme,
                            expression.value(), current.line_number}});
            }
            return {};
        }
    }

    return parse_expression_lvl_7();
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_7() noexcept {
    auto not_token = extract_token(lexer::TokenType::Keyword, "NOT", false);
    if (not_token) {
        auto expression = parse_expression_lvl_7();
        if (expression) {
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{cool::AST::NotExpression{
                    expression.value(), not_token.value().line_number}});
        }
        return {};
    }
    return parse_expression_lvl_6();
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_6() noexcept {
    auto left_expression = parse_expression_lvl_5();
    if (!left_expression) {
        return {};
    }
    while (true) {
        bool token_found = false;
        auto less_token =
            extract_token(lexer::TokenType::SpecialNotation, "<", false);
        if (less_token) {
            auto right_expression = parse_expression_lvl_6();
            if (right_expression) {
                token_found = true;
                left_expression = std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{cool::AST::LessExpression{
                        {left_expression.value(), right_expression.value(),
                            less_token.value().line_number}}});
                continue;
            }
            return {};
        }
        auto LE_token = extract_token(lexer::TokenType::LE, "", false);
        if (LE_token) {
            auto right_expression = parse_expression_lvl_6();
            if (right_expression) {
                token_found = true;
                left_expression = std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{cool::AST::LEExpression{
                        {left_expression.value(), right_expression.value(),
                            LE_token.value().line_number}}});
                continue;
            }
            return {};
        }
        auto equal_token =
            extract_token(lexer::TokenType::SpecialNotation, "=", false);
        if (equal_token) {
            auto right_expression = parse_expression_lvl_6();
            if (right_expression) {
                token_found = true;
                left_expression = std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{cool::AST::EqualExpression{
                        {left_expression.value(), right_expression.value(),
                            equal_token.value().line_number}}});
                continue;
            }
            return {};
        }
        if (!token_found) {
            break;
        }
    }
    return left_expression;
}
[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_5() noexcept {
    auto left_expression = parse_expression_lvl_4();
    if (!left_expression) {
        return {};
    }
    while (true) {
        bool token_found = false;
        auto plus_token =
            extract_token(lexer::TokenType::SpecialNotation, "+", false);
        if (plus_token) {
            token_found = true;
            auto right_expression = parse_expression_lvl_4();
            if (right_expression) {
                left_expression = std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{cool::AST::PlusExpression{
                        {left_expression.value(), right_expression.value(),
                            plus_token.value().line_number}}});
                continue;
            }
            return {};
        }
        auto minus_token =
            extract_token(lexer::TokenType::SpecialNotation, "-", false);
        if (minus_token) {
            token_found = true;
            auto right_expression = parse_expression_lvl_4();
            if (right_expression) {
                left_expression = std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{cool::AST::MinusExpression{
                        {left_expression.value(), right_expression.value(),
                            minus_token.value().line_number}}});
                continue;
            }
            return {};
        }
        if (!token_found) {
            break;
        }
    }
    return left_expression;
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_4() noexcept {
    auto left_expression = parse_expression_lvl_3();
    if (!left_expression) {
        return {};
    }
    while (true) {
        bool token_found = false;
        auto multiply_token =
            extract_token(lexer::TokenType::SpecialNotation, "*", false);
        if (multiply_token) {
            auto right_expression = parse_expression_lvl_3();
            if (right_expression) {
                token_found = true;
                left_expression = std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{cool::AST::MultiplyExpression{
                        {left_expression.value(), right_expression.value(),
                            multiply_token.value().line_number}}});
                continue;
            }
            return {};
        }
        auto divide_token =
            extract_token(lexer::TokenType::SpecialNotation, "/", false);
        if (divide_token) {
            auto right_expression = parse_expression_lvl_3();
            if (right_expression) {
                token_found = true;
                left_expression = std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{cool::AST::DivideExpression{
                        {left_expression.value(), right_expression.value(),
                            divide_token.value().line_number}}});
                continue;
            }
            return {};
        }
        if (!token_found) {
            break;
        }
    }
    return left_expression;
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_2() noexcept {
    auto tilde_token =
        extract_token(lexer::TokenType::SpecialNotation, "~", false);
    if (tilde_token) {
        auto expression = parse_expression_lvl_2();
        if (expression) {
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{cool::AST::TildeExpression{
                    expression.value(), tilde_token.value().line_number}});
        }
        return {};
    }
    return parse_expression_lvl_1();
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_3() noexcept {
    auto isvoid_token =
        extract_token(lexer::TokenType::Keyword, "ISVOID", false);
    if (isvoid_token) {
        auto expression = parse_expression_lvl_3();
        if (expression) {
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{cool::AST::IsVoidExpression{
                    expression.value(), isvoid_token.value().line_number}});
        }
        return {};
    }
    return parse_expression_lvl_2();
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_1() noexcept {
    auto expression = parse_expression_lvl_0();
    if (!expression) {
        return {};
    }
    while (true) {
        auto dot_token =
            extract_token(lexer::TokenType::SpecialNotation, ".", false);
        auto at_token =
            extract_token(lexer::TokenType::SpecialNotation, "@", false);
        if (dot_token || at_token) {
            expression =
                parse_dot(expression.value(), at_token.has_value(), dot_token);
            if (!expression) {
                return {};
            }
            continue;
        }
        return expression;
    }
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_expression_lvl_0() noexcept {
    auto if_token = extract_token(lexer::TokenType::Keyword, "IF", false);
    if (if_token) {
        auto if_result = parse_if();
        if (if_result) {
            if_result.value().line_number = if_token.value().line_number;
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{if_result.value()});
        }
        return {};
    }
    auto while_token = extract_token(lexer::TokenType::Keyword, "WHILE", false);
    if (while_token) {
        auto while_result = parse_while();
        if (while_result) {
            while_result.value().line_number = while_token.value().line_number;
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{while_result.value()});
        }
        return {};
    }
    auto opening_brace_token =
        extract_token(lexer::TokenType::SpecialNotation, "{", false);
    if (opening_brace_token) {
        auto compound_expression_result = parse_compound_expression();
        if (compound_expression_result) {
            compound_expression_result.value().line_number =
                opening_brace_token.value().line_number;
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{compound_expression_result.value()});
        }
        return {};
    }
    auto let_token = extract_token(lexer::TokenType::Keyword, "LET", false);
    if (let_token) {
        auto let_result = parse_let();
        if (let_result) {
            let_result.value().line_number = let_token.value().line_number;
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{let_result.value()});
        }
        return {};
    }
    auto case_token = extract_token(lexer::TokenType::Keyword, "CASE", false);
    if (case_token) {
        auto case_result = parse_case();
        if (case_result) {
            case_result.value().line_number = case_token.value().line_number;
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{case_result.value()});
        }
        return {};
    }
    auto new_token = extract_token(lexer::TokenType::Keyword, "NEW", false);
    if (new_token) {
        auto new_result = parse_new();
        if (new_result) {
            return std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{new_result.value()});
        }
        return {};
    }
    auto opening_bracket_token =
        extract_token(lexer::TokenType::SpecialNotation, "(", false);
    if (opening_bracket_token) {
        auto bracket_result = parse_expression_lvl_8();
        auto closing_bracket_token =
            extract_token(lexer::TokenType::SpecialNotation, ")");
        if (bracket_result && closing_bracket_token) {
            return bracket_result;
        }
        return {};
    }
    auto object_id_token =
        extract_token(lexer::TokenType::ObjectIdentifier, "", false);
    if (object_id_token) {
        if (extract_token(lexer::TokenType::SpecialNotation, "(", false)) {
            auto expression = parse_function(object_id_token.value());
            if (expression) {
                expression.value().line_number =
                    object_id_token.value().line_number;
                return std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{std::move(expression.value())});
            }
            return {};
        }
        return std::make_shared<cool::AST::Expression>(
            cool::AST::Expression{cool::AST::ObjectExpression{
                std::string{object_id_token.value().lexeme},
                object_id_token.value().line_number}});
    }
    auto int_token = extract_token(lexer::TokenType::Integer, "", false);
    if (int_token) {
        return std::make_shared<cool::AST::Expression>(
            cool::AST::Expression{cool::AST::IntExpression{
                std::stoi(std::string{int_token.value().lexeme}),
                int_token.value().line_number}});
    }
    auto string_token = extract_token(lexer::TokenType::String, "", false);
    if (string_token) {
        return std::make_shared<cool::AST::Expression>(
            cool::AST::Expression{cool::AST::StringExpression{
                std::string{string_token.value().lexeme},
                string_token.value().line_number}});
    }
    auto true_token = extract_token(lexer::TokenType::BoolConst, "true", false);
    if (true_token) {
        return std::make_shared<cool::AST::Expression>(
            cool::AST::Expression{cool::AST::TrueExpression{
                {detail::string_view_to_bool(true_token.value().lexeme),
                    true_token.value().line_number}}});
    }
    auto false_token =
        extract_token(lexer::TokenType::BoolConst, "false", false);
    if (false_token) {
        return std::make_shared<cool::AST::Expression>(
            cool::AST::Expression{cool::AST::FalseExpression{
                {detail::string_view_to_bool(false_token.value().lexeme),
                    false_token.value().line_number}}});
    }
    error << "Expected expression." << std::endl;
    return {};
}

[[nodiscard]] std::optional<std::shared_ptr<cool::AST::Expression>>
cool::parser::Parser::parse_dot(
    std::shared_ptr<cool::AST::Expression> expression_lvl_1, bool with_type,
    auto dot_token) noexcept {
    std::optional<std::string> type_token;
    if (with_type) {

        auto type_token_temp = extract_token(lexer::TokenType::TypeIdentifier);
        dot_token = extract_token(lexer::TokenType::SpecialNotation, ".");
        if (!type_token_temp || !dot_token) {
            return {};
        }
        type_token = type_token_temp.value().lexeme;
    }
    auto object_token = extract_token(lexer::TokenType::ObjectIdentifier);
    auto opening_bracket_token =
        extract_token(lexer::TokenType::SpecialNotation, "(");

    if (!opening_bracket_token || !object_token) {
        return {};
    }
    std::vector<std::shared_ptr<cool::AST::Expression>> expressions;

    bool found_comma = false;
    while (true) {
        if (!found_comma &&
            extract_token(lexer::TokenType::SpecialNotation, ")", false)) {
            break;
        }
        found_comma = false;
        if (token_it.is_exhausted()) {
            error << "Expected \")\"" << std::endl;
            return {};
        }
        auto expression_lvl_7 = parse_expression_lvl_8();

        if (expression_lvl_7) {
            expressions.push_back(std::move(expression_lvl_7.value()));
            found_comma =
                extract_token(lexer::TokenType::SpecialNotation, ",", false)
                    .has_value();
            continue;
        }
        return {};
    }
    return std::make_shared<cool::AST::Expression>(
        cool::AST::Expression{cool::AST::DotExpression{expression_lvl_1,
            type_token, std::string{object_token.value().lexeme},
            std::move(expressions), dot_token.value().line_number}});
}

[[nodiscard]] std::optional<cool::AST::FunctionExpression>
cool::parser::Parser::parse_function(auto object_id) noexcept {
    cool::AST::FunctionExpression fun_expression;
    fun_expression.object_id = object_id.lexeme;
    bool found_comma = false;
    while (true) {
        if (!found_comma &&
            extract_token(lexer::TokenType::SpecialNotation, ")", false)) {
            return fun_expression;
        }
        found_comma = false;
        if (token_it.is_exhausted()) {
            error << "Expected \")\"." << std::endl;
        }
        auto expression = parse_expression_lvl_8();
        if (!expression) {
            return {};
        }
        fun_expression.expressions.push_back(expression.value());
        if (extract_token(lexer::TokenType::SpecialNotation, ",", false)) {
            found_comma = true;
        }
    }
}

[[nodiscard]] std::optional<cool::AST::IfExpression>
cool::parser::Parser::parse_if() noexcept {
    auto condition_expression = parse_expression_lvl_8();
    if (!condition_expression) {
        return {};
    }
    if (!extract_token(lexer::TokenType::Keyword, "THEN")) {
        return {};
    }
    auto if_expression = parse_expression_lvl_8();
    if (!if_expression) {
        return {};
    }
    if (!extract_token(lexer::TokenType::Keyword, "ELSE")) {
        return {};
    }
    auto else_expression = parse_expression_lvl_8();
    if (!else_expression) {
        return {};
    }
    if (!extract_token(lexer::TokenType::Keyword, "FI")) {
        return {};
    }
    return {{condition_expression.value(), if_expression.value(),
        else_expression.value()}};
}

[[nodiscard]] std::optional<cool::AST::WhileExpression>
cool::parser::Parser::parse_while() noexcept {
    auto condition_expression = parse_expression_lvl_8();
    if (!condition_expression) {
        return {};
    }
    if (!extract_token(lexer::TokenType::Keyword, "LOOP")) {
        return {};
    }
    auto body_expression = parse_expression_lvl_8();
    if (!body_expression) {
        return {};
    }
    if (!extract_token(lexer::TokenType::Keyword, "POOL")) {
        return {};
    }
    return {{condition_expression.value(), body_expression.value()}};
}

[[nodiscard]] std::optional<cool::AST::CompoundExpression>
cool::parser::Parser::parse_compound_expression() noexcept {
    std::vector<std::shared_ptr<cool::AST::Expression>> expressions;
    while (true) {
        auto closing_brace_token =
            extract_token(lexer::TokenType::SpecialNotation, "}", false);
        if (closing_brace_token) {
            if (!expressions.empty()) {
                return {cool::AST::CompoundExpression{std::move(expressions)}};
            }
            error << "Expected expression." << std::endl;
            return {};
        }
        auto brace_result = parse_expression_lvl_8();
        auto semicolon_token =
            extract_token(lexer::TokenType::SpecialNotation, ";");
        if (!brace_result || !semicolon_token) {
            return {};
        }
        expressions.push_back(std::move(brace_result.value()));
    }
}

[[nodiscard]] std::optional<cool::AST::CaseExpression>
cool::parser::Parser::parse_case() noexcept {
    auto case_expression = parse_expression_lvl_8();
    if (!case_expression) {
        return {};
    }
    if (!extract_token(lexer::TokenType::Keyword, "OF")) {
        return {};
    }
    std::vector<cool::AST::CaseBranch> branch_expressions;
    while (true) {
        auto esac_token =
            extract_token(lexer::TokenType::Keyword, "ESAC", false);
        if (esac_token) {
            if (!branch_expressions.empty()) {
                return {cool::AST::CaseExpression{
                    case_expression.value(), std::move(branch_expressions)}};
            }
            error << "Expected expression." << std::endl;
            return {};
        }
        auto object_id = extract_token(lexer::TokenType::ObjectIdentifier);
        auto colon = extract_token(lexer::TokenType::SpecialNotation, ":");
        auto type_id = extract_token(lexer::TokenType::TypeIdentifier);
        auto darrow = extract_token(lexer::TokenType::DArrow);
        auto branch_expression = parse_expression_lvl_8();
        auto semicolon_token =
            extract_token(lexer::TokenType::SpecialNotation, ";");

        if (object_id && colon && type_id && darrow && branch_expression &&
            semicolon_token) {
            branch_expressions.push_back(
                cool::AST::CaseBranch{std::string{object_id.value().lexeme},
                    std::string{type_id.value().lexeme},
                    std::move(branch_expression.value()),
                    object_id.value().line_number});
            continue;
        }
        return {};
    }
}

[[nodiscard]] std::optional<cool::AST::LetExpression>
cool::parser::Parser::parse_let() noexcept {
    cool::AST::LetExpression let_expression;
    while (true) {
        auto object_id = extract_token(lexer::TokenType::ObjectIdentifier);
        auto colon = extract_token(lexer::TokenType::SpecialNotation, ":");
        auto type_id = extract_token(lexer::TokenType::TypeIdentifier);
        auto assign_token = extract_token(lexer::TokenType::Assign, "", false);
        std::optional<std::shared_ptr<cool::AST::Expression>> assign_expression;

        if (assign_token) {
            assign_expression = parse_expression_lvl_8();
            if (!assign_expression) {
                return {};
            }
        }
        if (object_id && colon && type_id) {
            let_expression.let_expressions.push_back(
                cool::AST::LetEntry{std::string{object_id.value().lexeme},
                    std::string{type_id.value().lexeme}, assign_expression,
                    object_id.value().line_number});
        } else {
            return {};
        }
        if (extract_token(lexer::TokenType::SpecialNotation, ",", false)) {
            continue;
        }
        if (!extract_token(lexer::TokenType::Keyword, "IN")) {
            return {};
        }
        break;
    }
    auto expression = parse_expression_lvl_8();
    if (expression) {
        let_expression.expression = expression.value();
        return let_expression;
    }
    return {};
}

[[nodiscard]] std::optional<cool::AST::NewExpression>
cool::parser::Parser::parse_new() noexcept {
    auto type_id = extract_token(lexer::TokenType::TypeIdentifier);
    if (!type_id) {
        return {};
    }
    return {{std::string{type_id.value().lexeme}, type_id.value().line_number}};
}

[[nodiscard]] std::optional<cool::lexer::Token>
cool::parser::Parser::extract_token(lexer::TokenType required_token_type,
    std::string_view required_lexeme, bool raise_error) noexcept {
    if (!token_it.is_exhausted()) {
        const auto& token = token_it.current();
        if (token.token_type == required_token_type) {
            if (required_lexeme == "" || token.lexeme == required_lexeme) {
                token_it.advance();
                return token;
            }
        }
    }
    if (raise_error) {
        error << "Expected ";
        if (!lexer::token_name[required_token_type].empty()) {
            error << lexer::token_name[required_token_type] << " ";
        }
        error << required_lexeme << std::endl;
    }
    return {};
}
