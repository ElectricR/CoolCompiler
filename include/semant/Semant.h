#pragma once

#include "parser/Parser.h"

namespace cool::semant {

class Semant {
public:
    explicit Semant(const std::vector<parser::Parser>& parsers) {
        for (auto &parser : parsers) {
            if (auto parser_result = parser.get_result()) {
                std::ranges::copy(parser.get_result().value().classes, std::back_inserter(program.classes));
            }
        }
        check_semantics();
    }

    [[nodiscard]] bool is_successfull() const noexcept {
        return !error_msg.has_value();
    }

    [[nodiscard]] std::string get_error() const {
        return error_msg.value();
    }

    [[nodiscard]] const auto& get_result() const noexcept {
        return program;
    }

private:
    void check_semantics(); 

    void check_entry_point() noexcept; 

    void get_class_names(auto& all_types) noexcept; 

    [[nodiscard]] std::optional<std::string> get_object_id_type(const auto& object_id, const auto& vars_stack) noexcept; 

    [[nodiscard]] auto get_class_fields(const auto& class_, const auto& all_types) noexcept; 

    void infer_types(const auto& all_types) noexcept; 

    void check_inheritance(auto& all_types) noexcept; 

    void check_class_features(auto& class_, auto& vars_stack, const auto& all_types) noexcept; 

    void check_class_field(auto& field_feature, auto& vars_stack, const auto& all_types, const auto& class_) noexcept; 

    void check_class_method(auto& method_feature, auto& vars_stack, const auto& all_types, const auto& class_) noexcept; 

    void check_expression(std::shared_ptr<AST::Expression> expression,
    auto& vars_stack, const auto& all_types, const auto& class_) noexcept;

    [[nodiscard]] bool check_expression_callable(const auto& parameter_expressions,
        const auto& all_types, std::string& target_class,
        const auto& requested_method_id, const auto& class_) noexcept;

    void check_inherited_redefinitions(auto& all_types) noexcept;

    void populate_all_types(auto& all_types) noexcept; 

    [[nodiscard]] bool check_type_permissibility(std::string curr_type, const auto& result_type, const auto& all_types, const auto& class_) noexcept;

    AST::Program program;
    std::optional<std::string> error_msg;
};

} // namespace cool::semant
