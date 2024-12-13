#include <string>
#include <variant>
#include "catch.hpp"
#include "cljonic-vector.hpp"

using namespace cljonic;

SCENARIO("Vector", "[CljonicVector]")
{
    {
        auto v0{Vector<int, 10>{}};
        auto v1{Vector<int, 10>{1, 2, 3, 4}};
        auto v2{Vector<int, 4>{1, 2, 3, 4}};
        auto v3{Vector<int, 4>{1, 2, 3, 4, 5, 6}};
        auto v4{Vector{1, 2, 3, 4}};

        CHECK(0 == v0.Count());
        CHECK(4 == v1.Count());
        CHECK(4 == v2.Count());
        CHECK(4 == v3.Count());
        CHECK(4 == v4.Count());

        CHECK(0 == v0[0]);

        CHECK(1 == v1[0]);
        CHECK(2 == v1[1]);
        CHECK(3 == v1[2]);
        CHECK(4 == v1[3]);
        CHECK(0 == v1[4]);

        CHECK(1 == v2[0]);
        CHECK(2 == v2[1]);
        CHECK(3 == v2[2]);
        CHECK(4 == v2[3]);
        CHECK(0 == v2[4]);

        CHECK(1 == v3[0]);
        CHECK(2 == v3[1]);
        CHECK(3 == v3[2]);
        CHECK(4 == v3[3]);
        CHECK(0 == v3[4]);

        CHECK(1 == v4[0]);
        CHECK(2 == v4[1]);
        CHECK(3 == v4[2]);
        CHECK(4 == v4[3]);
        CHECK(0 == v4[4]);
    }

    {
        auto v0{Vector<float, 10>{}};
        auto v1{Vector<float, 10>{1.1, 2.1, 3.1, 4.1}};
        auto v2{Vector<float, 4>{1.1, 2.1, 3.1, 4.1}};
        auto v3{Vector<float, 4>{1.1, 2.1, 3.1, 4.1, 5.1, 6.1}};
        auto v4{Vector{1.1f, 2.1f, 3.1f, 4.1f}};

        CHECK(0 == v0.Count());
        CHECK(4 == v1.Count());
        CHECK(4 == v2.Count());
        CHECK(4 == v3.Count());
        CHECK(4 == v4.Count());

        CHECK(1.1f == v1[0]);
        CHECK(2.1f == v1[1]);
        CHECK(3.1f == v1[2]);
        CHECK(4.1f == v1[3]);
        CHECK(0.0f == v1[4]);

        CHECK(1.1f == v2[0]);
        CHECK(2.1f == v2[1]);
        CHECK(3.1f == v2[2]);
        CHECK(4.1f == v2[3]);
        CHECK(0.0f == v2[4]);

        CHECK(1.1f == v3[0]);
        CHECK(2.1f == v3[1]);
        CHECK(3.1f == v3[2]);
        CHECK(4.1f == v3[3]);
        CHECK(0.0f == v3[4]);

        CHECK(1.1f == v4[0]);
        CHECK(2.1f == v4[1]);
        CHECK(3.1f == v4[2]);
        CHECK(4.1f == v4[3]);
        CHECK(0.0f == v4[4]);
    }

    {
        auto v0{Vector<double, 10>{}};
        auto v1{Vector<double, 10>{1.1, 2.1, 3.1, 4.1}};
        auto v2{Vector<double, 4>{1.1, 2.1, 3.1, 4.1}};
        auto v3{Vector<double, 4>{1.1, 2.1, 3.1, 4.1, 5.1, 6.1}};
        auto v4{Vector{1.1d, 2.1d, 3.1d, 4.1d}};

        CHECK(0 == v0.Count());
        CHECK(4 == v1.Count());
        CHECK(4 == v2.Count());
        CHECK(4 == v3.Count());
        CHECK(4 == v4.Count());

        CHECK(1.1d == v1[0]);
        CHECK(2.1d == v1[1]);
        CHECK(3.1d == v1[2]);
        CHECK(4.1d == v1[3]);
        CHECK(0.0d == v1[4]);

        CHECK(1.1d == v2[0]);
        CHECK(2.1d == v2[1]);
        CHECK(3.1d == v2[2]);
        CHECK(4.1d == v2[3]);
        CHECK(0.0d == v2[4]);

        CHECK(1.1d == v3[0]);
        CHECK(2.1d == v3[1]);
        CHECK(3.1d == v3[2]);
        CHECK(4.1d == v3[3]);
        CHECK(0.0d == v3[4]);

        CHECK(1.1d == v4[0]);
        CHECK(2.1d == v4[1]);
        CHECK(3.1d == v4[2]);
        CHECK(4.1d == v4[3]);
        CHECK(0.0d == v4[4]);
    }

    {
        auto v0{Vector<const char*, 10>{}};
        auto v1{Vector<const char*, 10>{"1", "2", "3", "4"}};
        auto v2{Vector<const char*, 4>{"1", "2", "3", "4"}};
        auto v3{Vector<const char*, 4>{"1", "2", "3", "4", "5", "6"}};
        auto v4{Vector{"1", "2", "3", "4"}};

        CHECK(0 == v0.Count());
        CHECK(4 == v1.Count());
        CHECK(4 == v2.Count());
        CHECK(4 == v3.Count());
        CHECK(4 == v4.Count());

        CHECK(std::string{"1"} == v1[0]);
        CHECK(std::string{"2"} == v1[1]);
        CHECK(std::string{"3"} == v1[2]);
        CHECK(std::string{"4"} == v1[3]);
        CHECK(nullptr == v1[4]);

        CHECK(std::string{"1"} == v2[0]);
        CHECK(std::string{"2"} == v2[1]);
        CHECK(std::string{"3"} == v2[2]);
        CHECK(std::string{"4"} == v2[3]);
        CHECK(nullptr == v2[4]);

        CHECK(std::string{"1"} == v3[0]);
        CHECK(std::string{"2"} == v3[1]);
        CHECK(std::string{"3"} == v3[2]);
        CHECK(std::string{"4"} == v3[3]);
        CHECK(nullptr == v3[4]);

        CHECK(std::string{"1"} == v4[0]);
        CHECK(std::string{"2"} == v4[1]);
        CHECK(std::string{"3"} == v4[2]);
        CHECK(std::string{"4"} == v4[3]);
        CHECK(nullptr == v4[4]);
    }

    {
        using T = std::variant<int, double, char, const char*>;

        auto v0{Vector<T, 10>{}};
        auto v1{Vector<T, 10>{1, 2.1, '3', "4"}};
        auto v2{Vector<T, 4>{1, 2.1, '3', "4"}};
        auto v3{Vector<T, 4>{1, 2.1, '3', "4", 5, 6.1}};
        auto v4{Vector{T{1}, T{2.1}, T{'3'}, T{"4"}}};

        CHECK(0 == v0.Count());
        CHECK(4 == v1.Count());
        CHECK(4 == v2.Count());
        CHECK(4 == v3.Count());
        CHECK(4 == v4.Count());

        CHECK(1 == *std::get_if<int>(&v1[0]));
        CHECK(2.1 == *std::get_if<double>(&v1[1]));
        CHECK('3' == *std::get_if<char>(&v1[2]));
        CHECK(std::string{"4"} == *std::get_if<const char*>(&v1[3]));
        CHECK(0 == *std::get_if<int>(&v1[4]));

        CHECK(1 == *std::get_if<int>(&v2[0]));
        CHECK(2.1 == *std::get_if<double>(&v2[1]));
        CHECK('3' == *std::get_if<char>(&v2[2]));
        CHECK(std::string{"4"} == *std::get_if<const char*>(&v2[3]));
        CHECK(0 == *std::get_if<int>(&v2[4]));

        CHECK(1 == *std::get_if<int>(&v3[0]));
        CHECK(2.1 == *std::get_if<double>(&v3[1]));
        CHECK('3' == *std::get_if<char>(&v3[2]));
        CHECK(std::string{"4"} == *std::get_if<const char*>(&v3[3]));
        CHECK(0 == *std::get_if<int>(&v3[4]));

        CHECK(1 == *std::get_if<int>(&v4[0]));
        CHECK(2.1 == *std::get_if<double>(&v4[1]));
        CHECK('3' == *std::get_if<char>(&v4[2]));
        CHECK(std::string{"4"} == *std::get_if<const char*>(&v4[3]));
        CHECK(0 == *std::get_if<int>(&v4[4]));
    }
}
