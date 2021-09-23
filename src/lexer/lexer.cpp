#include "../../include/lexer/Lexer.h"
#include "../../include/lexer/Token.h"

#include <iterator>

void print_lexer_result(auto result) {
    for (auto token : result) {
        switch (token.token_type) {
        case cool::lexer::TokenType::Keyword: {
            std::cout << '#' << token.line_number << ' ' << token.lexeme
                      << std::endl;
            break;
        }
        case cool::lexer::TokenType::SpecialNotation: {
            std::cout << '#' << token.line_number << ' ' << '\'' << token.lexeme
                      << '\'' << std::endl;
            break;
        }
        case cool::lexer::TokenType::ObjectIdentifier: {
            std::cout << '#' << token.line_number << " OBJECTID "
                      << token.lexeme << std::endl;
            break;
        }
        case cool::lexer::TokenType::TypeIdentifier: {
            std::cout << '#' << token.line_number << " TYPEID " << token.lexeme
                      << std::endl;
            break;
        }
        case cool::lexer::TokenType::LE: {
            std::cout << '#' << token.line_number << " LE" << std::endl;
            break;
        }
        case cool::lexer::TokenType::DArrow: {
            std::cout << '#' << token.line_number << " DARROW" << std::endl;
            break;
        }
        case cool::lexer::TokenType::Assign: {
            std::cout << '#' << token.line_number << " ASSIGN" << std::endl;
            break;
        }
        case cool::lexer::TokenType::Integer: {
            std::cout << '#' << token.line_number << " INT_CONST "
                      << token.lexeme << std::endl;
            break;
        }
        case cool::lexer::TokenType::BoolConst: {
            std::cout << '#' << token.line_number << " BOOL_CONST "
                      << token.lexeme << std::endl;
            break;
        }
        case cool::lexer::TokenType::String: {
            std::cout << '#' << token.line_number << " STR_CONST ";
            std::ranges::transform(token.lexeme,
                std::ostream_iterator<char>(std::cout), [](char ch) {
                    if (ch == '\n') {
                        return 'n';
                    }
                    return ch;
                });
            std::cout << std::endl;
            break;
        }
        case cool::lexer::TokenType::Error: {
            std::cout << '#' << token.line_number << " ERROR \"" << token.lexeme << '"'
                      << std::endl;
            break;
        }
        }
    }
}

int main(int argc, char* argv[]) {
    for (int i = 1; i != argc; ++i) {
        cool::lexer::Lexer lexer(argv[i]);
        std::cout << "#name \"" << argv[i] << '\"' << std::endl;
        print_lexer_result(lexer.get_result());
    }
}
