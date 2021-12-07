#pragma once
#include "AST/AST.h"
#include "codegen/MIPS32/ConstantGenerator.h"
#include "codegen/MIPS32/DispTableGen.h"
#include "codegen/MIPS32/PrototypeGen.h"
#include "codegen/MIPS32/TextGen.h"
#include "codegen/MIPS32/StackRepresentation.h"

#include <unordered_map>

namespace cool::codegen::MIPS32 {

class ASTVisitor {
public:
    ASTVisitor() = delete;

    ASTVisitor(std::ostream& in_out,
        ConstantsDataGeneneror* in_constants_data_gen,
        DispTableDataGenerator* in_disptable_data_gen, PrototypeDataGenerator* in_prototype_data_gen, TextGen* in_text_gen)
        : out(in_out), constants_data_gen(in_constants_data_gen),
          disptable_data_gen(in_disptable_data_gen), prototype_data_gen(in_prototype_data_gen), text_gen(in_text_gen) {}

    void operator()(const AST::Program& program) noexcept {
        for (auto& class_ : program.classes) {
            (*this)(class_);
        }
    }
    
    void operator()(const AST::Class& class_) noexcept {
        set_current_class(class_.type_id);
        for (const auto& feature : class_.features) {
            std::visit(*this, feature.feature);
        }
    }

    // TODO vars
    void operator()(const AST::MethodFeature& expr) noexcept {
        out << current_class_name << '.' << expr.object_id << ":\n";
        text_gen->print_prologue(out);
        std::visit(*this, expr.value->value);
        text_gen->print_epilogue(out);
    }

    void operator()(const AST::FieldFeature&) noexcept {}

    template <AST::Arithmetic T> void operator()(const T& op) noexcept {
        std::visit(*this, op.left_expression->value);
        text_gen->save_to_stack(out);
        stack_representation.add_padding(4);
        std::visit(*this, op.right_expression->value);
        if constexpr (std::is_same_v<T, AST::PlusExpression>) {
            text_gen->generate_binary_add(out);
        } else if constexpr (std::is_same_v<T, AST::MinusExpression>) {
            text_gen->generate_binary_sub(out);
        } else if constexpr (std::is_same_v<T, AST::MultiplyExpression>) {
            text_gen->generate_binary_mul(out);
        } else {
            text_gen->generate_binary_div(out);
        }
        stack_representation.pop();
    }

    void operator()(const AST::DotExpression& expr) noexcept {
        std::for_each(expr.parameter_expressions.crbegin(),
            expr.parameter_expressions.crend(), [this](auto& arg) {
                std::visit(*this, arg->value);
                text_gen->save_to_stack(out);
                stack_representation.add_padding(4);
            });
        std::visit(*this, expr.expression->value);
        if (expr.expression->type == "SELF_TYPE") {
            text_gen->generate_method_call(expr.line_number,
                disptable_data_gen->get_disptable()
                    .at(current_class_name).first
                    .at(expr.object_id),
                out);
        } else {
            text_gen->generate_method_call(expr.line_number,
                disptable_data_gen->get_disptable()
                    .at(expr.expression->type).first
                    .at(expr.object_id),
                out);
        }
        for (size_t i = 0; i != expr.parameter_expressions.size(); ++i) {
            stack_representation.pop();
        }
    }

    void operator()(const AST::IfExpression& if_expression) noexcept {
        std::visit(*this, if_expression.condition_expression->value);
        unsigned else_label = text_gen->reserve_label();
        unsigned end_label = text_gen->reserve_label();
        text_gen->generate_condition_check(else_label, out);
        std::visit(*this, if_expression.if_expression->value);
        text_gen->generate_if_end(end_label, out);
        text_gen->print_label(else_label, out);
        std::visit(*this, if_expression.else_expression->value);
        text_gen->generate_else_end(end_label, out);
    }

    void operator()(const AST::WhileExpression& while_expression) noexcept {
        unsigned start_loop_label = text_gen->reserve_label();
        text_gen->print_label(start_loop_label, out);
        std::visit(*this, while_expression.condition_expression->value);
        unsigned end_loop_label = text_gen->reserve_label();
        text_gen->generate_loop_check(end_loop_label, out);
        std::visit(*this, while_expression.body_expression->value);
        text_gen->generate_loop_end(start_loop_label, end_loop_label, out);
    }

    void operator()(const AST::CompoundExpression& compound_expr) noexcept {
        for (auto& expr : compound_expr.expressions) {
            std::visit(*this, expr->value);
        }
    }

    // TODO
    void operator()(const AST::CaseExpression&) noexcept {}

