#pragma once
#include "AST/AST.h"
#include "codegen/MIPS32/ASTVisitor.h"
#include "codegen/MIPS32/ConstantGenerator.h"
#include "codegen/MIPS32/DispTableGen.h"
#include "codegen/MIPS32/MiscGen.h"
#include "codegen/MIPS32/PrototypeGen.h"

#include <iostream>
#include <sstream>
#include <unordered_set>

namespace cool::codegen {

class Codegen {
public:
    Codegen() = delete;

    explicit Codegen(const AST::Program& AST, std::ostream& out) {
        make_class_map(AST);
        generate_assembly(AST, out);
    }

private:
    void generate_assembly(const AST::Program& AST, std::ostream& out) noexcept;

    void get_data_from_AST(const AST::Program& AST) noexcept;

    void generate_prototypes(
        const AST::Program& AST, std::ostream& out) noexcept;

    void generate_disptables(
        const AST::Program& AST, std::ostream& out) noexcept;

    void generate_objtab(
        const AST::Program& AST, std::ostream& out) const noexcept;

    void generate_nametab(
        const AST::Program& AST, std::ostream& out) const noexcept;

    void generate_constants(std::ostream& out) const noexcept;

    void generate_globals(std::ostream& out) const noexcept;

    void generate_basic_tags(std::ostream& out) const noexcept;

    void generate_heap_start(std::ostream& out) const noexcept;

    void generate_gc_stuff(std::ostream& out) const noexcept;

    void generate_text(const AST::Program& program) noexcept;

    void generate_inits(const AST::Program& program) noexcept;

    void print_data(std::ostream& out) noexcept;

    void print_text(std::ostream& out) noexcept;

    void make_class_map(const AST::Program& AST) noexcept;

    void populate_class_name_constants(const AST::Program& AST) noexcept;

    void generate_default_inits() noexcept;

    void generate_init(std::string_view class_name) noexcept;

private:
    MIPS32::PrototypeDataGenerator prototype_data_gen;
    MIPS32::DispTableDataGenerator disptable_data_gen;
    MIPS32::ConstantsDataGeneneror constants_data_gen;
    MIPS32::MiscGenerator misc_data_gen;
    MIPS32::TextGen text_gen;
    std::stringstream text;
    MIPS32::StackRepresentation stack_representation;
    MIPS32::ASTVisitor AST_visitor{text, &constants_data_gen,
        &disptable_data_gen, &prototype_data_gen, &stack_representation,
        &text_gen};
    std::stringstream data;
    std::unordered_map<std::string_view, const AST::Class&> class_map;
};

} // namespace cool::codegen
