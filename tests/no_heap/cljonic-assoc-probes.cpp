#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto assoc_probe() noexcept -> bool {
    const auto m = Map<int, int, 4>{};
    const auto m1 = assoc(m, 1, 100);
    const auto m2 = assoc(m, 1, 100, 2, 200);
    volatile char raw_replacement = 'B';
    const char replacement = raw_replacement;
    const auto text = String<4>{"A"};
    const auto text_replaced = assoc(text, 0, replacement);
    const auto text_appended = assoc(text, 1, replacement);
    const auto full_text = String<1>{"A"};
    const auto full_text_unchanged = assoc(full_text, 1, replacement);
    return m.count() == 0U && m1.count() == 1U && m1(1) == 100 && m2.count() == 2U && m2(2) == 200 &&
           text.count() == 1U && text(0) == 'A' && text_replaced(0) == 'B' && text_replaced.count() == 1U &&
           text_appended.count() == 2U && text_appended(0) == 'A' && text_appended(1) == 'B' &&
           full_text_unchanged.count() == 1U && full_text_unchanged(0) == 'A' && full_text.count() == 1U;
}

} // namespace cljonic::no_heap::probes
