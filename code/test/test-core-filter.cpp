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

    const auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    const auto af{Filter(Even, a)};

    const auto rng{Range<10>{}};
    const auto rngf{Filter(Even, rng)};

    const auto rpt{Repeat<10, int>{1}};
    const auto rptf{Filter(Even, rpt)};

    const auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    const auto sf{Filter(Even, s)};

    const auto strf{Filter([](const char c) { return ('l' == c); }, String{"Hello"})};
}
