#ifndef CLJONIC_CORE_DROP_HPP
#define CLJONIC_CORE_DROP_HPP

#include "cljonic-array.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Drop
* The \b Drop function returns a \b cljonic \b Array with the same \b MaximumCount as its second parameter, which must
* be a \b cljonic \b collection, containing copies of the elements in its second parameter after skipping the number
* of elements given by its first parameter. If its second parameter contains fewer elements than the first parameter
* specifies the returned \b Array will be empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto t0{Drop(0, a)};                               // immutable, full Array, with 0 to 9
    constexpr auto t1{Drop(1, a)};                               // immutable, sparse Array, with 1 to 9
    constexpr auto t5{Drop(5, a)};                               // immutable, sparse Array, with 5 to 9
    constexpr auto t50{Drop(50, a)};                             // immutable, empty Array
    constexpr auto tEmpty{Drop(50, Range<0>{})};                 // immutable, empty Array
    constexpr auto tRpt7{Drop(5, Repeat<7, const char*>{"11"})}; // immutable, sparse Array, with two "11"s
    constexpr auto tSet5{Drop(5, Set{'a', 'b'})};                // immutable, empty Array
    constexpr auto tStr3{Drop(3, String{"Hello"})};              // immutable, sparse Array, with 'l', and 'o'

    // Compiler Error: Drop's second parameter must be a cljonic collection
    // const auto t{Drop(10, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Drop(const SizeType count, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Drop's second parameter must be a cljonic collection");

    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    for (SizeType i{count}; (i < c.Count()); ++i)
        MConj(result, c[i]);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_DROP_HPP
