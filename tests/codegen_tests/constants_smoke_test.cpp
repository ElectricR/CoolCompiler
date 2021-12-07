#include "codegen/MIPS32/ConstantGenerator.h"

#include <gtest/gtest.h>

class ConstantsDataGenTestFixture : public ::testing::Test {
protected:
    cool::codegen::MIPS32::ConstantsDataGeneneror constants_data_gen;
    std::stringstream gen_result;
};

TEST_F(ConstantsDataGenTestFixture, InitialGen) {
    constants_data_gen.generate_constants(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
int_const0:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 0\n\n\
       .word -1\n\
bool_const0:\n\
       .word 2\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 0\n\n\
       .word -1\n\
bool_const1:\n\
       .word 2\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 1\n\n\
       .word -1\n\
str_const_path:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .byte 0\n\n\
       .word -1\n\
str_const0:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .byte 0\n\n\
");
}

TEST_F(ConstantsDataGenTestFixture, SimpleIntGen) {
    constants_data_gen.register_int(-1);
    constants_data_gen.register_int(12);
    constants_data_gen.generate_constants(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
int_const_1:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word -1\n\n\
       .word -1\n\
int_const0:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 0\n\n\
       .word -1\n\
int_const12:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 12\n\n\
       .word -1\n\
bool_const0:\n\
       .word 2\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 0\n\n\
       .word -1\n\
bool_const1:\n\
       .word 2\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 1\n\n\
       .word -1\n\
str_const_path:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .byte 0\n\n\
       .word -1\n\
str_const0:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .byte 0\n\n\
");
}

TEST_F(ConstantsDataGenTestFixture, SimpleStrGen) {
    constants_data_gen.register_class_name("Object");
    (void)constants_data_gen.register_string("Foo");
    constants_data_gen.generate_constants(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
int_const0:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 0\n\n\
       .word -1\n\
int_const3:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 3\n\n\
       .word -1\n\
int_const6:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 6\n\n\
       .word -1\n\
bool_const0:\n\
       .word 2\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 0\n\n\
       .word -1\n\
bool_const1:\n\
       .word 2\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 1\n\n\
       .word -1\n\
str_const_path:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .byte 0\n\n\
       .word -1\n\
str_constObject:\n\
       .word 3\n\
       .word 6\n\
       .word String_dispTab\n\
       .word int_const6\n\
      .ascii \"Object\"\n\
       .byte 0\n\n\
       .word -1\n\
str_const0:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .byte 0\n\n\
       .word -1\n\
str_const1:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const3\n\
      .ascii \"Foo\"\n\
       .byte 0\n\n\
");
}
