#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

namespace {
auto producer_first_probe(int value) noexcept -> bool {
    const auto yes = [](auto) noexcept { return true; };
    const auto no = [](auto) noexcept { return false; };
    const auto values = vector(value, value + 1);
    const auto prefix = take_while([limit = value + 1](int n) noexcept { return n < limit; }, values);
    auto result = first(repeat(values));
    result = assoc(result, 0, value + 2);
    return first(range(value, value + 2)) == value && first(repeat(value)) == value && first(cycle(values)) == value &&
           first(iterate([](int n) noexcept { return n + 1; }, value)) == value &&
           first(repeatedly([value]() noexcept { return value; })) == value && first(prefix) == value &&
           first(drop_while([value](int n) noexcept { return n == value; }, values)) == value + 1 &&
           first(cycle(cycle(prefix))) == value && first(take_while(yes, drop_while(no, values))) == value &&
           first(next(cycle(values))) == value + 1 && first(repeat(map_entry(value, 20))) == map_entry(value, 20) &&
           first(take_while(yes, hash_map(value, 20))) == map_entry(value, 20) &&
           first(take_while(yes, hash_set(value))) == value && first(drop_while(no, queue(value))) == value &&
           first(take_while(yes, string("AB"))) == 'A' && first(repeat(values))(0) == value && result(0) == value + 2 &&
           first(take_while(no, values)) == 0 && first(drop_while(yes, values)) == 0 &&
           first(cycle(cycle(take_while(no, values)))) == 0 && first(range(value, value)) == 0 &&
           first(repeat(0, value)) == 0 && first(cycle(vector<int>())) == 0;
}

auto producer_first_callbacks_probe(int value) noexcept -> bool {
    int produced = 0;
    int tested = 0;
    const auto source = repeatedly([&produced, value]() noexcept {
        ++produced;
        return value;
    });
    const auto prefix = take_while(
        [&tested](int) noexcept {
            ++tested;
            return true;
        },
        source);
    if (first(cycle(cycle(prefix))) != value || produced != 1 || tested != 1 || first(prefix) != value ||
        produced != 2 || tested != 2) {
        return false;
    }
    const auto finite = repeatedly(2, [&produced, value]() noexcept {
        ++produced;
        return value;
    });
    const auto empty = drop_while(
        [&tested](int) noexcept {
            ++tested;
            return true;
        },
        finite);
    if (first(cycle(empty)) != 0 || produced != 4 || tested != 4) {
        return false;
    }
    const auto suffix = drop_while(
        [&tested](int n) noexcept {
            ++tested;
            return n < 3;
        },
        repeatedly([&produced]() noexcept { return ++produced - 4; }));
    if (first(suffix) != 3 || produced != 7 || tested != 7) {
        return false;
    }
    return first(iterate(
               [&produced](int n) noexcept {
                   ++produced;
                   return n + 1;
               },
               value)) == value &&
           produced == 7 && first(repeatedly(0, [&produced]() noexcept { return ++produced; })) == 0 && produced == 7;
}

auto producer_first_state_probe(int value) noexcept -> bool {
    struct State {
        mutable int value;
        auto operator()() const noexcept -> int {
            return ++value;
        }
    };
    struct Element {
        int value = 42;
    };
    const auto source = repeatedly(State{value});
    int calls = 0;
    const auto empty = repeatedly(0, [&calls]() noexcept { return ++calls; });
    const auto rejected = take_while(
        [&calls](int) noexcept {
            ++calls;
            return false;
        },
        repeat(value));
    if (first(rejected) != 0 || calls != 1 || first(cycle(empty)) != 0 || calls != 1) {
        return false;
    }
    return first(source) == value + 1 && first(source) == value + 1 && first(repeat(Element{value})).value == value &&
           first(repeat(0, Element{value})).value == 42 &&
           first(take_while(
               [&calls](int) noexcept {
                   ++calls;
                   return true;
               },
               empty)) == 0 &&
           calls == 1;
}
} // namespace

[[nodiscard]] auto first_probe() noexcept -> bool {
    volatile int raw = 10;
    const int value = raw;
    const auto v = Vector<int, 4>{10, 20};
    const auto m = Map<int, int, 4>{MapEntry<int, int>{1, 100}};
    const auto s = Set<int, 4>{7};
    const auto q = Queue<int, 4>{5, 6};
    const auto text = String<4>{"AB"};
    const auto e = MapEntry<int, int>{3, 300};
    return first(v) == 10 && first(m) == MapEntry<int, int>{1, 100} && first(s) == 7 && first(q) == 5 &&
           first(text) == 'A' && first(e) == 3 && producer_first_probe(value) &&
           producer_first_callbacks_probe(value) && producer_first_state_probe(value);
}

} // namespace cljonic::no_heap::probes
