#pragma once
#include <iomanip>
#include <sstream>

namespace cool::codegen::MIPS32 {

class TextGen {
public:
    void print_prologue(std::ostream& out) noexcept;

    void print_epilogue(unsigned formals_size, std::ostream& out) noexcept;

    void generate_binary_add(std::ostream& out) const noexcept {
        generate_binary_op("add", out);
    }

    void save_to_stack(std::ostream& out) const noexcept {
        out << std::setw(12) << "sw" << ' ' << "$a0 0($sp)\n";
        out << std::setw(12) << "addiu" << ' ' << "$sp $sp -4\n";
    }

    void save_to_local(unsigned offset, std::ostream& out) const noexcept {
        out << std::setw(12) << "sw" << ' ' << "$a0 " << offset << "($sp)\n";
    }

    void grow_stack(unsigned size, std::ostream& out) const noexcept {
        out << std::setw(12) << "addiu" << ' ' << "$sp $sp -" << size << "\n";
    }

    void reduce_stack(unsigned size, std::ostream& out) const noexcept {
        out << std::setw(12) << "addiu" << ' ' << "$sp $sp " << size << "\n";
        out << '\n';
    }

    void save_to_field(unsigned offset, std::ostream& out) const noexcept {
        out << std::setw(12) << "sw" << ' ' << "$a0 " << offset << "($s0)\n";
        out << '\n';
    }

    void generate_loop_check(unsigned label, std::ostream& out) noexcept {
        out << std::setw(12) << "lw" << ' ' << "$t1 12($a0)\n";
        out << std::setw(12) << "beq" << ' ' << "$t1 $zero label" << label << "\n";
    }

    void generate_loop_end(unsigned start_loop_label, unsigned end_loop_label, std::ostream& out) noexcept {
        out << std::setw(12) << "b" << ' ' << "label" << start_loop_label << "\n";
        print_label(end_loop_label, out);
        out << std::setw(12) << "move" << ' ' << "$a0 $zero\n";
    }

    void generate_binary_sub(std::ostream& out) const noexcept {
        generate_binary_op("sub", out);
    }

    void generate_empty_init(
        std::string_view class_name, std::ostream& out) const noexcept {
        generate_init_label(class_name, out);
        out << std::setw(12) << "jr" << ' ' << "$ra\n\n";
    }

    void generate_init_label(std::string_view class_name, std::ostream& out) const noexcept {
        out << class_name << "_init:\n";
    }

    void generate_binary_mul(std::ostream& out) const noexcept {
        generate_binary_op("mul", out);
    }

    void generate_binary_div(std::ostream& out) const noexcept {
        generate_binary_op("div", out);
    }

    void generate_int_constant(int x, std::ostream& out) const noexcept;

    void generate_bool_constant(bool x, std::ostream& out) const noexcept;

    void generate_empty_string_constant(std::ostream& out) const noexcept;

    void generate_parent_init(std::string_view parent, std::ostream& out) const noexcept;

    void generate_method_call(
        unsigned line_number, unsigned offset, std::ostream& out) noexcept;

    void generate_string_constant(unsigned x, std::ostream& out) const noexcept;

    void generate_new(
        std::string_view type_id, std::ostream& out) const noexcept;

    [[nodiscard]] unsigned reserve_label() noexcept {
        return label_count++;
    }
    
    void print_label(unsigned label, std::ostream& out) noexcept;

    void generate_condition_check(unsigned label, std::ostream& out) noexcept;

    void generate_if_end(unsigned label, std::ostream& out) noexcept {
        out << std::setw(12) << "b" << ' ' << "label" << label << '\n';
    }

    void generate_else_end(unsigned label, std::ostream& out) noexcept {
        out << "label" << label << ":\n";
    }

    void generate_true(std::ostream& out) noexcept {
        out << std::setw(12) << "la" << ' ' << "$a0 bool_const1\n\n";
    }

    void generate_false(std::ostream& out) noexcept {
        out << std::setw(12) << "la" << ' ' << "$a0 bool_const0\n\n";
    }

    void generate_negate(std::ostream& out) noexcept {
        out << std::setw(12) << "jal" << ' ' << "Object.copy\n";
        out << std::setw(12) << "lw" << ' ' << "$t0 12($a0)\n";
        out << std::setw(12) << "neg" << ' ' << "$t0 $t0\n";
        out << std::setw(12) << "sw" << ' ' << "$t0 12($a0)\n";
    }

    void generate_zeros(std::ostream& out) noexcept {
        out << std::setw(12) << "move" << ' ' << "$a0 $zero\n\n";
    }

    void generate_not(std::ostream& out) noexcept;

    void set_current_class(std::string_view current_class) noexcept {
        current_class_name = current_class;
    }

    void load_self_object(std::ostream& out) noexcept;

    void load_field_object(unsigned offset, std::ostream& out) noexcept;

    void load_stack_object(unsigned offset, std::ostream& out) noexcept;

    void generate_equal(unsigned label, std::ostream& out) const noexcept;

    void generate_le(unsigned label, std::ostream& out) const noexcept;

    void generate_less(unsigned label, std::ostream& out) const noexcept;

    void generate_isvoid(std::ostream& out) noexcept;

    void generate_case_start(unsigned case_start_label, unsigned line_number, std::ostream& out) const noexcept;
private:
    //
    // Takes two pointers - one on top of stack and one in $a0.
    // Returns result in $a0 and pops stack.
    //
    void generate_binary_op(
        std::string_view op, std::ostream& out) const noexcept;

    unsigned label_count = 0;
    std::string_view current_class_name;
};

} // namespace cool::codegen::MIPS32
