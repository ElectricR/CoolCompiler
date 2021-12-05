#pragma once

#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>

namespace util {

template <class InputIt, class T, class BinaryOperation1,
    class BinaryOperation2>
constexpr auto adjacent_reduce(InputIt first2, InputIt last, T init,
    BinaryOperation1 reduce_op, BinaryOperation2 transform_op) {
    InputIt first1 = first2++;
    return std::inner_product(
        first1, --last, first2, init, reduce_op, transform_op);
}

static void print_string(std::string_view str, std::ostream& out) noexcept {
    out << '"';
    std::for_each(str.cbegin(), str.cend(), [&out](char ch) {
        switch (ch) {
        case '\\': {
            out << "\\\\";
            break;
        }
        case '\"': {
            out << "\\\"";
            break;
        }
        case '\t': {
            out << "\\t";
            break;
        }
        case '\b': {
            out << "\\b";
            break;
        }
        case '\f': {
            out << "\\f";
            break;
        }
        case '\n': {
            out << "\\n";
            break;
        }
        case '\033': {
            out << "\\033";
            break;
        }
        case '\015': {
            out << "\\015";
            break;
        }
        case '\022': {
            out << "\\022";
            break;
        }
        case '\013': {
            out << "\\013";
            break;
        }
        default: {
            out << ch;
            break;
        }
        }
    });
    out << '"' << '\n';
}

} // namespace util
