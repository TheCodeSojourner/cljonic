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
    constexpr auto no_initial_ok = cljonic::reduce(add, cljonic::vector(1, 2, 3)) == 6 &&
                                   cljonic::reduce(subtract, cljonic::vector(1, 2, 3)) == -4 &&
                                   cljonic::reduce(add, cljonic::vector<int, 0>()) == 0 &&
                                   cljonic::reduce(add, cljonic::queue(1, 2, 3)) == 6;

    // Explicit-initial form: the initial value seeds the accumulator.
    constexpr auto char_accumulator = [](char accumulator, int element) noexcept {
        return static_cast<char>(accumulator + element);
    };
    constexpr auto int_acc_long_element = [](int accumulator, long element) noexcept {
        return accumulator + (element > 0L ? 1 : 0);
    };
    constexpr auto with_initial_ok = cljonic::reduce(add, 10, cljonic::vector(1, 2, 3)) == 16 &&
                                     cljonic::reduce(add, 10, cljonic::vector<int, 0>()) == 10 &&
                                     cljonic::reduce(subtract, 10, cljonic::vector(1, 2, 3)) == 4 &&
                                     cljonic::reduce(char_accumulator, 'a', cljonic::vector(1, 2)) == 'd' &&
                                     cljonic::reduce(int_acc_long_element, 5, cljonic::vector(10L, 20L)) == 7;

    // Every admitted collection family, including unordered Map and Set sources.
    constexpr auto collection_families_ok = cljonic::reduce(entry_total, 0, cljonic::hash_map(1, 10, 2, 20)) == 30 &&
                                            cljonic::reduce(add, 0, cljonic::hash_set(1, 2, 3)) == 6 &&
                                            cljonic::reduce(maximum_char, cljonic::string<4>("abc")) == 'c';

    // Every admitted producer family, finite and unbounded; the unbounded cases
    // fold the saturated observable count, so every call terminates.
    constexpr auto producer_families_ok =
        cljonic::reduce(add, cljonic::Range<int>{1, 4, 1}) == 6 && cljonic::reduce(add, cljonic::repeat(3, 7)) == 21 &&
        cljonic::reduce(add, 0, cljonic::repeatedly(2, []() noexcept { return 9; })) == 18 &&
        cljonic::reduce(add, 0, cljonic::cycle(cljonic::vector(1, 2))) == 1500 &&
        cljonic::reduce(add, 0, cljonic::iterate(increment, 0)) == 499500;

    // A wider accumulator admits each element through a safe conversion.
    constexpr auto conversion_ok =
        cljonic::reduce([](long lhs, long rhs) noexcept { return lhs + rhs; }, cljonic::vector(1, 2, 3)) == 6L;

    // A generic callable is admitted by the explicit-initial form.
    constexpr auto uninspectable_ok =
        cljonic::reduce([](auto lhs, auto rhs) noexcept { return lhs + rhs; }, 5, cljonic::vector(1, 2, 3)) == 11;

    constexpr auto preserved_vector = cljonic::vector(1, 2, 3);
    constexpr auto preserve_ok = cljonic::reduce(add, preserved_vector) == 6 &&
                                 cljonic::equal(preserved_vector, cljonic::vector(1, 2, 3)) &&
                                 cljonic::count(cljonic::queue(1, 2, 3)) == 3;

    // Runtime probe defeats constexpr folding so the no-heap artifact truly
    // executes the fold at runtime.
    volatile int base_raw = 1;
    const auto base = base_raw;
    const auto runtime_values = cljonic::vector(base, base + 1, base + 2);
    const auto runtime_empty = cljonic::vector<int, 0>();
    const auto runtime_set = cljonic::hash_set(base, base + 1, base + 2);
    const auto runtime_map = cljonic::hash_map(base, base * 10, base + 1, (base + 1) * 10);
    const auto runtime_repeat = cljonic::repeat(2, base);
    auto runtime_ok = cljonic::reduce(add, runtime_empty) == 0 &&
                      cljonic::reduce(add, runtime_values) == base * 3 + 3 &&
                      cljonic::equal(runtime_values, cljonic::vector(base, base + 1, base + 2)) &&
                      cljonic::reduce(add, base * 10, runtime_values) == base * 13 + 3 &&
                      cljonic::reduce(add, 0, runtime_set) == base * 3 + 3 &&
                      cljonic::equal(runtime_set, cljonic::hash_set(base, base + 1, base + 2)) &&
                      cljonic::reduce(entry_total, 0, runtime_map) == base * 20 + 10 &&
                      cljonic::equal(runtime_map, cljonic::hash_map(base, base * 10, base + 1, (base + 1) * 10)) &&
                      cljonic::reduce(add, runtime_repeat) == base * 2 && cljonic::count(runtime_repeat) == 2 &&
                      *runtime_repeat.begin() == base;

    return no_initial_ok && with_initial_ok && collection_families_ok && producer_families_ok && conversion_ok &&
           uninspectable_ok && preserve_ok && runtime_ok;
}

} // namespace cljonic::no_heap::probes
