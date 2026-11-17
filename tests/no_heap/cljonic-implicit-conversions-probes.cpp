#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto implicit_conversions_probe() noexcept -> bool {
    volatile short input = 7;
    const short key = input;
    volatile float floating_input = 7.0F;
    const float value = floating_input;
    const auto map = assoc(Map<long long, double, 2>{}, key, value);
    const auto set = Set<long long, 2>{key};
    const auto vector = into(Vector<long long, 2>{}, repeat(2U, key));
    return contains(map, key) && get(map, key) == 7.0 && is_empty(dissoc(map, key)) && is_empty(disj(set, key)) &&
           get(vector, 1) == key;
}

} // namespace cljonic::no_heap::probes
