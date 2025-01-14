#include <string>
#include "catch.hpp"
#include "cljonic-array.hpp"

using namespace cljonic;

SCENARIO("Array", "[CljonicArray]")
{
    {
        constexpr auto a0{Array<int, 10>{}};
        constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};
        constexpr auto a2{Array<int, 4>{1, 2, 3, 4}};
        constexpr auto a3{Array<int, 4>{1, 2, 3, 4, 5, 6}};
        constexpr auto a4{Array{1, 2, 3, 4}};

        CHECK(0 == a0.Count());
        CHECK(4 == a1.Count());
        CHECK(4 == a2.Count());
        CHECK(4 == a3.Count());
        CHECK(4 == a4.Count());

        CHECK(0 == a0[0]);

        CHECK(1 == a1[0]);
        CHECK(2 == a1[1]);
        CHECK(3 == a1[2]);
        CHECK(4 == a1[3]);
        CHECK(0 == a1[4]);

        CHECK(1 == a2[0]);
        CHECK(2 == a2[1]);
        CHECK(3 == a2[2]);
        CHECK(4 == a2[3]);
        CHECK(0 == a2[4]);

        CHECK(1 == a3[0]);
        CHECK(2 == a3[1]);
        CHECK(3 == a3[2]);
        CHECK(4 == a3[3]);
        CHECK(0 == a3[4]);

        CHECK(1 == a4[0]);
        CHECK(2 == a4[1]);
        CHECK(3 == a4[2]);
        CHECK(4 == a4[3]);
        CHECK(0 == a4[4]);
    }

    {
        constexpr auto a0{Array<float, 10>{}};
        constexpr auto a1{Array<float, 10>{1.1, 2.1, 3.1, 4.1}};
        constexpr auto a2{Array<float, 4>{1.1, 2.1, 3.1, 4.1}};
        constexpr auto a3{Array<float, 4>{1.1, 2.1, 3.1, 4.1, 5.1, 6.1}};
        constexpr auto a4{Array{1.1f, 2.1f, 3.1f, 4.1f}};

        CHECK(0 == a0.Count());
        CHECK(4 == a1.Count());
        CHECK(4 == a2.Count());
        CHECK(4 == a3.Count());
        CHECK(4 == a4.Count());

        CHECK(1.1f == a1[0]);
        CHECK(2.1f == a1[1]);
        CHECK(3.1f == a1[2]);
        CHECK(4.1f == a1[3]);
        CHECK(0.0f == a1[4]);

        CHECK(1.1f == a2[0]);
        CHECK(2.1f == a2[1]);
        CHECK(3.1f == a2[2]);
        CHECK(4.1f == a2[3]);
        CHECK(0.0f == a2[4]);

        CHECK(1.1f == a3[0]);
        CHECK(2.1f == a3[1]);
        CHECK(3.1f == a3[2]);
        CHECK(4.1f == a3[3]);
        CHECK(0.0f == a3[4]);

        CHECK(1.1f == a4[0]);
        CHECK(2.1f == a4[1]);
        CHECK(3.1f == a4[2]);
        CHECK(4.1f == a4[3]);
        CHECK(0.0f == a4[4]);
    }

    {
        constexpr auto a0{Array<double, 10>{}};
        constexpr auto a1{Array<double, 10>{1.1, 2.1, 3.1, 4.1}};
        constexpr auto a2{Array<double, 4>{1.1, 2.1, 3.1, 4.1}};
        constexpr auto a3{Array<double, 4>{1.1, 2.1, 3.1, 4.1, 5.1, 6.1}};
        constexpr auto a4{Array{1.1d, 2.1d, 3.1d, 4.1d}};

        CHECK(0 == a0.Count());
        CHECK(4 == a1.Count());
        CHECK(4 == a2.Count());
        CHECK(4 == a3.Count());
        CHECK(4 == a4.Count());

        CHECK(1.1d == a1[0]);
        CHECK(2.1d == a1[1]);
        CHECK(3.1d == a1[2]);
        CHECK(4.1d == a1[3]);
        CHECK(0.0d == a1[4]);

        CHECK(1.1d == a2[0]);
        CHECK(2.1d == a2[1]);
        CHECK(3.1d == a2[2]);
        CHECK(4.1d == a2[3]);
        CHECK(0.0d == a2[4]);

        CHECK(1.1d == a3[0]);
        CHECK(2.1d == a3[1]);
        CHECK(3.1d == a3[2]);
        CHECK(4.1d == a3[3]);
        CHECK(0.0d == a3[4]);

        CHECK(1.1d == a4[0]);
        CHECK(2.1d == a4[1]);
        CHECK(3.1d == a4[2]);
        CHECK(4.1d == a4[3]);
        CHECK(0.0d == a4[4]);
    }

    {
        constexpr auto a0{Array<const char*, 10>{}};
        constexpr auto a1{Array<const char*, 10>{"1", "2", "3", "4"}};
        constexpr auto a2{Array<const char*, 4>{"1", "2", "3", "4"}};
        constexpr auto a3{Array<const char*, 4>{"1", "2", "3", "4", "5", "6"}};
        constexpr auto a4{Array{"1", "2", "3", "4"}};

        CHECK(0 == a0.Count());
        CHECK(4 == a1.Count());
        CHECK(4 == a2.Count());
        CHECK(4 == a3.Count());
        CHECK(4 == a4.Count());

        CHECK(std::string{"1"} == a1[0]);
        CHECK(std::string{"2"} == a1[1]);
        CHECK(std::string{"3"} == a1[2]);
        CHECK(std::string{"4"} == a1[3]);
        CHECK(nullptr == a1[4]);

        CHECK(std::string{"1"} == a2[0]);
        CHECK(std::string{"2"} == a2[1]);
        CHECK(std::string{"3"} == a2[2]);
        CHECK(std::string{"4"} == a2[3]);
        CHECK(nullptr == a2[4]);

        CHECK(std::string{"1"} == a3[0]);
        CHECK(std::string{"2"} == a3[1]);
        CHECK(std::string{"3"} == a3[2]);
        CHECK(std::string{"4"} == a3[3]);
        CHECK(nullptr == a3[4]);

        CHECK(std::string{"1"} == a4[0]);
        CHECK(std::string{"2"} == a4[1]);
        CHECK(std::string{"3"} == a4[2]);
        CHECK(std::string{"4"} == a4[3]);
        CHECK(nullptr == a4[4]);
    }

    {
        constexpr auto a{Array<int, 10>{1, 2, 3, 4}};
        auto i{1};
        for (const auto& element : a)
        {
            CHECK(i == element);
            ++i;
        }
        // *a.begin() = 1; // Error: assignment of read-only location '* a.Array<int, 10>::begin() const'
    }

    {
        // an Array is a function of its index
        constexpr auto a{Array<int, 10>{1, 2, 3, 4}};
        CHECK(1 == a(0));
        CHECK(2 == a(1));
        CHECK(3 == a(2));
        CHECK(4 == a(3));
        CHECK(0 == a(4)); // index out-of-bounds so return default element, which is 0 in this case
    }
}
