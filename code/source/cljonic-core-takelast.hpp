#ifndef CLJONIC_CORE_TAKELAST_HPP
#define CLJONIC_CORE_TAKELAST_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_TakeLast
* The \b TakeLast function returns a \b cljonic \b Array with a specified number of the last elements in a \b cljonic
* \b collection. If the \b cljonic \b collection contains fewer than the specified number of elements then all the
* elements are returned. If the \b cljonic \b collection contains no elements, or the specified number of last elements
* is zero, then an empty \b cljonic \b Array is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    constexpr auto t0{TakeLast(0, a)};                                // immutable, empty Array
    constexpr auto t1{TakeLast(1, a)};                                // immutable, sparse Array, with 9
    constexpr auto t5{TakeLast(5, a)};                                // immutable, sparse Array, with 5 to 9
    constexpr auto t50{TakeLast(50, a)};                              // immutable, full Array, with 0 to 9
    const auto tItr5{TakeLast(5, itr)};                               // immutable, sparse Array, with 996 to 1000
    constexpr auto tEmpty{TakeLast(50, Range<0>{})};                  // immutable, empty Array
    constexpr auto tRpt7{TakeLast(50, Repeat<7, const char*>{"11"})}; // immutable, full Array, with seven "11"s
    constexpr auto tSet5{TakeLast(5, Set{'a', 'b'})};                 // immutable, sparse Array, with 'a' and 'b'
    constexpr auto tStr3{TakeLast(3, String{"Hello"})};               // immutable, sparse Array, with 'l', 'l', and 'o'

    // Compiler Error: TakeLast's second parameter must be a cljonic collection
    // constexpr auto t{TakeLast(10, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto TakeLast(const SizeType count, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "TakeLast's second parameter must be a cljonic collection");

    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    auto startIndex{(c.Count() > count) ? (c.Count() - count) : 0};
    auto cBegin{c.begin()};
    auto cEnd{c.end()};
    for (auto it{cBegin + startIndex}; it != cEnd; ++it)
        result.MAppend(*it);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_TAKELAST_HPP
