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
    void register_prototype(std::string_view class_name,
        ClassPrototypeRepresentation&& representation) noexcept {
        if (representation.inherits) {
            children[representation.inherits.value()].emplace_back(class_name);
        }
        representations.emplace(class_name, std::move(representation));
        registered.emplace_back(class_name);
    }

    void generate_prototypes(std::ostream& out) noexcept;

    [[nodiscard]] unsigned get_field_offset(std::string_view class_name,
        std::string_view field_name) const noexcept;

    [[nodiscard]] std::pair<unsigned, unsigned> get_prototype_ids(
        std::string_view class_name) const noexcept {
        return ids.at(class_name);
    }

private:
    void calculate_ids() noexcept;

    void calculate_id(std::string_view class_name) noexcept;

    void generate_prototype(
        std::string_view class_name, std::ostream& out) noexcept;

    void generate_prototype_data(std::string_view class_name,
        unsigned class_size,
        const std::vector<std::pair<std::string_view, std::string_view>>&
            fields,
        std::ostream& out, unsigned id) noexcept;

    [[nodiscard]] std::vector<std::pair<std::string_view, std::string_view>>
    get_class_fields(const ClassPrototypeRepresentation&) noexcept;

private:
    std::vector<std::string_view> registered;
    std::unordered_map<std::string_view, ClassPrototypeRepresentation>
        representations;
    std::unordered_map<std::string_view,
        std::unordered_map<std::string_view, unsigned>>
        field_offsets;
    std::unordered_map<std::string_view, std::vector<std::string_view>>
        children;
    std::unordered_map<std::string_view, std::pair<unsigned, unsigned>> ids;
    unsigned current_id = 0;
};

} // namespace cool::codegen::MIPS32
