#ifndef CLJONIC_CORE_CONJ_HPP
#define CLJONIC_CORE_CONJ_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{
/** \anchor Core_Conj
* The \b Conj function returns a \b cljonic \b Array with all of the elements of its first parameter, which must be a
* \b cljonic \b collection, followed by its second parameter, which must be convertible to the value type of the
* first parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto cA{Conj(a, 15)};

    constexpr auto rng{Range<1, 5>{}};
    const auto cRng{Conj(rng, 15)};

    constexpr auto rpt{Repeat<3, int>{11}};
    const auto cRpt{Conj(rpt, 15)};

    const auto set{Set{1, 2, 1, 3, 4}};
    const auto cSet{Conj(set, 15)};

    constexpr auto str{String{"Hello"}};
    const auto cStr{Conj(str, '!')};;

    // Compiler Error: First Conj parameter must be a cljonic collection
    // const auto c{Conj("Hello", '!')};

    // Compiler Error: Second Conj parameter must be convertible to the specified cljonic collection value type
    // const auto c{Conj(Repeat<5, int>{1}, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C, typename E>
constexpr auto Conj(const C& c, const E& e) noexcept
{
    // #lizard forgives -- The length of this function is acceptable

    static_assert(IsCljonicCollection<C>, "First Conj parameter must be a cljonic collection");

    static_assert(std::convertible_to<E, typename C::value_type>,
                  "Second Conj parameter must be convertible to the specified cljonic collection value type");

    using ResultType = typename C::value_type;
    using SizeType = decltype(c.Count());
    constexpr auto count{C::MaximumCount() + 1};
    auto result{Array<ResultType, count>{}};
    for (SizeType i{0}; i < c.Count(); ++i)
        result.MConj(c[i]);
    result.MConj(e);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_CONJ_HPP
