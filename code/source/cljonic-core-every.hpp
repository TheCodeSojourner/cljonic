#ifndef CLJONIC_CORE_EVERY_HPP
#define CLJONIC_CORE_EVERY_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Every
* The \b Every function returns true if the first parameter, which must be a \b unary \b predicte, returns true when
* called with every element of the second parameter, which must be a \b cljonic collection, else false. Returns false if
* second parameter is an empty \b cljonic collection.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Less10 = [](const int i){ return i < 10; };
    const auto b0{Every(Less10, Array{})}; // true
    const auto b1{Every(Less10, Array{10})}; // false
    const auto b2{Every(Less10, Array{1, 2, 3, 4, 5, 6, 7, 8, 9})}; // true

    return 0;
}
~~~~~
*/
template <typename F, typename T>
auto Every(const F& f, const T& t) noexcept
{
    static_assert(IsUnaryPredicate<F, typename T::value_type>,
                  "Function is not a valid unary predicate for the collection value type");
    static_assert(IsCljonicCollection<T>, "cljonic collection required");
    using CountType = decltype(t.Count());
    auto result{true};
    for (CountType i = 0; (result and (i < t.Count())); ++i)
        result = f(t[i]);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EVERY_HPP
