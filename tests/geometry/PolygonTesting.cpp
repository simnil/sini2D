#include "geometry/Line.hpp"
#include "geometry/Polygon.hpp"

#include <catch.hpp>


using namespace sini;

void assertTriangleMeshesEqual(std::vector<vec3i> tm1, std::vector<vec3i> tm2) {
    REQUIRE(tm1.size() == tm2.size());
    for (size_t i = 0; i < tm1.size(); i++)
        REQUIRE(tm1[i] == tm2[i]);
}

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

TEST_CASE("Envelops point", "[sini::Polygon]")
{
    Polygon p = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f },
                 { 0.0f, 1.0f }};
    REQUIRE(p.envelops(vec2(0.2f, 0.5f)));
    REQUIRE(!p.envelops(vec2(0.7f, 0.5f)));
}

TEST_CASE("Build triangle mesh", "[sini::Polygon]")
{
    SECTION("No triangle mesh if fewer than 3 vertices") {
        Polygon p = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }};
        p.buildTriangleMesh();
        REQUIRE(!p.triangle_mesh);
    }
    SECTION("3 vertices -> same triangle") {
        Polygon p = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 1.0f }};
        p.buildTriangleMesh();
        REQUIRE(p.triangle_mesh);
        std::vector<vec3i> expected = { vec3i{ 0, 1, 2 } };
        REQUIRE(p.triangle_mesh->size() == 1);
        REQUIRE(p.triangle_mesh->at(0) == vec3i(0, 1, 2));
    }
    SECTION("Square -> 2 triangles") {
        Polygon p = {{ 0.0f, 0.0f }, { 1.0f, 0.0f },
                     { 1.0f, 1.0f }, { 0.0f, 1.0f }};
        p.buildTriangleMesh();
        REQUIRE(p.triangle_mesh);
        std::vector<vec3i> expected = {{ 0, 1, 2 }, { 0, 2, 3 }};
        assertTriangleMeshesEqual(*(p.triangle_mesh), expected);
    }
    SECTION("Simple polygon") {
        Polygon p = {{  0.0f,  0.5f },
                     { -0.5f, -0.5f },
                     {  0.5f, -0.5f },
                     {  0.5f,  0.0f },
                     {  0.6f,  0.6f }};
        p.buildTriangleMesh();
        REQUIRE(p.triangle_mesh);
        std::vector<vec3i> expected = {{ 0, 1, 2 }, { 0, 2, 3 }, { 0, 3, 4 }};
        assertTriangleMeshesEqual(*(p.triangle_mesh), expected);
    }
    SECTION("Slightly more complicated polygon") {
        Polygon p = {{  0.0f,  0.4f  }, { -0.2f,  0.1f  }, {  0.1f, -0.2f  },
                     {  0.5f,  0.0f  }, {  0.25f, 0.25f }, {  0.4f,  0.0f  },
                     {  0.0f,  0.0f  }};
        p.buildTriangleMesh();
        REQUIRE(p.triangle_mesh);
        std::vector<vec3i> expected = {{ 0, 1, 6 }, { 1, 2, 6 }, { 2, 5, 6 },
                                       { 2, 3, 5 }, { 3, 4, 5 }};
        assertTriangleMeshesEqual(*(p.triangle_mesh), expected);
    }
}
