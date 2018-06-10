// Testing of sini/gl/Camera using Catch
#include "catch.hpp"
#include "sini/math/Matrix.hpp"
#include "sini/gl/Camera.hpp"
#include "sini/math/MathUtilities.hpp"

using namespace sini;
using gl::Camera;

TEST_CASE("World to camera view matrix correctness", "[sini::Camera]")
{
    SECTION("Camera in origin with 1-1 aspect ratio and width 2") {
        Camera c{ {0, 0}, 1.0f, 2.0f };
        REQUIRE(approxEqual(c.worldToCameraViewMatrix(), mat3::identity()));
    }
    SECTION("Camera in (3, 5) with 1-1 aspect ratio and width 2") {
        Camera c{ {3.0f, 5.0f}, 1.0f, 2.0f };
        mat3 expected = {{ 1.0f, 0.0f, -3.0f },
                         { 0.0f, 1.0f, -5.0f },
                         { 0.0f, 0.0f,  1.0f }};
        REQUIRE(approxEqual(c.worldToCameraViewMatrix(), expected));
    }
    SECTION("Camera in origin with 2-1 aspect ratio and width 2") {
        Camera c{ {0.0f, 0.0f}, 2.0f, 2.0f };
        mat3 expected = {{ 1.0f, 0.0f, 0.0f },
                         { 0.0f, 2.0f, 0.0f },
                         { 0.0f, 0.0f, 1.0f }};
        REQUIRE(approxEqual(c.worldToCameraViewMatrix(), expected));
    }
    SECTION("Camera in origin with 1-1 aspect ration and width 3") {
        Camera c{ {0.0f, 0.0f}, 1.0f, 3.0f };
        mat3 expected = {{ 2.0f/3.0f,      0.0f, 0.0f },
                         {      0.0f, 2.0f/3.0f, 0.0f },
                         {      0.0f,      0.0f, 1.0f }};
        REQUIRE(approxEqual(c.worldToCameraViewMatrix(), expected));
    }
    SECTION("Camera in (3, 5) with 2-1 aspect ratio and width 3") {
        Camera c{ {3.0f, 5.0f}, 2.0f, 3.0f };
        mat3 expected = {{ 2.0f/3.0f,      0.0f,       -2.0f },
                         {      0.0f, 2.0f/1.5f, -10.0f/1.5f },
                         {      0.0f,      0.0f,        1.0f }};
        REQUIRE(approxEqual(c.worldToCameraViewMatrix(), expected));
    }
}
