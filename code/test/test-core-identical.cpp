#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-identical.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Identical", "[CljonicCoreIdentical]")
{
    constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto a2{Array<int, 10>{1, 2, 3, 4}};
    CHECK(Identical(a1));
    CHECK(Identical(a1, a1));
    CHECK(not Identical(a1, a2));

    CHECK(not Identical(Range<5>{}, Range<5>{}));

    constexpr auto x{12};
    constexpr auto y{1};
    CHECK(Identical(x, x));
    CHECK(not Identical(x, y));
}
