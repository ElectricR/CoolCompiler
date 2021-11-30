#include "codegen/MIPS32/PrototypeGen.h"

#include <gtest/gtest.h>

class PrototypeDataGenTestFixture : public ::testing::Test {
protected:
    cool::codegen::MIPS32::PrototypeDataGenerator prototype_data_gen;
    std::stringstream gen_result;
};

TEST_F(PrototypeDataGenTestFixture, GenObject) {
    prototype_data_gen.generate_object_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Object_protObj:\n\
       .word 0\n\
       .word 3\n\
       .word Object_dispTab\n\n\
");
}

TEST_F(PrototypeDataGenTestFixture, GenInt) {
    prototype_data_gen.generate_int_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Int_protObj:\n\
       .word 0\n\
       .word 4\n\
       .word Int_dispTab\n\
       .word 0\n\n\
");
}

TEST_F(PrototypeDataGenTestFixture, GenBool) {
    prototype_data_gen.generate_bool_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
Bool_protObj:\n\
       .word 0\n\
       .word 4\n\
       .word Bool_dispTab\n\
       .word 0\n\n\
");
}

TEST_F(PrototypeDataGenTestFixture, GenString) {
    prototype_data_gen.generate_string_prototype(gen_result);
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

TEST_F(PrototypeDataGenTestFixture, GenIO) {
    prototype_data_gen.generate_io_prototype(gen_result);
    ASSERT_EQ(gen_result.str(), "\
       .word -1\n\
IO_protObj:\n\
       .word 0\n\
       .word 3\n\
       .word IO_dispTab\n\n\
");
}

TEST_F(PrototypeDataGenTestFixture, GenWholeFamily) {
    prototype_data_gen.generate_object_prototype(gen_result);
    prototype_data_gen.generate_int_prototype(gen_result);
    prototype_data_gen.generate_bool_prototype(gen_result);
    prototype_data_gen.generate_string_prototype(gen_result);
    prototype_data_gen.generate_io_prototype(gen_result);
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

TEST_F(PrototypeDataGenTestFixture, GenMainWithFamily) {
    cool::codegen::MIPS32::ClassPrototypeRepresentation Foo = {
        {},
        {"Int", "IO"},
    };
    cool::codegen::MIPS32::ClassPrototypeRepresentation Main = {
        {"Foo"},
        {"Bool", "String", "Object", "Foo"},
    };
    std::unordered_map<std::string_view, cool::codegen::MIPS32::ClassPrototypeRepresentation> class_map = {{"Foo", Foo}, {"Main", Main}};
    prototype_data_gen.generate_prototype(class_map, "Main", gen_result);
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

