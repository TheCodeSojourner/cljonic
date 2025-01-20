#ifndef CLJONIC_COMMON_HPP
#define CLJONIC_COMMON_HPP

#include <concepts>
#include <cstring>
#include <limits>
#include "cljonic-concepts.hpp"
#include "cljonic-collection-maximum-element-count.hpp"

namespace cljonic
{

template <typename T>
class IndexInterface
{
  public:
    virtual constexpr SizeType Count() const noexcept = 0;
    virtual constexpr T operator[](const SizeType index) const noexcept = 0;
};

constexpr auto CLJONIC_INVALID_INDEX{std::numeric_limits<SizeType>::max()};

template <typename F, typename T, typename U>
constexpr bool AreEqualBy(F&& f, const T& t, const U& u) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (IsCljonicSet<T> or IsCljonicSet<U>)
    {
        auto result{t.Count() == u.Count()};
        for (SizeType i{0}; (result and (i < t.Count())); ++i)
            result = t.ContainsBy(f, u[i]);
        return result;
    }
    else if constexpr (IsCljonicCollection<T> or IsCljonicCollection<U>)
    {
        auto result{t.Count() == u.Count()};
        for (SizeType i{0}; (result and (i < t.Count())); ++i)
            result = f(t[i], u[i]);
        return result;
    }
    else
    {
        return f(t, u);
    }
}

template <typename T, typename U>
constexpr bool AreEqual(const T& t, const U& u) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (IsCString<T> and IsCString<U>)
    {
        return std::strcmp(t, u) == 0;
    }
    else if constexpr (IsCljonicSet<T> or IsCljonicSet<U>)
    {
        auto result{t.Count() == u.Count()};
        for (SizeType i{0}; (result and (i < t.Count())); ++i)
            result = t.Contains(u[i]);
        return result;
    }
    else if constexpr (IsCljonicCollection<T> or IsCljonicCollection<U>)
    {
        auto result{t.Count() == u.Count()};
        for (SizeType i{0}; (result and (i < t.Count())); ++i)
            result = AreEqual(t[i], u[i]);
        return result;
    }
    else
    {
        return t == u;
    }
}

template <typename T, typename U>
constexpr bool FirstLessThanSecond(const T& t, const U& u) noexcept
{
    if constexpr (IsCString<T> and IsCString<U>)
    {
        return std::strcmp(t, u) < 0;
    }
    else
    {
        return t < u;
    }
}

template <typename T, typename... Ts>
constexpr auto MinArgument(T a, Ts... args) noexcept
{
    if constexpr (sizeof...(args) == 0)
    {
        return a;
    }
    else
    {
        return (a < MinArgument(args...)) ? a : MinArgument(args...);
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
        return (MinArgument(C::MaximumCount(), Cs::MaximumCount()), ...);
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
    return MinArgument(count, CljonicCollectionMaximumElementCount);
}

} // namespace cljonic

#endif // CLJONIC_COMMON_HPP
