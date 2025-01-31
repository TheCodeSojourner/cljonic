#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-count.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-droplast.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-size.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("DropLast", "[CljonicCoreDropLast]")
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, DropLast(0, a)));
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8}, DropLast(1, a)));
    CHECK(Equal(Array{0, 1, 2, 3, 4}, DropLast(5, a)));
    CHECK(Equal(Array<int, 0>{}, DropLast(50, a)));
    CHECK(Equal(Array<int, 0>{}, DropLast(50, Range<0>{})));
    CHECK(Equal(Array{"11", "11"}, DropLast(5, Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array<int, 0>{}, DropLast(5, Set{'a', 'b'})));
    CHECK(Equal(Array{'H', 'e'}, DropLast(3, String{"Hello"})));

    const auto i{Iterate([](const SizeType i) { return i + 1_sz; }, 1_sz)};

    CHECK(Equal(Array{1_sz, 2_sz, 3_sz, 4_sz, 5_sz}, DropLast(Count(i) - 5, i)));
    CHECK(Equal(Array{1_sz, 2_sz, 3_sz, 4_sz, 5_sz}, DropLast<Size(i) - 5>(i)));
}
