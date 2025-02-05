#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-some.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Some", "[CljonicCoreSome]")
{
    constexpr auto Is13 = [](const int a) { return 13 == a; };
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};

    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(Some(Is13, a));

    constexpr auto b{Array{11, 12, 14}};
    CHECK(not Some(Is13, b));

    CHECK(Some(Is13, itr));

    CHECK(not Some(Is13, Range<0>{}));

    CHECK(not Some(Is13, Repeat<4, int>{11}));

    CHECK(Some(Is13, Set{11, 12, 13, 14}));
    CHECK(not Some(Is13, Set{11, 12, 14}));

    CHECK(not Some([](const char i) { return 'x' == i; }, String{"abcd"}));
    CHECK(Some([](const char i) { return 'x' == i; }, String{"axbycz"}));
}
