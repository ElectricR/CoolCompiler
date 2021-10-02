#pragma once
#include <numeric>

namespace util {

template< class InputIt, class T,
          class BinaryOperation1, class BinaryOperation2 >
constexpr auto adjacent_reduce( InputIt first2, InputIt last,
                           T init,
                           BinaryOperation1 reduce_op,
                           BinaryOperation2 transform_op ) 
{
    InputIt first1 = first2++;
    return std::inner_product(first1, --last, first2, init, reduce_op, transform_op);
}

} // namespace util
