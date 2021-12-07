#pragma once
#include <set>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cool::codegen::MIPS32 {

class ConstantsDataGeneneror {
public:
    ConstantsDataGeneneror() {
        (void) register_string("");
    }
    void generate_constants(std::ostream& out) const noexcept;

    void register_int(int i) noexcept {
        ints.insert(i);
    }

    void register_class_name(std::string_view class_name) noexcept {
        class_names.emplace_back(class_name);
        ints.insert(static_cast<int>(class_name.size()));
    }

    [[nodiscard]] unsigned register_string(std::string_view string) noexcept {
        unsigned curr_id = id++;
        strings[curr_id] = string;
        ints.insert(static_cast<int>(string.size()));
        return curr_id;
    }

    void register_filepath(std::string_view string) noexcept {
        filepath = string;
        ints.insert(static_cast<int>(string.size()));
    }

private:
    void generate_ints(std::ostream& out) const noexcept;

    void generate_bools(std::ostream& out) const noexcept;

    void generate_strings(std::ostream& out) const noexcept;

    void print_single_string(std::string_view string, std::string_view id,
        std::ostream& out) const noexcept;

private:
    std::set<int> ints = {0};
    std::vector<std::string_view> class_names;
    unsigned id = 0;
    std::unordered_map<unsigned, std::string_view> strings;
    std::string_view filepath;
};

} // namespace cool::codegen::MIPS32
