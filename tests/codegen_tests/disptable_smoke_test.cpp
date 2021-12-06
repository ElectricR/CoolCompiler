#include "codegen/MIPS32/DispTableGen.h"

#include <gtest/gtest.h>

class DisptableDataGenTestFixture : public ::testing::Test {
protected:
    cool::codegen::MIPS32::DispTableDataGenerator disptable_data_gen;
    std::stringstream gen_result;
};

TEST_F(DisptableDataGenTestFixture, GenInt) {
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation("Int", {"Object", {}});
    disptable_data_gen.generate_disptables(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
Int_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenBool) {
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation("Bool", {"Object", {}});
    disptable_data_gen.generate_disptables(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
Bool_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenString) {
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation(
        "String", {"Object", {"length", "concat", "substr"}});
    disptable_data_gen.generate_disptables(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
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
");
}

TEST_F(DisptableDataGenTestFixture, GenIO) {
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation(
        "IO", {"Object", {"out_string", "out_int", "in_string", "in_int"}});
    disptable_data_gen.generate_disptables(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
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
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation("Int", {"Object", {}});
    disptable_data_gen.register_class_representation("Bool", {"Object", {}});
    disptable_data_gen.register_class_representation(
        "String", {"Object", {"length", "concat", "substr"}});
    disptable_data_gen.register_class_representation(
        "IO", {"Object", {"out_string", "out_int", "in_string", "in_int"}});
    disptable_data_gen.generate_disptables(gen_result);
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
        {"Object"},
        {"foo"},
    };
    cool::codegen::MIPS32::ClassDispTableRepresentation Main = {
        {"Foo"},
        {"main", "bar"},
    };
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation(
            "Main", std::move(Main));
    disptable_data_gen.register_class_representation(
            "Foo", std::move(Foo));
    disptable_data_gen.generate_disptables(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
Foo_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\
       .word Foo.foo\n\n\
Main_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\
       .word Foo.foo\n\
       .word Main.main\n\
       .word Main.bar\n\n\
");
}

TEST_F(DisptableDataGenTestFixture, GenMainWithOverloads) {
    cool::codegen::MIPS32::ClassDispTableRepresentation Foo = {
        {"Object"},
        {"abort", "type_name", "foo"},
    };
    cool::codegen::MIPS32::ClassDispTableRepresentation Main = {
        {"Foo"},
        {"main", "foo", "copy", "abort"},
    };
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation(
            "Main", std::move(Main));
    disptable_data_gen.register_class_representation(
            "Foo", std::move(Foo));
    disptable_data_gen.generate_disptables(gen_result);
    ASSERT_EQ(gen_result.str(), "\
Object_dispTab:\n\
       .word Object.abort\n\
       .word Object.type_name\n\
       .word Object.copy\n\n\
Foo_dispTab:\n\
       .word Foo.abort\n\
       .word Foo.type_name\n\
       .word Object.copy\n\
       .word Foo.foo\n\n\
Main_dispTab:\n\
       .word Main.abort\n\
       .word Foo.type_name\n\
       .word Main.copy\n\
       .word Main.foo\n\
       .word Main.main\n\n\
");
}
