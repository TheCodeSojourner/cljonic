#include "cljonic-no-heap-api.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto comp_probe() noexcept -> bool {
    constexpr auto zero_function_composition = cljonic::core::comp();
    constexpr auto increment = [](int value) noexcept { return value + 1; };
    constexpr auto increment_composed = cljonic::core::comp(increment);
    constexpr auto twice = cljonic::core::comp(increment, increment);
    constexpr auto thrice = cljonic::core::comp(increment, increment, increment);

    const auto value = 5;
    const auto identity_result = zero_function_composition(value);
    const auto one_result = increment_composed(value);
    const auto two_result = twice(value);
    const auto three_result = thrice(value);
    return identity_result == 5 && one_result == 6 && two_result == 7 && three_result == 8;
}

} // namespace cljonic::no_heap::probes
