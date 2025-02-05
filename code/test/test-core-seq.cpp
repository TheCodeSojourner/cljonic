#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-seq.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Seq", "[CljonicCoreSeq]")
{
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    const auto s{Seq(itr)};
    CHECK(1 == *s.begin());
    CHECK(CljonicCollectionMaximumElementCount == *(s.end() - 1));

    CHECK(Equal(Array<int, 0>{}, Seq(Array<int, 0>{})));
    CHECK(Equal(Array{1, 2}, Seq(Array{1, 2})));
    CHECK(Equal(Array<int, 0>{}, Seq(Range<0>{})));
    CHECK(Equal(Array{"11", "11", "11", "11", "11", "11", "11"}, Seq(Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array{'a', 'b'}, Seq(Set{'a', 'b'})));
    CHECK(Equal(Array{'H', 'e', 'l', 'l', 'o'}, Seq(String{"Hello"})));
    CHECK(Equal(Array<char, 10>{'H', 'i'}, Seq(String<10>{"Hi"})));
}
