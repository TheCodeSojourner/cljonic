#pragma once

#include <cljonic-filter.hpp>

#include <functional>
#include <optional>
#include <ranges>
#include <utility>

namespace cljonic::concepts_detail {

struct PredicateProducerNextAccess;

template <typename Predicate, typename Source>
concept PredicateProducerArguments =
    FilterCallable<Predicate, Source> && std::is_nothrow_copy_constructible_v<Predicate> &&
    std::is_nothrow_copy_constructible_v<Source>;

template <typename Predicate, typename Source, bool DropPrefix>
    requires PredicateProducerArguments<Predicate, Source>
class PredicateProducer {
    friend struct PredicateProducerNextAccess;

  public:
    using value_type = std::ranges::range_value_t<const Source>;

    class const_iterator {
      public:
        using value_type = PredicateProducer::value_type;
        using difference_type = std::ptrdiff_t;

        constexpr const_iterator() noexcept = default;

        constexpr const_iterator(const Source& source, const Predicate& predicate, std::size_t output_skip) noexcept {
            if constexpr (CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE != 0U) {
                predicate_.emplace(predicate);
                current_ = std::ranges::begin(source);
                end_ = std::ranges::end(source);
                remaining_ = CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
                establish_boundary();
                while (output_skip != 0U && remaining_ != 0U) {
                    --output_skip;
                    ++(*this);
                }
            }
        }

        constexpr const_iterator(const const_iterator&) noexcept = default;

        constexpr auto operator=(const const_iterator& other) noexcept -> const_iterator& {
            if (this != &other) {
                predicate_.reset();
                if (other.predicate_) {
                    predicate_.emplace(*other.predicate_);
                }
                current_ = other.current_;
                end_ = other.end_;
                remaining_ = other.remaining_;
            }
            return *this;
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> decltype(auto) {
            return *current_;
        }

        constexpr auto operator++() noexcept -> const_iterator& {
            advance_source();
            if constexpr (!DropPrefix) {
                establish_boundary();
            }
            return *this;
        }

        constexpr auto operator++(int) noexcept -> const_iterator {
            auto previous = *this;
            ++(*this);
            return previous;
        }

        [[nodiscard]] friend constexpr auto operator==(const const_iterator& iterator,
                                                       [[maybe_unused]] std::default_sentinel_t sentinel) noexcept
            -> bool {
            return iterator.remaining_ == 0U;
        }

      private:
        constexpr void advance_source() noexcept {
            if (remaining_ == 0U) {
                return;
            }
            --remaining_;
            if (remaining_ != 0U) {
                ++current_;
                check_endpoint();
            }
        }

        constexpr void check_endpoint() noexcept {
            if (current_ == end_) {
                remaining_ = 0U;
            }
        }

        constexpr void establish_boundary() noexcept {
            check_endpoint();
            if constexpr (DropPrefix) {
                skip_prefix();
            } else {
                check_prefix();
            }
        }

        constexpr void check_prefix() noexcept {
            if (remaining_ != 0U && !std::invoke(std::as_const(*predicate_), *current_)) {
                remaining_ = 0U;
            }
        }

        constexpr void skip_prefix() noexcept {
            while (remaining_ != 0U && std::invoke(std::as_const(*predicate_), *current_)) {
                advance_source();
            }
        }

        std::optional<Predicate> predicate_{};
        std::ranges::iterator_t<const Source> current_{};
        std::ranges::sentinel_t<const Source> end_{};
        std::size_t remaining_{0U};
    };

    constexpr PredicateProducer() noexcept
        requires std::is_nothrow_default_constructible_v<Source> && std::is_nothrow_default_constructible_v<Predicate>
        : source_{}, predicate_{} {
    }

