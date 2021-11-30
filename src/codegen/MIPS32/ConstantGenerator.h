#pragma once
#include <set>
#include <string_view>

namespace cool::codegen::MIPS32 {

class ConstantsDataGeneneror {
public:
    void generate_constants(std::ostream& out) const noexcept;

    void register_int(int i) noexcept {
        ints.insert(i);
    }
private:
    void generate_ints(std::ostream& out) const noexcept;

    void generate_bools(std::ostream& out) const noexcept;

private:
    std::set<int> ints = {0};
};

} // namespace
