#ifndef CLJONIC_CORE_DEDUPE_HPP
#define CLJONIC_CORE_DEDUPE_HPP

#include "cljonic-core-dedupeby.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Dedupe
* The \b Dedupe function returns a \b cljonic \b Array of elements from a \b cljonic \b collection such that consecutive
* duplicates are removed.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a0{Array<int, 5>{}};
    constexpr auto d0{Dedupe(a0)};

    constexpr auto a1{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto d1{Dedupe(a1)};

    constexpr auto a2{Array{0, 0, 0, 0}};
    constexpr auto d2{Dedupe(a2)};

    constexpr auto a3{Array{0, 0, 0, 0, 1, 2, 3, 4, 5}};
    constexpr auto d3{Dedupe(a3)};

    constexpr auto a4{Array{0, 1, 2, 3, 4, 5, 5, 5, 5}};
    constexpr auto d4{Dedupe(a4)};

    constexpr auto a5{Array{0, 1, 2, 3, 3, 3, 3, 4, 5}};
    constexpr auto d5{Dedupe(a5)};

    constexpr auto a6{Array{0, 1, 1, 2, 3, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9}};
    constexpr auto d6{Dedupe(a6)};

    // Compiler Error: Dedupe's parameter must be a cljonic collection
    // constexpr auto d{Dedupe(1)};

    // Compiler Error: Dedupe should not compare cljonic floating point collection value types for equality
    // constexpr auto d{Dedupe(Array{1.1, 1.1})};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Dedupe(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Dedupe's parameter must be a cljonic collection");

    static_assert((not std::floating_point<typename C::value_type>),
                  "Dedupe should not compare cljonic floating point collection value types for equality");

    return DedupeBy([](const auto& a, const auto& b) { return AreEqual(a, b); }, c);
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_DEDUPE_HPP
