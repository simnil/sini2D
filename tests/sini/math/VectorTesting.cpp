// Testing of sini/math/Vector using Catch
#include <initializer_list>

#include "catch.hpp"
#include "sini/math/Vector.hpp"
#include "sini/math/MathUtilities.hpp"


using namespace sini;
template<typename T> using init_list = std::initializer_list<T>;

TEST_CASE("2D vector specialization", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int, 2>) == 2*sizeof(int) );
    }
    SECTION("Element access") {
        Vector<int, 2> vec;
        SECTION("[] operator") {
            vec[0] = 13;
            vec[1] = -11;
            REQUIRE(vec.x == 13);
            REQUIRE(vec.y == -11);
        }
        SECTION("Data pointer") {
            vec.data()[0] = 2;
            vec.data()[1] = 3;
            REQUIRE(vec[0] == 2);
            REQUIRE(vec[1] == 3);
        }
    }
    SECTION("Fill constructor") {
        Vector<int, 2> vec{1};
        REQUIRE(vec.x == 1);
        REQUIRE(vec.y == 1);
    }
    SECTION("Element constructor") {
        Vector<int, 2> vec{ 1, 2 };
        REQUIRE(vec.x == 1);
        REQUIRE(vec.y == 2);
    }
    SECTION("Array constructor") {
        const int arr[] = { 1, -2, 3 };

        Vector<int, 2> v1{arr};
        REQUIRE(v1.x == 1);
        REQUIRE(v1.y == -2);

        Vector<int, 2> v2{arr+1};
        REQUIRE(v2.x == -2);
        REQUIRE(v2.y == 3);
    }
    SECTION("Casting constructor") {
        Vector<int,2> vec(vec2{ 1.0f, -2.0f });
        REQUIRE(vec.x == 1);
        REQUIRE(vec.y == -2);
    }
}

TEST_CASE("3D vector specialization", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int, 3>) == 3 * sizeof(int) );
    }
    SECTION("Element access") {
        Vector<int, 3> vec;
        SECTION("[] operator") {
            vec[0] = 3;
            vec[1] = 2;
            vec[2] = 1;
            REQUIRE(vec.x == 3);
            REQUIRE(vec.y == 2);
            REQUIRE(vec.z == 1);
            REQUIRE(vec.xy == (Vector<int, 2> {3, 2}));
            REQUIRE(vec.yz == (Vector<int, 2> {2, 1}));
        }
        SECTION("Data pointer") {
            vec.data()[0] = -4;
            vec.data()[1] = -5;
            vec.data()[2] = -6;
            REQUIRE(vec[0] == -4);
            REQUIRE(vec[1] == -5);
            REQUIRE(vec[2] == -6);
            REQUIRE(vec.xy == (Vector<int, 2> {-4, -5}));
            REQUIRE(vec.yz == (Vector<int, 2> {-5, -6}));
        }
    }
    SECTION("Fill constructor") {
        Vector<int, 3> v{1};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 1);
        REQUIRE(v.z == 1);
    }
    SECTION("Element constructor") {
        SECTION("(x, y, z)") {
            Vector<int, 3> vec{ 3, 5, 7 };
            REQUIRE(vec.x == 3);
            REQUIRE(vec.y == 5);
            REQUIRE(vec.z == 7);
        }
        SECTION("(xy, z)") {
            Vector<int, 3> vec{ Vector<int,2> {11, 13}, 17 };
            REQUIRE(vec.x == 11);
            REQUIRE(vec.y == 13);
            REQUIRE(vec.z == 17);
        }
        SECTION("(x, yz)") {
            Vector<int, 3> vec{ -2, Vector<int,2> {3, -5} };
            REQUIRE(vec.x == -2);
            REQUIRE(vec.y == 3);
            REQUIRE(vec.z == -5);
        }
    }
    SECTION("Array constructor") {
        const int arr[] = { -11, 13, -17, -19 };
        Vector<int, 3> v1{arr};
        REQUIRE(v1.x == -11);
        REQUIRE(v1.y == 13);
        REQUIRE(v1.z == -17);

        Vector<int, 3> v2{arr+1};
        REQUIRE(v2.x == 13);
        REQUIRE(v2.y == -17);
        REQUIRE(v2.z == -19);
    }
    SECTION("Casting constructor") {
        Vector<int, 3> vec(vec3{ -3.0f, 5.0f, 7.0f });
        REQUIRE(vec.x == -3);
        REQUIRE(vec.y == 5);
        REQUIRE(vec.z == 7);
    }
}

