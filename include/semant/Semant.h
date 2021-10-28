#pragma once

#include "parser/Parser.h"

namespace cool::semant {

class Semant {
public:
    explicit Semant(const std::vector<parser::Parser>& parsers) {
        for (auto &parser : parsers) {
            if (auto parser_result = parser.get_result()) {
                std::ranges::copy(parser.get_result().value().classes, std::back_inserter(program.classes));
                check_semantics();
            }
        }
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
    void check_semantics() {
        if (!check_entry_point()) {
            return;
        }
    }

    [[nodiscard]] bool check_entry_point() noexcept {
        auto iter = std::ranges::find_if(
            program.classes,
            [](const std::string& type_id) { return type_id == "Main"; },
            &AST::Class::type_id);
        if (iter == program.classes.cend()) {
            error_msg = "Main class not found.";
            return false;
        }
        if (!std::ranges::any_of(
                iter->features.begin(), iter->features.end(),
                [](const std::variant<AST::MethodFeature, AST::FieldFeature>&
                        feature) {
                    return std::holds_alternative<AST::MethodFeature>(
                               feature) &&
                           std::get<AST::MethodFeature>(feature).object_id ==
                               "main";
                },
                &AST::Feature::feature)) {
            error_msg = "Method main not found.";
            return false;
        }
        return true;
    }

    AST::Program program;
    std::optional<std::string> error_msg;
};

} // namespace cool::semant
