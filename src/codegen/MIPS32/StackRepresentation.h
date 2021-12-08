#pragma once

#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cool::codegen::MIPS32 {

class StackRepresentation {
public:
    void register_vars(
        std::unordered_map<std::string_view, unsigned>&& vars) noexcept {
        stack.emplace_back(std::move(vars));
    }

    void add_padding(unsigned x) noexcept {
        stack.emplace_back(x);
    }

    [[nodiscard]] unsigned get_variable(
        std::string_view var_name) const noexcept {
        unsigned global_offset = 0;
        for (auto it = stack.crbegin(); it != stack.crend(); ++it) {
            if (std::holds_alternative<unsigned>(*it)) {
                global_offset += std::get<unsigned>(*it);
            } else {
                auto& entry =
                    std::get<std::unordered_map<std::string_view, unsigned>>(
                        *it);
                if (entry.contains(var_name)) {
                    return entry.at(var_name) + global_offset;
                }
                global_offset += static_cast<unsigned>(entry.size()) * 4;
            }
        }
        return {}; // unreachable
    }

    bool contains(std::string_view var_name) const noexcept {
        for (auto it = stack.crbegin(); it != stack.crend(); ++it) {
            if (!std::holds_alternative<unsigned>(*it)) {
                auto& entry =
                    std::get<std::unordered_map<std::string_view, unsigned>>(
                        *it);
                if (entry.contains(var_name)) {
                    return true;
                }
            }
        }
        return false;
    }

    void pop() noexcept {
        stack.pop_back();
    }

private:
    std::vector<
        std::variant<unsigned, std::unordered_map<std::string_view, unsigned>>>
        stack;
};

} // namespace cool::codegen::MIPS32
