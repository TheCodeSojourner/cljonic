#include <tuple>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-lastindexofby.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("LastIndexOfBy", "[CljonicCoreLastIndexOfBy]")
{
    constexpr auto EBF = [](const int a, const int b) { return a == b; };

    constexpr auto a{Array{11, 14, 13, 14}};
    CHECK(3 == LastIndexOfBy(EBF, a, 14));

    constexpr auto b{Array{11}};
    CHECK(0 == LastIndexOfBy(EBF, b, 11));

    CHECK(CLJONIC_INVALID_INDEX == LastIndexOfBy(EBF, Range<0>{}, 3));
    CHECK(3 == LastIndexOfBy(EBF, Repeat<4, int>{11}, 11));
    CHECK(2 == LastIndexOfBy(EBF, Set{11, 14, 13, 14}, 13));
    CHECK(2 == LastIndexOfBy(EBF, String{"adcb"}, 'c'));

    CHECK(1 == LastIndexOfBy([](const char* a, const char* b) { return std::strcmp(a, b) == 0; },
                             Array{"zero", "one", "two"},
                             "one"));
}
