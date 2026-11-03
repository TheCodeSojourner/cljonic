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
        cljonic::equal(cljonic::filterv(even, cljonic::vector(1, 2, 3, 4)),
                       cljonic::filter(even, cljonic::vector(1, 2, 3, 4))) &&
        cljonic::filterv(even, cljonic::Vector<int, 0>{}).count() == 0 &&
        cljonic::filterv(entry_even, cljonic::hash_map(1, 2, 2, 3, 3, 4)).count() == 2 &&
        cljonic::filterv(even, cljonic::hash_set(1, 2, 3, 4)).count() == 2 &&
        cljonic::equal(cljonic::filterv(even, cljonic::queue(1, 2, 3, 4)), cljonic::vector(2, 4)) &&
        cljonic::equal(cljonic::filterv(char_b, cljonic::string<4>("abc")), cljonic::vector('b')) &&
        cljonic::equal(cljonic::filterv(even, cljonic::Range<int>{1, 5, 1}), cljonic::vector(2, 4)) &&
        cljonic::equal(cljonic::filterv(even, cljonic::repeat(3, 2)), cljonic::vector(2, 2, 2)) &&
        cljonic::filterv(even, cljonic::cycle(cljonic::vector(1, 2))).count() == 500 &&
        cljonic::filterv(even, cljonic::iterate(increment, 0)).count() == 500 &&
        cljonic::equal(cljonic::filterv(even, cljonic::repeatedly(3, []() noexcept { return 2; })),
                       cljonic::vector(2, 2, 2));

    volatile int base_raw = 1;
    const auto base = base_raw;
    const auto source = cljonic::vector(base, base + 1, base + 2, base + 3);
    const auto filtered = cljonic::filterv(even, source);
    const auto runtime_ok =
        cljonic::equal(filtered, cljonic::filter(even, source)) &&
        cljonic::equal(source, cljonic::vector(base, base + 1, base + 2, base + 3)) &&
        cljonic::filterv(entry_even, cljonic::hash_map(base, base + 1, base + 1, base + 2)).count() == 1 &&
        cljonic::filterv(even, cljonic::hash_set(base, base + 1, base + 2)).count() == 1 &&
        cljonic::equal(cljonic::filterv(even, cljonic::queue(base, base + 1, base + 2)), cljonic::vector(base + 1)) &&
        cljonic::equal(cljonic::filterv(char_b, cljonic::string<4>("abc")), cljonic::vector('b')) &&
        cljonic::filterv(even, cljonic::Range<int>{base, base + 4, 1}).count() == 2 &&
        cljonic::filterv(even, cljonic::repeat(2, base + 1)).count() == 2 &&
        cljonic::filterv(even, cljonic::repeatedly(3, []() noexcept { return 2; })).count() == 3;

    return compile_time_ok && runtime_ok;
}

} // namespace cljonic::no_heap::probes
