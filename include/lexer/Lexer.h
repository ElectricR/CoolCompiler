#pragma once
#include "Regexes.h"
#include "Token.h"

#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <unordered_set>
#include <vector>

namespace cool::lexer {

class Lexer {
public:
    Lexer(std::istream& source_file) {
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
                if (!consume_cool_string(
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
    [[nodiscard]] bool consume_cool_string(std::string& cool_string_appendix,
        unsigned line_number, std::string& cleaned_line) noexcept {
        auto [is_extracted, is_slash_terminated, null_character_found,
            extracted_string, cleaned_line_start] =
            extract_cool_string(cool_string_appendix);

        if (is_extracted) {
            if (extracted_string.size() > 1024) {
                result.push_back(Token{
                    line_number, TokenType::Error, "String constant too long"});
            } else if (null_character_found) {
                result.push_back(Token{
                    line_number,
                    TokenType::Error,
                    {"String contains null character."},
                });
            } else {
                result.push_back(Token{
                    line_number,
                    TokenType::String,
                    {std::move(extracted_string)},
                });
            }

            cleaned_line = {cool_string_appendix.cbegin() +
                                static_cast<long>(cleaned_line_start),
                cool_string_appendix.cend()};
            cool_string_appendix.clear();
            return true;
        } else {
            if (!is_slash_terminated) {
                result.push_back(Token{
                    line_number + 1,
                    TokenType::Error,
                    "Unterminated string constant",
                });
                cool_string_appendix.clear();
                return true;
            }
        }
        return false;
    }

    //
    // First bool indicates whether the extraction was successful
    // Second bool indicates whether the trailing \ was found
    // Third bool indicates whether the null character was found
    // String is the extracted_string itself
    // Last value is the beginning of cleaned_line
    //
    [[nodiscard]] std::tuple<bool, bool, bool, std::string, size_t>
    extract_cool_string(std::string_view cool_string_appendix) const noexcept {
        std::string extracted_string;
        size_t last_escaped_char_index = 0;
        bool null_character_found = false;
        for (size_t i = 1; i <= cool_string_appendix.size() - 1; ++i) {
            if (cool_string_appendix[i] == '\\') {
                if (i != last_escaped_char_index) {
                    last_escaped_char_index = i + 1;
                    switch (cool_string_appendix[i + 1]) {
                    case '\n':
                    case 'n':
                        extracted_string += '\n';
                        break;
                    case '\t':
                    case 't':
                        extracted_string += '\t';
                        break;
                    case '\f':
                    case 'f':
                        extracted_string += '\f';
                        break;
                    case '\b':
                    case 'b':
                        extracted_string += '\b';
                        break;
                    default:
                        extracted_string += cool_string_appendix[i + 1];
                        break;
                    }
                }
            } else if (cool_string_appendix[i] == '"' &&
                       i != last_escaped_char_index) {
                return {true, true, null_character_found,
                    std::move(extracted_string), i + 1};
            } else {
                if (cool_string_appendix[i] == '\0') {
                    null_character_found = true;
                } else if (i != last_escaped_char_index) {
                    extracted_string += cool_string_appendix[i];
                }
            }
        }
        if (cool_string_appendix.back() == '\\' &&
            cool_string_appendix.size() - 1 != last_escaped_char_index) {
            return {false, true, null_character_found,
                std::move(extracted_string), cool_string_appendix.size()};
        } else if (cool_string_appendix.back() == '"' &&
                   cool_string_appendix.size() - 1 != last_escaped_char_index) {
            return {true, true, null_character_found,
                std::move(extracted_string), cool_string_appendix.size()};
        }
        return {false, false, null_character_found, std::move(extracted_string),
            cool_string_appendix.size()};
    }

    //
    // Attempts to extract multiline comment from appendix
    // and stores the rest of the multiline in cleaned_line
    //
    [[nodiscard]] bool extract_multiline_comment(
        std::string& cool_string_appendix,
        std::string& cleaned_line) const noexcept {
        auto result_it = find_multiline_comment_end(cool_string_appendix);

        if (result_it.has_value()) {
            cleaned_line = {result_it.value(), cool_string_appendix.cend()};
            cool_string_appendix.clear();
            return true;
        }
        return false;
    }

    //
    // Finds iterator to first non-comment piece of code
    // Returns iterator to start of cleaned line if all
    // comment ends were found or empty optional otherwise
    //
    [[nodiscard]] std::optional<std::string::const_iterator>
    find_multiline_comment_end(
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
                return {};
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
        result.push_back(Token{
            line_number,
            TokenType::Error,
            cause,
        });
    }

    //
    // Converts space-comment-quote-free string to tokens
    //
    void parse_buffer(std::string_view buffer, unsigned line_number) noexcept {
        while (!buffer.empty()) {
            std::cmatch token_match;

            if (check_errors(buffer, line_number)) {
                continue;
            }

            if (!check_default_patterns(token_match, buffer, line_number)) {
                result.push_back(Token{
                    line_number,
                    TokenType::Error,
                    std::string{buffer.cbegin(), buffer.cend()},
                });
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

    [[nodiscard]] bool check_errors(
        std::string_view& buffer, unsigned line_number) noexcept {
        std::cmatch error_match;
        for (auto& [error_regex, error_msg] : error_regexes) {
            if (std::regex_match(
                    buffer.cbegin(), buffer.cend(), error_match, error_regex)) {
                result.push_back(Token{line_number, TokenType::Error,
                    {error_msg.cbegin(), error_msg.cend()}});

                buffer.remove_prefix(
                    static_cast<size_t>(error_match[1].length()));
                return true;
            }
        }
        if (buffer[0] == '\0') {
            result.push_back(Token{line_number, TokenType::Error, {"\\000"}});

            buffer.remove_prefix(1);
            return true;
        }

        return false;
    }

    std::vector<Token> result;
};

} // namespace cool::lexer
