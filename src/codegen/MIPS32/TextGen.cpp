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

void TextGen::print_epilogue(std::ostream& out) noexcept {
    out << std::setw(12) << "lw" << ' ' << "$fp 12($sp)\n";
    out << std::setw(12) << "lw" << ' ' << "$s0 8($sp)\n";
    out << std::setw(12) << "lw" << ' ' << "$ra 4($sp)\n";
    out << std::setw(12) << "addiu" << ' ' << "$sp $sp 12\n";
    out << std::setw(12) << "jr" << ' ' << "$ra\n";
    out << '\n';
}

void TextGen::generate_method_call(
    unsigned line_number, unsigned offset, std::ostream& out) noexcept {
    out << std::setw(12) << "bne" << ' ' << "$a0 $zero label" << ++label_count
        << "\n";
    out << std::setw(12) << "la" << ' ' << "$a0 str_const_path\n";
    out << std::setw(12) << "li" << ' ' << "$t1 " << line_number << "\n";
    out << std::setw(12) << "jal" << ' ' << "_dispatch_abort\n";
    out << "label" << label_count << ":\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 8($a0)\n";
    out << std::setw(12) << "lw" << ' ' << "$t1 " << offset << "($t1)\n";
    out << std::setw(12) << "jalr" << ' ' << "$t1\n";
    out << '\n';
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
