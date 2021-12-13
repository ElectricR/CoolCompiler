#include "codegen/MIPS32/TextGen.h"

namespace cool::codegen::MIPS32 {

void TextGen::print_prologue(std::ostream& out) noexcept {
    out << std::setw(12) << "addiu" << ' ' << "$sp $sp -12\n";
    out << std::setw(12) << "sw" << ' ' << "$fp 12($sp)\n";
    out << std::setw(12) << "sw" << ' ' << "$s0 8($sp)\n";
    out << std::setw(12) << "sw" << ' ' << "$ra 4($sp)\n";
    out << std::setw(12) << "addiu" << ' ' << "$fp $sp 4\n";
    out << std::setw(12) << "move" << ' ' << "$s0 $a0\n";
    out << '\n';
}

void TextGen::print_epilogue(
    unsigned formals_size, std::ostream& out) noexcept {
    out << std::setw(12) << "lw" << ' ' << "$fp 12($sp)\n";
    out << std::setw(12) << "lw" << ' ' << "$s0 8($sp)\n";
    out << std::setw(12) << "lw" << ' ' << "$ra 4($sp)\n";
    out << std::setw(12) << "addiu" << ' ' << "$sp $sp " << 12 + formals_size
        << "\n";
    out << std::setw(12) << "jr" << ' ' << "$ra\n";
    out << '\n';
}

void TextGen::generate_method_call(
    unsigned line_number, unsigned offset, std::optional<std::string_view> cast, std::ostream& out) noexcept {
    unsigned label = label_count++;
    out << std::setw(12) << "bne" << ' ' << "$a0 $zero label" << label << '\n';
    out << std::setw(12) << "la" << ' ' << "$a0 str_const_path\n";
    out << std::setw(12) << "li" << ' ' << "$t1 " << line_number << "\n";
    out << std::setw(12) << "jal" << ' ' << "_dispatch_abort\n";
    out << "label" << label << ":\n";
    if (cast) {
        out << std::setw(12) << "la" << ' ' << "$t1 " << cast.value() << "_dispTab\n";
    } else {
        out << std::setw(12) << "lw" << ' ' << "$t1 8($a0)\n";
    }
    out << std::setw(12) << "lw" << ' ' << "$t1 " << offset << "($t1)\n";
    out << std::setw(12) << "jalr" << ' ' << "$t1\n";
    out << '\n';
}

void TextGen::generate_not(std::ostream& out) noexcept {
    unsigned label = label_count++;
    out << std::setw(12) << "lw" << ' ' << "$t1 12($a0)\n";
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const1\n";
    out << std::setw(12) << "beqz" << ' ' << "$t1 label" << label << '\n';
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const0\n";
    out << "label" << label << ":\n";
}

void TextGen::generate_isvoid(std::ostream& out) noexcept {
    unsigned label = label_count++;
    out << std::setw(12) << "move" << ' ' << "$t0 $a0\n";
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const1\n";
    out << std::setw(12) << "beqz" << ' ' << "$t0 label" << label << '\n';
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const0\n";
    out << "label" << label << ":\n";
}

void TextGen::generate_case_start(unsigned case_start_label,
    unsigned line_number, std::ostream& out) const noexcept {
    out << std::setw(12) << "bne" << ' ' << "$a0 $zero label"
        << case_start_label << "\n";
    out << std::setw(12) << "la" << ' ' << "$a0 str_const_path\n";
    out << std::setw(12) << "li" << ' ' << "$t1 " << line_number << "\n";
    out << std::setw(12) << "jal" << ' ' << "_case_abort2\n";
}

void TextGen::generate_case_check(unsigned case_next_label,
    std::pair<unsigned, unsigned> tag_range, std::ostream& out) const noexcept {
    out << std::setw(12) << "blt" << ' ' << "$t0 " << tag_range.first
        << " label" << case_next_label << "\n";
    out << std::setw(12) << "bgt" << ' ' << "$t0 " << tag_range.second
        << " label" << case_next_label << "\n";
}

void TextGen::load_self_object(std::ostream& out) noexcept {
    out << std::setw(12) << "move" << ' ' << "$a0 $s0\n";
    out << '\n';
}

void TextGen::load_field_object(unsigned offset, std::ostream& out) noexcept {
    out << std::setw(12) << "lw" << ' ' << "$a0 " << offset << "($s0)\n";
    out << '\n';
}

void TextGen::load_stack_object(unsigned offset, std::ostream& out) noexcept {
    out << std::setw(12) << "lw" << ' ' << "$a0 " << offset << "($sp)\n";
    out << '\n';
}

void TextGen::print_label(unsigned label, std::ostream& out) noexcept {
    out << "label" << label << ":\n";
}

void TextGen::generate_condition_check(
    unsigned label, std::ostream& out) noexcept {
    out << std::setw(12) << "lw" << ' ' << "$t1 12($a0)\n";
    out << std::setw(12) << "beqz" << ' ' << "$t1 label" << label << '\n';
    out << '\n';
}

void TextGen::generate_equal(unsigned label, std::ostream& out) const noexcept {
    out << std::setw(12) << "move" << ' ' << "$t2 $a0\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 4($sp)\n";
    out << std::setw(12) << "addiu" << ' ' << "$sp $sp 4\n";
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const1\n";
    out << std::setw(12) << "beq" << ' ' << "$t1 $t2 label" << label << "\n";
    out << std::setw(12) << "la" << ' ' << "$a1 bool_const0\n";
    out << std::setw(12) << "jal" << ' ' << "equality_test\n";
}

void TextGen::generate_le(unsigned label, std::ostream& out) const noexcept {
    out << std::setw(12) << "move" << ' ' << "$t2 $a0\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 4($sp)\n";
    out << std::setw(12) << "addiu" << ' ' << "$sp $sp 4\n";
    out << std::setw(12) << "lw" << ' ' << "$t2 12($t2)\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 12($t1)\n";
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const1\n";
    out << std::setw(12) << "ble" << ' ' << "$t1 $t2 label" << label << "\n";
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const0\n";
}

void TextGen::generate_less(unsigned label, std::ostream& out) const noexcept {
    out << std::setw(12) << "move" << ' ' << "$t2 $a0\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 4($sp)\n";
    out << std::setw(12) << "addiu" << ' ' << "$sp $sp 4\n";
    out << std::setw(12) << "lw" << ' ' << "$t2 12($t2)\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 12($t1)\n";
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const1\n";
    out << std::setw(12) << "blt" << ' ' << "$t1 $t2 label" << label << "\n";
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const0\n";
}

void TextGen::generate_binary_op(
    std::string_view op, std::ostream& out) const noexcept {
    out << std::setw(12) << "jal" << ' ' << "Object.copy\n";
    out << std::setw(12) << "lw" << ' ' << "$t0 4($sp)\n";
    out << std::setw(12) << "addiu" << ' ' << "$sp $sp 4\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 12($t0)\n";
    out << std::setw(12) << "lw" << ' ' << "$t2 12($a0)\n";
    out << std::setw(12) << op << ' ' << "$t1 $t1 $t2\n";
    out << std::setw(12) << "sw" << ' ' << "$t1 12($a0)\n";
    out << '\n';
}

void TextGen::generate_int_constant(int x, std::ostream& out) const noexcept {
    std::string x_str;
    if (x < 0) {
        x_str = "_" + std::to_string(-x);
    } else {
        x_str = std::to_string(x);
    }
    out << std::setw(12) << "la" << ' ' << "$a0 int_const" << x_str << "\n";
}

void TextGen::generate_bool_constant(bool x, std::ostream& out) const noexcept {
    out << std::setw(12) << "la" << ' ' << "$a0 bool_const" << x << "\n";
}

void TextGen::generate_empty_string_constant(std::ostream& out) const noexcept {
    out << std::setw(12) << "la" << ' ' << "$a0 str_const0"
        << "\n";
}

void TextGen::generate_parent_init(
    std::string_view parent, std::ostream& out) const noexcept {
    out << std::setw(12) << "jal" << ' ' << parent << "_init\n";
}

void TextGen::generate_string_constant(
    unsigned x, std::ostream& out) const noexcept {
    out << std::setw(12) << "la" << ' ' << "$a0 str_const" << x << "\n";
}

void TextGen::generate_new(
    std::string_view type_id, std::ostream& out) const noexcept {
    out << std::setw(12) << "la" << ' ' << "$a0 " << type_id << "_protObj\n";
    out << std::setw(12) << "jal" << ' ' << "Object.copy\n";
    out << std::setw(12) << "jal" << ' ' << type_id << "_init\n";
    out << '\n';
}

} // namespace cool::codegen::MIPS32
