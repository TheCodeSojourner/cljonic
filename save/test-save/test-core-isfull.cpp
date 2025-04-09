#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-isfull.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("IsFull", "[CljonicCoreIsFull]")
{
    CHECK(IsFull(Array<int, 0>{}));
    CHECK(IsFull(Array{1, 2}));
    CHECK(not IsFull(Array<int, 5>{1, 2}));

    CHECK(IsFull(Range<0>{}));
    CHECK(IsFull(Range<9>{}));

    CHECK(IsFull(Repeat<7, const char*>{"11"}));

    CHECK(IsFull(Set{'a', 'b'}));
    CHECK(not IsFull(Set<char, 5>{'a', 'b'}));

    CHECK(IsFull(String{"Hello"}));
    CHECK(not IsFull(String<10>{"Hi"}));
}
