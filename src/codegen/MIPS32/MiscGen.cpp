#include <codegen/MIPS32/MiscGen.h>
#include <iomanip>

namespace cool::codegen::MIPS32 {

void MiscGenerator::generate_globals(std::ostream& out) const noexcept {
    generate_single_global("class_nameTab", out);
    generate_single_global("Main_protObj", out);
    generate_single_global("Int_protObj", out);
    generate_single_global("String_protObj", out);
    generate_single_global("bool_const0", out);
    generate_single_global("bool_const1", out);
    generate_single_global("_int_tag", out);
    generate_single_global("_bool_tag", out);
    generate_single_global("_string_tag", out);
    generate_single_global("_MemMgr_INITIALIZER", out);
    generate_single_global("_MemMgr_COLLECTOR", out);
    generate_single_global("_MemMgr_TEST", out);
    generate_single_global("heap_start", out);
    generate_single_global("Main_init", out);
    generate_single_global("Int_init", out);
    generate_single_global("String_init", out);
    generate_single_global("Bool_init", out);
    generate_single_global("Main.main", out);
    out << '\n';
}

void MiscGenerator::generate_basic_tags(std::ostream& out) const noexcept {
    generate_single_basic_tag("int", 1, out);
    generate_single_basic_tag("bool", 2, out);
    generate_single_basic_tag("string", 3, out);
}

void MiscGenerator::generate_heap_start(std::ostream& out) const noexcept {
    out << "heap_start:\n";
    out << std::setw(12) << ".word" << ' ' << 0 << "\n\n";
}

void MiscGenerator::generate_gc_stuff(std::ostream& out) const noexcept {
    out << "_MemMgr_INITIALIZER:\n";
    out << std::setw(12) << ".word" << ' ' << "_NoGC_Init\n";
    out << "_MemMgr_COLLECTOR:\n";
    out << std::setw(12) << ".word" << ' ' << "_NoGC_Collect\n";
    out << "_MemMgr_TEST:\n";
    out << std::setw(12) << ".word" << ' ' << 0 << "\n\n";
}

void MiscGenerator::generate_objtab(std::vector<std::string_view> class_names,
    std::ostream& out) const noexcept {
    out << "class_objTab:\n";
    for (auto& sv : class_names) {
        out << std::setw(12) << ".word" << ' ' << sv << "_protObj\n";
        out << std::setw(12) << ".word" << ' ' << sv << "_init\n";
    }
    out << '\n';
}

void MiscGenerator::generate_single_global(
    std::string_view global, std::ostream& out) const noexcept {
    out << ".globl" << ' ' << global << '\n';
}

void MiscGenerator::generate_single_basic_tag(
    std::string_view tag, unsigned id, std::ostream& out) const noexcept {
    out << '_' << tag << "_tag:\n";
    out << std::setw(12) << ".word" << ' ' << id << '\n' << '\n';
}

} // namespace cool::codegen::MIPS32
