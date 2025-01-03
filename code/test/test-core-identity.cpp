#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-core-identity.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Identity", "[CljonicCoreIdentity]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto identityA{Identity(a)};

    constexpr auto rng{Range<1, 5>{}};
    const auto identityRng{Identity(rng)};

    const auto GetIdentity = [&]() { return Identity(a); };

    CHECK(identityA == GetIdentity());
    CHECK(identityA != identityRng);
}
