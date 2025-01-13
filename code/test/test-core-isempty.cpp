#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-isempty.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("IsEmpty", "[CljonicCoreIsEmpty]")
{
    CHECK(IsEmpty(Array<int, 0>{}));
    CHECK(not IsEmpty(Array{1, 2}));
    CHECK(not IsEmpty(Array<int, 5>{1, 2}));

    CHECK(IsEmpty(Range<0>{}));
    CHECK(not IsEmpty(Range<9>{}));

    CHECK(not IsEmpty(Repeat<7, const char*>{"11"}));

    CHECK(not IsEmpty(Set{'a', 'b'}));
    CHECK(not IsEmpty(Set<char, 5>{'a', 'b'}));

    CHECK(not IsEmpty(String{"Hello"}));
    CHECK(IsEmpty(String<10>{""}));
}
