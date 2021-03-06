#pragma once
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cool::codegen::MIPS32 {

class MiscGenerator {
public:
    void generate_globals(std::ostream& out) const noexcept;

    void generate_basic_tags(std::ostream& out) const noexcept;

    void generate_heap_start(std::ostream& out) const noexcept;

    void generate_gc_stuff(std::ostream& out) const noexcept;

    void generate_objtab(std::vector<std::string_view> class_names,
        std::ostream& out) const noexcept;

    void generate_nametab(
        const std::unordered_map<unsigned, std::string_view>& tags,
        std::ostream& out) const noexcept {
        out << "class_nameTab:\n";
        for (unsigned tag = 0; tag != static_cast<unsigned>(tags.size());
             ++tag) {
            out << std::setw(12) << ".word" << ' ' << "str_const"
                << tags.at(tag) << '\n';
        };
        out << '\n';
    }

private:
    void generate_single_global(
        std::string_view global, std::ostream& out) const noexcept;

    void generate_single_basic_tag(
        std::string_view tag, unsigned id, std::ostream& out) const noexcept;
};

} // namespace cool::codegen::MIPS32
