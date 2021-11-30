#include "codegen/Codegen.h"
#include <iomanip>
#include <ranges>

namespace cool::codegen {

template<class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

void Codegen::generate_assembly(const AST::Program& AST, std::ostream& out) noexcept {
    visit_program(AST);
    print_data(out);
    print_text(out);
}

void Codegen::visit_program(const AST::Program& AST) noexcept {
    generate_prototypes(AST, data);
    generate_disptables(AST, data);
    for (const auto& class_ : AST.classes) {
        visit_class(class_);
    }
}

void Codegen::generate_prototypes(const AST::Program& AST, std::ostream& out) noexcept {
    prototype_data_gen.generate_object_prototype(out);
    prototype_data_gen.generate_int_prototype(out);
    prototype_data_gen.generate_bool_prototype(out);
    prototype_data_gen.generate_string_prototype(out);
    prototype_data_gen.generate_io_prototype(out);
    for (auto& class_ : AST.classes) {
        auto *current_ancestor = &class_;
        std::unordered_map<std::string_view,
            MIPS32::ClassPrototypeRepresentation>
            family_map;
        while (true) {
            auto fields_range = current_ancestor->features |
                     std::ranges::views::filter([](auto& feature_variant) {
                         return std::holds_alternative<AST::FieldFeature>(
                             feature_variant.feature);
                     }) |
                     std::ranges::views::transform(
                         [](auto& field_feature) -> std::string_view {
                             return std::get<AST::FieldFeature>(
                                 field_feature.feature)
                                 .type_id;
                         });
            MIPS32::ClassPrototypeRepresentation cl = {
                current_ancestor->inherits, {fields_range.begin(), fields_range.end()}};
            family_map.emplace(current_ancestor->type_id, std::move(cl));
            if (!current_ancestor->inherits) {
                break;
            }
            current_ancestor =
                &class_map.at(current_ancestor->inherits.value());
        }

        prototype_data_gen.generate_prototype(family_map, class_.type_id, data);
    }
}

void Codegen::generate_disptables(const AST::Program& AST, std::ostream& out) noexcept {
    //disptable_data_gen.generate_object_disptable(out);
    //disptable_data_gen.generate_int_disptable(out);
    //disptable_data_gen.generate_bool_disptable(out);
    //disptable_data_gen.generate_string_disptable(out);
    //disptable_data_gen.generate_io_disptable(out);
    for (auto& class_ : AST.classes) {
        auto *current_ancestor = &class_;
        std::unordered_map<std::string_view,
            MIPS32::ClassDispTableRepresentation>
            family_map;
        while (true) {
            auto fields_range = current_ancestor->features |
                     std::ranges::views::filter([](auto& feature_variant) {
                         return std::holds_alternative<AST::MethodFeature>(
                             feature_variant.feature);
                     }) |
                     std::ranges::views::transform(
                         [](auto& field_feature) -> std::string_view {
                             return std::get<AST::MethodFeature>(
                                 field_feature.feature)
                                 .object_id;
                         });
            MIPS32::ClassDispTableRepresentation cl = {
                current_ancestor->inherits, {fields_range.begin(), fields_range.end()}};
            family_map.emplace(current_ancestor->type_id, std::move(cl));
            if (!current_ancestor->inherits) {
                break;
            }
            current_ancestor =
                &class_map.at(current_ancestor->inherits.value());
        }

        disptable_data_gen.generate_disptable(family_map, class_.type_id, data);
    }
}

void Codegen::visit_class(const AST::Class& class_) noexcept {
    for (const auto& feature : class_.features) {
        std::visit(Overloaded{[&class_, this](const AST::MethodFeature& method_feature) {
                visit_method(class_.type_id, method_feature);
                },

                       [](const AST::FieldFeature& ) {}},
            feature.feature);
    }
}

void Codegen::visit_method(std::string_view class_name, const AST::MethodFeature& method_feature) noexcept {
    text << class_name << '.' << method_feature.object_id << ":\n";
    print_prologue(text);
    print_epilogue(text);
}

void Codegen::print_prologue(std::ostream& out) noexcept {
    print_single_instruction({instructions::AddImmediateUnsigned{"sp", "sp", -12}}, out);
    print_single_instruction({instructions::SaveWord{"fp", 12}}, out);
    print_single_instruction({instructions::SaveWord{"s0", 8}}, out);
    print_single_instruction({instructions::SaveWord{"ra", 4}}, out);
    print_single_instruction({instructions::AddImmediateUnsigned{"fp", "sp", 4}}, out);
    out << '\n';
}

void Codegen::print_epilogue(std::ostream& out) noexcept {
    out << '\n';
    print_single_instruction({instructions::LoadWord{"fp", 12}}, out);
    print_single_instruction({instructions::LoadWord{"s0", 8}}, out);
    print_single_instruction({instructions::LoadWord{"ra", 4}}, out);
    print_single_instruction({instructions::AddImmediateUnsigned{"sp", "sp", 12}}, out);
    print_single_instruction({instructions::JumpRegister{"ra"}}, out);
    out << '\n';
}

void Codegen::print_data(std::ostream& out) noexcept {
    out << ".data\n";
    out << data.rdbuf();
}

void Codegen::print_text(std::ostream& out) noexcept {
    out << ".text\n";
    out << text.rdbuf();
}

void Codegen::print_single_instruction(const instructions::Instruction& instruction, std::ostream& out) const noexcept {
    std::visit(
        Overloaded{
            [&out](const instructions::MemoryInstruction& memory_instruction) {
                out << "    " << std::setw(8) << memory_instruction.op << " $"
                    << memory_instruction.dest
                    << std::setw(6) << memory_instruction.offset << " ($sp)\n";
            },
            [&out](const instructions::ImmediateInstruction& immediate_instruction) {
                out << "    " << std::setw(8) << immediate_instruction.op << " $"
                    << immediate_instruction.dest << std::setw(6 - static_cast<int>(immediate_instruction.operand.size()))
                    << '$' << immediate_instruction.operand << ' '
                    << immediate_instruction.value << '\n';
            },
            [&out](const instructions::Jump& jump_instruction) {
                out << "    " << std::setw(8) << "j" << " $"
                    << jump_instruction.value << '\n';
            },
            [&out](const instructions::JumpRegister& jump_immediate_instruction) {
                out << "    " << std::setw(8) << "jr" << " $"
                    << jump_immediate_instruction.dest << '\n';
            },
        },
        instruction.instruction);
}

void Codegen::make_class_map(const AST::Program& AST) noexcept {
    for (auto& class_ : AST.classes) {
        class_map.emplace(class_.type_id, class_);
    }
}

} //namespace
