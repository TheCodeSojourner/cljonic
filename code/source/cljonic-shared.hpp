#ifndef CLJONIC_COMMON_HPP
#define CLJONIC_COMMON_HPP

#include <concepts>
#include <cstring>
#include "cljonic-concepts.hpp"

namespace cljonic
{

template <typename F, typename T, typename U>
bool AreEqualBy(const F& f, const T& t, const U& u)
{
    static_assert(std::predicate<F, T, U>, "Function is not a valid binary predicate for the parameters");
    return f(t, u);
}

bool AreEqual(NotCString auto a, NotCString auto b)
{
    return a == b;
}

bool AreEqual(CString auto a, CString auto b)
{
    return std::strcmp(a, b) == 0;
}

} // namespace cljonic

#endif // CLJONIC_COMMON_HPP
