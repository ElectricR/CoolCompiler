#include "codegen/MIPS32/DispTableGen.h"

#include <gtest/gtest.h>

class DisptableDataGenTestFixture : public ::testing::Test {
protected:
    cool::codegen::MIPS32::DispTableDataGenerator disptable_data_gen;
    std::stringstream gen_result;
};

TEST_F(DisptableDataGenTestFixture, GenObject) {
    disptable_data_gen.generate_object_disptable(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenInt) {
    disptable_data_gen.generate_int_disptable(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Int_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenBool) {
    disptable_data_gen.generate_bool_disptable(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Bool_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenString) {
    disptable_data_gen.generate_string_disptable(gen_result);
    ASSERT_EQ(gen_result.str(), "\
String_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\
       .word String.length\n\
       .word String.concat\n\
       .word String.substr\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenIO) {
    disptable_data_gen.generate_io_disptable(gen_result);
    ASSERT_EQ(gen_result.str(), "\
IO_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\
       .word IO.out_string\n\
       .word IO.out_int\n\
       .word IO.in_string\n\
       .word IO.in_int\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenWholeFamily) {
    disptable_data_gen.generate_object_disptable(gen_result);
    disptable_data_gen.generate_int_disptable(gen_result);
    disptable_data_gen.generate_bool_disptable(gen_result);
    disptable_data_gen.generate_string_disptable(gen_result);
    disptable_data_gen.generate_io_disptable(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
Int_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
Bool_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
String_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\
       .word String.length\n\
       .word String.concat\n\
       .word String.substr\n\n\
IO_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\
       .word IO.out_string\n\
       .word IO.out_int\n\
       .word IO.in_string\n\
       .word IO.in_int\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenMain) {
    cool::codegen::MIPS32::ClassDispTableRepresentation Foo = {
        {},
        {"foo"},
    };
    cool::codegen::MIPS32::ClassDispTableRepresentation Main = {
        {"Foo"},
        {"main", "bar"},
    };
    std::unordered_map<std::string_view, cool::codegen::MIPS32::ClassDispTableRepresentation> class_map = {{"Foo", Foo}, {"Main", Main}};
    disptable_data_gen.generate_disptable(class_map, "Main", gen_result);
    ASSERT_EQ(gen_result.str(), "\
Main_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\
       .word Foo.foo\n\
       .word Main.main\n\
       .word Main.bar\n\n\
");
}

