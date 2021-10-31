#include "AST/AST.h"
#include "semant/Semant.h"

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <string>

TEST(SemantSmokeTest, MainCheck) {
    std::stringstream test_string{"class Main { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), true);
}

TEST(SemantSmokeTest, MainNoMainClass) {
    std::stringstream test_string{"class Foo { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Main class not found");
}

TEST(SemantSmokeTest, MainNoMainMethod) {
    std::stringstream test_string{"class Main { foo(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Method main not found");
}

TEST(SemantSmokeTest, MainIsSecond) {
    std::stringstream test_string{"class Foo { foo(): Object { 1 }; }; class Main { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), true);
}

TEST(SemantSmokeTest, MainClassRedefinition) {
    std::stringstream test_string{"class Main { main() : Object { 1 };  }; class Main { main() : Object { 1 };  };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Redefinition of class Main");
}

TEST(SemantSmokeTest, MainClassRedefinitionIsSecond) {
    std::stringstream test_string{"class Main { foo() : Object { 1 };  }; class Main { main() : Object { 1 };  };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Redefinition of class Main");
}

TEST(SemantSmokeTest, MainMethodRedefinition) {
    std::stringstream test_string{"class Main { main() : Object { 1 }; main() : Object { 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Redefinition of feature main");
}

TEST(SemantSmokeTest, InheritanceCyclic) {
    std::stringstream test_string{"class Foo inherits Bar { foo(): Object { 1 }; }; class Bar inherits Foo { bar(): Object { 1 }; }; class Main { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Cyclic dependency found in Bar");
}

TEST(SemantSmokeTest, InheritanceCyclicExtended) {
    std::stringstream test_string{"class A inherits B { foo(): Object { 1 }; }; class B inherits C { foo(): Object { 1 }; }; class C inherits D { foo(): Object { 1 }; }; class D inherits A { foo(): Object { 1 }; }; class Main { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Cyclic dependency found in B");
}

TEST(SemantSmokeTest, InheritancePrimitiveClass) {
    std::stringstream test_string{"class Main inherits Int { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Can't inherit Int");
}

TEST(SemantSmokeTest, InheritanceUnknownClass) {
    std::stringstream test_string{"class Main inherits Foo{ main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "No such type to inherit Foo");
}

TEST(SemantSmokeTest, FieldUnknownType) {
    std::stringstream test_string{"class Main { foo : Bar; main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "No such type Bar");
}

TEST(SemantSmokeTest, TypeCheckIntType) {
    std::stringstream test_string{"class Main { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::IntExpression{1}
            }, "Int"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckFieldId) {
    std::stringstream test_string{"class Main { foo : Int; main(): Object { foo }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {
            {cool::AST::FieldFeature{
                "foo",
                "Int",
                {},
            }},
            {cool::AST::MethodFeature{
                "main",
                {},
                "Object",
                std::make_shared<cool::AST::Expression>(cool::AST::Expression{
                    {cool::AST::ObjectExpression{"foo"}}, "Int"}),
            }},
        },
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckUnknownId) {
    std::stringstream test_string{"class Main { foo : Int; main(): Object { bar }; };"};

    cool::lexer::Lexer lexer(test_string);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
}

TEST(SemantSmokeTest, TypeCheckPlus) {
    std::stringstream test_string{"class Main { main() : Object { 2 + 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::PlusExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
                }},
            }, "Int"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckBadPlus) {
    std::stringstream test_string{"class Main { main() : Object { 2 + true }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Unknown operands for arithmetic expression: Int and Bool");
}

TEST(SemantSmokeTest, TypeCheckEqual) {
    std::stringstream test_string{"class Main { main() : Object { 2 = 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::EqualExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
                }},
            }, "Bool"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckBadEqualWithBase) {
    std::stringstream test_string{"class Main { main() : Object { 2 = true }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Unknown operands for compare expression: Int and Bool");
}

TEST(SemantSmokeTest, TypeCheckLess) {
    std::stringstream test_string{"class Main { main() : Object { 2 < 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::LessExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
                }},
            }, "Bool"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckBadLessWithBase) {
    std::stringstream test_string{"class Main { main() : Object { 2 < true }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Unknown operands for compare expression: Int and Bool");
}

TEST(SemantSmokeTest, TypeCheckWrongMethodReturnValue) {
    std::stringstream test_string{"class Main { main() : String { 2 + 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Can't assign Int to String");
}

TEST(SemantSmokeTest, TypeCheckCompoundStatement) {
    std::stringstream test_string{
        "class Main { main() : Object { {\"bar\"; 42; } }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::CompoundExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{
                            cool::AST::StringExpression{"bar"}, "String"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{42}, "Int"})},
                }},
            }, "Int"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckWhileLoop) {
    std::stringstream test_string{
        "class Main { main() : Object { while 2 loop \"bar\" pool }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Expected Bool expression in while statement");
}

TEST(SemantSmokeTest, TypeCheckIsVoid) {
    std::stringstream test_string{
        "class Main { main() : Bool { isvoid 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Bool",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::IsVoidExpression{
                    std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
            }, "Bool"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckNeg) {
    std::stringstream test_string{
        "class Main { main() : Object { ~2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::TildeExpression{
                    std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
            }, "Int"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckBadNeg) {
    std::stringstream test_string{
        "class Main { main() : Object { ~true }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Expected Int in ~, got Bool");
}

TEST(SemantSmokeTest, TypeCheckNot) {
    std::stringstream test_string{
        "class Main { main() : Object { not true }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::NotExpression{
                    std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::TrueExpression{{true}}, "Bool"})},
            }, "Bool"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckBadNot) {
    std::stringstream test_string{
        "class Main { main() : Object { not 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Expected Bool in \"not\", got Int");
}

