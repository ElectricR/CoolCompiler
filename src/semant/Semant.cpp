#include "semant/Semant.h"
#include "AST/AST.h"
#include <string>

template <class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

struct MethodRepresentation {
    std::string return_type;
    std::vector<std::string> attr_types;
};

struct ClassRepresentation {
    std::string inherits;
    std::unordered_map<std::string, MethodRepresentation> methods;
    std::unordered_map<std::string, std::string> fields;
};

const std::unordered_map<std::string, std::string> base_types = {
    {"Bool", "Object"}, {"Int", "Object"}, {"String", "Object"},
    {"IO", "Object"}, {"Object", ""}};

void cool::semant::Semant::get_class_names(auto& all_types) noexcept {
    for (auto& class_ : program.classes) {
        if (all_types.contains(class_.type_id) ||
            class_.type_id == "SELF_TYPE" ||
            base_types.contains(class_.type_id)) {
            error_msg = "Redefinition of class " + class_.type_id;
            break;
        }
        if (class_.inherits) {
            all_types[class_.type_id] = {class_.inherits.value(), {}, {}};
        } else {
            all_types[class_.type_id] = {"", {}, {}};
        }
    }
}

void cool::semant::Semant::check_semantics() {
    std::unordered_map<std::string, ClassRepresentation> all_types;
    get_class_names(all_types);
    if (error_msg)
        return;

    check_inheritance(all_types);
    if (error_msg)
        return;

    populate_all_types(all_types);
    if (error_msg)
        return;

    check_entry_point();
    if (error_msg)
        return;

    infer_types(all_types);
    if (error_msg)
        return;
}

void cool::semant::Semant::check_entry_point() noexcept {
    auto iter = std::ranges::find_if(
        program.classes,
        [](const std::string& type_id) { return type_id == "Main"; },
        &AST::Class::type_id);
    if (iter == program.classes.cend()) {
        error_msg = "Main class not found";
        return;
    }
    if (!std::ranges::any_of(
            iter->features.begin(), iter->features.end(),
            [](const std::variant<AST::MethodFeature, AST::FieldFeature>&
                    feature) {
                return std::holds_alternative<AST::MethodFeature>(feature) &&
                       std::get<AST::MethodFeature>(feature).object_id ==
                           "main";
            },
            &AST::Feature::feature)) {
        error_msg = "Method main not found";
        return;
    }
}

void cool::semant::Semant::check_inherited_redefinitions(
    auto& all_types) noexcept {
    for (auto& class_ : program.classes) {
        for (const auto& feature : class_.features) {
            std::visit(
                Overloaded{
                    [&](const AST::MethodFeature& method_feature) {
                        std::string curr_class =
                            all_types[class_.type_id].inherits;

                        while (curr_class.size()) {
                            if (all_types[curr_class].fields.contains(
                                    method_feature.object_id)) {
                                error_msg = "Redefinition of feature " +
                                            method_feature.object_id;
                                return;
                            }
                            if (all_types[curr_class].methods.contains(
                                    method_feature.object_id)) {
                                if (!std::ranges::equal(method_feature.formals,
                                        all_types[curr_class]
                                            .methods[method_feature.object_id]
                                            .attr_types,
                                        {}, &AST::Formal::type_id)) {
                                    error_msg =
                                        "Wrong redefinition of method " +
                                        method_feature.object_id;
                                    return;
                                }
                            }

                            curr_class = all_types[curr_class].inherits;
                        }
                    },
                    [&](const AST::FieldFeature& field_feature) {
                        std::string curr_class =
                            all_types[class_.type_id].inherits;

                        while (curr_class.size()) {
                            if (all_types[curr_class].methods.contains(
                                    field_feature.object_id) ||
                                all_types[curr_class].fields.contains(
                                    field_feature.object_id)) {
                                error_msg = "Redefinition of feature " +
                                            field_feature.object_id;
                                return;
                            }

                            curr_class = all_types[curr_class].inherits;
                        }
                    },
                },
                feature.feature);
        }
    }
}

