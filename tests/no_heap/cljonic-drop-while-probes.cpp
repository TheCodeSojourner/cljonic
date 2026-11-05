#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto drop_while_probe() noexcept -> bool {
    volatile int limit_raw = 3;
    const auto limit = limit_raw;
    const auto predicate = [limit](int n) noexcept { return n < limit; };
    int calls = 0;
    int tests = 0;
    const auto suffix = cljonic::core::drop_while(
        [&tests, limit](int n) noexcept {
            ++tests;
            return n < limit;
        },
        cljonic::core::repeatedly(4, [n = 0, &calls]() mutable noexcept {
            ++calls;
            return ++n;
        }));
    const auto deferred = calls == 0 && tests == 0;
    const auto length = suffix.count();
    const auto exact = length == 2 && calls == 4 && tests == 3;
    const auto result = cljonic::core::into(cljonic::core::vector<int, 2>(), suffix);
    const auto preserved = calls == 8 && tests == 6;
    const auto values = cljonic::core::drop_while(predicate, cljonic::core::vector(1, 2, 4, 1));
    return deferred && exact && preserved && cljonic::core::equal(result, cljonic::core::vector(3, 4)) &&
           cljonic::core::equal(values, cljonic::core::vector(4, 1)) && values.is_finite() &&
           cljonic::core::take_while([](int n) noexcept { return n > 2; }, values).count() == 1 &&
           cljonic::core::drop_while(predicate, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;
}

} // namespace cljonic::no_heap::probes
