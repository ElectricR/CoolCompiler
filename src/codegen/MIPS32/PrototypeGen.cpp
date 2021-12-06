#include "codegen/MIPS32/PrototypeGen.h"
#include <iomanip>

namespace cool::codegen::MIPS32 {

void PrototypeDataGenerator::generate_prototype(
    const std::unordered_map<std::string_view, ClassPrototypeRepresentation>&
        family_map,
    std::string_view class_name, std::ostream& out) noexcept {

    std::vector<std::pair<std::string_view, std::string_view>> fields;
    const ClassPrototypeRepresentation* curr_class =
        &family_map.at(class_name);
    while (true) {
        for (auto&& field : get_class_fields(*curr_class)) {
            fields.emplace_back(std::move(field));
        }
        if (curr_class->inherits) {
            curr_class = &family_map.at(curr_class->inherits.value());
        } else {
            break;
        }
    }
    std::reverse(fields.begin(), fields.end());
    generate_prototype_data(class_name,
        static_cast<unsigned>(fields.size()) + 3, fields, out, current_id++);
}

unsigned PrototypeDataGenerator::get_field_offset(std::string_view class_name, std::string_view field_name) const noexcept {
    return field_offsets.at(class_name).at(field_name);
}

[[nodiscard]] std::vector<std::pair<std::string_view, std::string_view>>
PrototypeDataGenerator::get_class_fields(const ClassPrototypeRepresentation&
        class_prototype_representation) noexcept {
    std::vector<std::pair<std::string_view, std::string_view>> fields;
    for (auto& field_pair : class_prototype_representation.fields) {
        if (field_pair.second == "Int") {
            fields.emplace_back(field_pair.first, "int_const0");
        } else if (field_pair.second == "Bool") {
            fields.emplace_back(field_pair.first, "bool_const0");
        } else if (field_pair.second == "String") {
            fields.emplace_back(field_pair.first, "str_const0");
        } else {
            fields.emplace_back(field_pair.first, "0");
        }
    }
    std::reverse(fields.begin(), fields.end());
    return fields;
}

void PrototypeDataGenerator::generate_object_prototype(
    std::ostream& out) noexcept {
    generate_prototype_data("Object", 3, {}, out, 0);
}

void PrototypeDataGenerator::generate_int_prototype(
    std::ostream& out) noexcept {
    generate_prototype_data("Int", 4, {{"", "0"}}, out, 1);
}

void PrototypeDataGenerator::generate_bool_prototype(
    std::ostream& out) noexcept {
    generate_prototype_data("Bool", 4, {{"", "0"}}, out, 2);
}

void PrototypeDataGenerator::generate_string_prototype(
    std::ostream& out) noexcept {
    generate_prototype_data("String", 5, {{"", "int_const0"}, {"", "0"}}, out, 3);
}

void PrototypeDataGenerator::generate_io_prototype(std::ostream& out) noexcept {
    generate_prototype_data("IO", 3, {}, out, 4);
}

void PrototypeDataGenerator::generate_prototype_data(
    std::string_view class_name, unsigned class_size,
    const std::vector<std::pair<std::string_view, std::string_view>>& fields, std::ostream& out,
    unsigned id) noexcept {

    unsigned curr_offset = 12;
    out << std::setw(12) << ".word" << ' ' << -1 << '\n';
    out << class_name << "_protObj:\n";
    out << std::setw(12) << ".word" << ' ' << id << '\n';
    out << std::setw(12) << ".word" << ' ' << class_size << '\n';
    out << std::setw(12) << ".word" << ' ' << class_name << "_dispTab" << '\n';
    for (const auto& field_pair : fields) {
        out << std::setw(12) << ".word" << ' ' << field_pair.second << '\n';
        field_offsets[class_name][field_pair.first] = curr_offset;
        curr_offset += 4;
    }
    out << '\n';
}

} // namespace cool::codegen::MIPS32
