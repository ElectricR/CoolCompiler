#include "AST/AST_Printer.h"
#include "semant/Semant.h"

int main(int argc, char* argv[]) {
    bool printed_program = false;
    std::vector<cool::AST::Program> ASTs;
    for (int i = 1; i != argc; ++i) {
        std::ifstream input(argv[i]);
        cool::lexer::Lexer lexer(input, argv[i]);

        cool::parser::Parser parser(lexer);

        const auto& program = parser.get_result();

        if (!program) {
            std::cerr << parser.get_error().str();
            return 1;
        }

        ASTs.push_back(program.value());
    }

    cool::semant::Semant semant(ASTs);
    cool::AST::AST_Printer printer;
    if (semant.is_successfull()) {
        printer.print_program(semant.get_result(), printed_program);
    } else {
        std::cout << semant.get_error() << std::endl;
        return 1;
    }
    return 0;
}
