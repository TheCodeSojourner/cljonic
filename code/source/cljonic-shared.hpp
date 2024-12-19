#ifndef CLJONIC_COMMON_HPP
#define CLJONIC_COMMON_HPP

#include <concepts>
#include <cstring>
#include "cljonic-concepts.hpp"

namespace cljonic
{

template <typename F, typename T, typename U>
auto AreEqualBy(const F& f, const T& t, const U& u)
{
    static_assert(std::predicate<F, T, U>, "Function is not a valid binary predicate for the parameters");
    return f(t, u);
}

template <typename T, typename U>
auto AreEqual(const T& t, const U& u)
{
    if constexpr (CString<T> and CString<U>)
        return (std::strcmp(t, u) == 0);
    else
    {
        static_assert((not std::floating_point<T>) and (not std::floating_point<U>),
                      "Floating point types should not be equality compared");
        static_assert(std::equality_comparable_with<T, U>, "Types are not equality comparable");
        return (t == u);
    }
}

} // namespace cljonic

#endif // CLJONIC_COMMON_HPP
