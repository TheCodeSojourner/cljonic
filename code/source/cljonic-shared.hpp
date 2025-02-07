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
    virtual constexpr bool ElementAtIndexIsEqualToElement(const SizeType index, const T& element) const noexcept = 0;
};

template <typename F, typename T, typename U>
constexpr bool AreEqualBy(F&& f, const T& t, const U& u) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (IsCljonicCollection<T> and IsCljonicCollection<U>)
    {
        auto result{t.Count() == u.Count()};
        auto tBegin{t.begin()};
        auto tEnd{t.end()};
        auto uIt{u.begin()};
        for (auto it{tBegin}; (result and (it != tEnd)); ++it, ++uIt)
            result = f(*it, *uIt);
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
        if constexpr (IsCljonicSet<T>)
        {
            auto result{t.Count() == u.Count()};
            auto uBegin{u.begin()};
            auto uEnd{u.end()};
            for (auto it{uBegin}; (result and (it != uEnd)); ++it)
                result = t.Contains(*it);
            return result;
        }
        else
        {
            auto result{t.Count() == u.Count()};
            auto tBegin{t.begin()};
            auto tEnd{t.end()};
            for (auto it{tBegin}; (result and (it != tEnd)); ++it)
                result = u.Contains(*it);
            return result;
        }
    }
    else if constexpr (IsCljonicCollection<T> and IsCljonicCollection<U>)
    {
        auto result{t.Count() == u.Count()};
        auto tBegin{t.begin()};
        auto tEnd{t.end()};
        auto uIt{u.begin()};
        for (auto it{tBegin}; (result and (it != tEnd)); ++it, ++uIt)
            result = AreEqual(*it, *uIt);
        return result;
    }
    else if constexpr ((not IsCljonicCollection<T>) and (not IsCljonicCollection<U>))
    {
        return t == u;
    }
    else
    {
        static_assert(false, "AreEqual arguments are not comparable");
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
