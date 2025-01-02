#ifndef CLJONIC_COMMON_HPP
#define CLJONIC_COMMON_HPP

#include <concepts>
#include <cstring>
#include "cljonic-concepts.hpp"
#include "cljonic-collection-maximum-element-count.hpp"

namespace cljonic
{

template <typename F, typename T, typename U>
constexpr bool AreEqualBy(const F& f, const T& t, const U& u) noexcept
{
    static_assert(std::predicate<F, T, U>, "Function is not a valid binary predicate for the parameters");
    return f(t, u);
}

constexpr bool AreEqual(NotCString auto a, NotCString auto b) noexcept
{
    return a == b;
}

constexpr bool AreEqual(CString auto a, CString auto b) noexcept
{
    return std::strcmp(a, b) == 0;
}

template <typename T, typename... Ts>
constexpr auto Min(T a, Ts... args) noexcept
{
    if constexpr (sizeof...(args) == 0)
    {
        return a;
    }
    else
    {
        return (a < Min(args...)) ? a : Min(args...);
    }
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

template <typename C, typename... Cs>
constexpr auto SumOfCljonicCollectionMaximumCounts()
{
    if constexpr (sizeof...(Cs) == 0)
    {
        return C::MaximumCount();
    }
    else
    {
        return (C::MaximumCount() + ... + Cs::MaximumCount());
    }
}

constexpr SizeType MaximumElements(const SizeType count) noexcept
{
    return Min(count, CljonicCollectionMaximumElementCount);
}

} // namespace cljonic

#endif // CLJONIC_COMMON_HPP
