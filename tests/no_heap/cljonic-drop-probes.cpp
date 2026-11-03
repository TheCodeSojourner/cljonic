#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

#include <limits>
#include <utility>

namespace cljonic::no_heap::probes {

[[nodiscard]] auto drop_probe() noexcept -> bool {
    volatile int count_raw = 1;
    const auto requested = count_raw;
    const auto values = cljonic::vector(1, 2, 3);
    const auto map = cljonic::hash_map(1, 10, 2, 20);
    const auto set = cljonic::hash_set(1, 2);
    const auto mapped = cljonic::drop(requested, map);
    const auto unique = cljonic::drop(requested, set);
    const auto map_values = cljonic::seq(map);
    const auto set_values = cljonic::seq(set);
    const auto collections_ok =
        cljonic::equal(cljonic::drop(requested, values), cljonic::vector(2, 3)) &&
        cljonic::equal(cljonic::drop(-requested, values), values) &&
        cljonic::drop(std::numeric_limits<std::size_t>::max(), values).count() == 0 &&
        cljonic::drop(0, cljonic::Vector<int, 0>{}).count() == 0 &&
        cljonic::drop(requested, cljonic::Vector<int, 4>{}).count() == 0 && mapped.count() == 1 &&
        mapped(0) == map_values(1) && unique.count() == 1 && unique(0) == set_values(1) &&
        cljonic::equal(map, cljonic::hash_map(1, 10, 2, 20)) && cljonic::equal(set, cljonic::hash_set(1, 2)) &&
        cljonic::equal(cljonic::drop(requested, cljonic::queue(1, 2, 3)), cljonic::vector(2, 3)) &&
        cljonic::equal(cljonic::drop(requested, cljonic::string<4>("abc")), cljonic::vector('b', 'c')) &&
        cljonic::equal(values, cljonic::vector(1, 2, 3));
    constexpr auto cap = CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto repeated = cljonic::drop(requested, cljonic::repeat(3, 7));
    const auto unbounded = cljonic::drop(requested, cljonic::repeat(7));
    const auto cycled = cljonic::drop(requested, cljonic::cycle(values));
    const auto producers_ok = cljonic::equal(cljonic::drop(requested, cljonic::range(1, 4)), cljonic::vector(2, 3)) &&
                              cljonic::equal(repeated, cljonic::vector(7, 7)) && unbounded.count() == cap - requested &&
                              unbounded(unbounded.count() - 1) == 7 && cycled.count() == cap - requested &&
                              cycled(0) == 2 &&
                              cljonic::drop(requested, cljonic::cycle(cljonic::Vector<int, 0>{})).count() == 0;
    int calls = 0;
    const auto producer = cljonic::repeatedly(4, [&calls] noexcept { return ++calls; });
    const auto empty = cljonic::drop(4, producer);
    const auto initial_calls = calls;
    const auto generated = cljonic::drop(requested, producer);
    const auto stateful = cljonic::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    const auto first = cljonic::drop(requested, stateful);
    const auto second = cljonic::drop(requested, stateful);
    int steps = 0;
    const auto iterated = cljonic::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto empty_iterated = cljonic::drop(cap, iterated);
    const auto initial_steps = steps;
    const auto suffix = cljonic::drop(requested, iterated);
    const auto callbacks_ok = empty.count() == 0 && initial_calls == 0 &&
                              cljonic::equal(generated, cljonic::vector(2, 3, 4)) && calls == 4 &&
                              cljonic::equal(first, cljonic::vector(2, 3, 4)) && cljonic::equal(first, second) &&
                              stateful.count() == 4 && empty_iterated.count() == 0 && initial_steps == 0 &&
                              suffix.count() == cap - requested && suffix(0) == 2 && std::cmp_equal(steps, cap - 1);
    return collections_ok && producers_ok && callbacks_ok;
}

} // namespace cljonic::no_heap::probes
