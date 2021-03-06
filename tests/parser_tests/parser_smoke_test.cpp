#include "AST/AST.h"
#include "parser/Parser.h"

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <string>

TEST(ParserSmokeTest, Empty) {
    std::stringstream test_string{""};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    ASSERT_FALSE(parser.get_result().has_value());
}

TEST(ParserSmokeTest, BasicEmptyClass) {
    std::stringstream test_string{"class Main {};"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{"Main", {}, {}}}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, BasicInheritedEmptyClass) {
    std::stringstream test_string{"class Main inherits IO {};"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{"Main", {"IO"}, {}}}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, MemberFunctionTest) {
    std::stringstream test_string{"class Main { get() : Int { 0 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{

        "Main",
        {},
        {{cool::AST::MethodFeature{
            "get",
            {},
            "Int",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{{cool::AST::IntExpression{0}}}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, MemberFunctionWithParametersTest) {
    std::stringstream test_string{"class Main { set(x : Int) : Int { 0 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "set",
            {{"x", "Int"}},
            "Int",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{{cool::AST::IntExpression{0}}}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, FieldTest) {
    std::stringstream test_string{"class Main { x : Int <- 0; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::FieldFeature{
            "x",
            "Int",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{{cool::AST::IntExpression{0}}}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, ObjectIDExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { a }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::ObjectExpression{"a"},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}
TEST(ParserSmokeTest, IntExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 42 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::IntExpression{42},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, StringExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { \"42bar\" }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::StringExpression{"42bar"},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, TrueExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { true }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::TrueExpression{{true}},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, FalseExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { false }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::FalseExpression{{false}},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, BracketExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { (42) }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::IntExpression{42},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, IfTest) {
    std::stringstream test_string{
        "class Main { foo() : Int { if 1 then 2 else 7 fi }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Int",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{{cool::AST::IfExpression{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{1}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{7}})},
                }}}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, WhileTest) {
    std::stringstream test_string{
        "class Main { foo() : Int { while 2 loop \"bar\" pool }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Int",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{{cool::AST::WhileExpression{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{
                            cool::AST::StringExpression{"bar"}})},
                }}}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, BracesExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { {\"bar\"; 42; } }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::CompoundExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{
                            cool::AST::StringExpression{"bar"}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{42}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, CaseExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { case 42 of a : Int => 1; b : String => "
        "\"bar\"; esac }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{
                {cool::AST::CaseExpression{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{42}})},
                    {
                        {
                            {"a"},
                            {"Int"},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{1}})},
                        },
                        {
                            {"b"},
                            {"String"},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::StringExpression{"bar"}})},
                        },
                    },
                }},
            }),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, DotExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { 2@String.len(42, 1) }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{
                {cool::AST::DotExpression{
                    {{std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})}},
                    {"String"},
                    {"len"},
                    {
                        {std::make_shared<cool::AST::Expression>(
                            cool::AST::Expression{
                                cool::AST::IntExpression{42}})},
                        {std::make_shared<cool::AST::Expression>(
                            cool::AST::Expression{
                                cool::AST::IntExpression{1}})},
                    },
                }},
            }),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, DotMultipleExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { 2.toStr().len(42, 1) }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(
                cool::AST::Expression{cool::AST::DotExpression{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::DotExpression{
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})},
                            {},
                            {"toStr"},
                            {},
                        }})},
                    {},
                    {"len"},
                    {
                        {std::make_shared<cool::AST::Expression>(
                            cool::AST::Expression{
                                cool::AST::IntExpression{42}})},
                        {std::make_shared<cool::AST::Expression>(
                            cool::AST::Expression{
                                cool::AST::IntExpression{1}})},
                    },
                }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, PlusSimpleExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 2 + 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::PlusExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, MinusSimpleExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 2 - 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::MinusExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, MultiplySimpleExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 2 * 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::MultiplyExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, DivideSimpleExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 2 / 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::DivideExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, ComplexMathExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { 2 * 2 + 1 / 3 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::PlusExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::MultiplyExpression{{
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})},
                        }}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::DivideExpression{{
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{1}})},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{3}})},
                        }}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, LessSimpleExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 2 < 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::LessExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, LESimpleExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 2 <= 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::LEExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, EqualSimpleExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { 2 = 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::EqualExpression{{
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
                }},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, SimpleTildeExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { ~2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::TildeExpression{
                    std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, DoubleTildeExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { ~~2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::TildeExpression{
                    std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::TildeExpression{
                            std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})}})},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, SimpleIsVoidExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { isvoid 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::IsVoidExpression{
                    std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, SimpleNotExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { not 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::NotExpression{
                    std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::IntExpression{2}})},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, SimpleNewExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { new A }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::NewExpression{"A"},
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, SimpleLetExpressionTest) {
    std::stringstream test_string{"class Main { foo() : Object { let a : Int "
                                  "<- 2, b : Int <- 1 in 2 + 2 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::LetExpression{
                    {
                        {"a", "Int",
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})}},
                        {"b", "Int",
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{1}})}},
                    },
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::PlusExpression{{
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})},
                        }}})},
                },
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}

TEST(ParserSmokeTest, SimpleAssignExpressionTest) {
    std::stringstream test_string{
        "class Main { foo() : Object { a <- 2 + 2}; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::AST::Program correct = {{{
        "Main",
        {},
        {{cool::AST::MethodFeature{
            "foo",
            {},
            "Object",
            std::make_shared<cool::AST::Expression>(cool::AST::Expression{{
                cool::AST::AssignExpression{
                    {"a"},
                    {std::make_shared<cool::AST::Expression>(
                        cool::AST::Expression{cool::AST::PlusExpression{{
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})},
                            {std::make_shared<cool::AST::Expression>(
                                cool::AST::Expression{
                                    cool::AST::IntExpression{2}})},
                        }}})},
                },
            }}),
        }}},
    }}};

    ASSERT_EQ(parser.get_result().value(), correct);
}
