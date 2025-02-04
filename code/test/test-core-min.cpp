#include <tuple>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-min.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Min", "[CljonicCoreMin]")
{
    constexpr auto a{Array{11, 14, 13, 14}};
    CHECK(11 == Min(a));

    constexpr auto b{Array{11}};
    CHECK(11 == Min(b));

    const auto itr{Iterate([](const SizeType i) { return i + 1_sz; }, 1_sz)};
    CHECK(1 == Min(itr));

    CHECK(0 == Min(Range<0>{}));
    CHECK(11 == Min(Repeat<4, int>{11}));
    CHECK(11 == Min(Set{11, 14, 13, 14}));
    CHECK('a' == Min(String{"dacb"}));
    CHECK(1 == Min(1, 2, 14, 7, 9));
    CHECK('1' == Min('2', '1', '9', '7', '9'));
}
