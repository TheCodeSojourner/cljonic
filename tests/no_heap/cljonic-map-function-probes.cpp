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

    constexpr auto unary_ok = cljonic::equal(cljonic::map(inc, cljonic::vector(1, 2, 3)), cljonic::vector(2, 3, 4)) &&
                              cljonic::count(cljonic::map(inc, cljonic::vector<int, 0>())) == 0 &&
                              cljonic::equal(cljonic::map(inc, cljonic::queue(7, 8)), cljonic::vector(8, 9));
    constexpr auto families_ok =
        cljonic::count(cljonic::map(entry_value, cljonic::hash_map(1, 10, 2, 20))) == 2 &&
        cljonic::count(cljonic::map(inc, cljonic::hash_set(3, 1, 2))) == 3 &&
        cljonic::count(cljonic::map([](char c) noexcept { return c; }, cljonic::string<4>("abc"))) == 3 &&
        cljonic::equal(cljonic::map(inc, cljonic::Range<int>{1, 4, 1}), cljonic::vector(2, 3, 4)) &&
        cljonic::equal(cljonic::map(inc, cljonic::repeat(3, 7)), cljonic::vector(8, 8, 8)) &&
        cljonic::count(cljonic::map(inc, cljonic::cycle(cljonic::vector(1, 2)))) == 1000 &&
        cljonic::count(cljonic::map(inc, cljonic::iterate(inc, 0))) == 1000 &&
        cljonic::equal(cljonic::map(inc, cljonic::repeatedly(2, []() noexcept { return 9; })), cljonic::vector(10, 10));
    constexpr auto multi_ok =
        cljonic::equal(cljonic::map(add, cljonic::vector(1, 2, 3), cljonic::queue(10, 20, 30, 40)),
                       cljonic::vector(11, 22, 33)) &&
        cljonic::equal(cljonic::map(add, cljonic::queue(10, 20, 30, 40), cljonic::vector(1, 2, 3)),
                       cljonic::vector(11, 22, 33));
    constexpr auto preserve_ok = cljonic::equal(cljonic::vector(1, 2, 3), cljonic::vector(1, 2, 3)) &&
                                 cljonic::count(cljonic::queue(10, 20, 30, 40)) == 4;

    // Runtime probe defeats constexpr folding so the no-heap artifact truly
    // executes the mapping at runtime.
    volatile int base_raw = 1;
    const auto base = base_raw;
    auto runtime_ok = cljonic::equal(cljonic::map(inc, cljonic::vector(base, base + 1, base + 2)),
                                     cljonic::vector(base + 1, base + 2, base + 3)) &&
                      cljonic::count(cljonic::map(add, cljonic::vector(base, base + 1),
                                                  cljonic::queue(base * 10, (base + 1) * 10, (base + 2) * 10))) == 2 &&
                      cljonic::equal(cljonic::map(inc, cljonic::repeat(2, base)), cljonic::vector(base + 1, base + 1));

    return unary_ok && families_ok && multi_ok && preserve_ok && runtime_ok;
}

} // namespace cljonic::no_heap::probes
