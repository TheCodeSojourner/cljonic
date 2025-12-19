#ifndef CLJONIC_CORE_DROPWHILE_HPP
#define CLJONIC_CORE_DROPWHILE_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_DropWhile
* The \b DropWhile function calls its first parameter, which must be a \b unary \b predicate, with each element of its
* second parameter, which must be a \b cljonic \b collection. While the call returns true elements are ignored, but
* the rest of the elements are copied to the result, which is a \b cljonic \b collecion.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

    constexpr auto a{Array<int, 10>{0, 2, 4, 5, 6, 7, 8, 9}};
    constexpr auto dwA{DropWhile(Even, a)}; // immutable, sparse Array, with 0, 2, and 4

    constexpr auto rng{Range<10>{}};
    constexpr auto dwRng{DropWhile(Even, rng)}; // immutable, sparse Array, with 1 to 9

    constexpr auto rpt{Repeat<10, int>{2}};
    constexpr auto dwRpt{DropWhile(Even, rpt)}; // immutable, empty Array

    constexpr auto s{Set<int, 4>{2, 4, 5, 6}};
    constexpr auto dwS{DropWhile(Even, s)}; // immutable, sparse Array, with 5 and 6

    constexpr auto dwStr{DropWhile([](const char c)
                                   { return ('l' != c); }, // immutable, sparse Array, with 'l' 'l' and 'o'
                                   String{"Hello"})};

    // Compiler Error: DropWhile's second parameter must be a cljonic collection
    // constexpr auto dw{DropWhile(Even, "Hello")};

    // Compiler Error: DropWhile's function is not a valid unary predicate for the collection value type
    // constexpr auto dw{DropWhile(Even, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
[[nodiscard]] constexpr auto DropWhile(F&& f, const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "DropWhile's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "DropWhile's function is not a valid unary predicate for the collection value type");

    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    auto conjElement(false);
    for (const auto& element : c)
        if (conjElement |= (not f(element)))
            MConj(result, element);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_DROPWHILE_HPP
