#pragma once

#include <cljonic-concepts.hpp>

#include <concepts>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace cljonic::core {

namespace comp_detail {

struct IdentityCallable {
    template <typename T>
    [[nodiscard]] constexpr decltype(auto) operator()(T&& value) const noexcept {
        return std::forward<T>(value);
    }
};

template <typename Tuple, std::size_t Index, typename Arg>
    requires(Index < std::tuple_size_v<Tuple>)
constexpr decltype(auto) invoke_right(const Tuple& functions, Arg&& argument) noexcept {
    if constexpr (Index == std::tuple_size_v<Tuple> - 1U) {
        return std::invoke(std::get<Index>(functions), std::forward<Arg>(argument));
    } else {
        auto&& right_result = invoke_right<Tuple, Index + 1U>(functions, std::forward<Arg>(argument));
        return std::invoke(std::get<Index>(functions), std::forward<decltype(right_result)>(right_result));
    }
}

template <typename Tuple, typename Arg>
// NOLINTBEGIN(bugprone-branch-clone)
inline constexpr bool ComposesNothrowV = [] {
    constexpr std::size_t size = std::tuple_size_v<Tuple>;
    if constexpr (size == 0U) {
        return false;
    } else if constexpr (size == 1U) {
        using F0 = const std::tuple_element_t<0U, Tuple>&;
        return std::is_nothrow_invocable_v<F0, Arg>;
    } else if constexpr (size == 2U) {
        using F1 = const std::tuple_element_t<1U, Tuple>&;
        using F0 = const std::tuple_element_t<0U, Tuple>&;
        if constexpr (!std::is_nothrow_invocable_v<F1, Arg>) {
            return false;
        } else {
            using R1 = std::invoke_result_t<F1, Arg>;
            return std::is_nothrow_invocable_v<F0, R1>;
        }
    } else if constexpr (size == 3U) {
        using F2 = const std::tuple_element_t<2U, Tuple>&;
        using F1 = const std::tuple_element_t<1U, Tuple>&;
        using F0 = const std::tuple_element_t<0U, Tuple>&;
        if constexpr (!std::is_nothrow_invocable_v<F2, Arg>) {
            return false;
        } else {
            using R2 = std::invoke_result_t<F2, Arg>;
            if constexpr (!std::is_nothrow_invocable_v<F1, R2>) {
                return false;
            } else {
                using R1 = std::invoke_result_t<F1, R2>;
                return std::is_nothrow_invocable_v<F0, R1>;
            }
        }
    } else if constexpr (size == 4U) {
        using F3 = const std::tuple_element_t<3U, Tuple>&;
        using F2 = const std::tuple_element_t<2U, Tuple>&;
        using F1 = const std::tuple_element_t<1U, Tuple>&;
        using F0 = const std::tuple_element_t<0U, Tuple>&;
        if constexpr (!std::is_nothrow_invocable_v<F3, Arg>) {
            return false;
        } else {
            using R3 = std::invoke_result_t<F3, Arg>;
            if constexpr (!std::is_nothrow_invocable_v<F2, R3>) {
                return false;
            } else {
                using R2 = std::invoke_result_t<F2, R3>;
                if constexpr (!std::is_nothrow_invocable_v<F1, R2>) {
                    return false;
                } else {
                    using R1 = std::invoke_result_t<F1, R2>;
                    return std::is_nothrow_invocable_v<F0, R1>;
                }
            }
        }
    } else if constexpr (size == 5U) {
        using F4 = const std::tuple_element_t<4U, Tuple>&;
        using F3 = const std::tuple_element_t<3U, Tuple>&;
        using F2 = const std::tuple_element_t<2U, Tuple>&;
        using F1 = const std::tuple_element_t<1U, Tuple>&;
        using F0 = const std::tuple_element_t<0U, Tuple>&;
        if constexpr (!std::is_nothrow_invocable_v<F4, Arg>) {
            return false;
        } else {
            using R4 = std::invoke_result_t<F4, Arg>;
            if constexpr (!std::is_nothrow_invocable_v<F3, R4>) {
                return false;
            } else {
                using R3 = std::invoke_result_t<F3, R4>;
                if constexpr (!std::is_nothrow_invocable_v<F2, R3>) {
                    return false;
                } else {
                    using R2 = std::invoke_result_t<F2, R3>;
                    if constexpr (!std::is_nothrow_invocable_v<F1, R2>) {
                        return false;
                    } else {
                        using R1 = std::invoke_result_t<F1, R2>;
                        return std::is_nothrow_invocable_v<F0, R1>;
                    }
                }
            }
        }
    } else {
        return false;
    }
}();
// NOLINTEND(bugprone-branch-clone)

} // namespace comp_detail

