#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

#include <limits>

namespace cljonic::no_heap::probes {

[[nodiscard]] auto take_nth_probe() noexcept -> bool {
    volatile int stride_raw = 2;
    const auto stride = stride_raw;
    const auto values = cljonic::core::vector(1, 2, 3, 4, 5);
    const auto map = cljonic::core::hash_map(1, 10, 2, 20, 3, 30);
    const auto set = cljonic::core::hash_set(1, 2, 3);
    const auto mapped = cljonic::core::take_nth(stride, map);
    const auto unique = cljonic::core::take_nth(stride, set);

    const auto collections_ok =
        cljonic::core::equal(cljonic::core::take_nth(stride, values), cljonic::core::vector(1, 3, 5)) &&
        cljonic::core::equal(cljonic::core::take_nth(1, values), values) &&
        cljonic::core::take_nth(0, values).count() == 0 && cljonic::core::take_nth(-stride, values).count() == 0 &&
        cljonic::core::take_nth(stride, cljonic::Vector<int, 0>{}).count() == 0 && mapped.count() > 0 &&
        unique.count() > 0 && cljonic::core::equal(map, cljonic::core::hash_map(1, 10, 2, 20, 3, 30)) &&
        cljonic::core::equal(set, cljonic::core::hash_set(1, 2, 3)) &&
        cljonic::core::equal(cljonic::core::take_nth(stride, cljonic::core::queue(10, 20, 30, 40)),
                             cljonic::core::vector(10, 30)) &&
        cljonic::core::equal(cljonic::core::take_nth(stride, cljonic::core::string<6>("abcde")),
                             cljonic::core::vector('a', 'c', 'e')) &&
        cljonic::core::equal(values, cljonic::core::vector(1, 2, 3, 4, 5));

    const auto repeated = cljonic::core::take_nth(stride, cljonic::core::repeat(3, 7));
    const auto unbounded = cljonic::core::take_nth(stride, cljonic::core::repeat(7));
    const auto cycled = cljonic::core::take_nth(stride, cljonic::core::cycle(values));
    const auto producers_ok =
        cljonic::core::equal(cljonic::core::take_nth(stride, cljonic::core::range(1, 6)),
                             cljonic::core::vector(1, 3, 5)) &&
        cljonic::core::take_nth(std::numeric_limits<std::size_t>::max(), values).count() == 1 &&
        cljonic::core::equal(repeated, cljonic::core::vector(7, 7)) && unbounded.count() > 0 && cycled.count() > 0 &&
        cljonic::core::take_nth(stride, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;

    int calls = 0;
    const auto producer = cljonic::core::repeatedly(5, [&calls] noexcept { return ++calls; });
    const auto empty = cljonic::core::take_nth(0, producer);
    const auto initial_calls = calls;
    const auto generated = cljonic::core::take_nth(stride, producer);
    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto suffix = cljonic::core::take_nth(stride, iterated);

    const auto callbacks_ok = empty.count() == 0 && initial_calls == 0 &&
                              cljonic::core::equal(generated, cljonic::core::vector(1, 3, 5)) && calls == 5 &&
                              suffix.count() > 0 && steps >= 1;

    return collections_ok && producers_ok && callbacks_ok;
}

} // namespace cljonic::no_heap::probes
