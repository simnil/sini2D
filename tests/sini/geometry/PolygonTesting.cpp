// Testing of sini/geometry/Polygon using Catch
#include "catch.hpp"
#include "sini/geometry/Polygon.hpp"

using namespace sini;

TEST_CASE("Polygon constructors", "[sini::Polygon]")
{
    SECTION("Copy std::vector") {
        std::vector<vec2> vertices{{ 1.0f }, { 2.0f }};
        Polygon p{ vertices };

        REQUIRE(p.vertices.size() == 2);
        REQUIRE(vertices.size() == 2);
    }
    SECTION("Move std::vector") {
        std::vector<vec2> vertices{{ 2.0f }, { 3.0f }};
        Polygon p{ std::move(vertices) };

        REQUIRE(vertices.size() == 0);
        REQUIRE(p.vertices.size() == 2);
    }
    SECTION("initializer_list") {
        Polygon p = {{ 1.0f }, { 2.0f }, { 3.0f }};

        REQUIRE(p.vertices.size() == 3);
    }
}

TEST_CASE("Get polygon lines"){
    Polygon p = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 1.0f }};
    std::vector<LineSegment> lines = p.lines();

    REQUIRE(lines[0] == LineSegment(p.vertices[0], p.vertices[1]));
    REQUIRE(lines[1] == LineSegment(p.vertices[1], p.vertices[2]));
    REQUIRE(lines[2] == LineSegment(p.vertices[2], p.vertices[0]));
}

TEST_CASE("Envelops point")
{
    Polygon p = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f },
                 { 0.0f, 1.0f }};
    REQUIRE(p.envelops(vec2(0.2f, 0.5f)));
    REQUIRE(!p.envelops(vec2(0.7f, 0.5f)));
}
