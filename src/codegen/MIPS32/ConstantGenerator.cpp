#include "codegen/MIPS32/ConstantGenerator.h"
#include <iomanip>
#include <algorithm>
#include <array>

namespace cool::codegen::MIPS32 {

void ConstantsDataGeneneror::generate_constants(std::ostream& out) const noexcept {
    generate_ints(out);
    generate_bools(out);
}

void ConstantsDataGeneneror::generate_ints(std::ostream& out) const noexcept {
    for (int i : ints) {
        auto str = std::to_string(i);
        std::replace(str.begin(), str.end(), '-', '_');
        out << std::setw(12) << ".word" << ' ' << -1 << '\n';
        out << "int_const" << str << ":\n";
        out << std::setw(12) << ".word" << ' ' << 1 << '\n';
        out << std::setw(12) << ".word" << ' ' << 4 << '\n';
        out << std::setw(12) << ".word" << ' ' << "Int_dispTab" << '\n';
        out << std::setw(12) << ".word" << ' ' << i << '\n' << '\n';
    }
}

void ConstantsDataGeneneror::generate_bools(std::ostream& out) const noexcept {
    for (int i : std::array{false, true}) {
        out << std::setw(12) << ".word" << ' ' << -1 << '\n';
        out << "bool_const" << i << ":\n";
        out << std::setw(12) << ".word" << ' ' << 2 << '\n';
        out << std::setw(12) << ".word" << ' ' << 4 << '\n';
        out << std::setw(12) << ".word" << ' ' << "Bool_dispTab" << '\n';
        out << std::setw(12) << ".word" << ' ' << i << '\n' << '\n';
    }
}

} // namespace
