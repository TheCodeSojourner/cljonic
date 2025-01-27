#include "catch.hpp"
#include "cljonic-core-inc.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Inc", "[CljonicCoreInc]")
{
    CHECK(-9 == Inc(-10));
    CHECK(11 == Inc(10));
    CHECK(-10.1 == Inc(-11.1));
    CHECK(12.1 == Inc(11.1));
    CHECK(std::numeric_limits<int>::max() == Inc(std::numeric_limits<int>::max()));
    CHECK(std::numeric_limits<double>::max() == Inc(std::numeric_limits<double>::max()));
}
