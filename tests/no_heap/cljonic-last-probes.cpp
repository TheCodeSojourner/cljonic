#include "cljonic-no-heap-api.hpp"

#include "poison.hpp"

#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto last_probe() noexcept -> bool {
    const auto values = cljonic::core::vector(1, 2, 3, 4);
    const auto map = cljonic::core::hash_map(1, 10, 2, 20);
    const auto set = cljonic::core::hash_set(1, 2, 3);
    const auto queue_source = cljonic::core::queue(1, 2, 3);
    const auto single_entry_map = cljonic::core::hash_map(5, 50);
    const auto mapped = cljonic::core::last(map);
    const auto unique = cljonic::core::last(set);
    const auto from_first = cljonic::core::last(cljonic::core::first(single_entry_map));

    const auto collections_ok =
        cljonic::core::last(values) == 4 && cljonic::core::last(cljonic::core::string<5>("abcd")) == 'd' &&
        cljonic::core::last(queue_source) == 3 && cljonic::core::last(cljonic::MapEntry<int, int>{3, 300}) == 300 &&
        from_first == 50 &&
        (mapped == cljonic::MapEntry<int, int>{1, 10} || mapped == cljonic::MapEntry<int, int>{2, 20}) &&
        (unique == 1 || unique == 2 || unique == 3) && cljonic::core::last(cljonic::Vector<int, 0>{}) == 0 &&
        cljonic::core::last(cljonic::Queue<int, 4>{}) == 0 && cljonic::core::last(cljonic::String<4>{}) == '\0' &&
        cljonic::core::equal(values, cljonic::core::vector(1, 2, 3, 4)) &&
        cljonic::core::equal(queue_source, cljonic::core::queue(1, 2, 3)) &&
        cljonic::core::equal(map, cljonic::core::hash_map(1, 10, 2, 20)) &&
        cljonic::core::equal(set, cljonic::core::hash_set(1, 2, 3));

    const auto below_three = [](int n) noexcept { return n < 3; };

    const auto yielded_entry = cljonic::core::last(cljonic::core::repeat(2, cljonic::core::map_entry(1, 10)));
    const auto cycled = cljonic::core::last(cljonic::core::cycle(cljonic::core::vector(4, 5)));

    const auto producers_ok =
        cljonic::core::last(cljonic::core::range(1, 5)) == 4 && cljonic::core::last(cljonic::core::repeat(9)) == 9 &&
        cljonic::core::last(cljonic::core::repeat(3, 7)) == 7 &&
        cljonic::core::last(cljonic::core::repeat(0, 7)) == 0 && cljonic::core::last(cljonic::core::range(2, 2)) == 0 &&
        cljonic::core::last(cljonic::core::cycle(cljonic::Vector<int, 0>{})) == 0 &&
        yielded_entry == cljonic::MapEntry<int, int>{1, 10} &&
        cycled == ((cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1U) % 2U == 0U ? 4 : 5) &&
        cljonic::core::last(cljonic::core::take_while(below_three, cljonic::core::vector(1, 2, 3))) == 2 &&
        cljonic::core::last(cljonic::core::take_while(below_three, cljonic::core::vector(1, 2))) == 2 &&
        cljonic::core::last(cljonic::core::drop_while(below_three, cljonic::core::vector(1, 2, 4))) == 4;

    int calls = 0;
    const auto repeatedly = cljonic::core::repeatedly(4, [&calls] noexcept { return ++calls; });
    const auto repeatedly_result = cljonic::core::last(repeatedly);
    const auto stateful = cljonic::core::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    const auto first = cljonic::core::last(stateful);
    const auto second = cljonic::core::last(stateful);

    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto iterated_result = cljonic::core::last(iterated);

    int shared = 0;
    const auto shared_state = cljonic::core::repeatedly(4, [&shared] noexcept { return ++shared; });
    const auto shared_first = cljonic::core::last(shared_state);
    const auto shared_second = cljonic::core::last(shared_state);

    int zero_calls = 0;
    const auto zero_cap =
        cljonic::core::last(cljonic::core::repeatedly(0, [&zero_calls] noexcept { return ++zero_calls; }));

    const auto callbacks_ok = repeatedly_result == 4 && calls == 4 && first == 4 && second == 4 && shared_first == 4 &&
                              shared_second == 8 && shared == 8 && zero_cap == 0 && zero_calls == 0 &&
                              iterated_result == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE &&
                              steps == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1;

    struct Element {
        int value = 42;
    };
    const auto element = cljonic::core::last(cljonic::core::repeat(4, Element{7}));
    const auto element_default = cljonic::core::last(cljonic::core::repeat(0, Element{7}));
    const auto elements_ok = element.value == 7 && element_default.value == 42;

    return collections_ok && producers_ok && callbacks_ok && elements_ok;
}

} // namespace cljonic::no_heap::probes
