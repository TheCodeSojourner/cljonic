#ifndef CLJONIC_CORE_DROPLAST_HPP
#define CLJONIC_CORE_DROPLAST_HPP

#include "cljonic-array.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_DropLast
* The \b DropLast function returns a \b cljonic \b Array with the same \b MaximumCount as its second parameter, which
* must be a \b cljonic \b collection, containing copies of the elements in its second parameter except for the last
* number of elements given by its first parameter. If its second parameter contains fewer elements than the first
* parameter specifies the returned \b Array will be empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto t0{DropLast(0, a)};                                             // full Array, with 0 to 9
    constexpr auto t1{DropLast(1, a)};                                             // sparse Array, with 0 to 8
    constexpr auto t5{DropLast(5, a)};                                             // sparse Array, with 0 to 4
    constexpr auto t50{DropLast(50, a)};                                           // empty Array
    const auto tItr{DropLast(990, Iterate([](const int i) { return i + 1; }, 0))}; // sparse Array, with 0 to 9
    constexpr auto tEmpty{DropLast(50, Range<0>{})};                               // empty Array
    constexpr auto tRpt7{DropLast(5, Repeat<7, const char*>{"11"})};               // sparse Array, with two "11"s
    constexpr auto tSet5{DropLast(5, Set{'a', 'b'})};                              // empty Array
    constexpr auto tStr3{DropLast(3, String{"Hello"})};                            // sparse Array, with 'H', and 'e'

    // Compiler Error: DropLast's second parameter must be a cljonic collection
    // constexpr auto t{DropLast(10, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto DropLast(const SizeType count, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "DropLast's second parameter must be a cljonic collection");

    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    auto takeCount{(count > c.Count()) ? 0 : (c.Count() - count)};
    for (auto it{c.begin()}; (takeCount > 0); ++it)
    {
        MConj(result, *it);
        --takeCount;
    }
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_DROPLAST_HPP
