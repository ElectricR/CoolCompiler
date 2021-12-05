#include "codegen/MIPS32/ConstantGenerator.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>

namespace cool::codegen::MIPS32 {

void ConstantsDataGeneneror::generate_constants(
    std::ostream& out) const noexcept {
    generate_ints(out);
    generate_bools(out);
    generate_strings(out);
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

void ConstantsDataGeneneror::generate_strings(
    std::ostream& out) const noexcept {
    print_single_string(filepath, "_path", out);
    for (auto& class_name : class_names) {
        print_single_string(class_name, class_name, out);
    }
    for (unsigned curr_id = 0; curr_id != id; ++curr_id) {
        print_single_string(strings.at(curr_id), std::to_string(curr_id), out);
    }
}

void ConstantsDataGeneneror::print_single_string(std::string_view string,
    std::string_view curr_id, std::ostream& out) const noexcept {
    out << std::setw(12) << ".word" << ' ' << -1 << '\n';
    out << "str_const" << curr_id << ":\n";
    out << std::setw(12) << ".word" << ' ' << 3 << '\n';
    out << std::setw(12) << ".word" << ' '
        << 4 + static_cast<unsigned>(
                   std::ceil(static_cast<double>(string.size() + 1) / 4))
        << '\n';
    out << std::setw(12) << ".word" << ' ' << "String_dispTab" << '\n';
    out << std::setw(12) << ".word" << ' ' << "int_const" << string.size()
        << '\n';
    if (!string.empty()) {
        out << std::setw(12) << ".ascii" << ' ';
        util::print_string(string, out);
    }
    out << std::setw(12) << ".byte" << ' ' << 0 << '\n' << '\n';
}
} // namespace cool::codegen::MIPS32
