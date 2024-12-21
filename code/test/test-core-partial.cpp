#include "catch.hpp"
#include "cljonic-core-partial.hpp"

using namespace cljonic;
using namespace cljonic::core;

auto multiply2 = [](int x, int y) { return x * y; };
auto multiply3 = [](int x, int y, int z) { return x * y * z; };
auto multiply2_10 = Partial(multiply2, 10);
auto multiply3_10 = Partial(multiply3, 10);
auto multiply3_10_11 = Partial(multiply3, 10, 11);
auto multiply3_10_11_12 = Partial(multiply3, 10, 11, 12);

SCENARIO("Partial", "[CljonicCorePartial]")
{
    // CHECK(0 == multiply2_10(1, 2)); // Error: Partial function cannot be called with the specified arguments
    CHECK(20 == multiply2_10(2));
    CHECK(80 == multiply3_10(2, 4));
    CHECK(220 == multiply3_10_11(2));
    CHECK(1320 == multiply3_10_11_12());
}
