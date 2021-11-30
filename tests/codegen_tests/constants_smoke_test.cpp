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
");
}

TEST_F(ConstantsDataGenTestFixture, SimpleGen) {
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
");
}
