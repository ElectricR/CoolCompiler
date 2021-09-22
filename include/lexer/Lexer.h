#pragma once
#include "Token.h"
#include "Regexes.h"

#include <vector>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <ranges>

namespace cool {


namespace lexer {


class Lexer {
public:
    Lexer(const std::filesystem::path& source_file_path) {
        std::ifstream source_file{source_file_path};
        std::string line;
        std::string cool_string_appendix;

        for (unsigned line_number = 1; std::getline(source_file, line); ++line_number) {
            if (!cool_string_appendix.empty()) {
                line = std::move(cool_string_appendix) + '\n' + std::move(line); 
            }
            cool_string_appendix = this->parse_line(line, line_number);
        }
    }

    [[nodiscard]] const auto& get_result() const noexcept {
        return result;
    }

private:
    std::string parse_line(std::string& line, unsigned line_number) noexcept {
        std::string cool_string_appendix;

        while (true) { 
            trim_line(line, cool_string_appendix);
            consume_line(line, line_number);
            if (cool_string_appendix.empty()) {
                return {};
            } 
            if (!extract_cool_string(cool_string_appendix, line_number, line)) {
                return cool_string_appendix;
            }
        }
    }

    // 
    // Trims line until first quote or comment
    // 
    void trim_line(std::string& line, std::string& cool_string_appendix) noexcept {
        auto result_it = std::ranges::find(std::as_const(line), '"');
        if (result_it != line.cend()){
            cool_string_appendix = std::string(result_it, line.cend());
            line.resize(static_cast<size_t>(std::distance(line.cbegin(), result_it)));
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
            this->parse_buffer(buffer, line_number);
        }
        line.clear();
    }

    bool extract_cool_string(std::string& cool_string_appendix, unsigned line_number, std::string& cleaned_line) noexcept {
        auto result_it = std::ranges::adjacent_find(std::as_const(cool_string_appendix),
                [](char curr, char next) { return curr != '\\' && next == '"'; }
        );

        if (result_it != cool_string_appendix.cend()) {
            Token token = {
                line_number,
                TokenType::String, 
                { cool_string_appendix.cbegin(), result_it + 2 }
            };

            cleaned_line = {result_it + 2, cool_string_appendix.cend()};
            cool_string_appendix.clear();
            result.push_back(std::move(token));
            return true;
        }
        return false;
    }

    void parse_buffer(std::string_view buffer, unsigned line_number) noexcept {
        while (!buffer.empty()) {
            std::cmatch token_match;

            if (!this->check_default_patterns(token_match, buffer, line_number)) {
                Token token = {
                    line_number,
                    TokenType::Error, 
                    std::string{
                        buffer.cbegin(), 
                        buffer.cend()
                    }
                };

                result.push_back(std::move(token));
                break;
            }
        }
    }

    [[nodiscard]] bool check_default_patterns(std::cmatch& token_match, std::string_view& buffer, unsigned line_number) noexcept {
        for (auto& [token_regex, token_type] : token_type_regexes) {
            if (std::regex_match(buffer.cbegin(), buffer.cend(), token_match, token_regex)) {
                Token token = {
                    line_number,
                    token_type, 
                    std::string{
                        buffer.cbegin(), 
                        buffer.cbegin() + token_match[1].length()
                    }
                };

                switch (token_type) {
                case TokenType::BoolConst: {
                    std::transform(token.lexeme.begin(), token.lexeme.end(), token.lexeme.begin(), ::tolower);
                    break;
                }
                case TokenType::Keyword: {
                    std::transform(token.lexeme.begin(), token.lexeme.end(), token.lexeme.begin(), ::toupper);
                    break;
                }
                default: break;
                }

                result.push_back(std::move(token));
                buffer.remove_prefix(static_cast<size_t>(token_match[1].length()));
                return true;
            }
        }
        return false;
    }

    std::vector<Token> result;
};


} // namespace lexer


} // namespace cool
