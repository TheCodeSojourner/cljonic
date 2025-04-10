#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-identity.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Identity", "[CljonicCoreIdentity]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto identityA{Identity(a)};
    CHECK_CLJONIC(Equal(a, identityA));

    const auto itr{Iterator{[](const int i) { return i + 1; }, 0}};
    const auto identityItr{Identity(itr)};
    CHECK_CLJONIC(Equal(itr, identityItr));

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto identityRng{Identity(rng)};
    CHECK_CLJONIC(Equal(rng, identityRng));

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto identityRpt{Identity(rpt)};
    CHECK_CLJONIC(Equal(rpt, identityRpt));

    constexpr auto set{Set{1, 2, 3, 1}};
    constexpr auto identitySet{Identity(set)};
    CHECK_CLJONIC(Equal(set, identitySet));

    constexpr auto str{String{"Hello"}};
    constexpr auto identityStr{Identity(str)};
    CHECK_CLJONIC(Equal(str, identityStr));

    DisableNoHeapMessagePrinting();
}
