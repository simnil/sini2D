// Testing of sini/math/Vector using Catch
#include <initializer_list>

#include "catch.hpp"
#include "sini/util/testutil.hpp"
#include "sini/math/Vector.hpp"
#include "sini/math/VectorUtilities.hpp"


using namespace sini;

void verifyInitialization(vec2i vec, const int32_t (&expected_components)[2])
{
    REQUIRE(vec.x == expected_components[0]);
    REQUIRE(vec.y == expected_components[1]);

    REQUIRE(vec.components[0] == expected_components[0]);
    REQUIRE(vec.components[1] == expected_components[1]);
}
void verifyInitialization(vec3i vec, const int32_t (&expected_components)[3])
{
    REQUIRE(vec.x == expected_components[0]);
    REQUIRE(vec.y == expected_components[1]);
    REQUIRE(vec.z == expected_components[2]);

    for (int i = 0; i < 3; i++)
        REQUIRE(vec.components[i] == expected_components[i]);

    REQUIRE(vec.xy == vec2i( expected_components[0], expected_components[1] ));
    REQUIRE(vec.yz == vec2i( expected_components[1], expected_components[2] ));
}
void verifyInitialization(vec4i vec, const int32_t (&expected_components)[4])
{
    REQUIRE(vec.x == expected_components[0]);
    REQUIRE(vec.y == expected_components[1]);
    REQUIRE(vec.z == expected_components[2]);
    REQUIRE(vec.w == expected_components[3]);

    for (int i = 0; i < 4; i++)
        REQUIRE(vec.components[i] == expected_components[i]);

    REQUIRE(vec.xy == vec2i( expected_components[0], expected_components[1] ));
    REQUIRE(vec.yz == vec2i( expected_components[1], expected_components[2] ));
    REQUIRE(vec.zw == vec2i( expected_components[2], expected_components[3] ));

    REQUIRE(vec.xyz == vec3i( expected_components[0], expected_components[1],
            expected_components[2] ));
    REQUIRE(vec.yzw == vec3i( expected_components[1], expected_components[2],
            expected_components[3] ));
}

TEST_CASE("2D vector specialization", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int,2>) == 2 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        vec2i vec{1};
        verifyInitialization(vec, { 1, 1 });
    }
    SECTION("Element constructor") {
        vec2i vec{ 1, 2 };
        verifyInitialization(vec, { 1, 2 });
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[] = { 1, -2, 3 };
        vec2i v1{ &arr[0] };
        verifyInitialization(v1, { 1, -2 });

        vec2i v2{ &arr[1] };
        verifyInitialization(v2, { -2, 3 });
    }
    SECTION("Array constructor") {
        vec2i vec{{ 1, 2 }};
        verifyInitialization(vec, { 1, 2 });
    }
    SECTION("Casting constructor") {
        vec2i vec(vec2{ 1.0f, -2.0f });
        verifyInitialization(vec, { 1, -2 });
    }
    SECTION("Element access") {
        vec2i vec;
        SECTION("[] operator") {
            SECTION("get values") {
                vec = { 0, 1 };

                REQUIRE(vec[0] == 0);
                REQUIRE(vec[1] == 1);
            }
            SECTION("set values") {
                vec[0] =  13;
                vec[1] = -11;

                REQUIRE(vec.x ==  13);
                REQUIRE(vec.y == -11);
            }
        }
        SECTION("Data pointer") {
            vec.data()[0] = 2;
            vec.data()[1] = 3;

            REQUIRE(vec.x == 2);
            REQUIRE(vec.y == 3);
        }
    }
}

