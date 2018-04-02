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
        REQUIRE(!line.intersectsAlongDirection(p1));
        REQUIRE(!line.intersectsAlongDirection(p2));
    }
    SECTION("\"Forward\" intersection") {
        vec2 p1 = {0.8f, 0.0f},
             p2 = {0.9f, 0.1f};

        REQUIRE(!line.intersects(p1));
        REQUIRE(line.intersects(p2));
        REQUIRE(!line.intersectsAlongDirection(p1));
        REQUIRE(line.intersectsAlongDirection(p2));
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

TEST_CASE("Line-line intersection distance", "[sini::Line]")
{
    SECTION("No intersection") {
        Line l1( {0.0f, 0.0f}, {1.0f, 0.0f} ),
             l2( {0.0f, 1.0f}, {1.0f, 0.0f} );

        IntersectionDistance id = intersectionDistance(l1, l2);
        REQUIRE(!id.intersect);

        id = intersectionDistance(l2, l1);
        REQUIRE(!id.intersect);
    }
    SECTION("\"Forward\" distance") {
        Line l1( {0.0f, 0.0f}, {1.0f, 0.0f} ),
             l2( {1.0f, 0.0f}, {0.0f, 1.0f} );

        IntersectionDistance id = intersectionDistance(l1, l2);
        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, 1.0f));

        id = intersectionDistance(l2, l1);
        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, 0.0f));
    }
    SECTION("\"Backward\" distance") {
        Line l1({0.0f, 0.0f}, {1.0f, 0.0f}),
             l2({-0.5f, -1.0f}, {0.0f, 1.0f});

        IntersectionDistance id = intersectionDistance(l1, l2);
        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, -0.5f));

        id = intersectionDistance(l2, l1);
        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, 1.0f));
    }
}

TEST_CASE("Line-line segment intersection distance", "[sini::Line]")
{
    Line l1( {-4.0f, -3.0f}, normalize(vec2{4.0f, 3.0f}) );
    SECTION("No intersection") {
        LineSegment l2( {0.0f, -1.0f}, {0.0f, -0.1f} );
        IntersectionDistance id = intersectionDistance(l1, l2);
        REQUIRE(!id.intersect);
    }
    SECTION("\"Forward\" distance") {
        LineSegment l2( {0.0f, -0.2f}, {0.0f, 0.2f} );
        IntersectionDistance id = intersectionDistance(l1, l2);

        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, 5.0f));
    }
    SECTION("\"Backward\" distance") {
        LineSegment l2( {-4.4f, 0.0f}, {-4.4f, -4.0f} );
        IntersectionDistance id = intersectionDistance(l1, l2);

        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, -0.5f));
    }
}

TEST_CASE("Line segment-line segment intersection distace")
{
    LineSegment l1( {-4.0f, -3.0f}, {0.4f, 0.3f} );
    SECTION("No intersection") {
        LineSegment l2( {1.0f, -1.0f}, {0.5f, 1.0f});
        IntersectionDistance id = intersectionDistance(l1, l2);

        REQUIRE(!id.intersect);
    }
    SECTION("Intersection") {
        LineSegment l2( {1.0f, -1.0f}, {-1.0f, 1.0f} );
        IntersectionDistance id = intersectionDistance(l1, l2);

        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, 5.0f / 5.5f));
    }
}

TEST_CASE("Line segment-line intersection distance")
{
    LineSegment l1( {-4.0f, -3.0f}, {0.4f, 0.3f} );
    SECTION("No intersection") {
        Line l2( {0.0f, -0.3f}, normalize(vec2{0.4f, 0.5f}) );
        IntersectionDistance id = intersectionDistance(l1, l2);

        REQUIRE(!id.intersect);
    }
    SECTION("Intersection") {
        Line l2( {0.0f, -0.3f}, {0.0f, 1.0f} );
        IntersectionDistance id = intersectionDistance(l1, l2);

        REQUIRE(id.intersect);
        REQUIRE(approxEqual(id.intersection_distance, 5.0f / 5.5f));
    }
}

TEST_CASE("Approximately equivalent lines")
{
    Line a( {0.0f, 0.0f}, normalize(vec2( 0.8f,  0.6f)) ),
         b( {1.6f, 1.2f}, normalize(vec2( 0.8f+1e-6f, 0.6f-1e-6)) ),
         c( {0.4f, 0.3f}, normalize(vec2(-0.8f, -0.6f)) ),
         d( {0.0f, 0.0f}, normalize(vec2( 0.0f,  1.0f)) ),
         e( {0.0f, 0.2f}, normalize(vec2( 0.8f,  0.6f)) );

    REQUIRE(approxEquivalent(a, b));
    REQUIRE(approxEquivalent(a, c));
    REQUIRE(approxEquivalent(b, c));
    REQUIRE(!approxEquivalent(a, d));
    REQUIRE(!approxEquivalent(a, e));
}
