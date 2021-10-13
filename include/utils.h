#pragma once
#include <numeric>
#include <string_view>
#include <ranges>
#include <iostream>

namespace util {

template <class InputIt, class T, class BinaryOperation1,
    class BinaryOperation2>
constexpr auto adjacent_reduce(InputIt first2, InputIt last, T init,
    BinaryOperation1 reduce_op, BinaryOperation2 transform_op) {
    InputIt first1 = first2++;
    return std::inner_product(
        first1, --last, first2, init, reduce_op, transform_op);
}

void print_string(std::string_view str) noexcept {
    std::cout << '"';
    std::ranges::for_each(str, [](char ch) {
        switch (ch) {
        case '\\': {
            std::cout << "\\\\";
            break;
        }
        case '\"': {
            std::cout << "\\\"";
            break;
        }
        case '\t': {
            std::cout << "\\t";
            break;
        }
        case '\b': {
            std::cout << "\\b";
            break;
        }
        case '\f': {
            std::cout << "\\f";
            break;
        }
        case '\n': {
            std::cout << "\\n";
            break;
        }
        case '\033': {
            std::cout << "\\033";
            break;
        }
        case '\015': {
            std::cout << "\\015";
            break;
        }
        case '\022': {
            std::cout << "\\022";
            break;
        }
        case '\013': {
            std::cout << "\\013";
            break;
        }
        default: {
            std::cout << ch;
            break;
        }
        }
    });
    std::cout << '"' << std::endl;
}

} // namespace util
