#pragma once

#include <unordered_map>
#include <string_view>
#include <vector>
#include <optional>

namespace cool::codegen::MIPS32 {

struct ClassPrototypeRepresentation {
    std::optional<std::string_view> inherits;
    std::vector<std::string_view> field_types;
};

class PrototypeDataGenerator {
public:
    void generate_prototype(const std::unordered_map<std::string_view, ClassPrototypeRepresentation>& class_prototype_map, std::string_view class_name, std::ostream& out) noexcept;

    void generate_object_prototype(std::ostream& out) noexcept;

    void generate_int_prototype(std::ostream& out) noexcept;

    void generate_bool_prototype(std::ostream& out) noexcept;

    void generate_string_prototype(std::ostream& out) noexcept;

    void generate_io_prototype(std::ostream& out) noexcept;

private:
    void generate_prototype_data(std::string_view class_name, unsigned class_size, const std::vector<std::string_view>& field_addresses, std::ostream& out, unsigned id) const noexcept;

    [[nodiscard]] std::vector<std::string_view> get_class_fields(const ClassPrototypeRepresentation&) noexcept;

    unsigned current_id = 5; // Because of 5 base classes
};

} // namespace