TEST(SemantSmokeTest, TypeCheckAssign) {
    std::stringstream test_string{
        "class Main { foo : Int; main() : Object { foo <- 2 + 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::FieldFeature{
             "foo",
             "Int",
             {},
         }},
            {cool::AST::MethodFeature{
                "main",
                {},
                "Object",
                std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                    cool::AST::AssignExpression{
                        {"foo"},
                        {std::make_shared<cool::AST::Expression>(
                            cool::AST::Expression{cool::AST::PlusExpression{{
                                {std::make_shared<cool::AST::Expression>(
                                    cool::AST::Expression{
                                        cool::AST::IntExpression{2}, "Int"})},
                                {std::make_shared<cool::AST::Expression>(
                                    cool::AST::Expression{
                                        cool::AST::IntExpression{2}, "Int"})},
                            }}, "Int"})},
                    },
                }, "Int"}),
            }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckBadAssign) {
    std::stringstream test_string{
        "class Main { foo : Bool; main() : Object { foo <- 2 + 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Can't assign Int to Bool");
}

TEST(SemantSmokeTest, TypeCheckIf) {
    std::stringstream test_string{
        "class Main { main() : Int { if true then 2 else 7 fi }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Int",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{{cool::AST::IfExpression{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::TrueExpression{{true}}, "Bool"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}, "Int"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{7}, "Int"})},
                }}, "Int"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckIfObject) {
    std::stringstream test_string{
        "class Main { main() : Object { if true then \"foo\" else 7 fi }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {
            {cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{{cool::AST::IfExpression{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::TrueExpression{{true}}, "Bool"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::StringExpression{"foo"}, "String"})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{7}, "Int"})},
                }}, "Object"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckIfBad) {
    std::stringstream test_string{
        "class Main { main() : Object { if 2 then \"foo\" else 7 fi }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Expected Bool expression in if statement");
}

TEST(SemantSmokeTest, TypeCheckNew) {
    std::stringstream test_string{"class A {}; class Main { main() : Object { new A }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{
        {
            "A",
            {},
            {},
        },
        {
            "Main",
            {},
            {{cool::AST::MethodFeature{
                "main",
                {},
                "Object",
                std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{{
                                              cool::AST::NewExpression{"A"},
                                          },
                        "A"}),
            }}},
        },
    }};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckNewSelfType) {
    std::stringstream test_string{"class A { foo() : Object { new SELF_TYPE }; }; class Main { main() : Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{
        {
            "A",
            {},
            {{cool::AST::MethodFeature{
                "foo",
                {},
                "Object",
                std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{{
                                              cool::AST::NewExpression{"SELF_TYPE"},
                                          },
                        "SELF_TYPE"}),
            }}},
        },
        {
            "Main",
            {},
            {{cool::AST::MethodFeature{
                "main",
                {},
                "Object",
                std::make_shared<cool::AST::Expression>(
                    cool::AST::Expression{{
                                              cool::AST::IntExpression{1},
                                          },
                        "Int"}),
            }}},
        },
    }};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckNewBad) {
    std::stringstream test_string{"class Main { main() : Object { new A }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Type A not found");
}

TEST(SemantSmokeTest, TypeCheckCase) {
    std::stringstream test_string{
        "class Main { main() : Object { case 42 of a : Int => 1; b : String => \"bar\"; esac }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{
                {cool::AST::CaseExpression{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{
                            cool::AST::IntExpression{42}, "Int"})},
                    {
                        {
                            {"a"},
                            {"Int"},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{1}, "Int"})},
                        },
                        {
                            {"b"},
                            {"String"},
                            {std::make_shared<
                                cool::AST::Expression>(cool::AST::Expression{
                                cool::AST::StringExpression{"bar"}, "String"})},
                        },
                    },
                }},
                "Object"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckCaseBadBranches) {
    std::stringstream test_string{
        "class Main { main() : Object { case true of a : Int => 1; b : Int => \"bar\"; esac }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Not all branch types are distinct");
}

TEST(SemantSmokeTest, TypeCheckDotMultipleBad) {
    std::stringstream test_string{
        "class Main { main() : Object { 2.toStr() }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Could not find suitable method toStr");
}

TEST(SemantSmokeTest, TypeCheckLet) {
    std::stringstream test_string{"class Main { main() : Int { let a : Int <- 2, b : Int <- 1 in a + b }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Int",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::LetExpression{
                    {
                        {"a", "Int",
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}, "Int"})}},
                        {"b", "Int",
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{1}, "Int"})}},
                    },
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::PlusExpression{{
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::ObjectExpression{"a"}, "Int"})},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::ObjectExpression{"b"}, "Int"})},
                        }}, "Int"})},
                },
            }, "Int"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckLetDependentAssign) {
    std::stringstream test_string{"class Main { main() : Int { let a : Int <- 2, b : Int <- a in a + b }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "main",
            {},
            "Int",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::LetExpression{
                    {
                        {"a", "Int",
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}, "Int"})}},
                        {"b", "Int",
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::ObjectExpression{"a"}, "Int"})}},
                    },
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::PlusExpression{{
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::ObjectExpression{"a"}, "Int"})},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::ObjectExpression{"b"}, "Int"})},
                        }}, "Int"})},
                },
            }, "Int"}),
        }}},
    }}};

    ASSERT_EQ(semant.is_successfull(), true);
    ASSERT_EQ(semant.get_result(), correct);
}

TEST(SemantSmokeTest, TypeCheckLetBad) {
    std::stringstream test_string{"class Main { main() : Int { let a : Int <- b, b : Int <- a in a + b }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    std::vector<cool::parser::Parser> parsers;
    parsers.emplace_back(lexer);

    cool::semant::Semant semant(parsers);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "ID b not found");
}
