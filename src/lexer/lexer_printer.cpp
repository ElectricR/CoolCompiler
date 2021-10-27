#include "lexer/Lexer.h"
#include "lexer/Token.h"
#include "utils.h"

#include <iterator>

void print_lexer_result(auto result) {
    for (auto token : result) {
        switch (token.token_type) {
        case cool::lexer::TokenType::SpecialNotation: {
            std::cout << '#' << token.line_number << ' ' << '\'' << token.lexeme
                      << '\'' << std::endl;
            break;
        }
        case cool::lexer::TokenType::Keyword: {
            std::cout << '#' << token.line_number << " " << token.lexeme
                      << std::endl;
            break;
        }
        case cool::lexer::TokenType::BoolConst:
        case cool::lexer::TokenType::Integer:
        case cool::lexer::TokenType::TypeIdentifier:
        case cool::lexer::TokenType::ObjectIdentifier: {
            std::cout << '#' << token.line_number << " "
                      << cool::lexer::token_name[token.token_type] << " "
                      << token.lexeme << std::endl;
            break;
        }
        case cool::lexer::TokenType::Assign:
        case cool::lexer::TokenType::DArrow:
        case cool::lexer::TokenType::LE: {
            std::cout << '#' << token.line_number << " "
                      << cool::lexer::token_name[token.token_type] << std::endl;
            break;
        }
        case cool::lexer::TokenType::String: {
            std::cout << '#' << token.line_number << " STR_CONST ";
            util::print_string(token.lexeme);
            break;
        }
        case cool::lexer::TokenType::Error: {
            std::cout << '#' << token.line_number << " ERROR \"" << token.lexeme
                      << '"' << std::endl;
            break;
        }
        }
    }
}

int main(int argc, char* argv[]) {
    for (int i = 1; i != argc; ++i) {
        std::ifstream input(argv[i]);
        cool::lexer::Lexer lexer(input);
        std::cout << "#name \"" << argv[i] << '\"' << std::endl;
        print_lexer_result(lexer.get_result());
    }
}
