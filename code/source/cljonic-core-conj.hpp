#ifndef CLJONIC_CORE_CONJ_HPP
#define CLJONIC_CORE_CONJ_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{
/** \anchor Core_Conj
* The \b Conj function returns a \b cljonic \b Array with all of the elements of its first parameter, which must be a
* \b cljonic \b collection, followed by its second through last parameters, which all must be convertible to the value
* type of the first parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto cA{Conj(a, 15, 16)};

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto cRng{Conj(rng, 15)};

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto cRpt{Conj(rpt, 15)};

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto cSet{Conj(set, 15)};

    constexpr auto str{String{"Hello"}};
    constexpr auto cStr{Conj(str)};;
    constexpr auto cStrBang{Conj(str, '!', '!', '!')};;

    // Compiler Error: First Conj parameter must be a cljonic collection
    // const auto c{Conj("Hello", '!')};

    // Compiler Error: Second through last Conj parameters must be convertible to cljonic collection value type
    // const auto c{Conj(Repeat<5, int>{1}, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C, typename... Es>
constexpr auto Conj(const C& c, const Es&... es) noexcept
{
    // #lizard forgives -- The length of this function is acceptable

    static_assert(IsCljonicCollection<C>, "First Conj parameter must be a cljonic collection");

    static_assert(AllConvertibleTypes<typename C::value_type, Es...>,
                  "Second through last Conj parameters must be convertible to cljonic collection value type");

    using ResultType = typename C::value_type;

    constexpr auto count{C::MaximumCount() + sizeof...(Es)};
    auto result{Array<ResultType, count>{}};
    const auto MConjElementOntoResult = [&](const auto& e) { MConj(result, e); };
    for (SizeType i{0}; i < c.Count(); ++i)
        MConjElementOntoResult(c[i]);
    (MConjElementOntoResult(es), ...);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_CONJ_HPP
