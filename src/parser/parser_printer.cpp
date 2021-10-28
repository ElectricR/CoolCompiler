#include "AST/AST_Printer.h"
#include "parser/Parser.h"

int main(int argc, char* argv[]) {
    bool printed_program = false;
    for (int i = 1; i != argc; ++i) {
        std::ifstream input(argv[i]);
        cool::lexer::Lexer lexer(input, argv[i]);
        cool::parser::Parser parser(lexer);
        cool::AST::AST_Printer printer;

        const auto& program = parser.get_result();
        if (program) {
            printer.print_program(program.value(), printed_program);
            printed_program = true;
        } else {
            std::cerr << parser.get_error().str();
            return 1;
        }
    }
    return 0;
}
