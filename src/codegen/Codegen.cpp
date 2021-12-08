#include "codegen/Codegen.h"
#include <iomanip>
#include <ranges>

namespace cool::codegen {

template <class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

void Codegen::generate_assembly(
    const AST::Program& AST, std::ostream& out) noexcept {
    populate_class_name_constants(AST);
    generate_basic_tags(data);
    generate_gc_stuff(data);
    get_data_from_AST(AST);
    generate_text(AST);
    generate_constants(data);
    generate_heap_start(data);
    generate_globals(data);
    print_data(out);
    print_text(out);
}

void Codegen::generate_text(const AST::Program& program) noexcept {
    generate_inits(program);
    AST_visitor(program);
}

void Codegen::generate_inits(const AST::Program& program) noexcept {
    generate_default_inits();
    for (auto& class_ : program.classes) {
        generate_init(class_.type_id);
    }
}

void Codegen::get_data_from_AST(const AST::Program& AST) noexcept {
    generate_prototypes(AST, data);
    generate_disptables(AST, data);
    generate_objtab(AST, data);
    generate_nametab(AST, data);
}

void Codegen::generate_prototypes(
    const AST::Program& AST, std::ostream& out) noexcept {
    prototype_data_gen.register_prototype("Object", {{}, {}});
    prototype_data_gen.register_prototype("Int", {"Object", {{"", ""}}});
    prototype_data_gen.register_prototype("Bool", {"Object", {{"", ""}}});
    prototype_data_gen.register_prototype(
        "String", {"Object", {{"", "Int"}, {"", ""}}});
    prototype_data_gen.register_prototype("IO", {"Object", {}});
    for (auto& class_ : AST.classes) {
        auto fields_range =
            class_.features |
            std::ranges::views::filter([](auto& feature_variant) {
                return std::holds_alternative<AST::FieldFeature>(
                    feature_variant.feature);
            }) |
            std::ranges::views::transform(
                [](auto& field_feature)
                    -> std::pair<std::string_view, std::string_view> {
                    return {std::get<AST::FieldFeature>(field_feature.feature)
                                .object_id,
                        std::get<AST::FieldFeature>(field_feature.feature)
                            .type_id};
                });
        MIPS32::ClassPrototypeRepresentation cl = {
            class_.inherits, {fields_range.begin(), fields_range.end()}};
        if (!cl.inherits) {
            cl.inherits = "Object";
        }

        prototype_data_gen.register_prototype(class_.type_id, std::move(cl));
    }
    prototype_data_gen.generate_prototypes(out);
}

void Codegen::generate_disptables(
    const AST::Program& AST, std::ostream& out) noexcept {
    disptable_data_gen.register_class_representation(
        "Object", {{}, {"abort", "type_name", "copy"}});
    disptable_data_gen.register_class_representation("Int", {"Object", {}});
    disptable_data_gen.register_class_representation("Bool", {"Object", {}});
    disptable_data_gen.register_class_representation(
        "String", {"Object", {"length", "concat", "substr"}});
    disptable_data_gen.register_class_representation(
        "IO", {"Object", {"out_string", "out_int", "in_string", "in_int"}});

    for (auto& class_ : AST.classes) {
        auto fields_range =
            class_.features |
            std::ranges::views::filter([](auto& feature_variant) {
                return std::holds_alternative<AST::MethodFeature>(
                    feature_variant.feature);
            }) |
            std::ranges::views::transform(
                [](auto& field_feature) -> std::string_view {
                    return std::get<AST::MethodFeature>(field_feature.feature)
                        .object_id;
                });
        MIPS32::ClassDispTableRepresentation cl = {
            class_.inherits, {fields_range.begin(), fields_range.end()}};
        if (!cl.inherits) {
            cl.inherits = "Object";
        }
        disptable_data_gen.register_class_representation(
            class_.type_id, std::move(cl));
    }
    disptable_data_gen.generate_disptables(out);
}

void Codegen::generate_objtab(
    const AST::Program& AST, std::ostream& out) const noexcept {
    std::vector<std::string_view> class_names = {
        "Object", "Int", "Bool", "String", "IO"};
    for (auto& class_ : AST.classes) {
        class_names.emplace_back(class_.type_id);
    }
    misc_data_gen.generate_objtab(class_names, out);
}

void Codegen::generate_nametab(
    const AST::Program& AST, std::ostream& out) const noexcept {
    std::vector<std::string_view> class_names = {
        "Object", "Int", "Bool", "String", "IO"};
    std::transform(AST.classes.cbegin(), AST.classes.cend(),
        std::back_inserter(class_names),
        [](auto& class_) -> std::string_view { return class_.type_id; });
    std::unordered_map<unsigned, std::string_view> tags;
    for (auto& class_name : class_names) {
        tags.emplace(prototype_data_gen.get_prototype_ids(class_name).first, class_name);
    }
    misc_data_gen.generate_nametab(tags, out);
}

void Codegen::generate_constants(std::ostream& out) const noexcept {
    constants_data_gen.generate_constants(out);
}

void Codegen::generate_globals(std::ostream& out) const noexcept {
    misc_data_gen.generate_globals(out);
}

void Codegen::generate_basic_tags(std::ostream& out) const noexcept {
    misc_data_gen.generate_basic_tags(out);
}

void Codegen::generate_heap_start(std::ostream& out) const noexcept {
    misc_data_gen.generate_heap_start(out);
}

void Codegen::generate_gc_stuff(std::ostream& out) const noexcept {
    misc_data_gen.generate_gc_stuff(out);
}

void Codegen::print_data(std::ostream& out) noexcept {
    out << ".data\n";
    out << data.rdbuf();
}

void Codegen::print_text(std::ostream& out) noexcept {
    out << ".text\n";
    out << text.rdbuf();
}

void Codegen::make_class_map(const AST::Program& AST) noexcept {
    for (auto& class_ : AST.classes) {
        class_map.emplace(class_.type_id, class_);
    }
}

void Codegen::populate_class_name_constants(const AST::Program& AST) noexcept {
    constants_data_gen.register_filepath(AST.classes.front().filepath);
    constants_data_gen.register_class_name("Object");
    constants_data_gen.register_class_name("Int");
    constants_data_gen.register_class_name("Bool");
    constants_data_gen.register_class_name("String");
    constants_data_gen.register_class_name("IO");
    for (auto& class_ : AST.classes) {
        constants_data_gen.register_class_name(class_.type_id);
    }
}

void Codegen::generate_default_inits() noexcept {
    text_gen.generate_empty_init("Object", text);
    text_gen.generate_empty_init("Int", text);
    text_gen.generate_empty_init("Bool", text);
    text_gen.generate_empty_init("String", text);
    text_gen.generate_empty_init("IO", text);
}

[[nodiscard]] bool check_if_empty(
    const std::unordered_map<std::string_view, const AST::Class&>& class_map,
    std::string_view class_name) noexcept {
    bool is_empty = true;
    while (true) {
        for (auto& feature : class_map.at(class_name).features) {
            if (std::holds_alternative<AST::FieldFeature>(feature.feature)) {
                is_empty = false;
                break;
            }
        }
        if (!class_map.at(class_name).inherits) {
            break;
        }
        class_name = class_map.at(class_name).inherits.value();
        if (std::unordered_set<std::string_view>{
                "Object", "Int", "Bool", "String", "IO"}
                .contains(class_name)) {
            break;
        }
    }
    return is_empty;
}

void Codegen::generate_init(std::string_view class_name) noexcept {
    if (check_if_empty(class_map, class_name)) {
        text_gen.generate_empty_init(class_name, text);
    } else {
        AST_visitor.set_current_class(class_name);
        std::string_view parent;
        if (class_map.at(class_name).inherits) {
            parent = class_map.at(class_name).inherits.value();
        } else {
            parent = "Object";
        }
        text_gen.generate_init_label(class_name, text);
        text_gen.print_prologue(text);
        stack_representation.add_padding(12);
        text_gen.generate_parent_init(parent, text);
        for (auto& feature : class_map.at(class_name).features) {
            if (std::holds_alternative<AST::FieldFeature>(feature.feature)) {
                if (std::get<AST::FieldFeature>(feature.feature).value) {
                    std::visit(AST_visitor,
                        std::get<AST::FieldFeature>(feature.feature)
                            .value.value()
                            ->value);
                    text_gen.save_to_field(
                        prototype_data_gen.get_field_offset(class_name,
                            std::get<AST::FieldFeature>(feature.feature)
                                .object_id),
                        text);
                }
            }
        }
        text_gen.load_self_object(text);
        text_gen.print_epilogue(0, text);
        stack_representation.pop();
    }
}

} // namespace cool::codegen
