#pragma once

#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cool::codegen::MIPS32 {

struct ClassPrototypeRepresentation {
    std::optional<std::string_view> inherits;
    std::vector<std::pair<std::string_view, std::string_view>> fields;
};

class PrototypeDataGenerator {
public:
    void generate_prototype(
        const std::unordered_map<std::string_view,
            ClassPrototypeRepresentation>& class_prototype_map,
        std::string_view class_name, std::ostream& out) noexcept;

    void generate_object_prototype(std::ostream& out) noexcept;

    void generate_int_prototype(std::ostream& out) noexcept;

    void generate_bool_prototype(std::ostream& out) noexcept;

    void generate_string_prototype(std::ostream& out) noexcept;

    void generate_io_prototype(std::ostream& out) noexcept;

    unsigned get_field_offset(std::string_view class_name, std::string_view field_name) const noexcept;

private:
    void generate_prototype_data(std::string_view class_name,
        unsigned class_size,
        const std::vector<std::pair<std::string_view, std::string_view>>& fields, std::ostream& out,
        unsigned id) noexcept;

    [[nodiscard]] std::vector<std::pair<std::string_view, std::string_view>> get_class_fields(
        const ClassPrototypeRepresentation&) noexcept;

private:
    std::unordered_map<std::string_view, std::unordered_map<std::string_view, unsigned>> field_offsets;
    unsigned current_id = 5; // Because of 5 base classes
};

} // namespace cool::codegen::MIPS32