void cool::semant::Semant::populate_all_types(auto& all_types) noexcept {
    for (auto& [base_type_name, base_type_inherits] : base_types) {
        all_types[base_type_name] = {base_type_inherits, {}, {}};
        if (base_type_name == "String") {
            all_types[base_type_name].methods.insert({"length", {"Int", {}}});
            all_types[base_type_name].methods.insert(
                {"substr", {"String", {"Int", "Int"}}});
            all_types[base_type_name].methods.insert(
                {"concat", {"String", {"String"}}});
            all_types[base_type_name].methods.insert({"copy", {"String", {}}});
        }
        if (base_type_name == "Object") {
            all_types[base_type_name].methods.insert({"abort", {"Object", {}}});
        }
        if (base_type_name == "IO") {
            all_types[base_type_name].methods.insert(
                {"out_string", {"SELF_TYPE", {"String"}}});
            all_types[base_type_name].methods.insert(
                {"out_int", {"SELF_TYPE", {"Int"}}});
            all_types[base_type_name].methods.insert(
                {"in_string", {"String", {}}});
            all_types[base_type_name].methods.insert({"in_int", {"Int", {}}});
        }
        if (base_type_name == "Int") {
            all_types[base_type_name].methods.insert({"copy", {"Int", {}}});
        }
        if (base_type_name == "Bool") {
            all_types[base_type_name].methods.insert({"copy", {"Bool", {}}});
        }
    }
    for (auto& class_ : program.classes) {
        for (const auto& feature : class_.features) {
            std::visit(
                Overloaded{
                    [&](const AST::MethodFeature& method_feature) {
                        if (all_types[class_.type_id].methods.contains(
                                method_feature.object_id) ||
                            all_types[class_.type_id].fields.contains(
                                method_feature.object_id)) {
                            error_msg = "Redefinition of feature " +
                                        method_feature.object_id;
                            return;
                        }
                        all_types[class_.type_id]
                            .methods[method_feature.object_id] = {
                            method_feature.type_id, {}};
                        std::ranges::transform(method_feature.formals,
                            std::back_inserter(
                                all_types[class_.type_id]
                                    .methods[method_feature.object_id]
                                    .attr_types),
                            &AST::Formal::type_id);
                        if (std::ranges::any_of(
                                method_feature.formals,
                                [](auto& attr_type) {
                                    return attr_type == "SELF_TYPE";
                                },
                                &AST::Formal::type_id)) {
                            error_msg = "SELF_TYPE in method parameter";
                            return;
                        }
                        if (std::ranges::any_of(
                                method_feature.formals,
                                [](auto& attr_type) {
                                    return attr_type == "self";
                                },
                                &AST::Formal::object_id)) {
                            error_msg = "self in method parameter";
                            return;
                        }
                    },
                    [&](const AST::FieldFeature& field_feature) {
                        if (field_feature.object_id == "self") {
                            error_msg = "self as field";
                            return;
                        }
                        if (all_types[class_.type_id].methods.contains(
                                field_feature.object_id) ||
                            all_types[class_.type_id].fields.contains(
                                field_feature.object_id)) {
                            error_msg = "Redefinition of feature " +
                                        field_feature.object_id;
                            return;
                        }
                        all_types[class_.type_id].fields.insert(
                            {field_feature.object_id, field_feature.type_id});
                    },
                },
                feature.feature);
        }
    }
    check_inherited_redefinitions(all_types);
}

[[nodiscard]] bool DFS(const auto& all_types, const auto& init) noexcept {
    auto key = init;
    while (all_types.contains(key)) {
        if (all_types.at(key).inherits == init) {
            return true;
        }
        key = all_types.at(key).inherits;
    }
    return false;
}

void cool::semant::Semant::check_inheritance(auto& all_types) noexcept {
    for (auto& [class_name, class_representation] : all_types) {
        if (class_representation.inherits.length()) {
            if (base_types.contains(class_representation.inherits) &&
                class_representation.inherits != "IO" &&
                class_representation.inherits != "Object") {
                error_msg = "Can't inherit " + class_representation.inherits;
                break;
            }
            if (!all_types.contains(class_representation.inherits) &&
                class_representation.inherits != "IO" &&
                class_representation.inherits != "Object") {
                error_msg =
                    "No such type to inherit " + class_representation.inherits;
                break;
            }
        } else {
            class_representation.inherits = "Object";
        }
        if (DFS(all_types, class_name)) {
            error_msg = "Cyclic dependency found in " + class_name;
            break;
        }
    }
}

[[nodiscard]] std::optional<std::string>
cool::semant::Semant::get_object_id_type(
    const auto& object_id, const auto& vars_stack) noexcept {
    if (object_id == "self") {
        return "SELF_TYPE";
    }
    auto it = std::find_if(vars_stack.crbegin(), vars_stack.crend(),
        [object_id](auto& map) { return map.contains(object_id); });
    if (it != vars_stack.crend()) {
        return it->at(object_id);
    }
    error_msg = "ID " + object_id + " not found";
    return {};
}