TEST_CASE("4D vector specialization", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int, 4>) == 4 * sizeof(int) );
    }
    SECTION("Element access") {
        Vector<int, 4> vec; 
        SECTION("[] operator") {
            vec[0] = 11;
            vec[1] = -13;
            vec[2] = 17;
            vec[3] = -19;
            REQUIRE(vec.x == 11);
            REQUIRE(vec.y == -13);
            REQUIRE(vec.z == 17);
            REQUIRE(vec.w == -19);
        }
        SECTION("Data pointer") {
            vec.data()[0] = 7;
            vec.data()[1] = 5;
            vec.data()[2] = 3;
            vec.data()[3] = 2;
            REQUIRE(vec[0] == 7);
            REQUIRE(vec[1] == 5);
            REQUIRE(vec[2] == 3);
            REQUIRE(vec[3] == 2);
        }
    }
    SECTION("Fill constructor") {
        Vector<int, 4> vec{ 3 };
        REQUIRE(vec[0] == 3);
        REQUIRE(vec[1] == 3);
        REQUIRE(vec[2] == 3);
        REQUIRE(vec[3] == 3);
    }
    SECTION("Element constructor") {
        SECTION("(x, y, z, w)") {
            Vector<int, 4> vec{ 1, 2, 3, 4 };
            REQUIRE(vec.x == 1);
            REQUIRE(vec.y == 2);
            REQUIRE(vec.z == 3);
            REQUIRE(vec.w == 4);
        }
        SECTION("(xy, z, w)") {
            Vector<int, 4> vec{ Vector<int,2> {4, 3}, 2, 1 };
            REQUIRE(vec.x == 4);
            REQUIRE(vec.y == 3);
            REQUIRE(vec.z == 2);
            REQUIRE(vec.w == 1);
        }
        SECTION("(x, yz, w)") {
            Vector<int, 4> vec{ 2, Vector<int,2> {4, 6}, 8 };
            REQUIRE(vec.x == 2);
            REQUIRE(vec.y == 4);
            REQUIRE(vec.z == 6);
            REQUIRE(vec.w == 8);
        }
        SECTION("(x, y, zw)") {
            Vector<int, 4> vec{ -1, -2, Vector<int,2> {-3, -4} };
            REQUIRE(vec.x == -1);
            REQUIRE(vec.y == -2);
            REQUIRE(vec.z == -3);
            REQUIRE(vec.w == -4);
        }
        SECTION("(xy, yz)") {
            Vector<int, 4> vec{ Vector<int,2> {3, 5}, Vector<int,2> {11, 13} };
            REQUIRE(vec.x == 3);
            REQUIRE(vec.y == 5);
            REQUIRE(vec.z == 11);
            REQUIRE(vec.w == 13);
        }
    }
    SECTION("Array constructor") {
        const int arr[] = { 19, 17, 13, 11, 7 };
        Vector<int, 4> v1{arr};
        REQUIRE(v1.x == 19);
        REQUIRE(v1.y == 17);
        REQUIRE(v1.z == 13);
        REQUIRE(v1.w == 11);

        Vector<int, 4> v2{arr+1};
        REQUIRE(v2.x == 17);
        REQUIRE(v2.y == 13);
        REQUIRE(v2.z == 11);
        REQUIRE(v2.w == 7);
    }
    SECTION("Casting constructor") {
        Vector<int, 4> vec(vec4{ 16.0f, 8.0f, 4.0f, 2.0f });
        REQUIRE(vec.x == 16);
        REQUIRE(vec.y == 8);
        REQUIRE(vec.z == 4);
        REQUIRE(vec.w == 2);
    }
}