/** \anchor Comp
 * The \b comp free function creates a right-to-left callable composition.
 *
 * With no functions, \c comp() returns the identity callable. With one function, it returns an equivalent callable.
 * With two or more functions, invocation executes the rightmost function first and the leftmost function last, so \c
 * comp(f, g, h)(value) is \c f(g(h(value))).
 *
 * \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 constexpr auto triple(int value) noexcept -> int { return value * 3; }

 constexpr auto count_with_bonus(const Vector<int, 4> &values) noexcept {
   return count(values) + 1U;
 }

 constexpr auto lookup_two(const Map<int, int, 2> &values) noexcept {
   return values(2);
 }

 int main() {
   constexpr auto increment = [](int value) noexcept { return value + 1; };
   constexpr auto twice_increment = comp(increment, increment);
   constexpr auto three_increment =
       comp([](int value) noexcept { return value + 1; },
            [](int value) noexcept { return value + 1; },
            [](int value) noexcept { return value + 1; });

   static_assert(comp()(42) == 42);
   static_assert(comp(increment)(5) == 6);
   static_assert(twice_increment(5) == 7);
   static_assert(three_increment(5) == 8);
   constexpr auto triple_pipeline =
       comp(triple, [](int value) noexcept { return value + 1; });
   static_assert(triple_pipeline(4) == 15);

   // cljonic::Vector: append a value, then count the updated Vector.
   constexpr auto numbers = vector<int, 4>(10, 20, 30);
   constexpr auto vector_pipeline =
       comp(count_with_bonus,
            [](const auto &values) noexcept { return conj(values, 40); });
   static_assert(vector_pipeline(numbers) == 5);

   constexpr auto scores = hash_map<int, int, 2>(1, 70, 2, 100);
   constexpr auto map_pipeline =
       comp(lookup_two, [value = 200](const auto &values) noexcept {
         return assoc(values, 2, value);
       });
   static_assert(map_pipeline(scores) == 200);

   // cljonic::Set: add a value, then count the updated Set.
   constexpr auto selected = hash_set<int, 4>(1, 2, 3);
   constexpr auto set_pipeline = comp(
       [](const auto &values) noexcept { return count(values); },
       [value = 4](const auto &values) noexcept { return conj(values, value); });
   static_assert(set_pipeline(selected) == 4);

   // cljonic::Range: materialize a producer, then count the resulting Vector.
   constexpr auto observed = range(10, 13);
   constexpr auto range_pipeline =
       comp([](const auto &values) noexcept { return count(values); },
            [](const auto &values) noexcept { return seq(take(2, values)); });
   static_assert(range_pipeline(observed) == 2);

   // cljonic::Queue: append a value, then inspect the Queue's front.
   constexpr auto queued = queue(20, 21);
   constexpr auto queue_pipeline =
       comp([](const auto &values) noexcept { return peek(values); },
            [value = 22](const auto &values) noexcept {
              return conj(values, value);
            });
   static_assert(queue_pipeline(queued) == 20);

   const auto runtime_value = 5;
   const auto runtime_result = twice_increment(runtime_value);

   const auto runtime_vector =
       vector<int, 4>(runtime_value, runtime_value + 1, runtime_value + 2);
   const auto runtime_vector_result = vector_pipeline(runtime_vector);

   const auto runtime_bonus = 300;
   const auto runtime_scores = hash_map<int, int, 2>(1, 70, 2, 100);
   const auto runtime_map_pipeline =
       comp([key = 2](const auto &values) noexcept { return values(key); },
            [bonus = runtime_bonus](const auto &values) noexcept {
              return assoc(values, 2, bonus);
            });
   const auto runtime_map_result = runtime_map_pipeline(runtime_scores);

   const auto runtime_selected = hash_set<int, 4>(1, 2, 3);
   const auto runtime_set_pipeline =
       comp([](const auto &values) noexcept { return count(values); },
            [](const auto &values) noexcept { return conj(values, 4); });
   const auto runtime_set_result = runtime_set_pipeline(runtime_selected);

   const auto runtime_start = 10;
   const auto runtime_range = range(runtime_start, runtime_start + 5);
   const auto runtime_range_pipeline =
       comp([](const auto &values) noexcept { return count(values); },
            [](const auto &values) noexcept { return take(3, values); });
   const auto runtime_range_result = runtime_range_pipeline(runtime_range);

   return comp()(42) == 42 && comp(increment)(runtime_value) == 6 &&
                  runtime_result == 7 && runtime_vector_result == 5 &&
                  runtime_map_result == 300 && runtime_set_result == 4 &&
                  runtime_range_result == 3
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename... Functions>
class Composed;

template <typename Function>
class Composed<Function> {
    using StoredFunction = std::decay_t<Function>;
    std::tuple<StoredFunction> functions_;

  public:
    constexpr explicit Composed(const Function& function)
        requires(std::is_nothrow_copy_constructible_v<StoredFunction>)
        : functions_(function) {
    }

    template <typename Arg>
        requires(comp_detail::ComposesNothrowV<std::tuple<StoredFunction>, Arg>)
    [[nodiscard]] constexpr decltype(auto) operator()(Arg&& argument) const noexcept {
        return comp_detail::invoke_right<std::tuple<StoredFunction>, 0U>(functions_, std::forward<Arg>(argument));
    }

    template <typename Arg>
        requires(!comp_detail::ComposesNothrowV<std::tuple<StoredFunction>, Arg>)
    [[nodiscard]] constexpr decltype(auto) operator()([[maybe_unused]] Arg&& argument) const {
        (void)argument;
        static_assert(
            concepts_detail::dependent_false<Arg>,
            "cljonic::comp: a function must be noexcept-invocable and the composed call chain must be compatible.");
    }
};

template <typename First, typename Second, typename... Rest>
class Composed<First, Second, Rest...> {
    using Tuple = std::tuple<std::decay_t<First>, std::decay_t<Second>, std::decay_t<Rest>...>;
    Tuple functions_;

  public:
    constexpr explicit Composed(const First& first, const Second& second, const Rest&... rest)
        requires(std::is_nothrow_copy_constructible_v<std::decay_t<First>> &&
                 std::is_nothrow_copy_constructible_v<std::decay_t<Second>> &&
                 (std::is_nothrow_copy_constructible_v<std::decay_t<Rest>> && ...))
        : functions_(first, second, rest...) {
    }

    template <typename Arg>
        requires(comp_detail::ComposesNothrowV<Tuple, Arg>)
    [[nodiscard]] constexpr decltype(auto) operator()(Arg&& argument) const noexcept {
        return comp_detail::invoke_right<Tuple, 0U>(functions_, std::forward<Arg>(argument));
    }

    template <typename Arg>
        requires(!comp_detail::ComposesNothrowV<Tuple, Arg>)
    [[nodiscard]] constexpr decltype(auto) operator()([[maybe_unused]] Arg&& argument) const {
        (void)argument;
        static_assert(
            concepts_detail::dependent_false<Arg>,
            "cljonic::comp: a function must be noexcept-invocable and the composed call chain must be compatible.");
    }
};

[[nodiscard]] constexpr auto comp() noexcept -> comp_detail::IdentityCallable {
    return {};
}

template <typename Function>
    requires(std::is_nothrow_copy_constructible_v<std::decay_t<Function>>)
[[nodiscard]] constexpr auto comp(Function&& function) noexcept -> Composed<std::decay_t<Function>> {
    return Composed<std::decay_t<Function>>{std::forward<Function>(function)};
}

template <typename First, typename Second, typename... Rest>
    requires(std::is_nothrow_copy_constructible_v<std::decay_t<First>> &&
             std::is_nothrow_copy_constructible_v<std::decay_t<Second>> &&
             (std::is_nothrow_copy_constructible_v<std::decay_t<Rest>> && ...))
[[nodiscard]] constexpr auto comp(First&& first, Second&& second, Rest&&... rest) noexcept
    -> Composed<std::decay_t<First>, std::decay_t<Second>, std::decay_t<Rest>...> {
    return Composed<std::decay_t<First>, std::decay_t<Second>, std::decay_t<Rest>...>{
        std::forward<First>(first), std::forward<Second>(second), std::forward<Rest>(rest)...};
}

template <typename Function>
    requires(!std::is_nothrow_copy_constructible_v<std::decay_t<Function>>)
constexpr auto comp([[maybe_unused]] Function&& function) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Function>,
                  "cljonic::comp: every function must be nothrow copy constructible.");
}

template <typename First, typename Second, typename... Rest>
    requires(!(std::is_nothrow_copy_constructible_v<std::decay_t<First>> &&
               std::is_nothrow_copy_constructible_v<std::decay_t<Second>> &&
               (std::is_nothrow_copy_constructible_v<std::decay_t<Rest>> && ...)))
constexpr auto comp([[maybe_unused]] First&& first, [[maybe_unused]] Second&& second,
                    [[maybe_unused]] Rest&&... rest) noexcept -> void {
    static_assert(concepts_detail::dependent_false<First, Second, Rest...>,
                  "cljonic::comp: every function must be nothrow copy constructible.");
}

} // namespace cljonic::core
