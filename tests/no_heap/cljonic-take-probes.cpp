#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

#include <limits>

namespace cljonic::no_heap::probes {

[[nodiscard]] auto take_probe() noexcept -> bool {
    volatile int base_raw = 1;
    volatile int count_raw = 2;
    const auto base = base_raw;
    const auto requested = count_raw;
    const auto values = cljonic::core::vector(base, base + 1, base + 2);
    const auto map = cljonic::core::hash_map(base, base + 10, base + 1, base + 11);
    const auto set = cljonic::core::hash_set(base, base + 1);
    const auto queued = cljonic::core::queue(base, base + 1, base + 2);
    const auto text = cljonic::core::string<4>("abc");
    const auto prefix = cljonic::core::take(requested, values);
    const auto map_prefix = cljonic::core::take(1, map);
    const auto set_prefix = cljonic::core::take(1, set);
    constexpr auto cap = CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto full =
        cljonic::core::take(std::numeric_limits<std::size_t>::max(), cljonic::core::repeat(cap + 1, base));
    const auto unbounded = cljonic::core::take(std::numeric_limits<std::size_t>::max(), cljonic::core::repeat(base));
    const auto collections_ok =
        cljonic::core::equal(prefix, cljonic::core::vector(base, base + 1)) && prefix.capacity() == values.capacity() &&
        cljonic::core::equal(values, cljonic::core::vector(base, base + 1, base + 2)) &&
        cljonic::core::equal(cljonic::core::take(10, values), values) && cljonic::core::take(0, values).count() == 0 &&
        cljonic::core::equal(cljonic::core::take(static_cast<short>(requested), values), prefix) &&
        cljonic::core::equal(cljonic::core::take(static_cast<std::size_t>(requested), values), prefix) &&
        cljonic::core::take(-requested, values).count() == 0 &&
        cljonic::core::take(requested, cljonic::Vector<int, 0>{}).count() == 0 &&
        cljonic::core::take(requested, cljonic::Vector<int, 3>{}).count() == 0 && map_prefix.count() == 1 &&
        map_prefix(0) == *map.begin() && map.count() == 2 && set_prefix.count() == 1 && set_prefix(0) == *set.begin() &&
        set.count() == 2 && cljonic::core::equal(map, cljonic::core::hash_map(base, base + 10, base + 1, base + 11)) &&
        cljonic::core::equal(set, cljonic::core::hash_set(base, base + 1)) &&
        cljonic::core::equal(cljonic::core::take(requested, queued), cljonic::core::vector(base, base + 1)) &&
        cljonic::core::equal(queued, cljonic::core::queue(base, base + 1, base + 2)) &&
        cljonic::core::equal(cljonic::core::take(requested, text), cljonic::core::vector('a', 'b')) &&
        cljonic::core::equal(text, cljonic::core::string<4>("abc"));
    const auto producers_ok =
        cljonic::core::equal(cljonic::core::take(requested, cljonic::core::range(base, base + 3)),
                             cljonic::core::vector(base, base + 1)) &&
        cljonic::core::equal(cljonic::core::take(requested, cljonic::core::repeat(1, base)),
                             cljonic::core::vector(base)) &&
        cljonic::core::equal(
            cljonic::core::take(requested, cljonic::core::cycle(cljonic::core::vector(base, base + 1))),
            cljonic::core::vector(base, base + 1)) &&
        full.count() == cap && full.capacity() == cap && full(0) == base && full(cap - 1) == base &&
        unbounded.count() == cap && unbounded(cap - 1) == base &&
        cljonic::core::take(requested, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;
    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        base);
    const auto empty_iterated = cljonic::core::take(0, iterated);
    const auto single_iterated = cljonic::core::take(1, iterated);
    const auto initial_steps = steps;
    const auto iterate_prefix = cljonic::core::take(requested, iterated);
    int calls = 0;
    const auto produced = cljonic::core::repeatedly([&calls, base]() noexcept {
        ++calls;
        return base + calls;
    });
    const auto empty_produced = cljonic::core::take(-requested, produced);
    const auto empty_source = cljonic::core::take(requested, cljonic::core::repeatedly(0, [&calls]() noexcept {
                                                      ++calls;
                                                      return 0;
                                                  }));
    const auto initial_calls = calls;
    const auto produced_prefix = cljonic::core::take(requested, produced);
    const auto next_prefix = cljonic::core::take(1, produced);
    const auto stateful = cljonic::core::repeatedly(3, [value = base]() mutable noexcept { return value++; });
    const auto stateful_prefix = cljonic::core::take(requested, stateful);
    const auto callbacks_ok =
        empty_iterated.count() == 0 && single_iterated(0) == base && initial_steps == 0 &&
        cljonic::core::equal(iterate_prefix, cljonic::core::vector(base, base + 1)) && steps == requested - 1 &&
        empty_produced.count() == 0 && empty_source.count() == 0 && initial_calls == 0 &&
        cljonic::core::equal(produced_prefix, cljonic::core::vector(base + 1, base + 2)) && calls == requested + 1 &&
        next_prefix(0) == base + requested + 1 &&
        cljonic::core::equal(stateful_prefix, cljonic::core::vector(base, base + 1)) &&
        cljonic::core::equal(stateful_prefix, cljonic::core::take(requested, stateful)) && stateful.count() == 3;
    return collections_ok && producers_ok && callbacks_ok;
}

} // namespace cljonic::no_heap::probes
