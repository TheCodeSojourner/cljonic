#ifndef CLJONIC_CORE_TAKEWHILE_HPP
#define CLJONIC_CORE_TAKEWHILE_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_TakeWhile
* The \b TakeWhile function calls its first parameter, which must be a \b unary \b predicate, with each element of its
* second parameter, which must be a \b cljonic \b collection, and while the result is true a copy of the element is
* concatenated to the end of \b TakeWhile's result, which is a \b cljonic \b Array with the same \b MaximumCount as the
* second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

    const auto a{Array<int, 10>{0, 2, 4, 5, 6, 7, 8, 9}};
    const auto twA{TakeWhile(Even, a)}; // immutable, sparse Array, with 0, 2, and 4

    const auto rng{Range<10>{}};
    const auto twRng{TakeWhile(Even, rng)}; // immutable, sparse Array, with 0

    const auto rpt{Repeat<10, int>{2}};
    const auto twRpt{TakeWhile(Even, rpt)}; // immutable, full array, of 2s

    const auto s{Set<int, 4>{2, 4, 5, 6}};
    const auto twS{TakeWhile(Even, s)}; // immutable, sparse Array, with 2 and 4

    const auto twStr{TakeWhile([](const char c) { return ('l' != c); }, // immutable, sparse Array, with 'H' and 'e'
                               String{"Hello"})};

    // Compiler Error: TakeWhile's second parameter must be a cljonic collection
    // const auto tw{TakeWhile(Even, "Hello")};

    // Compiler Error: TakeWhile's function is not a valid unary predicate for the collection value type
    // const auto tw{TakeWhile(Even, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto TakeWhile(const F& f, const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "TakeWhile's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<F, typename C::value_type>,
                  "TakeWhile's function is not a valid unary predicate for the collection value type");

    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    for (const auto& element : c)
        if (f(element))
            result.MConj(element);
        else
            break;
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_TAKEWHILE_HPP