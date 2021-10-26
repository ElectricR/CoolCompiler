#pragma once

#include <memory>
#include <ranges>
#include <string>
#include <variant>
#include <vector>

namespace cool::AST {

struct Expression;

struct IntExpression {
    int32_t value;
    unsigned line_number = 0;

    bool operator==(const IntExpression& other) const {
        return value == other.value;
    }
};

struct ObjectExpression {
    std::string object_id;
    unsigned line_number = 0;

    bool operator==(const ObjectExpression& other) const {
        return object_id == other.object_id;
    }
};

struct StringExpression {
    std::string value;
    unsigned line_number = 0;

    bool operator==(const StringExpression& other) const {
        return value == other.value;
    }
};

struct BoolExpression {
    bool value;
    unsigned line_number = 0;

    bool operator==(const BoolExpression& other) const {
        return value == other.value;
    }
};

struct TrueExpression : public BoolExpression {};

struct FalseExpression : public BoolExpression {};

struct BinaryExpression {
    std::shared_ptr<Expression> left_expression;
    std::shared_ptr<Expression> right_expression;
    unsigned line_number = 0;

    bool operator==(const BinaryExpression& other) const;
};

struct PlusExpression : public BinaryExpression {};

struct MinusExpression : public BinaryExpression {};

struct MultiplyExpression : public BinaryExpression {};

struct DivideExpression : public BinaryExpression {};

struct LessExpression : public BinaryExpression {};

struct LEExpression : public BinaryExpression {};

struct EqualExpression : public BinaryExpression {};

struct DotExpression {
    std::shared_ptr<Expression> expression;
    std::optional<std::string> type_id;
    std::string object_id;
    std::vector<std::shared_ptr<Expression>> parameter_expressions;
    unsigned line_number = 0;

    bool operator==(const DotExpression& other) const;
};

struct TildeExpression {
    std::shared_ptr<Expression> expression;
    unsigned line_number = 0;

    bool operator==(const TildeExpression& other) const;
};

struct IfExpression {
    std::shared_ptr<Expression> condition_expression;
    std::shared_ptr<Expression> if_expression;
    std::shared_ptr<Expression> else_expression;
    unsigned line_number = 0;

    bool operator==(const IfExpression& other) const;
};

struct CompoundExpression {
    std::vector<std::shared_ptr<Expression>> expressions;
    unsigned line_number = 0;

    bool operator==(const CompoundExpression& other) const;
};

struct WhileExpression {
    std::shared_ptr<Expression> condition_expression;
    std::shared_ptr<Expression> body_expression;
    unsigned line_number = 0;

    bool operator==(const WhileExpression& other) const;
};

struct CaseBranch {
    std::string object_id;
    std::string type_id;
    std::shared_ptr<Expression> expression;
    unsigned line_number = 0;

    bool operator==(const CaseBranch& other) const;
};

struct CaseExpression {
    std::shared_ptr<Expression> case_expression;
    std::vector<CaseBranch> branch_expressions;
    unsigned line_number = 0;

    bool operator==(const CaseExpression& other) const;
};

struct LetEntry {
    std::string object_id;
    std::string type_id;
    std::optional<std::shared_ptr<Expression>> assign_expression;
    unsigned line_number = 0;

    bool operator==(const LetEntry& other) const;
};

struct LetExpression {
    std::vector<LetEntry> let_expressions;
    std::shared_ptr<Expression> expression;
    unsigned line_number = 0;

    bool operator==(const LetExpression& other) const;
};

struct NewExpression {
    std::string type_id;
    unsigned line_number = 0;

    bool operator==(const NewExpression& other) const {
        return type_id == other.type_id;
    }
};

struct IsVoidExpression {
    std::shared_ptr<Expression> expression;
    unsigned line_number = 0;

    bool operator==(const IsVoidExpression& other) const;
};

struct NotExpression {
    std::shared_ptr<Expression> expression;
    unsigned line_number = 0;

    bool operator==(const NotExpression& other) const;
};

struct AssignExpression {
    std::string object_id;
    std::shared_ptr<Expression> expression;
    unsigned line_number = 0;

    bool operator==(const AssignExpression& other) const;
};

struct FunctionExpression {
    std::string object_id;
    std::vector<std::shared_ptr<Expression>> expressions;
    unsigned line_number = 0;