TEST_CASE("3D vector specialization", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int,3>) == 3 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        vec3i vec{1};
        verifyInitialization(vec, { 1, 1, 1 });
    }
    SECTION("Element constructor") {
        SECTION("(x, y, z)") {
            vec3i vec{ 3, 5, 7 };
            verifyInitialization(vec, { 3, 5, 7 });
        }
        SECTION("(xy, z)") {
            vec3i vec{ {11, 13}, 17 };
            verifyInitialization(vec, { 11, 13, 17 });
        }
        SECTION("(x, yz)") {
            vec3i vec{ -2, {3, -5} };
            verifyInitialization(vec, { -2, 3, -5 });
        }
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[] = { -11, 13, -17, -19 };
        vec3i v1{ &arr[0] };
        verifyInitialization(v1, { -11, 13, -17 });

        vec3i v2{ &arr[1] };
        verifyInitialization(v2, { 13, -17, -19 });
    }
    SECTION("Array constructor") {
        vec3i vec{{ 2, 3, 5 }};
        verifyInitialization(vec, { 2, 3, 5 });
    }
    SECTION("Casting constructor") {
        vec3i vec(vec3{ -3.0f, 5.0f, 7.0f });
        verifyInitialization(vec, { -3, 5, 7 });
    }
    SECTION("Element access") {
        Vector<int, 3> vec;
        SECTION("[] operator") {
            SECTION("get values") {
                vec = { 1, 2, 3 };

                REQUIRE(vec[0] == 1);
                REQUIRE(vec[1] == 2);
                REQUIRE(vec[2] == 3);
            }
            SECTION("set values") {
                vec[0] = 3;
                vec[1] = 2;
                vec[2] = 1;

                REQUIRE(vec.x == 3);
                REQUIRE(vec.y == 2);
                REQUIRE(vec.z == 1);
            }
        }
        SECTION("Data pointer") {
            vec.data()[0] = -4;
            vec.data()[1] = -5;
            vec.data()[2] = -6;

            REQUIRE(vec.x == -4);
            REQUIRE(vec.y == -5);
            REQUIRE(vec.z == -6);
        }
    }
}

TEST_CASE("4D vector specialization", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int,4>) == 4 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        vec4i vec{ 3 };
        verifyInitialization(vec, { 3, 3, 3, 3 });
    }
    SECTION("Element constructor") {
        SECTION("(x, y, z, w)") {
            vec4i vec{ 1, 2, 3, 4 };
            verifyInitialization(vec, { 1, 2, 3, 4 });
        }
        SECTION("(xy, z, w)") {
            vec4i vec{ {4, 3}, 2, 1 };
            verifyInitialization(vec, { 4, 3, 2, 1 });
        }
        SECTION("(x, yz, w)") {
            vec4i vec{ 2, {4, 6}, 8 };
            verifyInitialization(vec, { 2, 4, 6, 8 });
        }
        SECTION("(x, y, zw)") {
            vec4i vec{ -1, -2, {-3, -4} };
            verifyInitialization(vec, { -1, -2, -3, -4 });
        }
        SECTION("(xy, yz)") {
            vec4i vec{ {3, 5}, {11, 13} };
            verifyInitialization(vec, { 3, 5, 11, 13 });
        }
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[] = { 19, 17, 13, 11, 7 };
        vec4i v1{ &arr[0] };
        verifyInitialization(v1, { 19, 17, 13, 11 });

        vec4i v2{ &arr[1] };
        verifyInitialization(v2, { 17, 13, 11, 7 });
    }
    SECTION("Array constructor") {
        vec4i vec{{ -1, 2, -3, 4 }};
        verifyInitialization(vec, { -1, 2, -3, 4 });
    }
    SECTION("Casting constructor") {
        vec4i vec(vec4{ 16.0f, 8.0f, 4.0f, 2.0f });
        verifyInitialization(vec, { 16, 8, 4, 2 });
    }
    SECTION("Element access") {
        vec4i vec;
        SECTION("[] operator") {
            SECTION("get values") {
                vec = { 1, 3, 5, 7 };

                REQUIRE(vec[0] == 1);
                REQUIRE(vec[1] == 3);
                REQUIRE(vec[2] == 5);
                REQUIRE(vec[3] == 7);
            }
            SECTION("set values") {
                vec[0] =  11;
                vec[1] = -13;
                vec[2] =  17;
                vec[3] = -19;

                REQUIRE(vec.x ==  11);
                REQUIRE(vec.y == -13);
                REQUIRE(vec.z ==  17);
                REQUIRE(vec.w == -19);
            }
        }
        SECTION("Data pointer") {
            vec.data()[0] = 7;
            vec.data()[1] = 5;
            vec.data()[2] = 3;
            vec.data()[3] = 2;

            REQUIRE(vec.x == 7);
            REQUIRE(vec.y == 5);
            REQUIRE(vec.z == 3);
            REQUIRE(vec.w == 2);
        }
    }
}

