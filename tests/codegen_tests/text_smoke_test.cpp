#include "codegen/MIPS32/ASTVisitor.h"

#include <gtest/gtest.h>

class TextGenTestFixture : public ::testing::Test {
protected:
    std::stringstream gen_result;
    cool::codegen::MIPS32::ConstantsDataGeneneror constants_data_gen;
    cool::codegen::MIPS32::ASTVisitor visitor{
        gen_result, &constants_data_gen, nullptr, nullptr, nullptr};
};

TEST_F(TextGenTestFixture, PlusExpression) {
    cool::AST::Expression expr = {cool::AST::PlusExpression{
        {std::make_shared<cool::AST::Expression>(cool::AST::IntExpression{42}),
            std::make_shared<cool::AST::Expression>(
                cool::AST::IntExpression{115})},
    }};
    std::visit(visitor, expr.value);
    ASSERT_EQ(gen_result.str(), "\
          la $a0 int_const42\n\
          sw $a0 0($sp)\n\
       addiu $sp $sp -4\n\
          la $a0 int_const115\n\
         jal Object.copy\n\
          lw $t0 4($sp)\n\
       addiu $sp $sp 4\n\
          lw $t1 12($t0)\n\
          lw $t2 12($a0)\n\
         add $t1 $t1 $t2\n\
          sw $t1 12($a0)\n\n\
");
}

TEST_F(TextGenTestFixture, MinusExpression) {
    cool::AST::Expression expr = {cool::AST::MinusExpression{
        {std::make_shared<cool::AST::Expression>(cool::AST::IntExpression{42}),
            std::make_shared<cool::AST::Expression>(
                cool::AST::IntExpression{115})},
    }};
    std::visit(visitor, expr.value);
    ASSERT_EQ(gen_result.str(), "\
          la $a0 int_const42\n\
          sw $a0 0($sp)\n\
       addiu $sp $sp -4\n\
          la $a0 int_const115\n\
         jal Object.copy\n\
          lw $t0 4($sp)\n\
       addiu $sp $sp 4\n\
          lw $t1 12($t0)\n\
          lw $t2 12($a0)\n\
         sub $t1 $t1 $t2\n\
          sw $t1 12($a0)\n\n\
");
}

TEST_F(TextGenTestFixture, MultiplyExpression) {
    cool::AST::Expression expr = {cool::AST::MultiplyExpression{
        {std::make_shared<cool::AST::Expression>(cool::AST::IntExpression{42}),
            std::make_shared<cool::AST::Expression>(
                cool::AST::IntExpression{115})},
    }};
    std::visit(visitor, expr.value);
    ASSERT_EQ(gen_result.str(), "\
          la $a0 int_const42\n\
          sw $a0 0($sp)\n\
       addiu $sp $sp -4\n\
          la $a0 int_const115\n\
         jal Object.copy\n\
          lw $t0 4($sp)\n\
       addiu $sp $sp 4\n\
          lw $t1 12($t0)\n\
          lw $t2 12($a0)\n\
         mul $t1 $t1 $t2\n\
          sw $t1 12($a0)\n\n\
");
}

TEST_F(TextGenTestFixture, DivideExpression) {
    cool::AST::Expression expr = {cool::AST::DivideExpression{
        {std::make_shared<cool::AST::Expression>(cool::AST::IntExpression{42}),
            std::make_shared<cool::AST::Expression>(
                cool::AST::IntExpression{115})},
    }};
    std::visit(visitor, expr.value);
    ASSERT_EQ(gen_result.str(), "\
          la $a0 int_const42\n\
          sw $a0 0($sp)\n\
       addiu $sp $sp -4\n\
          la $a0 int_const115\n\
         jal Object.copy\n\
          lw $t0 4($sp)\n\
       addiu $sp $sp 4\n\
          lw $t1 12($t0)\n\
          lw $t2 12($a0)\n\
         div $t1 $t1 $t2\n\
          sw $t1 12($a0)\n\n\
");
}

TEST_F(TextGenTestFixture, MethodFeature) {
    cool::AST::Class class_ = {.type_id = "Foo",
        .inherits = {},
        .features = {{cool::AST::MethodFeature{.object_id = "foo",
            .formals = {},
            .type_id = "Object",
            .value = std::make_shared<
                cool::AST::Expression>(cool::AST::PlusExpression{
                {
                    .left_expression = std::make_shared<cool::AST::Expression>(
                        cool::AST::IntExpression{42}),
                    .right_expression = std::make_shared<cool::AST::Expression>(
                        cool::AST::IntExpression{115}),
                },
            })}}}};
    visitor(class_);
    ASSERT_EQ(gen_result.str(), "\
Foo.foo:\n\
       addiu $sp $sp -12\n\
          sw $fp 12($sp)\n\
          sw $s0 8($sp)\n\
          sw $ra 4($sp)\n\
       addiu $fp $sp 4\n\
        move $s0 $a0\n\n\
          la $a0 int_const42\n\
          sw $a0 0($sp)\n\
       addiu $sp $sp -4\n\
          la $a0 int_const115\n\
         jal Object.copy\n\
          lw $t0 4($sp)\n\
       addiu $sp $sp 4\n\
          lw $t1 12($t0)\n\
          lw $t2 12($a0)\n\
         add $t1 $t1 $t2\n\
          sw $t1 12($a0)\n\n\
          lw $fp 12($sp)\n\
          lw $s0 8($sp)\n\
          lw $ra 4($sp)\n\
       addiu $sp $sp 12\n\
          jr $ra\n\n\
");
}
