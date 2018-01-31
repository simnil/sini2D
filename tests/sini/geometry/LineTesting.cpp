// Testing of sini/geometry/Line using Catch
#include "catch.hpp"
#include "sini/geometry/Line.hpp"
#include "sini/math/MathUtilities.hpp"

using namespace sini;

TEST_CASE("Line-line intersection", "[sini::Line]")
{
   Line l1( {0.0f, 0.0f}, {1.0f, 0.0f} ),
        l2( {0.5f, 1.0f}, {0.0f, -1.0f} ),
        l3( {0.0f, 1.0f}, {1.0f, 0.0f} );

   REQUIRE(intersect(l1, l2));
   REQUIRE(intersect(l2, l3));
   REQUIRE(!intersect(l1, l3));

}

TEST_CASE("Line segment-line segment intersection", "[sini::LineSegment]")
{
    LineSegment l1( {0.0f, 0.0f}, {1.0f, 1.0f} ),
                l2( {1.0f, -0.5f}, {0.0f, 0.5f} ),
                l3( {0.5f, -0.3f}, {0.8f, -0.01f} );

    SECTION("Intersection test") {
        REQUIRE(intersect(l1, l2));
        REQUIRE(!intersect(l1, l3));
        REQUIRE(intersect(l2, l3));
    }
    SECTION("Intersection point") {
        IntersectionPoint ip = intersection(l1, l2);
        REQUIRE(ip.intersect);
        REQUIRE(approxEqual(ip.intersection_point, {0.25f, 0.25f}));

        ip = intersection(l1, l3);
        REQUIRE(!ip.intersect);

        ip = intersection(l2, l3);
        REQUIRE(ip.intersect);
        REQUIRE(approxEqual(ip.intersection_point, {77.0f/118.0f, -9.0f/59.0f}));
    }
}

TEST_CASE("Line-line segment intersection", "[sini::Line]")
{
    Line        l1( {0.0f, 0.0f}, normalize(vec2{1.0f, 1.0f}) );
    LineSegment l2( {0.0f, 1.0f}, {0.8f, 1.0f} ),
                l3( {0.0f, 1.0f}, {1.2f, 1.0f});

    SECTION("Intersection test") {
        REQUIRE(!intersect(l1, l2));
        REQUIRE(intersect(l1, l3));;
    }
    SECTION("Intersection point") {
        IntersectionPoint ip = intersection(l1, l2);
        REQUIRE(!ip.intersect);

        ip = intersection(l1, l3);
        REQUIRE(ip.intersect);
        REQUIRE(approxEqual(ip.intersection_point, {1.0f, 1.0f}));
    }
}

TEST_CASE("Line-point intersection", "[sini::Line]")
{
    Line line( {0.0f, 1.0f}, normalize(vec2{1.0f, -1.0f}) );

    SECTION("\"Backward\" intersection") {
        vec2 p1 = {-0.5f, 1.5f},
             p2 = {-0.2f, 1.0f};

        REQUIRE(line.intersects(p1));
        REQUIRE(!line.intersects(p2));
    }
    SECTION("\"Forward\" intersection") {
        vec2 p1 = {0.8f, 0.0f},
             p2 = {0.9f, 0.1f};

        REQUIRE(!line.intersects(p1));
        REQUIRE(line.intersects(p2));
    }
}

TEST_CASE("Line segment-point intersection", "[sini::LineSegment]")
{
    LineSegment line( {0.0f, 1.0f}, {1.0f, 0.0f} );

    SECTION("Intersection along line segment") {
        vec2 p1 = {0.4f, 0.4f},
             p2 = {0.6f, 0.4f};

        REQUIRE(!line.intersects(p1));
        REQUIRE(line.intersects(p2));
    }
    SECTION("Intersection at/before segment start") {
        vec2 p1 = {0.0f, 1.0f},
             p2 = p1 + 1e-6f * normalize(vec2{-1.0f, 0.0f}),
             p3 = p1 + 0.1f * normalize(vec2{-1.0f, 1.0f});

        REQUIRE(line.intersects(p1));
        REQUIRE(line.intersects(p2)); // Default tolerance is 1e-5
        REQUIRE(!line.intersects(p3));
    }
    SECTION("Intersection at/after segment end") {
        vec2 p1 = {1.0f, 0.0f},
             p2 = p1 + 1e-6f * normalize(vec2{1.0f, -0.9f}),
             p3 = p1 + 0.1f * normalize(vec2{1.0f, -1.1f});

        REQUIRE(line.intersects(p1));
        REQUIRE(line.intersects(p2)); // Default tolerance is 1e-5
        REQUIRE(!line.intersects(p3));
    }
}
