#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-core-inc.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Inc", "[CljonicCoreInc]")
{
    EnableNoHeapMessagePrinting();

    CHECK_CLJONIC(-9 == Inc(-10));
    CHECK_CLJONIC(11 == Inc(10));
    CHECK_CLJONIC(-10.1 == Inc(-11.1));
    CHECK_CLJONIC(12.1 == Inc(11.1));
    CHECK_CLJONIC(std::numeric_limits<int>::max() == Inc(std::numeric_limits<int>::max()));
    CHECK_CLJONIC(std::numeric_limits<double>::max() == Inc(std::numeric_limits<double>::max()));

    DisableNoHeapMessagePrinting();
}
