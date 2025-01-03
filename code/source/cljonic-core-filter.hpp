#ifndef CLJONIC_CORE_FILTER_HPP
#define CLJONIC_CORE_FILTER_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Filter
* The \b Filter function calls its first parameter, which must be a \b unary \b predicate, with each element of its
* second parameter, which must be a \b cljonic \b collection, and if the result is true a copy of the element is
* concatenated to the end of \b Filter's result, which is a \b cljonic \b Array with the same \b MaximumCount as the
* second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

    const auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    const auto fA{Filter(Even, a)};                                 // immutable, sparse Array, with 0, 2, 4, 6, and 8

    const auto rng{Range<10>{}};
    const auto fRng{Filter(Even, rng)};                             // immutable, sparse Array, with 0, 2, 4, 6, and 8

    const auto rpt{Repeat<10, int>{1}};
    const auto fRpt{Filter(Even, rpt)};                             // immutable, empty Array

    const auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    const auto fS{Filter(Even, s)};                                 // immutable, sparse Array, with 2 and 4

    const auto fStr{Filter([](const char c) { return ('l' == c); }, // immutable, sparse Array, with 'l' and 'l'
                           String{"Hello"})};

    // Compiler Error: Filter's second parameter must be a cljonic collection
    // const auto tw{Filter(Even, "Hello")};

    // Compiler Error: Filter's function is not a valid unary predicate for the collection value type
    // const auto tw{Filter(Even, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto Filter(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Filter's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "Filter's function is not a valid unary predicate for the collection value type");

    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    for (const auto& element : c)
        if (f(element))
            result.MConj(element);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_FILTER_HPP