    void operator()(const AST::NewExpression& expr) noexcept {
        text_gen->generate_new(expr.type_id, out);
    }

    // TODO
    void operator()(const AST::IsVoidExpression&) noexcept {}

    // TODO
    template <AST::CompareButNotEqual T> void operator()(const T&) noexcept {}

    void operator()(const AST::EqualExpression& eq_expression) noexcept {
        std::visit(*this, eq_expression.left_expression->value);
        text_gen->save_to_stack(out);
        stack_representation.add_padding(4);
        std::visit(*this, eq_expression.right_expression->value);
        unsigned label = text_gen->reserve_label();
        text_gen->generate_equal(label, out);
        stack_representation.pop();
        text_gen->print_label(label, out);
    }

    // TODO
    void operator()(const AST::TildeExpression&) noexcept {}

    void operator()(const AST::ObjectExpression& expr) noexcept {
        if (expr.object_id == "self") {
            text_gen->load_self_object(out);
        } else {
            unsigned offset = 0;
            if (stack_representation.contains(expr.object_id)) {
                offset = stack_representation.get_variable(expr.object_id);
                text_gen->load_stack_object(offset, out);
            } else {
                offset = prototype_data_gen->get_field_offset(current_class_name, expr.object_id);
                text_gen->load_field_object(offset, out);
            }
        }
    }

    void operator()(const AST::IntExpression& expr) noexcept {
        constants_data_gen->register_int(expr.value);
        text_gen->generate_int_constant(expr.value, out);
    }

    void operator()(const AST::StringExpression& expr) noexcept {
        unsigned str_id = constants_data_gen->register_string(expr.value);
        text_gen->generate_string_constant(str_id, out);
    }

    template <AST::Boolean T> void operator()(const T&) noexcept {
        if constexpr (std::is_same_v<T, AST::TrueExpression>) {
            text_gen->generate_true(out);
        } else {
            text_gen->generate_false(out);
        }
    }

    void operator()(const AST::NotExpression& not_expression) noexcept {
        std::visit(*this, not_expression.expression->value);
        text_gen->generate_not(out);
    }

    void operator()(const AST::LetExpression& let_expression) noexcept {
        std::unordered_map<std::string_view, unsigned> vars;
        unsigned current_offset = 4;
        for (auto it = let_expression.let_expressions.crbegin(); it != let_expression.let_expressions.crend(); ++it) {
            vars.emplace(it->object_id, current_offset);
            current_offset += 4;
        }
        stack_representation.register_vars(std::move(vars));
        text_gen->grow_stack(static_cast<unsigned>(let_expression.let_expressions.size()) * 4, out);
        for (auto& entry : let_expression.let_expressions) {
            unsigned offset = stack_representation.get_variable(entry.object_id);
            if (entry.assign_expression) {
                std::visit(*this, entry.assign_expression.value()->value);
            } else {
                if (entry.type_id == "Int") {
                    text_gen->generate_int_constant(0, out);
                } else if (entry.type_id == "Bool") {
                    text_gen->generate_bool_constant(false, out);
                } else if (entry.type_id == "String") {
                    text_gen->generate_empty_string_constant(out);
                } else {
                    text_gen->generate_zeros(out);
                }
            }
            text_gen->save_to_local(offset, out);
        }
        std::visit(*this, let_expression.expression->value);
        stack_representation.pop();
        text_gen->reduce_stack(static_cast<unsigned>(let_expression.let_expressions.size()) * 4, out);
    }

    void operator()(const AST::AssignExpression& assign_expr) noexcept {
        std::visit(*this, assign_expr.expression->value);
        unsigned offset = 0;
        if (stack_representation.contains(assign_expr.object_id)) {
            offset = stack_representation.get_variable(assign_expr.object_id);
            text_gen->save_to_local(offset, out);
        } else {
            offset = prototype_data_gen->get_field_offset(current_class_name, assign_expr.object_id);
            text_gen->save_to_field(offset, out);
        }
    }

    void set_current_class(std::string_view class_name) noexcept {
        current_class_name = class_name;
    }

private:
    std::ostream& out;
    std::string_view current_class_name;
    MIPS32::ConstantsDataGeneneror* constants_data_gen = nullptr;
    MIPS32::DispTableDataGenerator* disptable_data_gen = nullptr;
    MIPS32::PrototypeDataGenerator* prototype_data_gen = nullptr;
    MIPS32::TextGen* text_gen = nullptr;
    StackRepresentation stack_representation;
};

} // namespace cool::codegen::MIPS32
