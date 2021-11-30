#pragma once
#include <string_view>
#include <vector>

namespace cool::codegen::MIPS32 {

class MiscGenerator {
public:
    void generate_globals(std::ostream& out) const noexcept;

    void generate_basic_tags(std::ostream& out) const noexcept;

    void generate_heap_start(std::ostream& out) const noexcept;

    void generate_gc_stuff(std::ostream& out) const noexcept;

    void generate_objtab(std::vector<std::string_view> class_names, std::ostream& out) const noexcept;

    void generate_nametab(unsigned name_count, std::ostream& out) const noexcept;

private:
    void generate_single_global(std::string_view global, std::ostream& out) const noexcept;

    void generate_single_basic_tag(std::string_view tag, unsigned id, std::ostream& out) const noexcept;
};

} // namespace
