#ifndef CLJONIC_COMMON_HPP
#define CLJONIC_COMMON_HPP

#include <concepts>
#include <cstring>
#include "cljonic-concepts.hpp"

namespace cljonic
{

template <typename F, typename T, typename U>
bool AreEqualBy(const F& f, const T& t, const U& u) noexcept
{
    static_assert(std::predicate<F, T, U>, "Function is not a valid binary predicate for the parameters");
    return f(t, u);
}

bool AreEqual(NotCString auto a, NotCString auto b) noexcept
{
    return a == b;
}

bool AreEqual(CString auto a, CString auto b) noexcept
{
    return std::strcmp(a, b) == 0;
}

constexpr auto Min(auto a, auto b) noexcept
{
    return (a < b) ? a : b;
}

template <typename C, typename... Cs>
constexpr auto MinimumOfCljonicCollectionMaximumCounts()
{
    if constexpr (sizeof...(Cs) == 0)
    {
        return C::MaximumCount();
    }
    else
    {
        return (Min(C::MaximumCount(), Cs::MaximumCount()), ...);
    }
}

} // namespace cljonic

#endif // CLJONIC_COMMON_HPP