TEST_CASE("General vector", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int,5>) == 5 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        Vector<int, 5> vec{-1};

        for (int i = 0; i < 5; i++)
            REQUIRE(vec.components[i] == -1);
    }
    SECTION("Data pointer constructor") {
        const int arr[] = { 1, -2, 4, -8, 16, -32 };
        Vector<int, 5> v1{ &arr[0] };

        REQUIRE(v1.components[0] ==  1);
        REQUIRE(v1.components[1] == -2);
        REQUIRE(v1.components[2] ==  4);
        REQUIRE(v1.components[3] == -8);
        REQUIRE(v1.components[4] == 16);

        Vector<int, 5> v2{ &arr[1] };
        REQUIRE(v2.components[0] ==  -2);
        REQUIRE(v2.components[1] ==   4);
        REQUIRE(v2.components[2] ==  -8);
        REQUIRE(v2.components[3] ==  16);
        REQUIRE(v2.components[4] == -32);
    }
    SECTION("Array constructor") {
        Vector<int, 5> vec{{ 1, 2, 4, 8, 16 }};

        REQUIRE(vec.components[0] ==  1);
        REQUIRE(vec.components[1] ==  2);
        REQUIRE(vec.components[2] ==  4);
        REQUIRE(vec.components[3] ==  8);
        REQUIRE(vec.components[4] == 16);
    }
    SECTION("Casting constructor") {
        Vector<int, 5> vec(Vector<float, 5>{ 3.0f });

        for (int i = 0; i < 5; i++)
            REQUIRE(vec.components[i] == 3);
    }
    SECTION("Element access") {
        Vector<int, 5> vec;
        SECTION("[] operator") {
            SECTION("get values") {
                vec = Vector<int, 5>{{ 1, 2, 3, 4, 5 }};

                for (int i = 0; i < 5; i++)
                    REQUIRE(vec[i] == i+1);
            }
            SECTION("set values") {
                vec[0] = 2;
                vec[1] = 3;
                vec[2] = 5;
                vec[3] = 7;
                vec[4] = 11;

                REQUIRE(vec.components[0] ==  2);
                REQUIRE(vec.components[1] ==  3);
                REQUIRE(vec.components[2] ==  5);
                REQUIRE(vec.components[3] ==  7);
                REQUIRE(vec.components[4] == 11);
            }
        }
        SECTION("Data pointer") {
            vec.data()[0] = -11;
            vec.data()[1] =  -7;
            vec.data()[2] =  -5;
            vec.data()[3] =  -3;
            vec.data()[4] =  -2;

            REQUIRE(vec.components[0] == -11);
            REQUIRE(vec.components[1] ==  -7);
            REQUIRE(vec.components[2] ==  -5);
            REQUIRE(vec.components[3] ==  -3);
            REQUIRE(vec.components[4] ==  -2);
        }
    }
}

TEST_CASE("Vector comparisons", "[sini::Vector]")
{
    vec3i v1{  1, -2,  3 },
          v2{  1,  3,  5 },
          v3{  1,  3,  5 },
          v4{ -1,  2, -3 };

    REQUIRE(v1 != v2);
    REQUIRE(v2 == v3);
    REQUIRE(v1 == -v4);
}

