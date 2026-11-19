#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

#include <limits>

namespace cljonic::no_heap::probes {

[[nodiscard]] auto drop_last_probe() noexcept -> bool {
    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto values = cljonic::core::vector(1, 2, 3, 4);
    const auto map = cljonic::core::hash_map(1, 10, 2, 20);
    const auto set = cljonic::core::hash_set(1, 2, 3);
    const auto mapped = cljonic::core::drop_last(requested, map);
    const auto unique = cljonic::core::drop_last(requested, set);

    const auto collections_ok =
        cljonic::core::equal(cljonic::core::drop_last(requested, values), cljonic::core::vector(1, 2)) &&
        cljonic::core::equal(cljonic::core::drop_last(0, values), values) &&
        cljonic::core::equal(cljonic::core::drop_last(-requested, values), values) &&
        cljonic::core::drop_last(std::numeric_limits<std::size_t>::max(), values).count() == 0 &&
        cljonic::core::drop_last(requested, cljonic::Vector<int, 0>{}).count() == 0 && mapped.count() >= 0 &&
        unique.count() >= 0 && cljonic::core::equal(map, cljonic::core::hash_map(1, 10, 2, 20)) &&
        cljonic::core::equal(set, cljonic::core::hash_set(1, 2, 3)) &&
        cljonic::core::equal(cljonic::core::drop_last(requested, cljonic::core::queue(1, 2, 3, 4)),
                             cljonic::core::vector(1, 2)) &&
        cljonic::core::equal(cljonic::core::drop_last(requested, cljonic::core::string<5>("abcd")),
                             cljonic::core::vector('a', 'b')) &&
        cljonic::core::equal(values, cljonic::core::vector(1, 2, 3, 4));

    const auto repeated = cljonic::core::drop_last(requested, cljonic::core::repeat(3, 7));
    const auto unbounded = cljonic::core::drop_last(requested, cljonic::core::repeat(7));
    const auto cycled = cljonic::core::drop_last(requested, cljonic::core::cycle(values));
    const auto producers_ok =
        cljonic::core::equal(cljonic::core::drop_last(requested, cljonic::core::range(1, 5)),
                             cljonic::core::vector(1, 2)) &&
        cljonic::core::equal(repeated, cljonic::core::vector(7)) && unbounded.count() > 0 &&
        cycled.count() == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - requested && cycled(0) == 1 &&
        cycled(1) == 2 &&
        cljonic::core::drop_last(requested, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;

    int calls = 0;
    const auto producer = cljonic::core::repeatedly(4, [&calls] noexcept { return ++calls; });
    const auto empty = cljonic::core::drop_last(0, producer);
    const auto initial_calls = calls;
    const auto generated = cljonic::core::drop_last(requested, producer);
    const auto stateful = cljonic::core::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    const auto first = cljonic::core::drop_last(requested, stateful);
    const auto second = cljonic::core::drop_last(requested, stateful);
    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto empty_iterated = cljonic::core::drop_last(0, iterated);
    const auto initial_steps = steps;
    const auto prefix = cljonic::core::drop_last(requested, iterated);

    const auto callbacks_ok = empty.count() == 4 && initial_calls == 4 && generated.count() == 2 && calls == 8 &&
                              cljonic::core::equal(first, cljonic::core::vector(1, 2)) &&
                              cljonic::core::equal(first, second) && stateful.count() == 4 &&
                              empty_iterated.count() > 0 && initial_steps > 0 && prefix.count() > 0 &&
                              steps >= initial_steps;

    return collections_ok && producers_ok && callbacks_ok;
}

} // namespace cljonic::no_heap::probes
