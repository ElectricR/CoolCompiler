#pragma once
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cool::codegen::MIPS32 {

struct ClassDispTableRepresentation {
    std::optional<std::string_view> inherits;
    std::vector<std::string_view> method_names;
};

class DispTableDataGenerator {
public:
    void register_class_representation(std::string_view class_name, ClassDispTableRepresentation&& representation) noexcept;
    
    const auto& get_disptable() const noexcept {
        return disptable;
    }

    void generate_disptables(std::ostream& out) noexcept;

private:
    void generate_disptable(std::string_view class_name) noexcept;

    void print_disptable(std::string_view class_name, std::ostream& out) noexcept;

    void print_object_methods(std::ostream& out) noexcept;

    void print_method(std::string_view class_name,
        std::string_view method_name, std::ostream& out) noexcept;

    std::unordered_map<std::string_view,
        std::pair<std::unordered_map<std::string_view, unsigned>,
            std::unordered_map<unsigned, std::pair<std::string_view, std::string_view>>>>
        disptable;

    std::unordered_map<std::string_view, ClassDispTableRepresentation> representations;
    std::vector<std::string_view> registered;
};

} // namespace cool::codegen::MIPS32