    constexpr PredicateProducer(const Predicate& predicate, const Source& source) noexcept
        : source_(source), predicate_(predicate) {
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator {
        return const_iterator{source_, predicate_, output_skip_};
    }

    [[nodiscard]] constexpr auto end() const noexcept -> std::default_sentinel_t {
        return {};
    }

    [[nodiscard]] constexpr auto count() const noexcept -> std::size_t {
        std::size_t result = 0U;
        for ([[maybe_unused]] const auto& element : *this) {
            ++result;
        }
        return result;
    }

    [[nodiscard]] constexpr auto is_finite() const noexcept -> bool {
        return true;
    }

  private:
    constexpr PredicateProducer(const Predicate& predicate, const Source& source, std::size_t output_skip) noexcept
        : source_(source), predicate_(predicate), output_skip_(output_skip) {
    }

    Source source_;
    Predicate predicate_;
    std::size_t output_skip_{0U};
};

template <typename Predicate, typename Source, bool DropPrefix>
struct producer_traits<PredicateProducer<Predicate, Source, DropPrefix>> {
    static constexpr bool is_cljonic_producer = true;
    static constexpr producer_kind kind = DropPrefix ? producer_kind::drop_while : producer_kind::take_while;
};

template <typename Predicate, typename Source, bool DropPrefix>
struct contains_callable<PredicateProducer<Predicate, Source, DropPrefix>> : std::true_type {};

template <typename Predicate, typename Source, bool DropPrefix>
struct contains_floating_point<PredicateProducer<Predicate, Source, DropPrefix>> : contains_floating_point<Source> {};

template <typename Predicate, typename Source, bool DropPrefix>
struct contains_standard_range<PredicateProducer<Predicate, Source, DropPrefix>> : contains_standard_range<Source> {};

template <typename Predicate, typename Source, bool DropPrefix>
struct walker_coverage<PredicateProducer<Predicate, Source, DropPrefix>> {
    static constexpr bool declared = true;
};

} // namespace cljonic::concepts_detail

namespace cljonic {

/** \anchor TakeWhile
 * \c take_while(predicate, source) returns a producer yielding consecutive values from the start of \c source while \c
 * predicate returns \c true. The first value for which the predicate returns \c false and all later values are
 * excluded. An empty source or a predicate that returns \c false for the first value gives an empty result.
 *
 * Pass any cljonic collection or producer as the source. The predicate must return \c bool, accept a const source
 * element without throwing, and must not allocate. Declare lambdas \c noexcept and avoid side effects, as with \ref
 * FilterFunction "filter". Source and predicate must be copyable without throwing; captured lambdas are supported.
 *
 * The result keeps its own copies of the inputs and leaves them unchanged. Creating it does not evaluate values;
 * evaluation happens when you traverse it or call an observation such as \ref Count "count". Each traversal starts
 * again, so counting and then consuming can run callbacks twice. If a source callback depends on changing external
 * data, the two traversals may give different results.
 *
 * Even an unbounded source is inspected only up to the configured collection maximum. Map and Set have no guaranteed
 * traversal order, so use an ordered source when a particular prefix matters. External C++ ranges and individual
 * MapEntry values are not accepted as sources.
 *
 * \ref Next "next" and \ref Rest "rest" preserve the producer type and defer omitting one output element, without
 * evaluating callbacks at tail creation. Observation replays the original prefix within the same bounded window. A
 * \ref Cycle "Cycle" over this producer also supports tails, determining its bounded source length eagerly.
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  constexpr auto below_three = [](int n) noexcept { return n < 3; };
  constexpr auto prefix = take_while(below_three, vector(1, 2, 4, 1));
  static_assert(count(prefix) == 2);
  static_assert(equal(prefix, vector(1, 2)));
  static_assert(equal(next(prefix), vector(2)));
  static_assert(
      equal(into(vector<int, 4>(), next(cycle(prefix))), vector(2, 1, 2, 1)));
  static_assert(count(take_while(below_three, queue(1, 2, 4))) == 2);
  static_assert(count(take_while([](char c) noexcept { return c < 'c'; },
                                 string("abcd"))) == 2);
  static_assert(count(take_while([](auto) noexcept { return true; },
                                 hash_map(1, 2))) == 1);
  static_assert(count(take_while([](int) noexcept { return true; },
                                 hash_set(1, 2))) == 2);
  static_assert(count(take_while(below_three, repeat(2, 1))) == 2);
  static_assert(count(take_while(below_three, cycle(vector(1, 4)))) == 1);
  static_assert(
      count(take_while(below_three,
                       iterate([](int n) noexcept { return n + 1; }, 1))) == 2);
  static_assert(count(take_while(below_three, repeatedly(2, []() noexcept {
                                   return 1;
                                 }))) == 2);
  static_assert(count(take_while(below_three, prefix)) == 2);
  const auto limit = 3;
  const auto runtime =
      take_while([limit](int n) noexcept { return n < limit; }, range(1, 5));
  return equal(runtime, vector(1, 2)) ? 0 : 1;
}
~~~~~
 */
template <typename Predicate, typename Source>
using TakeWhile = concepts_detail::PredicateProducer<Predicate, Source, false>;

} // namespace cljonic

namespace cljonic::core {

template <typename Predicate, typename Source>
    requires concepts_detail::PredicateProducerArguments<std::decay_t<Predicate>, Source>
[[nodiscard]] constexpr auto take_while(const Predicate& predicate, const Source& source) noexcept
    -> TakeWhile<std::decay_t<Predicate>, Source> {
    return {predicate, source};
}

template <typename... Args>
constexpr auto take_while([[maybe_unused]] const Args&... args) noexcept -> concepts_detail::RejectedProducerFactory {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::take_while: expected a predicate and one cljonic source, both copyable without throwing. "
                  "The predicate must return exactly bool without throwing on a const source element, with safe "
                  "inspectable parameter conversions. External C++ ranges and MapEntry sources are not supported.");
    return {};
}

} // namespace cljonic::core
