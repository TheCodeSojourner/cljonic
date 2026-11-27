#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

namespace cljonic::no_heap::probes {

[[nodiscard]] auto assoc_in_probe() noexcept -> bool {
    using Root = Map<int, Map<int, int, 4>, 4>;
    volatile int runtime_key = 1;
    const int key = runtime_key;
    const auto root = Root{};
    const auto path = vector<int, 2>(key, 2);
    const auto result = assoc_in(root, path, 42);
    const auto unchanged = assoc_in(Vector<int, 2>{10, 20}, vector<int, 1>(3), 99);
    return result(key, Map<int, int, 4>{})(2, 0) == 42 && can_assoc_in(root, path, 42) && unchanged(0) == 10 &&
           unchanged(1) == 20 && !can_assoc_in(Vector<int, 2>{10, 20}, vector<int, 1>(3), 99);
}

} // namespace cljonic::no_heap::probes
