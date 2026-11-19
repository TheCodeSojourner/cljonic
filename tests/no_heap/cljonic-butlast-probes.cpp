#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto butlast_probe() noexcept -> bool {
    const auto values = cljonic::core::vector(1, 2, 3, 4);
    const auto map = cljonic::core::hash_map(1, 10, 2, 20);
    const auto set = cljonic::core::hash_set(1, 2, 3);
    const auto mapped = cljonic::core::butlast(map);
    const auto unique = cljonic::core::butlast(set);

    const auto collections_ok =
        cljonic::core::equal(cljonic::core::butlast(values), cljonic::core::vector(1, 2, 3)) &&
        cljonic::core::butlast(cljonic::Vector<int, 1>{1}).count() == 0 &&
        cljonic::core::butlast(cljonic::Vector<int, 0>{}).count() == 0 && mapped.count() <= map.count() &&
        unique.count() <= set.count() &&
        cljonic::core::equal(cljonic::core::butlast(cljonic::core::queue(1, 2, 3)), cljonic::core::vector(1, 2)) &&
        cljonic::core::equal(cljonic::core::butlast(cljonic::core::string<5>("abcd")),
                             cljonic::core::vector('a', 'b', 'c')) &&
        cljonic::core::equal(map, cljonic::core::hash_map(1, 10, 2, 20)) &&
        cljonic::core::equal(set, cljonic::core::hash_set(1, 2, 3)) &&
        cljonic::core::equal(values, cljonic::core::vector(1, 2, 3, 4));

    const auto repeated = cljonic::core::butlast(cljonic::core::repeat(3, 7));
    const auto cycled = cljonic::core::butlast(cljonic::core::cycle(values));
    const auto producers_ok =
        cljonic::core::equal(cljonic::core::butlast(cljonic::core::range(1, 5)), cljonic::core::vector(1, 2, 3)) &&
        cljonic::core::equal(repeated, cljonic::core::vector(7, 7)) &&
        cycled.count() == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1 &&
        cljonic::core::butlast(cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;

    int calls = 0;
    const auto repeatedly = cljonic::core::repeatedly(4, [&calls] noexcept { return ++calls; });
    const auto repeatedly_result = cljonic::core::butlast(repeatedly);
    const auto stateful = cljonic::core::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    const auto first = cljonic::core::butlast(stateful);
    const auto second = cljonic::core::butlast(stateful);

    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto iterated_result = cljonic::core::butlast(iterated);

    const auto callbacks_ok =
        repeatedly_result.count() == 3 && calls == 4 && cljonic::core::equal(first, cljonic::core::vector(1, 2, 3)) &&
        cljonic::core::equal(first, second) &&
        iterated_result.count() == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1 && steps >= 1;

    return collections_ok && producers_ok && callbacks_ok;
}

} // namespace cljonic::no_heap::probes
