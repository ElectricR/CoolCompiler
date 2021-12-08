#include "codegen/MIPS32/PrototypeGen.h"
#include <iomanip>

namespace cool::codegen::MIPS32 {

void PrototypeDataGenerator::generate_prototypes(std::ostream& out) noexcept {
    calculate_ids();
    for (auto& class_name : registered) {
        generate_prototype(class_name, out);
    }
}

void PrototypeDataGenerator::calculate_ids() noexcept {
    calculate_id("Object");
}

void PrototypeDataGenerator::calculate_id(
    std::string_view class_name) noexcept {
    ids[class_name] = {current_id++, 0};
    for (auto& child : children[class_name]) {
        calculate_id(child);
    }
    if (!children[class_name].empty()) {
        ids[class_name].second = ids[children[class_name].back()].second;
    } else {
        ids[class_name].second = ids[class_name].first;
    }
}

void PrototypeDataGenerator::generate_prototype(
    std::string_view class_name, std::ostream& out) noexcept {

    std::vector<std::pair<std::string_view, std::string_view>> fields;
    const ClassPrototypeRepresentation* curr_class =
        &representations.at(class_name);
    while (true) {
        for (auto&& field : get_class_fields(*curr_class)) {
            fields.emplace_back(std::move(field));
        }
        if (curr_class->inherits) {
            curr_class = &representations.at(curr_class->inherits.value());
        } else {
            break;
        }
    }
    std::reverse(fields.begin(), fields.end());
    generate_prototype_data(class_name,
        static_cast<unsigned>(fields.size()) + 3, fields, out,
        ids.at(class_name).first);
}

[[nodiscard]] unsigned PrototypeDataGenerator::get_field_offset(
    std::string_view class_name, std::string_view field_name) const noexcept {
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

void PrototypeDataGenerator::generate_prototype_data(
    std::string_view class_name, unsigned class_size,
    const std::vector<std::pair<std::string_view, std::string_view>>& fields,
    std::ostream& out, unsigned id) noexcept {

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