TEST_CASE("Vector arithmetics", "[sini::Vector]")
{
    vec3i v1{  3, 5, 7 },
          v2{ -1, 0, 1 };

    SECTION("Addition") {
        vec3i v3 = v1 + v2;

        REQUIRE(v3 == vec3i( 2, 5, 8));
        REQUIRE(v1 == vec3i( 3, 5, 7));
        REQUIRE(v2 == vec3i(-1, 0, 1));
    }
    SECTION("Addition assignment") {
        v1 += v2;

        REQUIRE(v1 == vec3i( 2, 5, 8));
        REQUIRE(v2 == vec3i(-1, 0, 1));
    }
    SECTION("Subtraction") {
        vec3i v3 = v1 - v2;

        REQUIRE(v3 == vec3i( 4, 5, 6));
        REQUIRE(v1 == vec3i( 3, 5, 7));
        REQUIRE(v2 == vec3i(-1, 0, 1));
    }
    SECTION("Subtraction assignment") {
        v1 -= v2;

        REQUIRE(v1 == vec3i( 4, 5, 6));
        REQUIRE(v2 == vec3i(-1, 0, 1));
    }
    SECTION("Negation") {
        vec3i v3 = -v1;

        REQUIRE(v3 == vec3i(-3, -5, -7));
        REQUIRE(v1 == vec3i( 3,  5,  7));
    }
    SECTION("Multiplication with scalar") {
        vec3i v3 = 2 * v1;

        REQUIRE(v3 == vec3i(6, 10, 14));
        REQUIRE(v1 == vec3i(3,  5,  7));
    }
    SECTION("Scalar multiplication assignment") {
        v1 *= 2;

        REQUIRE(v1 == vec3i(6, 10, 14));
    }
    SECTION("Element-wise multiplication") {
        vec3i v3 = v1 * v2;

        REQUIRE(v3 == vec3i(-3, 0, 7));
        REQUIRE(v1 == vec3i( 3, 5, 7));
        REQUIRE(v2 == vec3i(-1, 0, 1));
    }
    SECTION("Element-wise multiplication assignment") {
        v1 *= v2;

        REQUIRE(v1 == vec3i(-3, 0, 7));
        REQUIRE(v2 == vec3i(-1, 0, 1));
    }
    SECTION("Division with scalar") {
        vec3i v3 = { 6, 9, 27 },
              v4 = v3 / 3;

        REQUIRE(v4 == vec3i(2, 3,  9));
        REQUIRE(v3 == vec3i(6, 9, 27));
    }
    SECTION("Scalar division assignment") {
        vec3i v3 = Vector<int, 3>{ 6, 9, 27 };
        v3 /= 3;

        REQUIRE(v3 == vec3i(2, 3, 9));
    }
    SECTION("Element-wise division") {
        v1 = { 4, 9, 16 };
        v2 = { 2, 3, 4 };
        vec3i v3 = v1 / v2;

        REQUIRE(v3 == vec3i(2, 3,  4));
        REQUIRE(v1 == vec3i(4, 9, 16));
        REQUIRE(v2 == vec3i(2, 3,  4));
    }
    SECTION("Element-wise division assignment") {
        vec3i v3{ 4, 9, 16 },
              v4{ 2, 3, 4 };
        v3 /= v4;

        REQUIRE(v3 == vec3i(2, 3, 4));
        REQUIRE(v4 == vec3i(2, 3, 4));
    }
    SECTION("Modulus with scalar") {
        vec3i v3 = v1 % 3;

        REQUIRE(v3 == vec3i(0, 2, 1));
        REQUIRE(v1 == vec3i(3, 5, 7));
    }
    SECTION("Element-wise modulus") {
        vec3i v3 = { 2, 3, 4 },
              v4 = v1 % v3;

        REQUIRE(v4 == vec3i(1, 2, 3));
        REQUIRE(v1 == vec3i(3, 5, 7));
        REQUIRE(v3 == vec3i(2, 3, 4));
    }
}

TEST_CASE("Dot/scalar product", "[sini::Vector]")
{
    vec3i v1{  3, 5, 7 },
          v2{ -1, 0, 1 };

    SECTION("Function form") {
        REQUIRE(dot(v1, v2) == 4);
        REQUIRE(dot(v2, v1) == 4);
        REQUIRE(dot(v2, v2) == 2);
    }
    SECTION("Operator form") {
        REQUIRE((v1 | v2) == 4);
        REQUIRE((v2 | v1) == 4);
        REQUIRE((v2 | v2) == 2);
    }

    REQUIRE(v1 == vec3i( 3, 5, 7));
    REQUIRE(v2 == vec3i(-1, 0, 1));
}

TEST_CASE("Cross product", "[sini::Vector]")
{
    vec3i v1{  3, 5, 7 },
          v2{ -1, 0, 1 };

    SECTION("Function form") {
        vec3i v3 = cross(v1, v2);
        REQUIRE(v3 == vec3i(5, -10, 5));

        v3 = cross(v2, v1);
        REQUIRE(v3 == vec3i(-5, 10, -5));

        v3 = cross(v1, v1);
        REQUIRE(v3 == vec3i(0, 0, 0));
    }
    SECTION("Operator form") {
        vec3i v3 = v1 ^ v2;
        REQUIRE(v3 == vec3i(5, -10, 5));

        v3 = v2 ^ v1;
        REQUIRE(v3 == vec3i(-5, 10, -5));

        v3 = v1 ^ v1;
        REQUIRE(v3 == vec3i(0, 0, 0));
    }

    REQUIRE(v1 == vec3i( 3, 5, 7));
    REQUIRE(v2 == vec3i(-1, 0, 1));
}

