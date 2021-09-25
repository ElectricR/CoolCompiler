#pragma once
#include "Regexes.h"
#include "Token.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <vector>

namespace cool {

namespace lexer {

class Lexer {
public:
    Lexer(const std::filesystem::path& source_file_path) {
        std::ifstream source_file{source_file_path};
        std::string line;
        std::string cool_string_appendix;
        unsigned line_number = 1;

        for (; std::getline(source_file, line); ++line_number) {
            if (!cool_string_appendix.empty()) {
                line = std::move(cool_string_appendix) + '\n' + std::move(line);
            }
            cool_string_appendix = parse_multiline(line, line_number);
        }
        if (!cool_string_appendix.empty()) {
            add_eof_error(cool_string_appendix, line_number);
        }
    }

    [[nodiscard]] const auto& get_result() const noexcept {
        return result;
    }

private:
    std::string parse_multiline(
        std::string& multiline, unsigned line_number) noexcept {
        std::string cool_string_appendix;

        while (true) {
            trim_multiline(multiline, cool_string_appendix);
            consume_line(multiline, line_number);
            if (cool_string_appendix.empty()) {
                return {};
            }
            if (cool_string_appendix[0] == '"') {
                if (!extract_cool_string(
                        cool_string_appendix, line_number, multiline)) {
                    return cool_string_appendix;
                }
            } else {
                if (!extract_multiline_comment(
                        cool_string_appendix, multiline)) {
                    return cool_string_appendix;
                }
            }
        }
    }

    //
    // Trims multiline until first quote or comment
    //
    void trim_multiline(
        std::string& multiline, std::string& cool_string_appendix) noexcept {
        auto quote_it = std::ranges::find(std::as_const(multiline), '"');
        auto one_line_comment_it =
            std::ranges::adjacent_find(std::as_const(multiline),
                [](char a, char b) { return a == '-' && b == '-'; });

        auto multi_line_comment_it =
            std::ranges::adjacent_find(std::as_const(multiline),
                [](char a, char b) { return a == '(' && b == '*'; });

        auto min_it =
            std::min({quote_it, one_line_comment_it, multi_line_comment_it});

        if (min_it != multiline.cend()) {
            if (min_it != one_line_comment_it) {
                cool_string_appendix = std::string(min_it, multiline.cend());
            }
            multiline.resize(
                static_cast<size_t>(std::distance(multiline.cbegin(), min_it)));
        }
    }

    //
    // Converts quote-and-comment-free line to tokens
    // Clears line at the end
    //
    void consume_line(std::string& line, unsigned line_number) noexcept {
        std::stringstream line_stream{std::move(line)};
        while (!line_stream.eof()) {
            std::string buffer;
            line_stream >> buffer;
            parse_buffer(buffer, line_number);
        }
        line.clear();
    }

    //
    // Attempts to extract string literal from appendix
    // and stores the rest of the multiline in cleaned_line
    // Adds an error if it couldn't extract string and '\' wasn't found
    //
    [[nodiscard]] bool extract_cool_string(std::string& cool_string_appendix,
        unsigned line_number, std::string& cleaned_line) noexcept {
        auto result_it = std::ranges::adjacent_find(
            std::as_const(cool_string_appendix),
            [](char curr, char next) { return curr != '\\' && next == '"'; });

        if (result_it != cool_string_appendix.cend()) {
            Token token = {
                line_number,
                TokenType::String,
                {cool_string_appendix.cbegin(), result_it + 2},
            };

            cleaned_line = {result_it + 2, cool_string_appendix.cend()};
            cool_string_appendix.clear();
            result.push_back(std::move(token));
            return true;
        }
        if (cool_string_appendix.back() != '\\') {
            Token token = {
                line_number,
                TokenType::Error,
                "Unterminated string constant",
            };
            cool_string_appendix.clear();
            result.push_back(std::move(token));
            return true;
        }

        return false;
    }

    //
    // Attempts to extract multiline comment from appendix
    // and stores the rest of the multiline in cleaned_line
    //
    [[nodiscard]] bool extract_multiline_comment(
        std::string& cool_string_appendix,
        std::string& cleaned_line) const noexcept {
        auto result_it = find_multiline_comment_end(cool_string_appendix);

        if (result_it != cool_string_appendix.cend()) {
            cleaned_line = {result_it, cool_string_appendix.cend()};
            cool_string_appendix.clear();
            return true;
        }
        return false;
    }

    //
    // Finds iterator to first non-comment piece of code
    //
    [[nodiscard]] std::string::const_iterator find_multiline_comment_end(
        const std::string& cool_string_appendix) const noexcept {
        int stack_size = 1;

        std::string::const_iterator result_it = cool_string_appendix.cbegin();
        while (stack_size) {
            result_it = std::ranges::adjacent_find(result_it + 1,
                cool_string_appendix.cend(), [](char curr, char next) {
                    return (curr == '(' && next == '*') ||
                           (curr == '*' && next == ')');
                });

            if (result_it == cool_string_appendix.cend()) {
                return result_it;
            } else {
                if (*result_it == '(') {
                    ++stack_size;
                } else {
                    --stack_size;
                }
            }
        }
        return result_it + 2;
    }

    void add_eof_error(const std::string& cool_string_appendix,
        unsigned line_number) noexcept {
        std::string cause;

        if (cool_string_appendix.front() == '(') {
            cause = "EOF in comment";
        } else {
            cause = "EOF in string constant";
        }

        Token token = {
            line_number,
            TokenType::Error,
            std::move(cause),
        };
        result.push_back(std::move(token));
    }

    //
    // Converts space-comment-quote-free string to tokens
    //
    void parse_buffer(std::string_view buffer, unsigned line_number) noexcept {
        while (!buffer.empty()) {
            std::cmatch token_match;

            if (!check_default_patterns(token_match, buffer, line_number)) {
                Token token = {
                    line_number,
                    TokenType::Error,
                    std::string{buffer.cbegin(), buffer.cend()},
                };
                result.push_back(std::move(token));
                break;
            }
        }
    }

    //
    // Applies prioritised regex check to buffer and
    // shrinks it by matched value
    // Returns false if it couldn't match any of the patterns
    //
    [[nodiscard]] bool check_default_patterns(std::cmatch& token_match,
        std::string_view& buffer, unsigned line_number) noexcept {
        for (auto& [token_regex, token_type] : token_type_regexes) {
            if (std::regex_match(
                    buffer.cbegin(), buffer.cend(), token_match, token_regex)) {
                Token token = {
                    line_number,
                    token_type,
                    std::string{
                        buffer.cbegin(),
                        buffer.cbegin() + token_match[1].length(),
                    },
                };

                switch (token_type) {
                case TokenType::BoolConst: {
                    std::transform(token.lexeme.begin(), token.lexeme.end(),
                        token.lexeme.begin(), ::tolower);
                    break;
                }
                case TokenType::Keyword: {
                    std::transform(token.lexeme.begin(), token.lexeme.end(),
                        token.lexeme.begin(), ::toupper);
                    break;
                }
                default:
                    break;
                }

                result.push_back(std::move(token));
                buffer.remove_prefix(
                    static_cast<size_t>(token_match[1].length()));
                return true;
            }
        }
        return false;
    }

    std::vector<Token> result;
};

} // namespace lexer

} // namespace cool
