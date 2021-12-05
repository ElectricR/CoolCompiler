#include "codegen/MIPS32/DispTableGen.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

namespace cool::codegen::MIPS32 {

void DispTableDataGenerator::generate_disptable(
    const std::unordered_map<std::string_view, ClassDispTableRepresentation>&
        class_map,
    std::string_view class_name, std::ostream& out) noexcept {
    current_offset = 0;
    out << class_name << "_dispTab:\n";
    print_object_methods(out);
    std::vector<std::pair<std::string_view, std::string_view>> methods;
    const ClassDispTableRepresentation* curr_representation =
        &class_map.at(class_name);
    std::string_view curr_class_name = class_name;
    while (true) {
        std::for_each(curr_representation->method_names.crbegin(),
            curr_representation->method_names.crend(),
            [&methods, &curr_class_name](auto& method_name) {
                methods.emplace_back(curr_class_name, method_name);
            });
        if (!curr_representation->inherits) {
            break;
        }
        curr_class_name = curr_representation->inherits.value();
        curr_representation =
            &class_map.at(curr_representation->inherits.value());
    }
    std::for_each(methods.crbegin(), methods.crend(), [this, &out](auto& pair) {
        register_method(pair.first, pair.second, out);
    });
    out << '\n';
}

void DispTableDataGenerator::generate_object_disptable(
    std::ostream& out) noexcept {
    current_offset = 0;
    out << "Object_dispTab:\n";
    print_object_methods(out);
    out << '\n';
}

void DispTableDataGenerator::generate_int_disptable(
    std::ostream& out) noexcept {
    current_offset = 0;
    out << "Int_dispTab:\n";
    print_object_methods(out);
    out << '\n';
}

void DispTableDataGenerator::generate_bool_disptable(
    std::ostream& out) noexcept {
    current_offset = 0;
    out << "Bool_dispTab:\n";
    print_object_methods(out);
    out << '\n';
}

void DispTableDataGenerator::generate_string_disptable(
    std::ostream& out) noexcept {
    current_offset = 0;
    out << "String_dispTab:\n";
    print_object_methods(out);
    register_method("String", "length", out);
    register_method("String", "concat", out);
    register_method("String", "substr", out);
    out << '\n';
}

void DispTableDataGenerator::generate_io_disptable(std::ostream& out) noexcept {
    current_offset = 0;
    out << "IO_dispTab:\n";
    print_object_methods(out);
    register_method("IO", "out_string", out);
    register_method("IO", "out_int", out);
    register_method("IO", "in_string", out);
    register_method("IO", "in_int", out);
    out << '\n';
}

void DispTableDataGenerator::print_object_methods(std::ostream& out) noexcept {
    register_method("Object", "abort", out);
    register_method("Object", "type_name", out);
    register_method("Object", "copy", out);
}

void DispTableDataGenerator::register_method(std::string_view class_name,
    std::string_view method_name, std::ostream& out) noexcept {
    out << std::setw(12) << ".word" << ' ' << class_name << '.' << method_name
        << '\n';
    disptable[class_name][method_name] = current_offset;
    current_offset += 4;
}

} // namespace cool::codegen::MIPS32
