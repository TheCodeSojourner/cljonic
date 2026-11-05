#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

#include <limits>
#include <utility>

namespace cljonic::no_heap::probes {

[[nodiscard]] auto drop_probe() noexcept -> bool {
    volatile int count_raw = 1;
    const auto requested = count_raw;
    const auto values = cljonic::core::vector(1, 2, 3);
    const auto map = cljonic::core::hash_map(1, 10, 2, 20);
    const auto set = cljonic::core::hash_set(1, 2);
    const auto mapped = cljonic::core::drop(requested, map);
    const auto unique = cljonic::core::drop(requested, set);
    const auto map_values = cljonic::core::seq(map);
    const auto set_values = cljonic::core::seq(set);
    const auto collections_ok =
        cljonic::core::equal(cljonic::core::drop(requested, values), cljonic::core::vector(2, 3)) &&
        cljonic::core::equal(cljonic::core::drop(-requested, values), values) &&
        cljonic::core::drop(std::numeric_limits<std::size_t>::max(), values).count() == 0 &&
        cljonic::core::drop(0, cljonic::Vector<int, 0>{}).count() == 0 &&
        cljonic::core::drop(requested, cljonic::Vector<int, 4>{}).count() == 0 && mapped.count() == 1 &&
        mapped(0) == map_values(1) && unique.count() == 1 && unique(0) == set_values(1) &&
        cljonic::core::equal(map, cljonic::core::hash_map(1, 10, 2, 20)) &&
        cljonic::core::equal(set, cljonic::core::hash_set(1, 2)) &&
        cljonic::core::equal(cljonic::core::drop(requested, cljonic::core::queue(1, 2, 3)),
                             cljonic::core::vector(2, 3)) &&
        cljonic::core::equal(cljonic::core::drop(requested, cljonic::core::string<4>("abc")),
                             cljonic::core::vector('b', 'c')) &&
        cljonic::core::equal(values, cljonic::core::vector(1, 2, 3));
    constexpr auto cap = CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto repeated = cljonic::core::drop(requested, cljonic::core::repeat(3, 7));
    const auto unbounded = cljonic::core::drop(requested, cljonic::core::repeat(7));
    const auto cycled = cljonic::core::drop(requested, cljonic::core::cycle(values));
    const auto producers_ok =
        cljonic::core::equal(cljonic::core::drop(requested, cljonic::core::range(1, 4)), cljonic::core::vector(2, 3)) &&
        cljonic::core::equal(repeated, cljonic::core::vector(7, 7)) && unbounded.count() == cap - requested &&
        unbounded(unbounded.count() - 1) == 7 && cycled.count() == cap - requested && cycled(0) == 2 &&
        cljonic::core::drop(requested, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;
    int calls = 0;
    const auto producer = cljonic::core::repeatedly(4, [&calls] noexcept { return ++calls; });
    const auto empty = cljonic::core::drop(4, producer);
    const auto initial_calls = calls;
    const auto generated = cljonic::core::drop(requested, producer);
    const auto stateful = cljonic::core::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    const auto first = cljonic::core::drop(requested, stateful);
    const auto second = cljonic::core::drop(requested, stateful);
    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto empty_iterated = cljonic::core::drop(cap, iterated);
    const auto initial_steps = steps;
    const auto suffix = cljonic::core::drop(requested, iterated);
    const auto callbacks_ok =
        empty.count() == 0 && initial_calls == 0 && cljonic::core::equal(generated, cljonic::core::vector(2, 3, 4)) &&
        calls == 4 && cljonic::core::equal(first, cljonic::core::vector(2, 3, 4)) &&
        cljonic::core::equal(first, second) && stateful.count() == 4 && empty_iterated.count() == 0 &&
        initial_steps == 0 && suffix.count() == cap - requested && suffix(0) == 2 && std::cmp_equal(steps, cap - 1);
    return collections_ok && producers_ok && callbacks_ok;
}

} // namespace cljonic::no_heap::probes
