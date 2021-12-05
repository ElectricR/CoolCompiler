#pragma once
#include <iomanip>
#include <sstream>

namespace cool::codegen::MIPS32 {

class TextGen {
public:
    void print_prologue(std::ostream& out) noexcept;

    void print_epilogue(std::ostream& out) noexcept;

    void generate_binary_add(std::ostream& out) const noexcept {
        generate_binary_op("add", out);
    }

    void save_to_stack(std::ostream& out) const noexcept {
        out << std::setw(12) << "sw" << ' ' << "$a0 0($sp)\n";
        out << std::setw(12) << "addiu" << ' ' << "$sp $sp -4\n";
    }

    void generate_binary_sub(std::ostream& out) const noexcept {
        generate_binary_op("sub", out);
    }

    void generate_empty_init(
        std::string_view class_name, std::ostream& out) const noexcept {
        out << class_name << "_init:\n";
        out << std::setw(12) << "jr" << ' ' << "$ra\n\n";
    }

    void generate_binary_mul(std::ostream& out) const noexcept {
        generate_binary_op("mul", out);
    }

    void generate_binary_div(std::ostream& out) const noexcept {
        generate_binary_op("div", out);
    }

    void generate_int_constant(int x, std::ostream& out) const noexcept;

    void generate_method_call(
        unsigned line_number, unsigned offset, std::ostream& out) noexcept;

    void generate_string_constant(unsigned x, std::ostream& out) const noexcept;

    void generate_new(
        std::string_view type_id, std::ostream& out) const noexcept;

private:
    //
    // Takes two pointers - one on top of stack and one in $a0.
    // Returns result in $a0 and pops stack.
    //
    void generate_binary_op(
        std::string_view op, std::ostream& out) const noexcept;

    unsigned label_count = 0;
};

} // namespace cool::codegen::MIPS32
