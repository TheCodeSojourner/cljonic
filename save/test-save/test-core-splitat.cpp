#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-splitat.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("SplitAt", "[CljonicCoreSplitAt]")
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{Array<int, 10>{}, Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}}, SplitAt(0, a)));
    CHECK(Equal(Array{Array<int, 10>{0}, Array<int, 10>{1, 2, 3, 4, 5, 6, 7, 8, 9}}, SplitAt(1, a)));
    CHECK(Equal(Array{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, Array<int, 10>{}}, SplitAt(50, a)));
    CHECK(Equal(Array{Array<int, 0>{}, Array<int, 0>{}}, SplitAt(50, Range<0>{})));
    CHECK(Equal(Array{Array<const char*, 7>{"11", "11", "11", "11", "11", "11", "11"}, Array{Array<const char*, 7>{}}},
                SplitAt(50, Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array{Array<char, 2>{'a', 'b'}, Array<char, 2>{}}, SplitAt(5, Set{'a', 'b'})));
    CHECK(Equal(Array{Array<char, 5>{'H', 'e', 'l'}, Array<char, 5>{'l', 'o'}}, SplitAt(3, String{"Hello"})));
}
