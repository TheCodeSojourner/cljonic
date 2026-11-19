#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

#include <limits>
#include <utility>

namespace cljonic::no_heap::probes {

[[nodiscard]] auto take_last_probe() noexcept -> bool {
    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto values = cljonic::core::vector(1, 2, 3, 4);
    const auto map = cljonic::core::hash_map(1, 10, 2, 20);
    const auto set = cljonic::core::hash_set(1, 2);
    const auto map_values = cljonic::core::seq(map);
    const auto set_values = cljonic::core::seq(set);
    const auto mapped = cljonic::core::take_last(requested, map);
    const auto unique = cljonic::core::take_last(requested, set);

    const auto collections_ok =
        cljonic::core::equal(cljonic::core::take_last(requested, values), cljonic::core::vector(3, 4)) &&
        cljonic::core::take_last(0, values).count() == 0 && cljonic::core::take_last(-requested, values).count() == 0 &&
        cljonic::core::equal(cljonic::core::take_last(std::numeric_limits<std::size_t>::max(), values), values) &&
        cljonic::core::take_last(requested, cljonic::Vector<int, 0>{}).count() == 0 && mapped.count() == 2 &&
        unique.count() == 2 && mapped(0) == map_values(map_values.count() - 2) &&
        mapped(1) == map_values(map_values.count() - 1) && unique(0) == set_values(set_values.count() - 2) &&
        unique(1) == set_values(set_values.count() - 1) &&
        cljonic::core::equal(map, cljonic::core::hash_map(1, 10, 2, 20)) &&
        cljonic::core::equal(set, cljonic::core::hash_set(1, 2)) &&
        cljonic::core::equal(cljonic::core::take_last(requested, cljonic::core::queue(1, 2, 3)),
                             cljonic::core::vector(2, 3)) &&
        cljonic::core::equal(cljonic::core::take_last(requested, cljonic::core::string<4>("abc")),
                             cljonic::core::vector('b', 'c')) &&
        cljonic::core::equal(values, cljonic::core::vector(1, 2, 3, 4));

    constexpr auto cap = CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto repeated = cljonic::core::take_last(requested, cljonic::core::repeat(3, 7));
    const auto unbounded = cljonic::core::take_last(requested, cljonic::core::repeat(7));
    const auto cycled = cljonic::core::take_last(requested, cljonic::core::cycle(values));
    const auto producers_ok =
        cljonic::core::equal(cljonic::core::take_last(requested, cljonic::core::range(1, 5)),
                             cljonic::core::vector(3, 4)) &&
        cljonic::core::equal(repeated, cljonic::core::vector(7, 7)) && unbounded.count() == 2 && unbounded(0) == 7 &&
        unbounded(1) == 7 && cycled.count() == 2 && cycled(0) == 3 && cycled(1) == 4 &&
        cljonic::core::take_last(requested, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;

    int calls = 0;
    const auto producer = cljonic::core::repeatedly(4, [&calls] noexcept { return ++calls; });
    const auto empty = cljonic::core::take_last(0, producer);
    const auto initial_calls = calls;
    const auto generated = cljonic::core::take_last(requested, producer);
    const auto stateful = cljonic::core::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    const auto first = cljonic::core::take_last(requested, stateful);
    const auto second = cljonic::core::take_last(requested, stateful);
    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto empty_iterated = cljonic::core::take_last(0, iterated);
    const auto initial_steps = steps;
    const auto suffix = cljonic::core::take_last(requested, iterated);

    const auto callbacks_ok =
        empty.count() == 0 && initial_calls == 0 && cljonic::core::equal(generated, cljonic::core::vector(3, 4)) &&
        calls == 4 && cljonic::core::equal(first, cljonic::core::vector(3, 4)) && cljonic::core::equal(first, second) &&
        stateful.count() == 4 && empty_iterated.count() == 0 && initial_steps == 0 && suffix.count() == 2 && steps >= 2;

    return collections_ok && producers_ok && callbacks_ok && cap >= 0;
}

} // namespace cljonic::no_heap::probes