    bool operator==(const FunctionExpression& other) const;
};

struct Expression {
    std::variant<DotExpression, IfExpression, WhileExpression,
        CompoundExpression, CaseExpression, NewExpression, IsVoidExpression,
        PlusExpression, MinusExpression, MultiplyExpression, DivideExpression,
        LessExpression, LEExpression, EqualExpression, TildeExpression,
        ObjectExpression, IntExpression, StringExpression, TrueExpression,
        FalseExpression, NotExpression, LetExpression, AssignExpression,
        FunctionExpression>
        value;
    bool operator==(const Expression& other) const = default;
};

bool inline IsVoidExpression::operator==(const IsVoidExpression& other) const {
    return *expression == *other.expression;
}

bool inline AssignExpression::operator==(const AssignExpression& other) const {
    return *expression == *other.expression && object_id == other.object_id;
}

bool inline FunctionExpression::operator==(
    const FunctionExpression& other) const {
    return std::ranges::equal(expressions, other.expressions, {},
               &std::shared_ptr<Expression>::operator*,
               &std::shared_ptr<Expression>::operator*) &&
           object_id == other.object_id;
}

bool inline LetEntry::operator==(const LetEntry& other) const {
    return type_id == other.type_id && object_id == other.object_id &&
           assign_expression.has_value() ==
               other.assign_expression.has_value() &&
           (assign_expression.has_value() == false ||
               *assign_expression.value() == *other.assign_expression.value());
}

bool inline LetExpression::operator==(const LetExpression& other) const {
    return *expression == *other.expression &&
           let_expressions == other.let_expressions;
}

bool inline NotExpression::operator==(const NotExpression& other) const {
    return *expression == *other.expression;
}

bool inline TildeExpression::operator==(const TildeExpression& other) const {
    return *expression == *other.expression;
}

bool inline DotExpression::operator==(const DotExpression& other) const {
    return *expression == *other.expression && type_id == other.type_id &&
           object_id == other.object_id &&
           std::ranges::equal(parameter_expressions,
               other.parameter_expressions, {},
               &std::shared_ptr<Expression>::operator*,
               &std::shared_ptr<Expression>::operator*);
}

bool inline WhileExpression::operator==(const WhileExpression& other) const {
    return *body_expression == *other.body_expression &&
           *condition_expression == *other.condition_expression;
}

bool inline BinaryExpression::operator==(const BinaryExpression& other) const {
    return *left_expression == *other.left_expression &&
           *right_expression == *other.right_expression;
}
bool inline CaseBranch::operator==(const CaseBranch& other) const {
    return *expression == *other.expression && object_id == other.object_id &&
           type_id == other.type_id;
}

bool inline CaseExpression::operator==(const CaseExpression& other) const {
    return branch_expressions == other.branch_expressions &&
           *case_expression == *other.case_expression;
}

bool inline CompoundExpression::operator==(
    const CompoundExpression& other) const {
    return std::ranges::equal(expressions, other.expressions, {},
        &std::shared_ptr<Expression>::operator*,
        &std::shared_ptr<Expression>::operator*);
}

bool inline IfExpression::operator==(const IfExpression& other) const {
    return *if_expression == *other.if_expression &&
           *condition_expression == *other.condition_expression &&
           *else_expression == *other.else_expression;
}

struct Formal {
    std::string object_id;
    std::string type_id;
    unsigned line_number = 0;

    bool operator==(const Formal& other) const {
        return object_id == other.object_id && type_id == other.type_id;
    }
};

struct MethodFeature {
    std::string object_id;
    std::vector<Formal> formals;
    std::string type_id;
    std::shared_ptr<Expression> value;
    unsigned line_number = 0;

    bool operator==(const MethodFeature& other) const {
        return object_id == other.object_id && formals == other.formals &&
               type_id == other.type_id && *value == *other.value;
    }
};

struct FieldFeature {
    std::string object_id;
    std::string type_id;
    std::optional<std::shared_ptr<Expression>> value;
    unsigned line_number = 0;

    bool operator==(const FieldFeature& other) const {
        return object_id == other.object_id && type_id == other.type_id &&
               ((!value.has_value() && other.value.has_value()) ||
                   (value.has_value() &&
                       *value.value() == *other.value.value()));
    }
};

struct Feature {
    std::variant<MethodFeature, FieldFeature> feature;

    bool operator==(const Feature& other) const = default;
};

struct Class {
    std::string type_id;
    std::optional<std::string> inherits;
    std::vector<Feature> features;
    unsigned line_number = 0;

    bool operator==(const Class& other) const {
        return type_id == other.type_id && inherits == other.inherits &&
               features == other.features;
    }
};

struct Program {
    std::vector<Class> classes;

    bool operator==(const Program& other) const = default;
};

} // namespace cool::parser::symbol
