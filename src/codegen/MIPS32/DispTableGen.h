#pragma once
#include <optional>
#include <string_view>
#include <vector>
#include <unordered_map>

namespace cool::codegen::MIPS32 {

struct ClassDispTableRepresentation {
    std::optional<std::string_view> inherits;
    std::vector<std::string_view> method_names;
};

class DispTableDataGenerator {
public:
    void generate_disptable(const std::unordered_map<std::string_view, ClassDispTableRepresentation>& class_map, std::string_view class_name, std::ostream& out) const noexcept;

    void generate_object_disptable(std::ostream& out) const noexcept;

    void generate_int_disptable(std::ostream& out) const noexcept;

    void generate_bool_disptable(std::ostream& out) const noexcept;

    void generate_string_disptable(std::ostream& out) const noexcept;

    void generate_io_disptable(std::ostream& out) const noexcept;

private:
    void print_object_methods(std::ostream& out) const noexcept;

    void print_method(std::string_view class_name, std::string_view method_name, std::ostream& out) const noexcept;
};

} // namespace