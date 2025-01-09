#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-filter.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

SCENARIO("Filter", "[CljonicCoreFilter]")
{

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto af{Filter(Even, a)};

    constexpr auto rng{Range<10>{}};
    constexpr auto rngf{Filter(Even, rng)};

    constexpr auto rpt{Repeat<10, int>{1}};
    constexpr auto rptf{Filter(Even, rpt)};

    constexpr auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    constexpr auto sf{Filter(Even, s)};

    constexpr auto strf{Filter([](const char c) { return ('l' == c); }, String{"Hello"})};
}
