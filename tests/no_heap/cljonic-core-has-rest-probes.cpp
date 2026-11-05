#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto has_rest_probe() noexcept -> bool {
    volatile int raw = 40;
    const int first = raw;
    const int second = first + 1;
    const auto vector = Vector<int, 4>{first, second};
    const auto map = Map<int, int, 4>{MapEntry{first, second}, MapEntry{second, first}};
    const auto set = Set<int, 4>{first, second};
    const auto queue = Queue<int, 4>{first, second};
    const auto text = String<4>{"AB"};
    const auto range = Range{first, first + 3};
    const auto repeated = repeat(3, first);
    const auto singleton = repeat(1, first);
    const auto cycle = cljonic::cycle(Vector<int, 1>{first});
    const auto empty_cycle = cljonic::cycle(Vector<int, 0>{});
    const auto iterated = iterate([](int value) noexcept { return value + 1; }, first);
    const auto generated = repeatedly(3, [] noexcept { return 7; });
    const auto taken = take_while([](int value) noexcept { return value < 43; }, Vector<int, 4>{first, second, 42});
    const auto dropped =
        drop_while([](int value) noexcept { return value < 43; }, Vector<int, 5>{first, second, 42, 43, 44});
    const auto adapter_cycle = cljonic::cycle(taken);

    const bool collections = has_rest(vector) && has_rest(map) && has_rest(set) && has_rest(queue) && has_rest(text);
    const bool producer_boundaries =
        has_rest(range) && has_rest(repeated) && !has_rest(singleton) && has_rest(iterated) && has_rest(generated) &&
        has_rest(taken) && has_rest(dropped) &&
        has_rest(cycle) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U) && !has_rest(empty_cycle) &&
        has_rest(adapter_cycle) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U);
    auto callback_calls = 0;
    const auto callback_producer = repeatedly([&callback_calls] noexcept { return ++callback_calls; });
    const bool callback_timing = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U
                                     ? !has_rest(callback_producer) && callback_calls == 0
                                     : has_rest(callback_producer) && callback_calls == 2;
    return collections && producer_boundaries && callback_timing && !has_rest(Vector<int, 0>{}) &&
           !has_rest(Map<int, int, 0>{}) && !has_rest(Set<int, 0>{}) && !has_rest(Queue<int, 0>{}) &&
           !has_rest(String<0>{}) && count(vector) == 2U && vector(0) == first && count(range) == 3U;
}

} // namespace cljonic::no_heap::probes
