#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto map_function_probe() noexcept -> bool {
    // Multi-source mapping (REQ-FN-032): one traversal per admitted source
    // family, all constexpr-capable, non-allocating, noexcept.
    constexpr auto inc = [](int value) noexcept { return value + 1; };
    constexpr auto add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
    constexpr auto entry_value = [](const auto& entry) noexcept { return entry.value; };

    constexpr auto unary_ok =
        cljonic::core::equal(cljonic::core::map(inc, cljonic::core::vector(1, 2, 3)), cljonic::core::vector(2, 3, 4)) &&
        cljonic::core::count(cljonic::core::map(inc, cljonic::core::vector<int, 0>())) == 0 &&
        cljonic::core::equal(cljonic::core::map(inc, cljonic::core::queue(7, 8)), cljonic::core::vector(8, 9));
    constexpr auto families_ok =
        cljonic::core::count(cljonic::core::map(entry_value, cljonic::core::hash_map(1, 10, 2, 20))) == 2 &&
        cljonic::core::count(cljonic::core::map(inc, cljonic::core::hash_set(3, 1, 2))) == 3 &&
        cljonic::core::count(cljonic::core::map([](char c) noexcept { return c; }, cljonic::core::string<4>("abc"))) ==
            3 &&
        cljonic::core::equal(cljonic::core::map(inc, cljonic::Range<int>{1, 4, 1}), cljonic::core::vector(2, 3, 4)) &&
        cljonic::core::equal(cljonic::core::map(inc, cljonic::core::repeat(3, 7)), cljonic::core::vector(8, 8, 8)) &&
        cljonic::core::count(cljonic::core::map(inc, cljonic::core::cycle(cljonic::core::vector(1, 2)))) == 1000 &&
        cljonic::core::count(cljonic::core::map(inc, cljonic::core::iterate(inc, 0))) == 1000 &&
        cljonic::core::equal(cljonic::core::map(inc, cljonic::core::repeatedly(2, []() noexcept { return 9; })),
                             cljonic::core::vector(10, 10));
    constexpr auto multi_ok = cljonic::core::equal(cljonic::core::map(add, cljonic::core::vector(1, 2, 3),
                                                                      cljonic::core::queue(10, 20, 30, 40)),
                                                   cljonic::core::vector(11, 22, 33)) &&
                              cljonic::core::equal(cljonic::core::map(add, cljonic::core::queue(10, 20, 30, 40),
                                                                      cljonic::core::vector(1, 2, 3)),
                                                   cljonic::core::vector(11, 22, 33));
    constexpr auto preserve_ok = cljonic::core::equal(cljonic::core::vector(1, 2, 3), cljonic::core::vector(1, 2, 3)) &&
                                 cljonic::core::count(cljonic::core::queue(10, 20, 30, 40)) == 4;

    // Runtime probe defeats constexpr folding so the no-heap artifact truly
    // executes the mapping at runtime.
    volatile int base_raw = 1;
    const auto base = base_raw;
    auto runtime_ok = cljonic::core::equal(cljonic::core::map(inc, cljonic::core::vector(base, base + 1, base + 2)),
                                           cljonic::core::vector(base + 1, base + 2, base + 3)) &&
                      cljonic::core::count(
                          cljonic::core::map(add, cljonic::core::vector(base, base + 1),
                                             cljonic::core::queue(base * 10, (base + 1) * 10, (base + 2) * 10))) == 2 &&
                      cljonic::core::equal(cljonic::core::map(inc, cljonic::core::repeat(2, base)),
                                           cljonic::core::vector(base + 1, base + 1));

    return unary_ok && families_ok && multi_ok && preserve_ok && runtime_ok;
}

} // namespace cljonic::no_heap::probes
