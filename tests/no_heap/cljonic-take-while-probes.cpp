#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto take_while_probe() noexcept -> bool {
    volatile int limit_raw = 3;
    const auto limit = limit_raw;
    const auto predicate = [limit](int n) noexcept { return n < limit; };
    const auto prefix = cljonic::core::take_while(predicate, cljonic::core::vector(1, 2, 4, 1));
    auto iterator = prefix.begin();
    auto copied = iterator++;
    copied = iterator;
    const auto copies_ok = *iterator == 2 && *copied == 2;
    int calls = 0;
    const auto generated =
        cljonic::core::take_while(predicate, cljonic::core::repeatedly(4, [n = 0, &calls]() mutable noexcept {
                                      ++calls;
                                      return ++n;
                                  }));
    const auto deferred = calls == 0;
    const auto length = generated.count();
    const auto exact = length == 2 && calls == 3;
    const auto result = cljonic::core::into(cljonic::core::vector<int, 4>(), generated);
    return copies_ok && deferred && exact && calls == 6 && cljonic::core::equal(result, cljonic::core::vector(1, 2)) &&
           prefix.is_finite() && prefix.count() == 2 && *prefix.begin() == 1 &&
           cljonic::core::fits_into(cljonic::core::vector<int, 2>(), prefix) &&
           cljonic::core::take_while(predicate, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0;
}

} // namespace cljonic::no_heap::probes
