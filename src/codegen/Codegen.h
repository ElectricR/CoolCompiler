#pragma once
#include "AST/AST.h"
#include "codegen/instructions.h"
#include "codegen/MIPS32/PrototypeGen.h"
#include "codegen/MIPS32/DispTableGen.h"

#include <iostream>
#include <sstream>

namespace cool::codegen {

class Codegen {
public:
    explicit Codegen(const AST::Program& AST, std::ostream &out) {
        make_class_map(AST);
        generate_assembly(AST, out);
    }
private:
    void generate_assembly(const AST::Program& AST, std::ostream& out) noexcept;

    void visit_program(const AST::Program& AST) noexcept;

    void generate_prototypes(const AST::Program& AST, std::ostream& out) noexcept;

    void generate_disptables(const AST::Program& AST, std::ostream& out) noexcept;

    void visit_class(const AST::Class& class_) noexcept;

    void visit_method(std::string_view class_name, const AST::MethodFeature& method_feature) noexcept;

    void print_prologue(std::ostream& out) noexcept;

    void print_epilogue(std::ostream& out) noexcept;

    void print_data(std::ostream& out) noexcept;

    void print_text(std::ostream& out) noexcept;

    void print_single_instruction(const instructions::Instruction& instruction, std::ostream& out) const noexcept;

    void make_class_map(const AST::Program& AST) noexcept;
private:
    MIPS32::PrototypeDataGenerator prototype_data_gen;
    MIPS32::DispTableDataGenerator disptable_data_gen;
    std::stringstream text;
    std::stringstream data;
    std::unordered_map<std::string_view, const AST::Class&> class_map;
};

} // namespace
