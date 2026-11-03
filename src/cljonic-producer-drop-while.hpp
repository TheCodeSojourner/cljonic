#pragma once

#include <cljonic-producer-take-while.hpp>

namespace cljonic {

/** \anchor DropWhile
 * \c drop_while(predicate, source) returns a producer that skips values from the start of \c source while \c predicate
 * returns \c true. It keeps the first value for which the predicate returns \c false and every value after it, without
 * calling the predicate again. An empty source or a predicate that returns \c true for every inspected value gives an
 * empty result.
 *
 * Pass any cljonic collection or producer as the source. The predicate must return \c bool, accept a const source
 * element without throwing, and must not allocate. Declare lambdas \c noexcept and avoid side effects, as with \ref
 * FilterFunction "filter". Source and predicate must be copyable without throwing; captured lambdas are supported.
 *
 * The result keeps its own copies of the inputs and leaves them unchanged. Creating it does not evaluate values;
 * accessing its first value may scan all initial values for which the predicate returns \c true. Each traversal starts
 * again, so \ref Count "count" followed by consumption can run callbacks twice. If a source callback depends on
 * changing external data, the two traversals may give different results.
 *
 * Even an unbounded source is inspected only up to the configured collection maximum. Skipped values count toward that
 * limit: with a limit of 100 and three skipped values, at most 97 values remain. Map and Set have no guaranteed
 * traversal order, so use an ordered source when a particular suffix matters. External C++ ranges and individual
 * MapEntry values are not accepted as sources.
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;

int main() {
  constexpr auto below_three = [](int n) noexcept { return n < 3; };
  constexpr auto suffix = drop_while(below_three, vector(1, 2, 4, 1));
  static_assert(count(suffix) == 2);
  static_assert(equal(suffix, vector(4, 1)));
  static_assert(count(drop_while(below_three, queue(1, 2, 4))) == 1);
  static_assert(count(drop_while([](char c) noexcept { return c < 'c'; },
                                 string("abcd"))) == 2);
  static_assert(count(drop_while([](auto) noexcept { return false; },
                                 hash_map(1, 2))) == 1);
  static_assert(count(drop_while([](int) noexcept { return false; },
                                 hash_set(1, 2))) == 2);
  static_assert(count(drop_while(below_three, repeat(2, 1))) == 0);
  static_assert(
      equal(take(1, drop_while(below_three, cycle(vector(1, 4)))), vector(4)));
  static_assert(equal(
      take(1, drop_while(below_three,
                         iterate([](int n) noexcept { return n + 1; }, 1))),
      vector(3)));
  static_assert(count(drop_while(below_three, repeatedly(2, []() noexcept {
                                   return 1;
                                 }))) == 0);
  static_assert(count(drop_while(below_three, suffix)) == 2);
  const auto limit = 3;
  const auto runtime =
      drop_while([limit](int n) noexcept { return n < limit; }, range(1, 5));
  return equal(runtime, vector(3, 4)) ? 0 : 1;
}
~~~~~
 */
template <typename Predicate, typename Source>
using DropWhile = concepts_detail::PredicateProducer<Predicate, Source, true>;

template <typename Predicate, typename Source>
    requires concepts_detail::PredicateProducerArguments<std::decay_t<Predicate>, Source>
[[nodiscard]] constexpr auto drop_while(const Predicate& predicate, const Source& source) noexcept
    -> DropWhile<std::decay_t<Predicate>, Source> {
    return {predicate, source};
}

template <typename... Args>
constexpr auto drop_while([[maybe_unused]] const Args&... args) noexcept -> concepts_detail::RejectedProducerFactory {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::drop_while: expected a predicate and one cljonic source, both copyable without throwing. "
                  "The predicate must return exactly bool without throwing on a const source element, with safe "
                  "inspectable parameter conversions. External C++ ranges and MapEntry sources are not supported.");
    return {};
}

} // namespace cljonic
