#include "parser/Parser.h"
#include "utils.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

template <class T>
concept Binary = std::is_base_of_v<cool::parser::symbol::BinaryExpression, T>;

template <Binary T> std::string get_binary_name(const T&) {
    if constexpr (std::is_same_v<T, cool::parser::symbol::PlusExpression>) {
        return "_plus";
    } else if (std::is_same_v<T, cool::parser::symbol::MinusExpression>) {
        return "_sub";
    } else if (std::is_same_v<T, cool::parser::symbol::MultiplyExpression>) {
        return "_mul";
    } else if (std::is_same_v<T, cool::parser::symbol::DivideExpression>) {
        return "_divide";
    } else if (std::is_same_v<T, cool::parser::symbol::LEExpression>) {
        return "_le";
    } else if (std::is_same_v<T, cool::parser::symbol::LessExpression>) {
        return "_lt";
    } else if (std::is_same_v<T, cool::parser::symbol::EqualExpression>) {
        return "_eq";
    } else {
        return "";
    }
}

template <class T>
concept Boolean = std::is_base_of_v<cool::parser::symbol::BoolExpression, T>;

template <class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

void print_expression(const auto& expr, int shift) {
    std::visit(
        Overloaded{
            [shift](
                const cool::parser::symbol::FunctionExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_dispatch" << std::endl;
                std::cout << std::setw(shift + 2) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift + 2) << ""
                          << "_object" << std::endl;
                std::cout << std::setw(shift + 4) << ""
                          << "self" << std::endl;
                std::cout << std::setw(shift + 2) << ""
                          << ": _no_type" << std::endl;
                std::cout << std::setw(shift + 2) << "" << expression.object_id
                          << std::endl;
                std::cout << std::setw(shift + 2) << ""
                          << "(" << std::endl;
                for (auto& expression_in : expression.expressions) {
                    print_expression(*expression_in, shift + 2);
                }
                std::cout << std::setw(shift + 2) << ""
                          << ")" << std::endl;
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },

            [shift](const cool::parser::symbol::DotExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                if (expression.type_id) {
                    std::cout << std::setw(shift) << ""
                              << "_static_dispatch" << std::endl;
                } else {
                    std::cout << std::setw(shift) << ""
                              << "_dispatch" << std::endl;
                }
                print_expression(*expression.expression, shift + 2);
                if (expression.type_id) {
                    std::cout << std::setw(shift + 2) << ""
                              << expression.type_id.value() << std::endl;
                }
                std::cout << std::setw(shift + 2) << "" << expression.object_id
                          << std::endl;
                std::cout << std::setw(shift + 2) << ""
                          << "(" << std::endl;
                for (auto& expression_in : expression.parameter_expressions) {
                    print_expression(*expression_in, shift + 2);
                }
                std::cout << std::setw(shift + 2) << ""
                          << ")" << std::endl;
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::IfExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_cond" << std::endl;
                print_expression(*expression.condition_expression, shift + 2);
                print_expression(*expression.if_expression, shift + 2);
                print_expression(*expression.else_expression, shift + 2);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::WhileExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_loop" << std::endl;
                print_expression(*expression.condition_expression, shift + 2);
                print_expression(*expression.body_expression, shift + 2);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](
                const cool::parser::symbol::CompoundExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_block" << std::endl;
                for (auto& expression_block : expression.expressions) {
                    print_expression(*expression_block, shift + 2);
                }
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::CaseExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_typcase" << std::endl;
                print_expression(*expression.case_expression, shift + 2);
                for (auto& branch : expression.branch_expressions) {
                    std::cout << std::setw(shift + 2) << ""
                              << "#" << branch.line_number << std::endl;
                    std::cout << std::setw(shift + 2) << ""
                              << "_branch" << std::endl;
                    std::cout << std::setw(shift + 4) << "" << branch.object_id
                              << std::endl;
                    std::cout << std::setw(shift + 4) << "" << branch.type_id
                              << std::endl;
                    print_expression(*branch.expression, shift + 4);
                }
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::NewExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_new" << std::endl;
                std::cout << std::setw(shift + 2) << "" << expression.type_id
                          << std::endl;
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::IsVoidExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_isvoid" << std::endl;
                print_expression(*expression.expression, shift + 2);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const Binary auto& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << get_binary_name(expression) << std::endl;
                print_expression(*expression.left_expression, shift + 2);
                print_expression(*expression.right_expression, shift + 2);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::TildeExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_neg" << std::endl;
                print_expression(*expression.expression, shift + 2);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::ObjectExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_object" << std::endl;
                std::cout << std::setw(shift + 2) << "" << expression.object_id
                          << std::endl;
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::IntExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_int" << std::endl;
                std::cout << std::setw(shift + 2) << "" << expression.value
                          << std::endl;
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::StringExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_string" << std::endl;
                std::cout << std::setw(shift + 2) << "";
                util::print_string(expression.value);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const Boolean auto& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_bool" << std::endl;
                std::cout << std::setw(shift + 2) << "" << expression.value
                          << std::endl;
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::NotExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_comp" << std::endl;
                print_expression(*expression.expression, shift + 2);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
            [shift](const cool::parser::symbol::LetExpression& expression) {
                auto shift_2 = shift;
                for (auto& let_entry : expression.let_expressions) {
                    std::cout << std::setw(shift_2) << ""
                              << "#" << let_entry.line_number << std::endl;
                    std::cout << std::setw(shift_2) << ""
                              << "_let" << std::endl;
                    shift_2 += 2;

                    std::cout << std::setw(shift_2) << "" << let_entry.object_id
                              << std::endl;
                    std::cout << std::setw(shift_2) << "" << let_entry.type_id
                              << std::endl;
                    if (let_entry.assign_expression) {
                        print_expression(
                            *let_entry.assign_expression.value(), shift_2);
                    } else {
                        std::cout << std::setw(shift_2) << ""
                                  << "#0" << std::endl;
                        std::cout << std::setw(shift_2) << ""
                                  << "_no_expr" << std::endl;
                        std::cout << std::setw(shift_2) << ""
                                  << ": _no_type" << std::endl;
                    }
                }
                print_expression(*expression.expression, shift_2);
                for (size_t i = 0; i != expression.let_expressions.size();
                     ++i) {
                    shift_2 -= 2;
                    std::cout << std::setw(shift_2) << ""
                              << ": _no_type" << std::endl;
                }
            },
            [shift](const cool::parser::symbol::AssignExpression& expression) {
                std::cout << std::setw(shift) << ""
                          << "#" << expression.line_number << std::endl;
                std::cout << std::setw(shift) << ""
                          << "_assign" << std::endl;
                std::cout << std::setw(shift + 2) << "" << expression.object_id
                          << std::endl;
                print_expression(*expression.expression, shift + 2);
                std::cout << std::setw(shift) << ""
                          << ": _no_type" << std::endl;
            },
        },
        expr.value);
}