[[nodiscard]] std::string LCA(
    const auto& all_types, const auto& first, const auto& second) noexcept {
    std::unordered_set<std::string> first_ancestors = {{first}};
    std::string iter_value = first;
    while (iter_value != "Object") {
        first_ancestors.insert(all_types.at(iter_value).inherits);
        iter_value = all_types.at(iter_value).inherits;
    }
    iter_value = second;
    while (true) {
        if (first_ancestors.contains(iter_value)) {
            return iter_value;
        }
        iter_value = all_types.at(iter_value).inherits;
    }
}

[[nodiscard]] bool cool::semant::Semant::check_expression_callable(
    const auto& parameter_expressions, const auto& all_types,
    std::string& target_class, const auto& requested_method_id,
    const auto& class_) noexcept {
    bool found_suitable_method = false;

    while (!found_suitable_method) {
        for (auto& [method_id, method_representation] :
            all_types.at(target_class).methods) {
            if (method_id != requested_method_id) {
                continue;
            }
            if (std::ranges::equal(
                    parameter_expressions, method_representation.attr_types,
                    [&](auto& actual_parameter_type,
                        auto& expected_parameter_type) {
                        return check_type_permissibility(actual_parameter_type,
                            expected_parameter_type, all_types, class_);
                    },
                    &AST::Expression::type)) {
                found_suitable_method = true;
                break;
            }
        }
        if (!found_suitable_method) {
            target_class = all_types.at(target_class).inherits;
            if (!target_class.size()) {
                error_msg =
                    "Could not find suitable method " + requested_method_id;
                return false;
            }
        }
    }
    return true;
}

