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
    Lexer(std::istream& source_file, std::string filepath = "");

    [[nodiscard]] const auto& get_result() const noexcept {
        return result;
    }

    [[nodiscard]] std::string get_filepath() const noexcept {
        return filepath;
    }

private:
    [[nodiscard]] std::string parse_multiline(
        std::string& multiline, unsigned line_number) noexcept;

    //
    // Trims multiline until first quote or comment
    //
    void trim_multiline(
        std::string& multiline, std::string& cool_string_appendix) noexcept;

    //
    // Converts quote-and-comment-free line to tokens
    // Clears line at the end
    //
    void consume_line(std::string& line, unsigned line_number) noexcept;

    //
    // Attempts to extract string literal from appendix
    // and stores the rest of the multiline in cleaned_line
    // Adds an error if it couldn't extract string and '\' wasn't found
    //
    [[nodiscard]] bool consume_cool_string(std::string& cool_string_appendix,
        unsigned line_number, std::string& cleaned_line) noexcept;

    //
    // First bool indicates whether the extraction was successful
    // Second bool indicates whether the trailing \ was found
    // Third bool indicates whether the null character was found
    // String is the extracted_string itself
    // Last value is the beginning of cleaned_line
    //
    [[nodiscard]] std::tuple<bool, bool, bool, std::string, size_t>
    extract_cool_string(std::string_view cool_string_appendix) const noexcept;

    //
    // Attempts to extract multiline comment from appendix
    // and stores the rest of the multiline in cleaned_line
    //
    [[nodiscard]] bool extract_multiline_comment(
        std::string& cool_string_appendix,
        std::string& cleaned_line) const noexcept;

    //
    // Finds iterator to first non-comment piece of code
    // Returns iterator to start of cleaned line if all
    // comment ends were found or empty optional otherwise
    //
    [[nodiscard]] std::optional<std::string::const_iterator>
    find_multiline_comment_end(
        const std::string& cool_string_appendix) const noexcept;

    void add_eof_error(
        const std::string& cool_string_appendix, unsigned line_number) noexcept;

    //
    // Converts space-comment-quote-free string to tokens
    //
    void parse_buffer(std::string_view buffer, unsigned line_number) noexcept;

    //
    // Applies prioritised regex check to buffer and
    // shrinks it by matched value
    // Returns false if it couldn't match any of the patterns
    //
    [[nodiscard]] bool check_default_patterns(std::cmatch& token_match,
        std::string_view& buffer, unsigned line_number) noexcept;

    [[nodiscard]] bool check_errors(
        std::string_view& buffer, unsigned line_number) noexcept;

    std::vector<Token> result;
    std::string filepath;
};

} // namespace cool::lexer