TEST_CASE("General vector", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int,5>) == 5 * sizeof(int) );
    }
    SECTION("Element access") {
        Vector<int, 5> vec;
        SECTION("[] operator") {
            vec[0] = 2;
            vec[1] = 3;
            vec[2] = 5;
            vec[3] = 7;
            vec[4] = 11;
            REQUIRE(vec[0] == 2);
            REQUIRE(vec[1] == 3);
            REQUIRE(vec[2] == 5);
            REQUIRE(vec[3] == 7);
            REQUIRE(vec[4] == 11);
        }
        SECTION("Data pointer") {
            vec.data()[0] = -11;
            vec.data()[1] = -7;
            vec.data()[2] = -5;
            vec.data()[3] = -3;
            vec.data()[4] = -2;
            REQUIRE(vec[0] == -11);
            REQUIRE(vec[1] == -7);
            REQUIRE(vec[2] == -5);
            REQUIRE(vec[3] == -3);
            REQUIRE(vec[4] == -2);
        }
    }
    SECTION("Fill constructor") {
        Vector<int, 5> vec{-1};
        REQUIRE(vec[0] == -1);
        REQUIRE(vec[1] == -1);
        REQUIRE(vec[2] == -1);
        REQUIRE(vec[3] == -1);
        REQUIRE(vec[4] == -1);
    }
    SECTION("Array constructor") {
        const int arr[] = { 1, -2, 4, -8, 16, -32 };
        Vector<int, 5> v1{arr};
        REQUIRE(v1[0] == 1);
        REQUIRE(v1[1] == -2);
        REQUIRE(v1[2] == 4);
        REQUIRE(v1[3] == -8);
        REQUIRE(v1[4] == 16);

        Vector<int, 5> v2{arr+1};
        REQUIRE(v2[0] == -2);
        REQUIRE(v2[1] == 4);
        REQUIRE(v2[2] == -8);
        REQUIRE(v2[3] == 16);
        REQUIRE(v2[4] == -32);
    }
    SECTION("Casting constructor") {
        Vector<float, 5> vecf{ init_list<float>{ 2.0f, 1.0f, 0.0f, -1.0f, -2.0f }.begin() };
        Vector<int, 5> vec(vecf);
        REQUIRE(vec[0] == 2);
        REQUIRE(vec[1] == 1);
        REQUIRE(vec[2] == 0);
        REQUIRE(vec[3] == -1);
        REQUIRE(vec[4] == -2);
    }
}

