#include "Codegen.h"
#include "semant/Semant.h"

int main(int argc, char* argv[]) {
    std::vector<cool::AST::Program> programs;

    for (int i = 1; i != argc; ++i) {
        std::ifstream input(argv[i]);
        cool::lexer::Lexer lexer(input);

        cool::parser::Parser parser(lexer);

        if (!parser.get_result().has_value()) {
            std::cout << parser.get_error().str();
            return 1;
        }
        programs.push_back(std::move(parser.get_result().value()));
    }
    cool::semant::Semant semant(programs);
    if (!semant.is_successfull()) {
        std::cout << semant.get_error();
        return 1;
    }
    std::ofstream out("a.s");
    cool::codegen::Codegen codegen(semant.get_result(), out);

    return 0;
}
