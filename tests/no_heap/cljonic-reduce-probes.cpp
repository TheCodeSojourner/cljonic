#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto reduce_function_probe() noexcept -> bool {
    // Reduction (REQ-FN-034): both arities, every admitted source family, the
    // saturation boundary, and input preservation. All calls are constexpr-capable,
    // non-allocating, and noexcept.
    constexpr auto add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
    constexpr auto subtract = [](int lhs, int rhs) noexcept { return lhs - rhs; };
    constexpr auto maximum_char = [](char lhs, char rhs) noexcept { return lhs > rhs ? lhs : rhs; };
    constexpr auto entry_total = [](int total, const auto& entry) noexcept { return total + entry.value; };
    constexpr auto increment = [](int value) noexcept { return value + 1; };

    // No-initial form: the first element seeds the accumulator.
    constexpr auto no_initial_ok = cljonic::core::reduce(add, cljonic::core::vector(1, 2, 3)) == 6 &&
                                   cljonic::core::reduce(subtract, cljonic::core::vector(1, 2, 3)) == -4 &&
                                   cljonic::core::reduce(add, cljonic::core::vector<int, 0>()) == 0 &&
                                   cljonic::core::reduce(add, cljonic::core::queue(1, 2, 3)) == 6;

    // Explicit-initial form: the initial value seeds the accumulator.
    constexpr auto char_accumulator = [](char accumulator, int element) noexcept {
        return static_cast<char>(accumulator + element);
    };
    constexpr auto int_acc_long_element = [](int accumulator, long element) noexcept {
        return accumulator + (element > 0L ? 1 : 0);
    };
    constexpr auto with_initial_ok =
        cljonic::core::reduce(add, 10, cljonic::core::vector(1, 2, 3)) == 16 &&
        cljonic::core::reduce(add, 10, cljonic::core::vector<int, 0>()) == 10 &&
        cljonic::core::reduce(subtract, 10, cljonic::core::vector(1, 2, 3)) == 4 &&
        cljonic::core::reduce(char_accumulator, 'a', cljonic::core::vector(1, 2)) == 'd' &&
        cljonic::core::reduce(int_acc_long_element, 5, cljonic::core::vector(10L, 20L)) == 7;

    // Every admitted collection family, including unordered Map and Set sources.
    constexpr auto collection_families_ok =
        cljonic::core::reduce(entry_total, 0, cljonic::core::hash_map(1, 10, 2, 20)) == 30 &&
        cljonic::core::reduce(add, 0, cljonic::core::hash_set(1, 2, 3)) == 6 &&
        cljonic::core::reduce(maximum_char, cljonic::core::string<4>("abc")) == 'c';

    // Every admitted producer family, finite and unbounded; the unbounded cases
    // fold the saturated observable count, so every call terminates.
    constexpr auto producer_families_ok =
        cljonic::core::reduce(add, cljonic::Range<int>{1, 4, 1}) == 6 &&
        cljonic::core::reduce(add, cljonic::core::repeat(3, 7)) == 21 &&
        cljonic::core::reduce(add, 0, cljonic::core::repeatedly(2, []() noexcept { return 9; })) == 18 &&
        cljonic::core::reduce(add, 0, cljonic::core::cycle(cljonic::core::vector(1, 2))) == 1500 &&
        cljonic::core::reduce(add, 0, cljonic::core::iterate(increment, 0)) == 499500;

    // A wider accumulator admits each element through a safe conversion.
    constexpr auto conversion_ok = cljonic::core::reduce([](long lhs, long rhs) noexcept { return lhs + rhs; },
                                                         cljonic::core::vector(1, 2, 3)) == 6L;

    // A generic callable is admitted by the explicit-initial form.
    constexpr auto uninspectable_ok = cljonic::core::reduce([](auto lhs, auto rhs) noexcept { return lhs + rhs; }, 5,
                                                            cljonic::core::vector(1, 2, 3)) == 11;

    constexpr auto preserved_vector = cljonic::core::vector(1, 2, 3);
    constexpr auto preserve_ok = cljonic::core::reduce(add, preserved_vector) == 6 &&
                                 cljonic::core::equal(preserved_vector, cljonic::core::vector(1, 2, 3)) &&
                                 cljonic::core::count(cljonic::core::queue(1, 2, 3)) == 3;

    // Runtime probe defeats constexpr folding so the no-heap artifact truly
    // executes the fold at runtime.
    volatile int base_raw = 1;
    const auto base = base_raw;
    const auto runtime_values = cljonic::core::vector(base, base + 1, base + 2);
    const auto runtime_empty = cljonic::core::vector<int, 0>();
    const auto runtime_set = cljonic::core::hash_set(base, base + 1, base + 2);
    const auto runtime_map = cljonic::core::hash_map(base, base * 10, base + 1, (base + 1) * 10);
    const auto runtime_repeat = cljonic::core::repeat(2, base);
    auto runtime_ok =
        cljonic::core::reduce(add, runtime_empty) == 0 && cljonic::core::reduce(add, runtime_values) == base * 3 + 3 &&
        cljonic::core::equal(runtime_values, cljonic::core::vector(base, base + 1, base + 2)) &&
        cljonic::core::reduce(add, base * 10, runtime_values) == base * 13 + 3 &&
        cljonic::core::reduce(add, 0, runtime_set) == base * 3 + 3 &&
        cljonic::core::equal(runtime_set, cljonic::core::hash_set(base, base + 1, base + 2)) &&
        cljonic::core::reduce(entry_total, 0, runtime_map) == base * 20 + 10 &&
        cljonic::core::equal(runtime_map, cljonic::core::hash_map(base, base * 10, base + 1, (base + 1) * 10)) &&
        cljonic::core::reduce(add, runtime_repeat) == base * 2 && cljonic::core::count(runtime_repeat) == 2 &&
        *runtime_repeat.begin() == base;

    return no_initial_ok && with_initial_ok && collection_families_ok && producer_families_ok && conversion_ok &&
           uninspectable_ok && preserve_ok && runtime_ok;
}

} // namespace cljonic::no_heap::probes
