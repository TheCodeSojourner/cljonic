#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-reduce.hpp"

using namespace cljonic;
using namespace cljonic::core;

int addCalled{0};
int Add(const int a, const int b)
{
    ++addCalled;
    return a + b;
}

SCENARIO("Reduce", "[CljonicCoreReduce]")
{
    addCalled = 0;
    const auto aAddEmpty{Array<int, 5>{}};
    CHECK(0 == Reduce(Add, aAddEmpty));
    CHECK(0 == addCalled);
    CHECK(11 == Reduce(Add, 11, aAddEmpty));
    CHECK(0 == addCalled);

    addCalled = 0;
    const auto aAddOne{Array{11}};
    CHECK(11 == Reduce(Add, aAddOne));
    CHECK(0 == addCalled);
    CHECK(22 == Reduce(Add, 11, aAddOne));
    CHECK(1 == addCalled);

    const auto a{Array{1, 2, 3, 4}};
    CHECK(10 == Reduce(Add, a));
    CHECK(21 == Reduce(Add, 11, a));

    const auto rng{Range(5)};
    CHECK(10 == Reduce(Add, rng));
    CHECK(21 == Reduce(Add, 11, rng));

    const auto rpt{Repeat(5, 5)};
    CHECK(25 == Reduce(Add, rpt));
    CHECK(36 == Reduce(Add, 11, rpt));

    const auto s{String{"Hello"}};
    CHECK(5 == Reduce([](const int count, [[maybe_unused]] const char b) { return count + 1; }, 0, s));

    // Compiler Error: Function cannot be called with two parameters of the collection value type
    // const auto ints{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([](const char* a, const int b) { return a[0] + b; }, a));

    // Compiler Error: Function cannot be called with parameters of function result type, and collection value type
    // const auto strs{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([]([[maybe_unused]] const int a, [[maybe_unused]] const int b) { return "Hello"; }, a));

    // Compiler Error: Function cannot be called with parameters of initial value type, and collection value type
    // const auto strs{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([](const int a, const int b) { return a + b; }, "Hello", a));

    // Compiler Error: Function cannot be called with parameters of function result type, and collection value type
    // const auto strs{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([]([[maybe_unused]] const int a, [[maybe_unused]] const int b) { return "Hello"; }, 11, a));
}