TEST_CASE("Vector arithmetics", "[sini::Vector]")
{
    vec3i v1{ 3, 5, 7 }, v2{ -1, 0, 1 };
    // Check v1
    REQUIRE(v1.x == 3);
    REQUIRE(v1.y == 5);
    REQUIRE(v1.z == 7);
    // Check v2
    REQUIRE(v2.x == -1);
    REQUIRE(v2.y == 0);
    REQUIRE(v2.z == 1);

    SECTION("Addition") {
        vec3i v3 = v1 + v2;
        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 5);
        REQUIRE(v3.z == 8);

        // Check that initial vectors didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);
    }
    SECTION("Addition assignment") {
        v1 += v2;
        REQUIRE(v1.x == 2);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 8);

        // Check that v2 wasn't altered
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);
    }
    SECTION("Subtraction") {
        vec3i v3 = v1 - v2;
        REQUIRE(v3.x == 4);
        REQUIRE(v3.y == 5);
        REQUIRE(v3.z == 6);

        // Check that initial vectors didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);
    }
    SECTION("Subtraction assignment") {
        v1 -= v2;
        REQUIRE(v1.x == 4);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 6);

        // Check that v2 wasn't altered
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);
    }
    SECTION("Negation") {
        vec3i v3 = -v1;
        REQUIRE(v3.x == -3);
        REQUIRE(v3.y == -5);
        REQUIRE(v3.z == -7);
    }
    SECTION("Multiplication with scalar") {
        vec3i v3 = 2 * v1;
        REQUIRE(v3.x == 6);
        REQUIRE(v3.y == 10);
        REQUIRE(v3.z == 14);

        // Check that initial vector didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
    }
    SECTION("Scalar multiplication assignment") {
        v1 *= 2;
        REQUIRE(v1.x == 6);
        REQUIRE(v1.y == 10);
        REQUIRE(v1.z == 14);
    }
    SECTION("Element-wise multiplication") {
        vec3i v3 = v1 * v2;
        REQUIRE(v3.x == -3);
        REQUIRE(v3.y == 0);
        REQUIRE(v3.z == 7);

        // Check that initial vectors didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);
    }
    SECTION("Element-wise multiplication assignment") {
        v1 *= v2;
        REQUIRE(v1.x == -3);
        REQUIRE(v1.y == 0);
        REQUIRE(v1.z == 7);

        // Check that v2 wasn't altered
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);
    }
    SECTION("Division with scalar") {
        vec3i v3 = Vector<int, 3>{ 6, 9, 27 } / 3;
        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 9);
    }
    SECTION("Scalar division assignment") {
        vec3i v3 = Vector<int, 3>{ 6, 9, 27 };
        REQUIRE(v3.x == 6);
        REQUIRE(v3.y == 9);
        REQUIRE(v3.z == 27);
        v3 /= 3;
        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 9);
    }
    SECTION("Element-wise division") {
        vec3i v3 = vec3i{ 4, 9, 16 } / vec3i{ 2, 3, 4 };
        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 4);
    }
    SECTION("Element-wise division assignment") {
        vec3i v3 = vec3i{ 4, 9, 16 };
        v3 /= vec3i{ 2, 3, 4 };
        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 4);
    }
    SECTION("Modulus with scalar") {
        vec3i v3 = v1 % 3;
        REQUIRE(v3.x == 0);
        REQUIRE(v3.y == 2);
        REQUIRE(v3.z == 1);

        // Check that initial vector didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
    }
    SECTION("Element-wise modulus") {
        vec3i v3 = v1 % vec3i{ 2, 3, 4 };
        REQUIRE(v3.x == 1);
        REQUIRE(v3.y == 2);
        REQUIRE(v3.z == 3);

        // Check that initial vector didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
    }
}

TEST_CASE("Vector comparisons", "[sini::Vector]")
{
    vec3i v1{ 1, 2, 3 },
          v2{ 1, 3, 5 },
          v3{ 1, 3, 5 };

    SECTION("==") {
        REQUIRE(v1 == v1);
        REQUIRE(v2 == v2);
        REQUIRE(v3 == v3);
        REQUIRE(v2 == v3);
        REQUIRE(v3 == v2);
    }
    SECTION("!=") {
        REQUIRE(v1 != v2);
        REQUIRE(v2 != v1);
        REQUIRE(v1 != v3);
        REQUIRE(v3 != v1);
    }
}

TEST_CASE("Dot/scalar product", "[sini::Vector]")
{
    vec3i v1{ 3, 5, 7 }, v2{ -1, 0, 1 };
    // Check v1
    REQUIRE(v1.x == 3);
    REQUIRE(v1.y == 5);
    REQUIRE(v1.z == 7);
    // Check v2
    REQUIRE(v2.x == -1);
    REQUIRE(v2.y == 0);
    REQUIRE(v2.z == 1);

    SECTION("Function form") {
        int32_t sp = dot(v1, v2);
        REQUIRE(sp == 4);

        sp = dot(v2, v1);
        REQUIRE(sp == 4);

        sp = dot(v2, v2);
        REQUIRE(sp == 2);
    }
    SECTION("Operator form") {
        int32_t sp = v1 | v2;
        REQUIRE(sp == 4);

        sp = v2 | v1;
        REQUIRE(sp == 4);

        sp = v2 | v2;
        REQUIRE(sp == 2);
    }

    // Check that initial vectors didn't change
    REQUIRE(v1.x == 3);
    REQUIRE(v1.y == 5);
    REQUIRE(v1.z == 7);
    REQUIRE(v2.x == -1);
    REQUIRE(v2.y == 0);
    REQUIRE(v2.z == 1);
}

