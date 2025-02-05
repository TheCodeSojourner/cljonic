#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-reverse.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Reverse", "[CljonicCoreReverse]")
{
    constexpr auto itr{Iterate([](const SizeType i) { return i + 1_sz; }, 1_sz)};
    const auto rItr{Reverse(itr)};
    auto nextValue{CljonicCollectionMaximumElementCount};
    for (const auto& v : rItr)
        CHECK(nextValue-- == v);

    CHECK(Equal(Array<int, 0>{}, Reverse(Array<int, 0>{})));
    CHECK(Equal(Array{2, 1}, Reverse(Array{1, 2})));
    CHECK(Equal(Array<int, 0>{}, Reverse(Range<0>{})));
    CHECK(Equal(Array{"11", "11", "11", "11", "11", "11", "11"}, Reverse(Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array{'b', 'a'}, Reverse(Set{'a', 'b'})));
    CHECK(Equal(Array{'o', 'l', 'l', 'e', 'H'}, Reverse(String{"Hello"})));
    CHECK(Equal(Array{'i', 'H'}, Reverse(String<10>{"Hi"})));
}
