#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-take.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Take", "[CljonicCoreTake]")
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    CHECK(Equal(Array<int, 0>{}, Take<0>(a)));
    CHECK(Equal(Array{0}, Take<1>(a)));
    CHECK(Equal(Array{0, 1, 2, 3, 4}, Take<5>(a)));
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, Take<50>(a)));
    CHECK(Equal(Array{1, 10, 100}, Take<3>(Iterate([](const int i) { return 10 * i; }, 1))));
    CHECK(Equal(Array<int, 0>{}, Take<50>(Range<0>{})));
    CHECK(Equal(Array{"11", "11", "11", "11", "11", "11", "11"}, Take<50>(Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array{'a', 'b'}, Take<5>(Set{'a', 'b'})));
    CHECK(Equal(Array{'H', 'e', 'l'}, Take<3>(String{"Hello"})));
}
