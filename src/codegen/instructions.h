#pragma once
#include <variant>
#include <string_view>

namespace cool::codegen::instructions {

struct MemoryInstruction {
    MemoryInstruction(std::string_view in_op, std::string_view in_dest, int in_offset):
        op(in_op),
        dest(in_dest),
        offset(in_offset)
    {}

    std::string_view op;
    std::string_view dest;
    int offset = 0;
};

struct LoadWord:
    public MemoryInstruction
{
    LoadWord(std::string_view in_dest, int in_offset):
        MemoryInstruction("lw", in_dest, in_offset)
    {}
};

struct SaveWord:
    public MemoryInstruction
{
    SaveWord(std::string_view in_dest, int in_offset):
        MemoryInstruction("sw", in_dest, in_offset)
    {}
};

struct ImmediateInstruction {
    ImmediateInstruction(std::string_view in_op, std::string_view in_dest, std::string_view in_operand, int in_value):
        op(in_op),
        dest(in_dest),
        operand(in_operand),
        value(in_value)
    {}

    std::string_view op;
    std::string_view dest;
    std::string_view operand;
    int value = 0;
};

struct AddImmediateUnsigned:
    public ImmediateInstruction
{
    AddImmediateUnsigned(std::string_view in_dest, std::string_view in_operand, int in_value):
        ImmediateInstruction("addiu", in_dest, in_operand, in_value)
    {}
};

struct Jump {
    Jump(int in_value):
        value(in_value)
    {}

    int value = 0;
};

struct JumpRegister {
    JumpRegister(std::string_view in_dest):
        dest(in_dest)
    {}

    std::string_view dest;
};

struct Instruction {
    std::variant<LoadWord, SaveWord, AddImmediateUnsigned, Jump, JumpRegister> instruction;
};

} //namespace
