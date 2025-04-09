#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-droplast.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("DropLast", "[CljonicCoreDropLast]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, DropLast(0, a)));
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8}, DropLast(1, a)));
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4}, DropLast(5, a)));
    CHECK_CLJONIC(Equal(Array<int, 0>{}, DropLast(50, a)));
    CHECK_CLJONIC(Equal(Array<int, 0>{}, DropLast(50, Range<0>{})));
    CHECK_CLJONIC(Equal(Array{"11", "11"}, DropLast(5, Repeat<7, const char *>{"11"})));
    CHECK_CLJONIC(Equal(Array<int, 0>{}, DropLast(5, Set{'a', 'b'})));
    CHECK_CLJONIC(Equal(Array{'H', 'e'}, DropLast(3, String{"Hello"})));

    DisableNoHeapMessagePrinting();
}
