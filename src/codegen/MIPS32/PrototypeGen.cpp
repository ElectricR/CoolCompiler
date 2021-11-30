#include "codegen/MIPS32/PrototypeGen.h"
#include <iomanip>
#include <ranges>


namespace cool::codegen::MIPS32 {

void PrototypeDataGenerator::generate_prototype(const std::unordered_map<std::string_view, ClassPrototypeRepresentation>& class_prototype_map, std::string_view class_name, std::ostream& out) noexcept {
    std::vector<std::string_view> fields;
    const ClassPrototypeRepresentation *curr_class = &class_prototype_map.at(class_name);
    while (true) {
        for (auto&& field : get_class_fields(*curr_class)) {
            fields.emplace_back(std::move(field));
        }
        if (curr_class->inherits) {
            curr_class = &class_prototype_map.at(curr_class->inherits.value());
        } else {
            break;
        }
    }
    std::reverse(fields.begin(), fields.end());
    generate_prototype_data(class_name, static_cast<unsigned>(fields.size()) + 3, fields, out);
}

[[nodiscard]] std::vector<std::string_view> PrototypeDataGenerator::get_class_fields(const ClassPrototypeRepresentation& class_prototype_representation) noexcept {
    std::vector<std::string_view> fields;
    for (auto& class_type_id : class_prototype_representation.field_types) {
        if (class_type_id == "Int") {
            fields.emplace_back("int_const0");
        } else if (class_type_id == "Bool") {
            fields.emplace_back("bool_const0");
        } else if (class_type_id == "String") {
            fields.emplace_back("str_const0");
        } else {
            fields.emplace_back("0");
        }
    }
    std::reverse(fields.begin(), fields.end());
    return fields;
}

void PrototypeDataGenerator::generate_object_prototype(std::ostream& out) noexcept {
    generate_prototype_data("Object", 3, {}, out);
}

void PrototypeDataGenerator::generate_int_prototype(std::ostream& out) noexcept {
    generate_prototype_data("Int", 4, {"0"}, out);
}

void PrototypeDataGenerator::generate_bool_prototype(std::ostream& out) noexcept {
    generate_prototype_data("Bool", 4, {"0"}, out);
}

void PrototypeDataGenerator::generate_string_prototype(std::ostream& out) noexcept {
    generate_prototype_data("String", 5, {"int_const0", "0"}, out);
}

void PrototypeDataGenerator::generate_io_prototype(std::ostream& out) noexcept {
    generate_prototype_data("IO", 3, {}, out);
}

void PrototypeDataGenerator::generate_prototype_data(std::string_view class_name, unsigned class_size, const std::vector<std::string_view>& field_addresses, std::ostream& out) noexcept {
    out << std::setw(12) << ".word" << ' ' << -1 << '\n';
    out << class_name << "_protObj:\n";
    out << std::setw(12) << ".word" << ' ' << current_id++ << '\n';
    out << std::setw(12) << ".word" << ' ' << class_size << '\n';
    out << std::setw(12) << ".word" << ' ' << class_name << "_dispTab" << '\n';
    for (const auto& sv : field_addresses) {
        out << std::setw(12) << ".word" << ' ' << sv << '\n';
    }
    out << '\n';
}

} //namespace
