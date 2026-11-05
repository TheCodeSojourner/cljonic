#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto filterv_probe() noexcept -> bool {
    constexpr auto even = [](int value) noexcept { return value % 2 == 0; };
    constexpr auto entry_even = [](const auto& entry) noexcept { return entry.value % 2 == 0; };
    constexpr auto increment = [](int value) noexcept { return value + 1; };
    constexpr auto char_b = [](char value) noexcept { return value == 'b'; };

    constexpr auto compile_time_ok =
        cljonic::core::equal(cljonic::core::filterv(even, cljonic::core::vector(1, 2, 3, 4)),
                             cljonic::core::filter(even, cljonic::core::vector(1, 2, 3, 4))) &&
        cljonic::core::filterv(even, cljonic::Vector<int, 0>{}).count() == 0 &&
        cljonic::core::filterv(entry_even, cljonic::core::hash_map(1, 2, 2, 3, 3, 4)).count() == 2 &&
        cljonic::core::filterv(even, cljonic::core::hash_set(1, 2, 3, 4)).count() == 2 &&
        cljonic::core::equal(cljonic::core::filterv(even, cljonic::core::queue(1, 2, 3, 4)),
                             cljonic::core::vector(2, 4)) &&
        cljonic::core::equal(cljonic::core::filterv(char_b, cljonic::core::string<4>("abc")),
                             cljonic::core::vector('b')) &&
        cljonic::core::equal(cljonic::core::filterv(even, cljonic::Range<int>{1, 5, 1}), cljonic::core::vector(2, 4)) &&
        cljonic::core::equal(cljonic::core::filterv(even, cljonic::core::repeat(3, 2)),
                             cljonic::core::vector(2, 2, 2)) &&
        cljonic::core::filterv(even, cljonic::core::cycle(cljonic::core::vector(1, 2))).count() == 500 &&
        cljonic::core::filterv(even, cljonic::core::iterate(increment, 0)).count() == 500 &&
        cljonic::core::equal(cljonic::core::filterv(even, cljonic::core::repeatedly(3, []() noexcept { return 2; })),
                             cljonic::core::vector(2, 2, 2));

    volatile int base_raw = 1;
    const auto base = base_raw;
    const auto source = cljonic::core::vector(base, base + 1, base + 2, base + 3);
    const auto filtered = cljonic::core::filterv(even, source);
    const auto runtime_ok =
        cljonic::core::equal(filtered, cljonic::core::filter(even, source)) &&
        cljonic::core::equal(source, cljonic::core::vector(base, base + 1, base + 2, base + 3)) &&
        cljonic::core::filterv(entry_even, cljonic::core::hash_map(base, base + 1, base + 1, base + 2)).count() == 1 &&
        cljonic::core::filterv(even, cljonic::core::hash_set(base, base + 1, base + 2)).count() == 1 &&
        cljonic::core::equal(cljonic::core::filterv(even, cljonic::core::queue(base, base + 1, base + 2)),
                             cljonic::core::vector(base + 1)) &&
        cljonic::core::equal(cljonic::core::filterv(char_b, cljonic::core::string<4>("abc")),
                             cljonic::core::vector('b')) &&
        cljonic::core::filterv(even, cljonic::Range<int>{base, base + 4, 1}).count() == 2 &&
        cljonic::core::filterv(even, cljonic::core::repeat(2, base + 1)).count() == 2 &&
        cljonic::core::filterv(even, cljonic::core::repeatedly(3, []() noexcept { return 2; })).count() == 3;

    return compile_time_ok && runtime_ok;
}

} // namespace cljonic::no_heap::probes
