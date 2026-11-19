#include "cljonic-no-heap-api.hpp"
#include "poison.hpp"
#include "probes.hpp"

#include <limits>

namespace cljonic::no_heap::probes {

[[nodiscard]] auto arithmetic_probe() noexcept -> bool {
    using namespace cljonic::core;

    const bool arities = add<int>() == 0 && add(1, 2, 3) == 6 && sub(7) == -7 && sub(10, 3, 2) == 5 &&
                         mult<int>() == 1 && mult(2, 3, 4) == 24 && div(2.0) == 0.5 && div(24, 3, 2) == 4;

    const bool preflights = can_add(1, 2, 3) && can_sub(10, 3, 2) && can_mult(2, 3, 4) && can_div(24.0, 3.0, 2.0);

    const bool division =
        quot(-7, 3) == -2 && rem(-7, 3) == -1 && mod(-7, 3) == 2 && can_quot(-7, 3) && can_rem(-7, 3) && can_mod(-7, 3);

    const bool steps = inc(7) == 8 && can_inc(7) && dec(7) == 6 && can_dec(7) && negate(7) == -7 && can_negate(7);

    const bool bitwise = bit_and(0b1100, 0b1010) == 0b1000 && bit_or(0b1100, 0b1010) == 0b1110 &&
                         bit_xor(0b1100, 0b1010) == 0b0110 && bit_and_not(0b1100, 0b1000) == 0b0100 &&
                         bit_not(0) == -1 && bit_set(0, 3) == 8 && bit_clear(0b1111, 2) == 0b1011 &&
                         bit_flip(0b1010, 0) == 0b1011 && bit_test(0b1010, 1);

    const bool shifts = bit_shift_left(1, 3) == 8 && can_bit_shift_left(1, 3) && bit_shift_right(-8, 2) == -2 &&
                        can_bit_shift_right(-8, 2) &&
                        unsigned_bit_shift_right(-8, 1) == std::numeric_limits<int>::max() - 3 &&
                        can_unsigned_bit_shift_right(-8, 1);

    return arities && preflights && division && steps && bitwise && shifts;
}

} // namespace cljonic::no_heap::probes
