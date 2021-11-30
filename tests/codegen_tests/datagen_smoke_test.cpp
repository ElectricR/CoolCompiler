#include "codegen/MIPS32/PrototypeGen.h"

#include <gtest/gtest.h>

class DataGenTestFixture : public ::testing::Test {
protected:
    cool::codegen::MIPS32::PrototypeDataGenerator data_gen;
    std::stringstream gen_result;
};

TEST_F(DataGenTestFixture, GenObject) {
    data_gen.generate_object_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Object_protObj:\n\
       .word 0\n\
       .word 3\n\
       .word Object_dispTab\n\n\
");
}

TEST_F(DataGenTestFixture, GenInt) {
    data_gen.generate_int_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Int_protObj:\n\
       .word 0\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 0\n\n\
");
}

TEST_F(DataGenTestFixture, GenBool) {
    data_gen.generate_bool_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Bool_protObj:\n\
       .word 0\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 0\n\n\
");
}

TEST_F(DataGenTestFixture, GenString) {
    data_gen.generate_string_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
String_protObj:\n\
       .word 0\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .word 0\n\n\
");
}

TEST_F(DataGenTestFixture, GenIO) {
    data_gen.generate_io_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
IO_protObj:\n\
       .word 0\n\
       .word 3\n\
       .word IO_dispTab\n\n\
");
}

TEST_F(DataGenTestFixture, GenWholeFamily) {
    data_gen.generate_object_prototype(gen_result);
    data_gen.generate_int_prototype(gen_result);
    data_gen.generate_bool_prototype(gen_result);
    data_gen.generate_string_prototype(gen_result);
    data_gen.generate_io_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Object_protObj:\n\
       .word 0\n\
       .word 3\n\
       .word Object_dispTab\n\n\
       .word -1\n\
Int_protObj:\n\
       .word 1\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 0\n\n\
       .word -1\n\
Bool_protObj:\n\
       .word 2\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 0\n\n\
       .word -1\n\
String_protObj:\n\
       .word 3\n\
       .word 5\n\
       .word String_dispTab\n\
       .word int_const0\n\
       .word 0\n\n\
       .word -1\n\
IO_protObj:\n\
       .word 4\n\
       .word 3\n\
       .word IO_dispTab\n\n\
");
}

TEST_F(DataGenTestFixture, GenMainWithFamily) {
    cool::codegen::MIPS32::ClassPrototypeRepresentation Foo = {
        {},
        {"Int", "IO"},
    };
    cool::codegen::MIPS32::ClassPrototypeRepresentation Main = {
        {"Foo"},
        {"Bool", "String", "Object", "Foo"},
    };
    std::unordered_map<std::string_view, cool::codegen::MIPS32::ClassPrototypeRepresentation> class_map = {{"Foo", Foo}, {"Main", Main}};
    data_gen.generate_prototype(class_map, "Main", gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Main_protObj:\n\
       .word 0\n\
       .word 9\n\
       .word Main_dispTab\n\
       .word int_const0\n\
       .word 0\n\
       .word bool_const0\n\
       .word str_const0\n\
       .word 0\n\
       .word 0\n\n\
");
}