TEST_CASE("Vector length and norm", "[sini::Vector]")
{
    vec3 vec{ 1.0f, 0.0f, -2.0f };
    float tol = 1e-10f;

    SECTION("Length") {
        REQUIRE_APPROX_EQUAL(length(vec), std::sqrt(5.0f), tol);
    }
    SECTION("2-norm (= length)") {
        REQUIRE_APPROX_EQUAL(norm(vec), std::sqrt(5.0f), tol);
    }
    SECTION("p-norm (testing p = 3, 4)") {
        REQUIRE_APPROX_EQUAL(norm(vec, 3),    std::pow(9.0f,  1.0f/3.0f), tol);
        REQUIRE_APPROX_EQUAL(norm(vec, 3.0f), std::pow(9.0f,  1.0f/3.0f), tol);
        REQUIRE_APPROX_EQUAL(norm(vec, 4),    std::pow(17.0f, 1.0f/4.0f), tol);
        REQUIRE_APPROX_EQUAL(norm(vec, 4.0f), std::pow(17.0f, 1.0f/4.0f), tol);
    }
    SECTION("lengthSquared()") {
        REQUIRE_APPROX_EQUAL(lengthSquared(vec), 5.0f, tol);
    }
    SECTION("normPowered()") {
        REQUIRE_APPROX_EQUAL(normPowered(vec),       5.0f,  tol);
        REQUIRE_APPROX_EQUAL(normPowered(vec, 3),    9.0f,  tol);
        REQUIRE_APPROX_EQUAL(normPowered(vec, 3.0f), 9.0f,  tol);
        REQUIRE_APPROX_EQUAL(normPowered(vec, 4),    17.0f, tol);
        REQUIRE_APPROX_EQUAL(normPowered(vec, 4.0f), 17.0f, tol);
    }
}

TEST_CASE("Vector normalization", "[sini::Vector]")
{
    float tol = 1e-10f;

    SECTION("Floating point vector") {
        vec4 vec = normalize(vec4{ -1.0f, 1.0f, 1.0f, -1.0f }),
             ans{ -0.5f, 0.5f, 0.5f, -0.5f };
        REQUIRE_APPROX_EQUAL(vec, ans, tol);
    }
    SECTION("Integer vector") {
        vec4 vec = normalize(vec4i{ -1, 1, 1, -1 }),
             ans{ -0.5f, 0.5f, 0.5f, -0.5f };
        REQUIRE_APPROX_EQUAL(vec, ans, tol);
    }
}

TEST_CASE("Vector Abs", "[sini::Vector]")
{
    Vector<int, 5> v1     = {{ -2, 3, -5, -7, 11 }},
                   v1_abs = {{  2, 3,  5,  7, 11 }};

    REQUIRE(abs(v1) == v1_abs);
}

TEST_CASE("Vector min and max element", "[sini::Vector]")
{
    Vector<int, 5> vec{{ -100, 3, 7, -6, 13 }};
    int max = maxElement(vec);
    REQUIRE(max == 13);
    int min = minElement(vec);
    REQUIRE(min == -100);
}

TEST_CASE("Vector to string", "[sini::Vector]")
{
    vec3i int_vec{ 5, 7, 13 };
    vec3 float_vec{ 3.14f, 1.59f, 2.6f };

    SECTION("To default string") {
        REQUIRE(toString(int_vec) == "(5, 7, 13)");
        REQUIRE(toString(float_vec) == "(3.14, 1.59, 2.6)");
    }
    SECTION("To formatted string") {
        REQUIRE(toFormattedString(float_vec, "%.1f") == "(3.1, 1.6, 2.6)");
    }
}

TEST_CASE("Vector is standard layout and is trivial", "[sini::Vector]")
{
    REQUIRE(std::is_standard_layout<vec2>::value);
    REQUIRE(std::is_standard_layout<vec3>::value);
    REQUIRE(std::is_standard_layout<vec4>::value);
    bool vec5_std_layout = std::is_standard_layout<Vector<float,5>>::value;
    REQUIRE(vec5_std_layout);

    REQUIRE(std::is_trivial<vec2>::value);
    REQUIRE(std::is_trivial<vec3>::value);
    REQUIRE(std::is_trivial<vec4>::value);
    bool vec5_trivial = std::is_trivial<Vector<float,5>>::value;
    REQUIRE(vec5_trivial);
}
