#include "codegen/MIPS32/MiscGen.h"

#include <gtest/gtest.h>

class MiscGenTestFixture : public ::testing::Test {
protected:
    cool::codegen::MIPS32::MiscGenerator misc_data_gen;
    std::stringstream gen_result;
};

TEST_F(MiscGenTestFixture, GlobalGen) {
    misc_data_gen.generate_globals(gen_result);
    ASSERT_EQ(gen_result.str(), "\
.globl class_nameTab\n\
.globl Main_protObj\n\
.globl Int_protObj\n\
.globl String_protObj\n\
.globl bool_const0\n\
.globl bool_const1\n\
.globl _int_tag\n\
.globl _bool_tag\n\
.globl _string_tag\n\
.globl _MemMgr_INITIALIZER\n\
.globl _MemMgr_COLLECTOR\n\
.globl _MemMgr_TEST\n\
.globl heap_start\n\
.globl Main_init\n\
.globl Int_init\n\
.globl String_init\n\
.globl Bool_init\n\
.globl Main.main\n\n\
");
}

TEST_F(MiscGenTestFixture, BasicTagsGen) {
    misc_data_gen.generate_basic_tags(gen_result);
    ASSERT_EQ(gen_result.str(), "\
_int_tag:\n\
       .word 1\n\n\
_bool_tag:\n\
       .word 2\n\n\
_string_tag:\n\
       .word 3\n\n\
");
}

TEST_F(MiscGenTestFixture, HeadStartGen) {
    misc_data_gen.generate_heap_start(gen_result);
    ASSERT_EQ(gen_result.str(), "\
heap_start:\n\
       .word 0\n\n\
");
}

TEST_F(MiscGenTestFixture, GarbageGen) {
    misc_data_gen.generate_gc_stuff(gen_result);
    ASSERT_EQ(gen_result.str(), "\
_MemMgr_INITIALIZER:\n\
       .word _NoGC_Init\n\
_MemMgr_COLLECTOR:\n\
       .word _NoGC_Collect\n\
_MemMgr_TEST:\n\
       .word 0\n\n\
");
}

TEST_F(MiscGenTestFixture, ObjTabGen) {
    misc_data_gen.generate_objtab({"Object", "Int", "Bool", "String", "IO", "Main"}, gen_result);
    ASSERT_EQ(gen_result.str(), "\
class_objTab:\n\
       .word Object_protObj\n\
       .word Object_init\n\
       .word Int_protObj\n\
       .word Int_init\n\
       .word Bool_protObj\n\
       .word Bool_init\n\
       .word String_protObj\n\
       .word String_init\n\
       .word IO_protObj\n\
       .word IO_init\n\
       .word Main_protObj\n\
       .word Main_init\n\n\
");
}

TEST_F(MiscGenTestFixture, NameTabGen) {
    misc_data_gen.generate_nametab(6, gen_result);
    ASSERT_EQ(gen_result.str(), "\
class_nameTab:\n\
       .word str_const0\n\
       .word str_const1\n\
       .word str_const2\n\
       .word str_const3\n\
       .word str_const4\n\
       .word str_const5\n\n\
");
}

