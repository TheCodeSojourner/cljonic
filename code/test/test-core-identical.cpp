#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-core-identical.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Identical", "[CljonicCoreIdentical]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto a2{Array<int, 10>{1, 2, 3, 4}};
    CHECK_CLJONIC(Identical(a1, a1));
    CHECK_CLJONIC_NOT(Identical(a1, a2));

    CHECK_CLJONIC_NOT(Identical(Range<5>{}, Range<5>{}));

    constexpr auto x{12};
    constexpr auto y{1};
    CHECK_CLJONIC(Identical(x, x));
    CHECK_CLJONIC_NOT(Identical(x, y));
    CHECK_CLJONIC_NOT(Identical(x, 12));
    CHECK_CLJONIC_NOT(Identical(12, 12));

    DisableNoHeapMessagePrinting();
}
