#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto conversion_probe() noexcept -> bool {
    using cljonic::core::can_to_int;
    using cljonic::core::to_float;
    using cljonic::core::conversion::nearest;

    return can_to_int(17) && to_float(16777217, nearest) == 16777216.0F && cljonic::core::to_int('\0') == 0 &&
           cljonic::core::to_char(0) == '\0';
}

} // namespace cljonic::no_heap::probes
