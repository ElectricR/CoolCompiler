#include "codegen/MIPS32/DispTableGen.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>

namespace cool::codegen::MIPS32 {

void DispTableDataGenerator::register_class_representation(
    std::string_view class_name,
    ClassDispTableRepresentation&& representation) noexcept {
    representations[class_name] = std::move(representation);
    registered.emplace_back(class_name);
}

void DispTableDataGenerator::generate_disptables(std::ostream& out) noexcept {
    for (auto& class_name : registered) {
        if (class_name == "Object") {
            continue;
        }
        std::stack<std::string_view> class_family;
        std::string_view current_ancestor = class_name;
        while (true) {
            class_family.emplace(current_ancestor);
            if (representations[current_ancestor].inherits == "Object") {
                class_family.emplace("Object");
                break;
            }
            current_ancestor =
                representations[current_ancestor].inherits.value();
        }
        while (!class_family.empty()) {
            current_ancestor = class_family.top();
            class_family.pop();
            if (!disptable.contains(current_ancestor)) {
                generate_disptable(current_ancestor);
                print_disptable(current_ancestor, out);
            }
        }
    }
}

void DispTableDataGenerator::generate_disptable(
    std::string_view class_name) noexcept {
    if (representations[class_name].inherits) {
        disptable[class_name] =
            disptable[representations[class_name].inherits.value()];
    } else {
        disptable[class_name] = disptable["Object"];
    }
    for (auto& method_name : representations[class_name].method_names) {
        if (disptable[class_name].first.contains(method_name)) {
            disptable[class_name]
                .second[disptable[class_name].first[method_name]] = {
                class_name, method_name};
        } else {
            unsigned offset =
                static_cast<unsigned>(disptable[class_name].first.size()) * 4;
            disptable[class_name].first[method_name] = offset;
            disptable[class_name].second[offset] = {class_name, method_name};
        }
    }
}

void DispTableDataGenerator::print_disptable(
    std::string_view class_name, std::ostream& out) noexcept {
    out << class_name << "_dispTab:\n";
    for (unsigned i = 0; i != disptable[class_name].second.size() * 4; i += 4) {
        print_method(disptable[class_name].second[i].first,
            disptable[class_name].second[i].second, out);
    }
    out << '\n';
}

void DispTableDataGenerator::print_method(std::string_view class_name,
    std::string_view method_name, std::ostream& out) noexcept {
    out << std::setw(12) << ".word" << ' ' << class_name << '.' << method_name
        << '\n';
}

} // namespace cool::codegen::MIPS32
