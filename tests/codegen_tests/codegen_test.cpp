#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "codegen/Codegen.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semant/Semant.h"

class CodegenTestFixture : public ::testing::Test {
protected:
    std::stringstream gen_result;
    std::stringstream answer;

    auto parse(std::filesystem::path path) {
        std::ifstream in(path);
        cool::lexer::Lexer lexer(in);
        cool::parser::Parser parser(lexer);
        cool::semant::Semant semant({parser.get_result().value()});
        cool::codegen::Codegen codegen(semant.get_result(), gen_result);
    }

    void read_answer(std::filesystem::path path) {
        std::ifstream in{path};
        answer << in.rdbuf();
    }
};

TEST_F(CodegenTestFixture, NewExpression) {
    parse("./tests/codegen_tests/examples/new.cl");
    read_answer("./tests/codegen_tests/examples/new.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SumOut) {
    parse("./tests/codegen_tests/examples/sum_out.cl");
    read_answer("./tests/codegen_tests/examples/sum_out.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleCompoundStatement) {
    parse("./tests/codegen_tests/examples/simple_compound_statement.cl");
    read_answer("./tests/codegen_tests/examples/simple_compound_statement.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, HelloWorld) {
    parse("./tests/codegen_tests/examples/hello_world.cl");
    read_answer("./tests/codegen_tests/examples/hello_world.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, HelloWorldWithInheritance) {
    parse("./tests/codegen_tests/examples/hello_world_with_inheritance.cl");
    read_answer("./tests/codegen_tests/examples/hello_world_with_inheritance.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, Conversation) {
    parse("./tests/codegen_tests/examples/conversation.cl");
    read_answer("./tests/codegen_tests/examples/conversation.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleIf) {
    parse("./tests/codegen_tests/examples/simple_if.cl");
    read_answer("./tests/codegen_tests/examples/simple_if.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, NestedIf) {
    parse("./tests/codegen_tests/examples/nested_if.cl");
    read_answer("./tests/codegen_tests/examples/nested_if.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleNot) {
    parse("./tests/codegen_tests/examples/simple_not.cl");
    read_answer("./tests/codegen_tests/examples/simple_not.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, NestedNot) {
    parse("./tests/codegen_tests/examples/nested_not.cl");
    read_answer("./tests/codegen_tests/examples/nested_not.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleInit) {
    parse("./tests/codegen_tests/examples/simple_init.cl");
    read_answer("./tests/codegen_tests/examples/simple_init.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, DependentInit) {
    parse("./tests/codegen_tests/examples/dependent_init.cl");
    read_answer("./tests/codegen_tests/examples/dependent_init.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, InheritedInit) {
    parse("./tests/codegen_tests/examples/inherited_init.cl");
    read_answer("./tests/codegen_tests/examples/inherited_init.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleEqualStrings) {
    parse("./tests/codegen_tests/examples/simple_equal_strings.cl");
    read_answer("./tests/codegen_tests/examples/simple_equal_strings.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, EqualWithIf) {
    parse("./tests/codegen_tests/examples/equal_with_if.cl");
    read_answer("./tests/codegen_tests/examples/equal_with_if.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, InfiniteLoop) {
    parse("./tests/codegen_tests/examples/infinite_loop.cl");
    read_answer("./tests/codegen_tests/examples/infinite_loop.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, ComplexLoop) {
    parse("./tests/codegen_tests/examples/complex_loop.cl");
    read_answer("./tests/codegen_tests/examples/complex_loop.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleLetInt) {
    parse("./tests/codegen_tests/examples/simple_let_int.cl");
    read_answer("./tests/codegen_tests/examples/simple_let_int.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleLetBool) {
    parse("./tests/codegen_tests/examples/simple_let_bool.cl");
    read_answer("./tests/codegen_tests/examples/simple_let_bool.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleLetStr) {
    parse("./tests/codegen_tests/examples/simple_let_str.cl");
    read_answer("./tests/codegen_tests/examples/simple_let_str.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleLetIo) {
    parse("./tests/codegen_tests/examples/simple_let_io.cl");
    read_answer("./tests/codegen_tests/examples/simple_let_io.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, LetWithShadowing) {
    parse("./tests/codegen_tests/examples/let_with_shadowing.cl");
    read_answer("./tests/codegen_tests/examples/let_with_shadowing.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, HelloWorldWithLet) {
    parse("./tests/codegen_tests/examples/hello_world_with_let.cl");
    read_answer("./tests/codegen_tests/examples/hello_world_with_let.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, MultipleLets) {
    parse("./tests/codegen_tests/examples/multiple_lets.cl");
    read_answer("./tests/codegen_tests/examples/multiple_lets.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, MultipleLetsWithField) {
    parse("./tests/codegen_tests/examples/multiple_lets_with_field.cl");
    read_answer("./tests/codegen_tests/examples/multiple_lets_with_field.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, LetWithAssigns) {
    parse("./tests/codegen_tests/examples/let_with_assigns.cl");
    read_answer("./tests/codegen_tests/examples/let_with_assigns.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleAttribute) {
    parse("./tests/codegen_tests/examples/simple_attribute.cl");
    read_answer("./tests/codegen_tests/examples/simple_attribute.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, AttributeUsage) {
    parse("./tests/codegen_tests/examples/attribute_usage.cl");
    read_answer("./tests/codegen_tests/examples/attribute_usage.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, MultipleAttributes) {
    parse("./tests/codegen_tests/examples/multiple_attributes.cl");
    read_answer("./tests/codegen_tests/examples/multiple_attributes.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, AttributeFieldAndLet) {
    parse("./tests/codegen_tests/examples/attribute_field_and_let.cl");
    read_answer("./tests/codegen_tests/examples/attribute_field_and_let.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, AttributeFieldAndLetButSame) {
    parse("./tests/codegen_tests/examples/attribute_field_and_let_but_same.cl");
    read_answer("./tests/codegen_tests/examples/attribute_field_and_let_but_same.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, BadFizzBuzz) {
    parse("./tests/codegen_tests/examples/bad_fizz_buzz.cl");
    read_answer("./tests/codegen_tests/examples/bad_fizz_buzz.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleLE) {
    parse("./tests/codegen_tests/examples/simple_le.cl");
    read_answer("./tests/codegen_tests/examples/simple_le.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, AdvancedLE) {
    parse("./tests/codegen_tests/examples/advanced_le.cl");
    read_answer("./tests/codegen_tests/examples/advanced_le.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, AdvancedLess) {
    parse("./tests/codegen_tests/examples/advanced_less.cl");
    read_answer("./tests/codegen_tests/examples/advanced_less.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleNegate) {
    parse("./tests/codegen_tests/examples/simple_negate.cl");
    read_answer("./tests/codegen_tests/examples/simple_negate.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleIsVoid) {
    parse("./tests/codegen_tests/examples/simple_isvoid.cl");
    read_answer("./tests/codegen_tests/examples/simple_isvoid.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}

TEST_F(CodegenTestFixture, SimpleNullIsVoid) {
    parse("./tests/codegen_tests/examples/simple_null_isvoid.cl");
    read_answer("./tests/codegen_tests/examples/simple_null_isvoid.s");
    ASSERT_EQ(gen_result.str(), answer.str());
}
