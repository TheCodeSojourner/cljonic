#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto has_first_probe() noexcept -> bool {
    volatile int raw = 40;
    const int value = raw;

    const auto vector = Vector<int, 4>{value, value + 1};
    const auto map = Map<int, int, 4>{MapEntry{value, value + 1}};
    const auto set = Set<int, 4>{value};
    const auto queue = Queue<int, 4>{value};
    const auto text = String<4>{"A"};
    const auto range = Range{value, value + 2};
    const auto finite_repeat = repeat(2, value);
    const auto empty_repeat = repeat(0, value);
    const auto iterated = iterate([](int item) noexcept { return item + 1; }, value);
    const auto repeatedly_value = repeatedly(2, [value]() noexcept { return value; });
    const auto prefix = take_while([](int) noexcept { return true; }, vector);
    const auto empty_prefix = take_while([](int) noexcept { return false; }, vector);
    const auto suffix = drop_while([](int item) noexcept { return item < 41; }, vector);
    const auto empty_suffix = drop_while([](int) noexcept { return true; }, vector);
    const auto map_entry = MapEntry<int, int>{value, value + 1};
    const auto default_map_entry = MapEntry<int, int>{};

    const bool collections = has_first(vector) && has_first(map) && has_first(set) && has_first(queue) &&
                             has_first(text) && !has_first(Vector<int, 0>{}) && !has_first(Map<int, int, 0>{}) &&
                             !has_first(Set<int, 0>{}) && !has_first(Queue<int, 0>{}) && !has_first(String<0>{});
    const bool producers =
        has_first(range) && has_first(finite_repeat) && !has_first(empty_repeat) &&
        has_first(repeatedly_value) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U) &&
        has_first(iterated) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U) &&
        has_first(prefix) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U) &&
        !has_first(empty_prefix) && has_first(suffix) && !has_first(empty_suffix) &&
        !has_first(cycle(Vector<int, 0>{})) &&
        has_first(cycle(Vector<int, 1>{value})) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U) &&
        !has_first(Range{value, value});
    const bool entries = has_first(map_entry) && has_first(default_map_entry);

    auto callback_calls = 0;
    const auto callback_producer = repeatedly([&callback_calls, value]() noexcept {
        ++callback_calls;
        return value;
    });
    const bool callback_timing = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U
                                     ? !has_first(callback_producer) && callback_calls == 0
                                     : has_first(callback_producer) && callback_calls == 1;

    auto predicate_calls = 0;
    const auto callback_prefix = take_while(
        [&predicate_calls](int) noexcept {
            ++predicate_calls;
            return true;
        },
        vector);
    const bool predicate_timing = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U
                                      ? !has_first(callback_prefix) && predicate_calls == 0
                                      : has_first(callback_prefix) && predicate_calls == 1;

    return collections && producers && entries && callback_timing && predicate_timing && count(vector) == 2U &&
           vector(0) == value;
}

} // namespace cljonic::no_heap::probes