void cool::semant::Semant::check_expression(
    std::shared_ptr<AST::Expression> expr, auto& vars_stack,
    const auto& all_types, const auto& class_) noexcept {
    std::string result_type;
    std::visit(
        Overloaded{
            [&](const cool::AST::DotExpression& expression) {
                check_expression(
                    expression.expression, vars_stack, all_types, class_);
                for (auto& subexpression : expression.parameter_expressions) {
                    check_expression(
                        subexpression, vars_stack, all_types, class_);
                }
                if (error_msg)
                    return;

                std::string target_class = expression.expression->type;
                if (target_class == "SELF_TYPE") {
                    target_class = class_.type_id;
                }
                if (!check_expression_callable(expression.parameter_expressions,
                        all_types, target_class, expression.object_id,
                        class_)) {
                    return;
                }
                if (all_types.at(target_class)
                        .methods.at(expression.object_id)
                        .return_type == "SELF_TYPE") {
                    result_type = expression.expression->type;
                } else {
                    result_type = all_types.at(target_class)
                                      .methods.at(expression.object_id)
                                      .return_type;
                }
            },
            [&](const cool::AST::IfExpression& expression) {
                check_expression(expression.condition_expression, vars_stack,
                    all_types, class_);
                check_expression(
                    expression.if_expression, vars_stack, all_types, class_);
                check_expression(
                    expression.else_expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                if (expression.condition_expression->type != "Bool") {
                    error_msg = "Expected Bool expression in if statement";
                    return;
                }
                if (expression.if_expression->type == "SELF_TYPE") {
                    result_type = LCA(all_types, class_.type_id,
                        expression.else_expression->type);
                } else {
                    result_type = LCA(all_types, expression.if_expression->type,
                        expression.else_expression->type);
                }
            },
            [&](const cool::AST::WhileExpression& expression) {
                check_expression(expression.condition_expression, vars_stack,
                    all_types, class_);
                check_expression(
                    expression.body_expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                if (expression.condition_expression->type == "Bool") {
                    result_type = "Object";
                } else {
                    error_msg = "Expected Bool expression in while statement";
                }
            },
            [&](const cool::AST::CompoundExpression& expression) {
                for (auto& subexpression : expression.expressions) {
                    check_expression(
                        subexpression, vars_stack, all_types, class_);
                    if (error_msg)
                        return;
                }
                result_type = expression.expressions.back()->type;
            },
            [&](const cool::AST::CaseExpression& expression) {
                std::unordered_set<std::string> branch_types;
                std::ranges::transform(expression.branch_expressions,
                    std::inserter(branch_types, branch_types.begin()),
                    &AST::CaseBranch::type_id);
                if (branch_types.size() !=
                    expression.branch_expressions.size()) {
                    error_msg = "Not all branch types are distinct";
                    return;
                }
                check_expression(
                    expression.case_expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                for (auto& case_branch : expression.branch_expressions) {
                    vars_stack.push_back(
                        std::unordered_map<std::string, std::string>{
                            {case_branch.object_id, case_branch.type_id}});
                    check_expression(
                        case_branch.expression, vars_stack, all_types, class_);
                    if (error_msg)
                        return;
                    vars_stack.pop_back();
                }
                result_type =
                    expression.branch_expressions.front().expression->type;
                std::ranges::for_each(
                    expression.branch_expressions.cbegin() + 1,
                    expression.branch_expressions.cend(),
                    [&](const auto& case_branch_expression_ptr) {
                        if (case_branch_expression_ptr->type == "SELF_TYPE") {
                            result_type =
                                LCA(all_types, result_type, class_.type_id);
                        } else {
                            result_type = LCA(all_types, result_type,
                                case_branch_expression_ptr->type);
                        }
                    },
                    &AST::CaseBranch::expression);
            },
            [&](const cool::AST::NewExpression& expression) {
                if (expression.type_id != "SELF_TYPE" &&
                    !all_types.contains(expression.type_id)) {
                    error_msg = "Type " + expression.type_id + " not found";
                    return;
                }
                result_type = expression.type_id;
            },
            [&](const cool::AST::IsVoidExpression& expression) {
                check_expression(
                    expression.expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                result_type = "Bool";
            },
            [&](const AST::Arithmetic auto& expression) {
                check_expression(
                    expression.left_expression, vars_stack, all_types, class_);
                check_expression(
                    expression.right_expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                if (expression.left_expression->type == "Int" &&
                    expression.right_expression->type == "Int") {
                    result_type = "Int";
                } else {
                    error_msg = "Unknown operands for arithmetic expression: " +
                                expression.left_expression->type + " and " +
                                expression.right_expression->type;
                }
            },
            [&](const AST::CompareButNotEqual auto& expression) {
                check_expression(
                    expression.left_expression, vars_stack, all_types, class_);
                check_expression(
                    expression.right_expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                if (expression.left_expression->type == "Int" &&
                    expression.right_expression->type == "Int") {
                    result_type = "Bool";
                } else {
                    error_msg = "Unknown operands for compare expression: " +
                                expression.left_expression->type + " and " +
                                expression.right_expression->type;
                }
            },
            [&](const AST::EqualExpression& expression) {
                std::vector allowed_base_types = {"Int", "Bool", "String"};
                check_expression(
                    expression.left_expression, vars_stack, all_types, class_);
                check_expression(
                    expression.right_expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                if (std::ranges::any_of(
                        allowed_base_types, [&](const auto& base_type) {
                            return base_type ==
                                       expression.left_expression->type ||
                                   base_type ==
                                       expression.right_expression->type;
                        })) {
                    if (expression.left_expression->type !=
                        expression.right_expression->type) {
                        error_msg =
                            "Unknown operands for compare expression: " +
                            expression.left_expression->type + " and " +
                            expression.right_expression->type;
                        return;
                    }
                }
                result_type = "Bool";
            },
            [&](const cool::AST::TildeExpression& expression) {
                check_expression(
                    expression.expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                if (expression.expression->type == "Int") {
                    result_type = "Int";
                } else {
                    error_msg =
                        "Expected Int in ~, got " + expression.expression->type;
                }
            },
            [&](const cool::AST::ObjectExpression& expression) {
                auto result =
                    get_object_id_type(expression.object_id, vars_stack);
                if (!result) {
                    return;
                } else {
                    result_type = result.value();
                }
            },
            [&](const cool::AST::IntExpression&) { result_type = "Int"; },
            [&](const cool::AST::StringExpression&) { result_type = "String"; },
            [&](const AST::Boolean auto&) { result_type = "Bool"; },
            [&](const cool::AST::NotExpression& expression) {
                check_expression(
                    expression.expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                if (expression.expression->type == "Bool") {
                    result_type = "Bool";
                } else {
                    error_msg = "Expected Bool in \"not\", got " +
                                expression.expression->type;
                }
            },
            [&](const cool::AST::LetExpression& expression) {
                for (auto& let_entry : expression.let_expressions) {
                    if (let_entry.object_id == "self") {
                        error_msg = "self in let init";
                        return;
                    }
                    if (let_entry.assign_expression) {
                        check_expression(let_entry.assign_expression.value(),
                            vars_stack, all_types, class_);
                        if (error_msg)
                            return;

                        std::string actual_type = let_entry.type_id;
                        if (!check_type_permissibility(
                                let_entry.assign_expression.value()->type,
                                actual_type, all_types, class_)) {
                            error_msg =
                                "Can't assign " +
                                let_entry.assign_expression.value()->type +
                                " to " + actual_type;
                            return;
                        }
                    }
                    vars_stack.push_back(
                        std::unordered_map<std::string, std::string>{
                            {let_entry.object_id, let_entry.type_id}});
                }
                check_expression(
                    expression.expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                for (size_t i = 0; i != expression.let_expressions.size();
                     ++i) {
                    vars_stack.pop_back();
                }
                result_type = expression.expression->type;
            },
            [&](const cool::AST::AssignExpression& expression) {
                if (expression.object_id == "self") {
                    error_msg = "Can't assign to self";
                    return;
                }
                check_expression(
                    expression.expression, vars_stack, all_types, class_);
                if (error_msg)
                    return;
                auto result =
                    get_object_id_type(expression.object_id, vars_stack);
                if (!result) {
                    return;
                }
                if (!check_type_permissibility(expression.expression->type,
                        result.value(), all_types, class_)) {
                    error_msg = "Can't assign " + expression.expression->type +
                                " to " + result.value();
                }
                if (error_msg)
                    return;
                result_type = expression.expression->type;
            },
        },
        expr->value);
    if (error_msg)
        return;
    expr->type = result_type;
}

void cool::semant::Semant::check_class_field(auto& field_feature,
    auto& vars_stack, const auto& all_types, const auto& class_) noexcept {
    if (!all_types.contains(field_feature.type_id)) {
        error_msg = "No such type " + field_feature.type_id;
        return;
    }

    if (field_feature.value) {
        check_expression(
            field_feature.value.value(), vars_stack, all_types, class_);
    }
}

[[nodiscard]] bool cool::semant::Semant::check_type_permissibility(
    std::string curr_type, const auto& result_type, const auto& all_types,
    const auto& class_) noexcept {
    if (curr_type == "SELF_TYPE") {
        if (result_type == "SELF_TYPE") {
            return true;
        }
        curr_type = class_.type_id;
    }
    if (curr_type == result_type)
        return true;
    while (all_types.contains(curr_type)) {
        if (all_types.at(curr_type).inherits == result_type) {
            return true;
        }
        curr_type = all_types.at(curr_type).inherits;
    }
    return false;
}

void cool::semant::Semant::check_class_method(auto& method_feature,
    auto& vars_stack, const auto& all_types, const auto& class_) noexcept {
    if (!all_types.contains(method_feature.type_id) &&
        method_feature.type_id != "SELF_TYPE") {
        error_msg = "No such type " + method_feature.type_id;
        return;
    }
    vars_stack.push_back({});
    for (auto& formal : method_feature.formals) {
        if (vars_stack.back().contains(formal.object_id)) {
            error_msg = "Redefinition of feature " + formal.object_id;
            return;
        }
        vars_stack.back().insert({formal.object_id, formal.type_id});
    }
    check_expression(method_feature.value, vars_stack, all_types, class_);
    if (error_msg)
        return;
    vars_stack.pop_back();
    if (!check_type_permissibility(method_feature.value->type,
            method_feature.type_id, all_types, class_)) {
        error_msg = "Can't assign " + method_feature.value->type + " to " +
                    method_feature.type_id;
    }
}

void cool::semant::Semant::check_class_features(
    auto& class_, auto& vars_stack, const auto& all_types) noexcept {
    for (const auto& feature : class_.features) {
        std::visit(Overloaded{[&](const AST::FieldFeature& field_feature) {
                                  check_class_field(field_feature, vars_stack,
                                      all_types, class_);
                              },
                       [&](const AST::MethodFeature& method_feature) {
                           check_class_method(
                               method_feature, vars_stack, all_types, class_);
                       }},
            feature.feature);
        if (error_msg) {
            return;
        }
    }
}

[[nodiscard]] auto cool::semant::Semant::get_class_fields(
    const auto& class_, const auto& all_types) noexcept {

    std::unordered_map<std::string, std::string> field_ids;
    std::string curr_class = class_.type_id;
    while (curr_class.size()) {
        for (auto& [object_id, type_id] : all_types.at(curr_class).fields) {
            if (field_ids.contains(object_id)) {
                error_msg = "Redefinition of feature " + object_id;
                break;
            }
            field_ids[object_id] = type_id;
        }
        curr_class = all_types.at(curr_class).inherits;
    }
    return field_ids;
}

void cool::semant::Semant::infer_types(const auto& all_types) noexcept {
    for (auto& class_ : program.classes) {
        std::vector<std::unordered_map<std::string, std::string>> vars_stack;
        auto field_ids = get_class_fields(class_, all_types);
        if (error_msg) {
            return;
        }
        vars_stack.push_back(std::move(field_ids));
        check_class_features(class_, vars_stack, all_types);
        if (error_msg) {
            return;
        }
    }
}
