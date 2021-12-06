#pragma once
#include "AST/AST.h"
#include "codegen/MIPS32/ConstantGenerator.h"
#include "codegen/MIPS32/DispTableGen.h"
#include "codegen/MIPS32/PrototypeGen.h"
#include "codegen/MIPS32/TextGen.h"

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
    }

    void operator()(const AST::DotExpression& expr) noexcept {
        std::for_each(expr.parameter_expressions.crbegin(),
            expr.parameter_expressions.crend(), [this](auto& arg) {
                std::visit(*this, arg->value);
                text_gen->save_to_stack(out);
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

    void operator()(const AST::WhileExpression&) noexcept {
    }

    void operator()(const AST::CompoundExpression& compound_expr) noexcept {
        for (auto& expr : compound_expr.expressions) {
            std::visit(*this, expr->value);
        }
    }

    void operator()(const AST::CaseExpression&) noexcept {}

    void operator()(const AST::NewExpression& expr) noexcept {
        text_gen->generate_new(expr.type_id, out);
    }

    void operator()(const AST::IsVoidExpression&) noexcept {}

    template <AST::CompareButNotEqual T> void operator()(const T&) noexcept {}

    void operator()(const AST::EqualExpression& eq_expression) noexcept {
        std::visit(*this, eq_expression.left_expression->value);
        text_gen->save_to_stack(out);
        std::visit(*this, eq_expression.right_expression->value);
        unsigned label = text_gen->reserve_label();
        text_gen->generate_equal(label, out);
        text_gen->print_label(label, out);
    }

    void operator()(const AST::TildeExpression&) noexcept {}

    void operator()(const AST::ObjectExpression& expr) noexcept {
        if (expr.object_id == "self") {
            text_gen->generate_self_object(out);
        } else {
            unsigned offset = prototype_data_gen->get_field_offset(current_class_name, expr.object_id);
            text_gen->generate_field_object(offset, out);
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

    void operator()(const AST::NotExpression&) noexcept {
        text_gen->generate_not(out);
    }

    void operator()(const AST::LetExpression&) noexcept {}

    void operator()(const AST::AssignExpression&) noexcept {}

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
};

} // namespace cool::codegen::MIPS32
