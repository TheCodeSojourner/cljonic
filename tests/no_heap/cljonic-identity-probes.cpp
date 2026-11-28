#include "cljonic-no-heap-api.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto identity_probe() noexcept -> bool {
    const auto value = 7;
    const auto integer_result = cljonic::core::identity(value);
    const auto vector_result = cljonic::core::identity(cljonic::Vector<int, 3>{1, 2, 3});
    const auto map_result = cljonic::core::identity(cljonic::Map<int, int, 1>{cljonic::MapEntry<int, int>{1, 100}});
    return integer_result == 7 && value == 7 && vector_result(0) == 1 && vector_result(2) == 3 && map_result(1) == 100;
}

} // namespace cljonic::no_heap::probes