void print_feature(const auto& feat, int shift) {
    std::visit(Overloaded{
                   [shift](const cool::parser::symbol::MethodFeature& feature) {
                       std::cout << std::setw(shift) << ""
                                 << "#" << feature.line_number << std::endl;
                       std::cout << std::setw(shift) << ""
                                 << "_method" << std::endl;
                       std::cout << std::setw(shift + 2) << ""
                                 << feature.object_id << std::endl;
                       for (auto& formal : feature.formals) {
                           std::cout << std::setw(shift + 2) << ""
                                     << "#" << formal.line_number << std::endl;
                           std::cout << std::setw(shift + 2) << ""
                                     << "_formal" << std::endl;
                           std::cout << std::setw(shift + 4) << ""
                                     << formal.object_id << std::endl;
                           std::cout << std::setw(shift + 4) << ""
                                     << formal.type_id << std::endl;
                       }
                       std::cout << std::setw(shift + 2) << ""
                                 << feature.type_id << std::endl;
                       print_expression(*feature.value, shift + 2);
                   },
                   [shift](const cool::parser::symbol::FieldFeature& feature) {
                       std::cout << std::setw(shift) << ""
                                 << "#" << feature.line_number << std::endl;
                       std::cout << std::setw(shift) << ""
                                 << "_attr" << std::endl;
                       std::cout << std::setw(shift + 2) << ""
                                 << feature.object_id << std::endl;
                       std::cout << std::setw(shift + 2) << ""
                                 << feature.type_id << std::endl;
                       if (!feature.value) {
                           std::cout << std::setw(shift + 2) << ""
                                     << "#0" << std::endl;
                           std::cout << std::setw(shift + 2) << ""
                                     << "_no_expr" << std::endl;
                           std::cout << std::setw(shift + 2) << ""
                                     << ": _no_type" << std::endl;
                       } else {
                           print_expression(*feature.value.value(), shift + 2);
                       }
                   },
               },
        feat.feature);
}

void print_class(const cool::parser::symbol::Class& class_, int shift,
    std::string_view path) {
    std::cout << std::setw(shift) << ""
              << "#" << class_.line_number << std::endl;
    std::cout << std::setw(shift) << ""
              << "_class" << std::endl;
    shift += 2;
    std::cout << std::setw(shift) << "" << class_.type_id << std::endl;
    if (class_.inherits) {
        std::cout << std::setw(shift) << "" << class_.inherits.value()
                  << std::endl;
    } else {
        std::cout << std::setw(shift) << ""
                  << "Object" << std::endl;
    }
    std::cout << std::setw(shift) << "" << '"' << path << '"' << std::endl;
    std::cout << std::setw(shift) << "" << '(' << std::endl;
    for (const auto& feature : class_.features) {
        print_feature(feature, shift);
    }
    std::cout << std::setw(shift) << "" << ')' << std::endl;
}

void print_program(const cool::parser::symbol::Program& program,
    std::string_view path, bool printed_program) noexcept {
    if (!printed_program) {
        std::cout << "#";
        std::cout << program.classes[0].line_number << std::endl;
        std::cout << "_program" << std::endl;
    }
    int shift = 2;
    for (auto class_ : program.classes) {
        print_class(class_, shift, path);
    }
}

int main(int argc, char* argv[]) {
    bool printed_program = false;
    for (int i = 1; i != argc; ++i) {
        std::ifstream input(argv[i]);
        cool::lexer::Lexer lexer(input);
        cool::parser::Parser parser(lexer);

        const auto& program = parser.get_result();
        if (program) {
            print_program(program.value(), argv[i], printed_program);
            printed_program = true;
        } else {
            std::cerr << parser.get_error().str();
            return 1;
        }
    }
    return 0;
}
