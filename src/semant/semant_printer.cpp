#include "AST/AST_Printer.h"
#include "semant/Semant.h"

int main(int argc, char* argv[]) {
    bool printed_program = false;
    std::vector<cool::parser::Parser> parsers;
    for (int i = 1; i != argc; ++i) {
        std::ifstream input(argv[i]);
        cool::lexer::Lexer lexer(input, argv[i]);
        parsers.emplace_back(lexer);

        const auto& program = parsers.back().get_result();
        if (!program) {
            std::cerr << parsers.back().get_error().str();
            return 1;
        }
    }
    cool::semant::Semant semant(parsers);
    cool::AST::AST_Printer printer;
    if (semant.is_successfull()) {
        printer.print_program(semant.get_result(), printed_program);
    } else {
        std::cout << semant.get_error() << std::endl;
        return 1;
    }
    return 0;
}
