#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "codegen/Codegen.h"

class CodegenTestFixture : public ::testing::Test {
protected:
    std::stringstream gen_result;
    std::stringstream answer;

    void read_answer(std::filesystem::path path) {
        std::ifstream in{path};
        answer << in.rdbuf();
    }
};

TEST_F(CodegenTestFixture, NewExpression) {
    cool::AST::Program program{{{
        .type_id = "Main",
        .inherits = {},
        .features =
            {
                {
                    cool::AST::MethodFeature{
                        .object_id = "main",
                        .formals = {},
                        .type_id = {},
                        .value = std::make_shared<cool::AST::Expression>(
                            cool::AST::NewExpression{.type_id = "IO"}),
                    },
                },
            },
    }}};

    read_answer("./tests/codegen_tests/examples/new.s");
    cool::codegen::Codegen codegen(program, gen_result);
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, HelloWorld) {
    cool::AST::Program
        program{
            {
                {
                    .type_id = "Main",
                    .inherits = {},
                    .features =
                        {
                            {cool::AST::MethodFeature{
                                .object_id = "main",
                                .formals = {},
                                .type_id = {},
                                .value =
                                    std::make_shared<cool::AST::Expression>(
                                        cool::AST::DotExpression{
                                            .expression = std::make_shared<
                                                cool::AST::Expression>(
                                                cool::AST::NewExpression{
                                                    .type_id = "IO"},
                                                "IO"),
                                            .type_id = {},
                                            .object_id = "out_string",
                                            .parameter_expressions =
                                                {
                                                    std::make_shared<cool::AST::
                                                            Expression>(cool::
                                                            AST::StringExpression{.value =
                                                                                      "Hello, World!\n"}),
                                                },
                                        }),
                            }},
                        },
                },
            }};

    read_answer("./tests/codegen_tests/examples/hello_world.s");
    cool::codegen::Codegen codegen(program, gen_result);
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SumOut) {
    cool::AST::Program
        program{
            {
                {
                    .type_id = "Main",
                    .inherits = {},
                    .features =
                        {
                            {cool::AST::MethodFeature{.object_id = "main",
                                .formals = {},
                                .type_id = {},
                                .value =
                                    std::make_shared<cool::AST::Expression>(
                                        cool::AST::DotExpression{
                                            .expression =
                                                std::make_shared<
                                                    cool::AST::Expression>(
                                                    cool::AST::DotExpression{
                                                        .expression =
                                                            std::make_shared<
                                                                cool::AST::
                                                                    Expression>(cool::AST::NewExpression{.type_id = "IO"},
                                                                "IO"),
                                                        .type_id = {},
                                                        .object_id =
                                                            "out_int",
                                                        .parameter_expressions =
                                                            {
                                                                std::make_shared<
                                                                    cool::
                                                                        AST::
                                                                            Expression>(cool::AST::PlusExpression{{.left_expression = std::make_shared<cool::AST::Expression>(cool::AST::IntExpression{.value = 41}), 
                                                                                    .right_expression = std::make_shared<cool::AST::Expression>(cool::AST::IntExpression{.value = 1}),}}),
                                                            },
                                                    }, "IO"),
                                            .type_id = {},
                                            .object_id = "out_string",
                                            .parameter_expressions =
                                                {
                                                    std::make_shared<cool::
                                                            AST::Expression>(cool::
                                                            AST::StringExpression{.value =
                                                                                      "\n"})},
                                        })}},
                        },
                },
            }};

    read_answer("./tests/codegen_tests/examples/sum_out.s");
    cool::codegen::Codegen codegen(program, gen_result);
    ASSERT_EQ(gen_result.str(), answer.str());
}
