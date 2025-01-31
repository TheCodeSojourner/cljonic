#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-count.hpp"
#include "cljonic-core-drop.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-size.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Drop", "[CljonicCoreDrop]")
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, Drop(0, a)));
    CHECK(Equal(Array{1, 2, 3, 4, 5, 6, 7, 8, 9}, Drop(1, a)));
    CHECK(Equal(Array{5, 6, 7, 8, 9}, Drop(5, a)));
    CHECK(Equal(Array<int, 0>{}, Drop(50, a)));
    CHECK(Equal(Array<int, 0>{}, Drop(50, Range<0>{})));
    CHECK(Equal(Array{"11", "11"}, Drop(5, Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array<int, 0>{}, Drop(5, Set{'a', 'b'})));
    CHECK(Equal(Array{'l', 'o'}, Drop(3, String{"Hello"})));

    const auto i{Iterate([](const SizeType i) { return i + 1_sz; }, 1_sz)};

    auto di{Drop(Count(i) - 5, i)};
    CHECK(Equal(Array{Count(i) - 4, Count(i) - 3, Count(i) - 2, Count(i) - 1, Count(i)}, Drop(Count(i) - 5, i)));
    CHECK(Equal(Array{Count(i) - 4, Count(i) - 3, Count(i) - 2, Count(i) - 1, Count(i)}, Drop<Size(i) - 5>(i)));
}