TEST_CASE("Cross product", "[sini::Vector]")
{
    vec3i v1{ 3, 5, 7 }, v2{ -1, 0, 1 };
    // Check v1
    REQUIRE(v1.x == 3);
    REQUIRE(v1.y == 5);
    REQUIRE(v1.z == 7);
    // Check v2
    REQUIRE(v2.x == -1);
    REQUIRE(v2.y == 0);
    REQUIRE(v2.z == 1);

    SECTION("Function form") {
        vec3i v3 = cross(v1, v2);
        REQUIRE(v3.x == 5);
        REQUIRE(v3.y == -10);
        REQUIRE(v3.z == 5);

        // Check that initial vectors didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);

        // cross(v1, v2) = - cross(v2, v1)
        v3 = cross(v2, v1);
        REQUIRE(v3.x == -5);
        REQUIRE(v3.y == 10);
        REQUIRE(v3.z == -5);

        // cross(v1, v1) = 0
        v3 = cross(v1, v1);
        REQUIRE(v3.x == 0);
        REQUIRE(v3.y == 0);
        REQUIRE(v3.z == 0);
    }
    SECTION("Operator form") {
        vec3i v3 = v1 ^ v2;
        REQUIRE(v3.x == 5);
        REQUIRE(v3.y == -10);
        REQUIRE(v3.z == 5);

        // Check that initial vectors didn't change
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y == 0);
        REQUIRE(v2.z == 1);
    }
}

TEST_CASE("Vector length and norm", "[sini::Vector]")
{
    vec3 vec{ 1.0f, 0.0f, -1.0f };
    float tol = 1e-10f;
    SECTION("Length") {
        float len = length(vec);
        REQUIRE( approxEqual(len, std::sqrt(2.0f), tol) );
    }
    SECTION("2-norm (= length)") {
        float len = norm(vec);
        REQUIRE( approxEqual(len, std::sqrt(2.0f), tol) );
    }
    SECTION("p-norm (testing p = 3, 4)") {
        float norm3 = norm(vec, 3);
        REQUIRE( approxEqual(norm3, 0.0f, tol) );

        float norm4 = norm(vec, 4);
        REQUIRE( approxEqual(norm4, std::pow(2.0f, 1.0f/4.0f), tol) );
    }
    SECTION("lengthSquared()") {
        float len_sq = lengthSquared(vec);
        REQUIRE( approxEqual(len_sq, 2.0f, tol) );
    }
    SECTION("normPowered()") {
        float norm_pow = normPowered(vec);
        REQUIRE( approxEqual(norm_pow, 2.0f, tol) );

        norm_pow = normPowered(vec, 3);
        REQUIRE( approxEqual(norm_pow, 0.0f, tol) );

        norm_pow = normPowered(vec, 4);
        REQUIRE( approxEqual(norm_pow, 2.0f, tol) );
    }
}

TEST_CASE("Vector normalization", "[sini::Vector]")
{
    float tol = 1e-10f;
    SECTION("Floating point vector") {
        Vector<float, 4> vec = normalize(Vector<float, 4>{ -1.0f, 1.0f, 1.0f, -1.0f });
        Vector<float, 4> ans{ -0.5f, 0.5f, 0.5f, -0.5f };
        REQUIRE( approxEqual(vec, ans, tol) );
    }
    SECTION("Integer vector") {
        Vector<float, 4> vec = normalize(Vector<int32_t, 4>{ -1, 1, 1, -1 });
        Vector<float, 4> ans{ -0.5f, 0.5f, 0.5f, -0.5f };
        REQUIRE(approxEqual(vec, ans, tol));
    }
}

TEST_CASE("Vector Abs", "[sini::Vector]")
{
    Vector<int32_t, 5> v1{ init_list<int32_t>{ -2, 3, -5, -7, 11 }.begin() };
    auto v2 = abs(v1);
    REQUIRE(v2[0] == 2);
    REQUIRE(v2[1] == 3);
    REQUIRE(v2[2] == 5);
    REQUIRE(v2[3] == 7);
    REQUIRE(v2[4] == 11);
}

TEST_CASE("Vector min and max element", "[sini::Vector]")
{
    Vector<int32_t, 5> vec{ init_list<int32_t>{ -100, 3, 7, -6, 13 }.begin() };
    int32_t max = maxElement(vec);
    REQUIRE(max == 13);
    int32_t min = minElement(vec);
    REQUIRE(min == -100);
}
