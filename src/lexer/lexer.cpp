#include "lexer/Lexer.h"
#include "lexer/Token.h"

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
            std::cout << '#' << token.line_number << " STR_CONST \"";
            std::ranges::for_each(token.lexeme, [] (char ch) {
                    switch (ch) {
                    case '\\': {
                        std::cout << "\\\\";
                        break;
                    }
                    case '\"': {
                        std::cout << "\\\"";
                        break;
                    }
                    case '\t': {
                        std::cout << "\\t";
                        break;
                    }
                    case '\b': {
                        std::cout << "\\b";
                        break;
                    }
                    case '\f': {
                        std::cout << "\\f";
                        break;
                    }
                    case '\n': {
                        std::cout << "\\n";
                        break;
                    }
                    case '\033': {
                        std::cout << "\\033";
                        break;
                    }
                    case '\015': {
                        std::cout << "\\015";
                        break;
                    }
                    case '\022': {
                        std::cout << "\\022";
                        break;
                    }
                    case '\013': {
                        std::cout << "\\013";
                        break;
                    }
                    default: {
                        std::cout << ch;
                        break;
                    }
                    }
                });
            std::cout << '"' << std::endl;
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
