#ifndef CLJONIC_CORE_Interpose_HPP
#define CLJONIC_CORE_Interpose_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Interpose
* The \b Interpose function returns a \b cljonic \b Array with its first parameter interposed between each element of
* its second parameter, which must be a \b cljonic \b collection. The \b MaximumCount of the return value is two times
* the \b MaximumCount of the second parameter minus one.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto i0{Interpose(1, Array{11, 12, 13})};            // 11, 1, 12, 1, 13
    constexpr auto i1{Interpose(7, Range<5>{})};                   // 0, 7, 1, 7, 2, 7, 3, 7, 4
    constexpr auto i2{Interpose(7, Repeat<4, int>{9})};            // 9, 7, 9, 7, 9, 7, 9
    constexpr auto i3{Interpose(" ", Set{"One", "Two", "Three"})}; // "One", " ", "Two", " ", "Three"
    constexpr auto i4{Interpose('-', String{"Hello"})};            // 'H', '-', 'e', '-', 'l', '-', 'l', '-', 'o'

    // Compiler Error: Interpose's second parameter must be a cljonic collection
    // constexpr auto m{Interpose(4, 4)};

    // Compiler Error: Interpose's first parameter must be convertible to cljonic collection value type
    // constexpr auto m{Interpose("Hello", Array{1, 2, 3, 4})};

return 0;
}
~~~~~
*/
template <typename T, typename C>
[[nodiscard]] constexpr auto Interpose(const T& t, const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "Interpose's second parameter must be a cljonic collection");

    static_assert(std::convertible_to<T, typename C::value_type>,
                  "Interpose's first parameter must be convertible to cljonic collection value type");

    using ResultType = typename C::value_type;
    constexpr auto collectionMaximumCount{c.MaximumCount()};
    constexpr auto resultMaximumCount{(2 * collectionMaximumCount) - 1};
    auto interposeValue{static_cast<ResultType>(t)};
    auto result{Array<ResultType, resultMaximumCount>{}};
    for (SizeType i{0}; i < collectionMaximumCount; ++i)
    {
        MConj(result, c[i]);
        MConj(result, interposeValue); // MConj will do nothing if the collection is full
    }
    return result;
}

[[nodiscard]] constexpr auto Interpose() noexcept
{
    return Array<int, 0>{};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_Interpose_HPP
