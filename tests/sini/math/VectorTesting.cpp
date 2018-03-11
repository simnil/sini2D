// Testing of sini/math/Vector using Catch
#include <initializer_list>

#include "catch.hpp"
#include "sini/math/Vector.hpp"
#include "sini/math/VectorUtilities.hpp"


using namespace sini;

TEST_CASE("2D vector specialization", "[sini::Vector]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Vector<int,2>) == 2 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        vec2i vec{1};

        REQUIRE(vec.x == 1);
        REQUIRE(vec.y == 1);
        REQUIRE(vec.components[0] == 1);
        REQUIRE(vec.components[1] == 1);
    }
    SECTION("Element constructor") {
        vec2i vec{ 1, 2 };

        REQUIRE(vec.x == 1);
        REQUIRE(vec.y == 2);
        REQUIRE(vec.components[0] == 1);
        REQUIRE(vec.components[1] == 2);
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[] = { 1, -2, 3 };
        vec2i v1{ &arr[0] };

        REQUIRE(v1.x ==  1);
        REQUIRE(v1.y == -2);
        REQUIRE(v1.components[0] ==  1);
        REQUIRE(v1.components[1] == -2);

        vec2i v2{ &arr[1] };
        REQUIRE(v2.x == -2);
        REQUIRE(v2.y ==  3);
        REQUIRE(v2.components[0] == -2);
        REQUIRE(v2.components[1] ==  3);
    }
    SECTION("Array constructor") {
        vec2i vec{{ 1, 2 }};

        REQUIRE(vec.x == 1);
        REQUIRE(vec.y == 2);
        REQUIRE(vec.components[0] == 1);
        REQUIRE(vec.components[1] == 2);
    }
    SECTION("Casting constructor") {
        vec2i vec(vec2{ 1.0f, -2.0f });

        REQUIRE(vec.x ==  1);
        REQUIRE(vec.y == -2);
        REQUIRE(vec.components[0] ==  1);
        REQUIRE(vec.components[1] == -2);
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
        vec2i subvec{1};

        REQUIRE(vec.x == 1);
        REQUIRE(vec.y == 1);
        REQUIRE(vec.z == 1);
        REQUIRE(vec.components[0] == 1);
        REQUIRE(vec.components[1] == 1);
        REQUIRE(vec.components[2] == 1);
        REQUIRE(vec.xy == subvec);
        REQUIRE(vec.yz == subvec);
    }
    SECTION("Element constructor") {
        SECTION("(x, y, z)") {
            vec3i vec{ 3, 5, 7 };

            REQUIRE(vec.x == 3);
            REQUIRE(vec.y == 5);
            REQUIRE(vec.z == 7);
            REQUIRE(vec.components[0] == 3);
            REQUIRE(vec.components[1] == 5);
            REQUIRE(vec.components[2] == 7);
            REQUIRE(vec.xy == vec2i(3, 5));
            REQUIRE(vec.yz == vec2i(5, 7));
        }
        SECTION("(xy, z)") {
            vec3i vec{ {11, 13}, 17 };

            REQUIRE(vec.x == 11);
            REQUIRE(vec.y == 13);
            REQUIRE(vec.z == 17);
            REQUIRE(vec.components[0] == 11);
            REQUIRE(vec.components[1] == 13);
            REQUIRE(vec.components[2] == 17);
            REQUIRE(vec.xy == vec2i(11, 13));
            REQUIRE(vec.yz == vec2i(13, 17));
        }
        SECTION("(x, yz)") {
            vec3i vec{ -2, {3, -5} };

            REQUIRE(vec.x == -2);
            REQUIRE(vec.y ==  3);
            REQUIRE(vec.z == -5);
            REQUIRE(vec.components[0] == -2);
            REQUIRE(vec.components[1] ==  3);
            REQUIRE(vec.components[2] == -5);
            REQUIRE(vec.xy == vec2i(-2,  3));
            REQUIRE(vec.yz == vec2i( 3, -5));
        }
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[] = { -11, 13, -17, -19 };
        vec3i v1{ &arr[0] };

        REQUIRE(v1.x == -11);
        REQUIRE(v1.y ==  13);
        REQUIRE(v1.z == -17);
        REQUIRE(v1.components[0] == -11);
        REQUIRE(v1.components[1] ==  13);
        REQUIRE(v1.components[2] == -17);
        REQUIRE(v1.xy == vec2i(-11,  13));
        REQUIRE(v1.yz == vec2i( 13, -17));

        vec3i v2{ &arr[1] };
        REQUIRE(v2.x ==  13);
        REQUIRE(v2.y == -17);
        REQUIRE(v2.z == -19);
        REQUIRE(v2.components[0] ==  13);
        REQUIRE(v2.components[1] == -17);
        REQUIRE(v2.components[2] == -19);
        REQUIRE(v2.xy == vec2i( 13, -17));
        REQUIRE(v2.yz == vec2i(-17, -19));
    }
    SECTION("Array constructor") {
        vec3i vec{{ 2, 3, 5 }};

        REQUIRE(vec.x == 2);
        REQUIRE(vec.y == 3);
        REQUIRE(vec.z == 5);
        REQUIRE(vec.components[0] == 2);
        REQUIRE(vec.components[1] == 3);
        REQUIRE(vec.components[2] == 5);
        REQUIRE(vec.xy == vec2i(2, 3));
        REQUIRE(vec.yz == vec2i(3, 5));
    }
    SECTION("Casting constructor") {
        vec3i vec(vec3{ -3.0f, 5.0f, 7.0f });

        REQUIRE(vec.x == -3);
        REQUIRE(vec.y ==  5);
        REQUIRE(vec.z ==  7);
        REQUIRE(vec.components[0] == -3);
        REQUIRE(vec.components[1] ==  5);
        REQUIRE(vec.components[2] ==  7);
        REQUIRE(vec.xy == vec2i(-3, 5));
        REQUIRE(vec.yz == vec2i( 5, 7));
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
        vec3i subvec_3{ 3 };
        vec2i subvec_2{ 3 };

        REQUIRE(vec.x == 3);
        REQUIRE(vec.y == 3);
        REQUIRE(vec.z == 3);
        REQUIRE(vec.w == 3);
        REQUIRE(vec.components[0] == 3);
        REQUIRE(vec.components[1] == 3);
        REQUIRE(vec.components[2] == 3);
        REQUIRE(vec.components[3] == 3);
        REQUIRE(vec.xy  == subvec_2);
        REQUIRE(vec.yz  == subvec_2);
        REQUIRE(vec.zw  == subvec_2);
        REQUIRE(vec.xyz == subvec_3);
        REQUIRE(vec.yzw == subvec_3);
    }
    SECTION("Element constructor") {
        SECTION("(x, y, z, w)") {
            vec4i vec{ 1, 2, 3, 4 };

            REQUIRE(vec.x == 1);
            REQUIRE(vec.y == 2);
            REQUIRE(vec.z == 3);
            REQUIRE(vec.w == 4);
            REQUIRE(vec.components[0] == 1);
            REQUIRE(vec.components[1] == 2);
            REQUIRE(vec.components[2] == 3);
            REQUIRE(vec.components[3] == 4);
            REQUIRE(vec.xy  == vec2i(1, 2));
            REQUIRE(vec.yz  == vec2i(2, 3));
            REQUIRE(vec.zw  == vec2i(3, 4));
            REQUIRE(vec.xyz == vec3i(1, 2, 3));
            REQUIRE(vec.yzw == vec3i(2, 3, 4));
        }
        SECTION("(xy, z, w)") {
            vec4i vec{ {4, 3}, 2, 1 };

            REQUIRE(vec.x == 4);
            REQUIRE(vec.y == 3);
            REQUIRE(vec.z == 2);
            REQUIRE(vec.w == 1);
            REQUIRE(vec.components[0] == 4);
            REQUIRE(vec.components[1] == 3);
            REQUIRE(vec.components[2] == 2);
            REQUIRE(vec.components[3] == 1);
            REQUIRE(vec.xy  == vec2i(4, 3));
            REQUIRE(vec.yz  == vec2i(3, 2));
            REQUIRE(vec.zw  == vec2i(2, 1));
            REQUIRE(vec.xyz == vec3i(4, 3, 2));
            REQUIRE(vec.yzw == vec3i(3, 2, 1));
        }
        SECTION("(x, yz, w)") {
            vec4i vec{ 2, {4, 6}, 8 };

            REQUIRE(vec.x == 2);
            REQUIRE(vec.y == 4);
            REQUIRE(vec.z == 6);
            REQUIRE(vec.w == 8);
            REQUIRE(vec.components[0] == 2);
            REQUIRE(vec.components[1] == 4);
            REQUIRE(vec.components[2] == 6);
            REQUIRE(vec.components[3] == 8);
            REQUIRE(vec.xy  == vec2i(2, 4));
            REQUIRE(vec.yz  == vec2i(4, 6));
            REQUIRE(vec.zw  == vec2i(6, 8));
            REQUIRE(vec.xyz == vec3i(2, 4, 6));
            REQUIRE(vec.yzw == vec3i(4, 6, 8));
        }
        SECTION("(x, y, zw)") {
            vec4i vec{ -1, -2, {-3, -4} };

            REQUIRE(vec.x == -1);
            REQUIRE(vec.y == -2);
            REQUIRE(vec.z == -3);
            REQUIRE(vec.w == -4);
            REQUIRE(vec.components[0] == -1);
            REQUIRE(vec.components[1] == -2);
            REQUIRE(vec.components[2] == -3);
            REQUIRE(vec.components[3] == -4);
            REQUIRE(vec.xy  == vec2i(-1, -2));
            REQUIRE(vec.yz  == vec2i(-2, -3));
            REQUIRE(vec.zw  == vec2i(-3, -4));
            REQUIRE(vec.xyz == vec3i(-1, -2, -3));
            REQUIRE(vec.yzw == vec3i(-2, -3, -4));
        }
        SECTION("(xy, yz)") {
            vec4i vec{ {3, 5}, {11, 13} };

            REQUIRE(vec.x ==  3);
            REQUIRE(vec.y ==  5);
            REQUIRE(vec.z == 11);
            REQUIRE(vec.w == 13);
            REQUIRE(vec.components[0] ==  3);
            REQUIRE(vec.components[1] ==  5);
            REQUIRE(vec.components[2] == 11);
            REQUIRE(vec.components[3] == 13);
            REQUIRE(vec.xy  == vec2i( 3,  5));
            REQUIRE(vec.yz  == vec2i( 5, 11));
            REQUIRE(vec.zw  == vec2i(11, 13));
            REQUIRE(vec.xyz == vec3i(3,  5, 11));
            REQUIRE(vec.yzw == vec3i(5, 11, 13));
        }
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[] = { 19, 17, 13, 11, 7 };
        vec4i v1{ &arr[0] };

        REQUIRE(v1.x == 19);
        REQUIRE(v1.y == 17);
        REQUIRE(v1.z == 13);
        REQUIRE(v1.w == 11);
        REQUIRE(v1.components[0] == 19);
        REQUIRE(v1.components[1] == 17);
        REQUIRE(v1.components[2] == 13);
        REQUIRE(v1.components[3] == 11);
        REQUIRE(v1.xy  == vec2i(19, 17));
        REQUIRE(v1.yz  == vec2i(17, 13));
        REQUIRE(v1.zw  == vec2i(13, 11));
        REQUIRE(v1.xyz == vec3i(19, 17, 13));
        REQUIRE(v1.yzw == vec3i(17, 13, 11));

        vec4i v2{ &arr[1] };
        REQUIRE(v2.x == 17);
        REQUIRE(v2.y == 13);
        REQUIRE(v2.z == 11);
        REQUIRE(v2.w ==  7);
        REQUIRE(v2.components[0] == 17);
        REQUIRE(v2.components[1] == 13);
        REQUIRE(v2.components[2] == 11);
        REQUIRE(v2.components[3] ==  7);
        REQUIRE(v2.xy  == vec2i(17, 13));
        REQUIRE(v2.yz  == vec2i(13, 11));
        REQUIRE(v2.zw  == vec2i(11,  7));
        REQUIRE(v2.xyz == vec3i(17, 13, 11));
        REQUIRE(v2.yzw == vec3i(13, 11,  7));
    }
    SECTION("Array constructor") {
        vec4i vec{{ -1, 2, -3, 4 }};

        REQUIRE(vec.x == -1);
        REQUIRE(vec.y ==  2);
        REQUIRE(vec.z == -3);
        REQUIRE(vec.w ==  4);
        REQUIRE(vec.components[0] == -1);
        REQUIRE(vec.components[1] ==  2);
        REQUIRE(vec.components[2] == -3);
        REQUIRE(vec.components[3] ==  4);
        REQUIRE(vec.xy == vec2i(-1,  2));
        REQUIRE(vec.yz == vec2i( 2, -3));
        REQUIRE(vec.zw == vec2i(-3,  4));
        REQUIRE(vec.xyz == vec3i(-1,  2, -3));
        REQUIRE(vec.yzw == vec3i( 2, -3,  4));
    }
    SECTION("Casting constructor") {
        vec4i vec(vec4{ 16.0f, 8.0f, 4.0f, 2.0f });

        REQUIRE(vec.x == 16);
        REQUIRE(vec.y ==  8);
        REQUIRE(vec.z ==  4);
        REQUIRE(vec.w ==  2);
        REQUIRE(vec.components[0] == 16);
        REQUIRE(vec.components[1] ==  8);
        REQUIRE(vec.components[2] ==  4);
        REQUIRE(vec.components[3] ==  2);
        REQUIRE(vec.xy  == vec2i(16, 8));
        REQUIRE(vec.yz  == vec2i( 8, 4));
        REQUIRE(vec.zw  == vec2i( 4, 2));
        REQUIRE(vec.xyz == vec3i(16, 8, 4));
        REQUIRE(vec.yzw == vec3i( 8, 4, 2));
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

            REQUIRE(vec[0] == 7);
            REQUIRE(vec[1] == 5);
            REQUIRE(vec[2] == 3);
            REQUIRE(vec[3] == 2);
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

            REQUIRE(vec[0] == -11);
            REQUIRE(vec[1] ==  -7);
            REQUIRE(vec[2] ==  -5);
            REQUIRE(vec[3] ==  -3);
            REQUIRE(vec[4] ==  -2);
        }
    }
}

TEST_CASE("Vector arithmetics", "[sini::Vector]")
{
    vec3i v1{  3, 5, 7 },
          v2{ -1, 0, 1 };

    SECTION("Addition") {
        vec3i v3 = v1 + v2;

        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 5);
        REQUIRE(v3.z == 8);

        // Check integrity of v1
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        // Check integrity of v2
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y ==  0);
        REQUIRE(v2.z ==  1);
    }
    SECTION("Addition assignment") {
        v1 += v2;

        REQUIRE(v1.x == 2);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 8);

        // Check integrity of v2
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y ==  0);
        REQUIRE(v2.z ==  1);
    }
    SECTION("Subtraction") {
        vec3i v3 = v1 - v2;

        REQUIRE(v3.x == 4);
        REQUIRE(v3.y == 5);
        REQUIRE(v3.z == 6);

        // Check integrity of v1
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        // Check integrity of v2
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y ==  0);
        REQUIRE(v2.z ==  1);
    }
    SECTION("Subtraction assignment") {
        v1 -= v2;

        REQUIRE(v1.x == 4);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 6);

        // Check integrity of v2
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y ==  0);
        REQUIRE(v2.z ==  1);
    }
    SECTION("Negation") {
        vec3i v3 = -v1;

        REQUIRE(v3.x == -3);
        REQUIRE(v3.y == -5);
        REQUIRE(v3.z == -7);

        // Check integrity of v1
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
    }
    SECTION("Multiplication with scalar") {
        vec3i v3 = 2 * v1;

        REQUIRE(v3.x ==  6);
        REQUIRE(v3.y == 10);
        REQUIRE(v3.z == 14);

        // Check integrity of v1
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
    }
    SECTION("Scalar multiplication assignment") {
        v1 *= 2;

        REQUIRE(v1.x ==  6);
        REQUIRE(v1.y == 10);
        REQUIRE(v1.z == 14);
    }
    SECTION("Element-wise multiplication") {
        vec3i v3 = v1 * v2;

        REQUIRE(v3.x == -3);
        REQUIRE(v3.y ==  0);
        REQUIRE(v3.z ==  7);

        // Check integrity of v1
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        // Check integrity of v2
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y ==  0);
        REQUIRE(v2.z ==  1);
    }
    SECTION("Element-wise multiplication assignment") {
        v1 *= v2;

        REQUIRE(v1.x == -3);
        REQUIRE(v1.y ==  0);
        REQUIRE(v1.z ==  7);

        // Check integrity of v2
        REQUIRE(v2.x == -1);
        REQUIRE(v2.y ==  0);
        REQUIRE(v2.z ==  1);
    }
    SECTION("Division with scalar") {
        vec3i v3{ 6, 9, 27 },
              v4 = v3 / 3;

        REQUIRE(v4.x == 2);
        REQUIRE(v4.y == 3);
        REQUIRE(v4.z == 9);

        // Check integrity of v3
        REQUIRE(v3.x == 6);
        REQUIRE(v3.y == 9);
        REQUIRE(v3.z == 27);
    }
    SECTION("Scalar division assignment") {
        vec3i v3 = Vector<int, 3>{ 6, 9, 27 };
        v3 /= 3;

        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 9);
    }
    SECTION("Element-wise division") {
        v1 = { 4, 9, 16 };
        v2 = { 2, 3, 4 };
        vec3i v3 = v1 / v2;

        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 4);

        // Check integrity of v1
        REQUIRE(v1.x ==  4);
        REQUIRE(v1.y ==  9);
        REQUIRE(v1.z == 16);
        // Check integrity of v2
        REQUIRE(v2.x == 2);
        REQUIRE(v2.y == 3);
        REQUIRE(v2.z == 4);
    }
    SECTION("Element-wise division assignment") {
        vec3i v3{ 4, 9, 16 },
              v4{ 2, 3, 4 };
        v3 /= v4;

        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 4);

        // Check integrity of v4
        REQUIRE(v4.x == 2);
        REQUIRE(v4.y == 3);
        REQUIRE(v4.z == 4);
    }
    SECTION("Modulus with scalar") {
        vec3i v3 = v1 % 3;

        REQUIRE(v3.x == 0);
        REQUIRE(v3.y == 2);
        REQUIRE(v3.z == 1);

        // Check integrity of v1
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
    }
    SECTION("Element-wise modulus") {
        vec3i v3 = { 2, 3, 4 },
              v4 = v1 % v3;

        REQUIRE(v4.x == 1);
        REQUIRE(v4.y == 2);
        REQUIRE(v4.z == 3);

        // Check integrity of v1
        REQUIRE(v1.x == 3);
        REQUIRE(v1.y == 5);
        REQUIRE(v1.z == 7);
        // Check integrity of v3
        REQUIRE(v3.x == 2);
        REQUIRE(v3.y == 3);
        REQUIRE(v3.z == 4);
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
    // Check integrity of v1 and v2
    REQUIRE(v1.x == 3);
    REQUIRE(v1.y == 5);
    REQUIRE(v1.z == 7);

    REQUIRE(v2.x == -1);
    REQUIRE(v2.y ==  0);
    REQUIRE(v2.z ==  1);
}

TEST_CASE("Cross product", "[sini::Vector]")
{
    vec3i v1{  3, 5, 7 },
          v2{ -1, 0, 1 };

    SECTION("Function form") {
        vec3i v3 = cross(v1, v2);
        REQUIRE(v3.x ==   5);
        REQUIRE(v3.y == -10);
        REQUIRE(v3.z ==   5);

        v3 = cross(v2, v1);
        REQUIRE(v3.x == -5);
        REQUIRE(v3.y == 10);
        REQUIRE(v3.z == -5);

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

        v3 = v2 ^ v1;
        REQUIRE(v3.x == -5);
        REQUIRE(v3.y == 10);
        REQUIRE(v3.z == -5);

        v3 = v1 ^ v1;
        REQUIRE(v3.x == 0);
        REQUIRE(v3.y == 0);
        REQUIRE(v3.z == 0);
    }
    // Check integrity of v1 and v2
    REQUIRE(v1.x == 3);
    REQUIRE(v1.y == 5);
    REQUIRE(v1.z == 7);

    REQUIRE(v2.x == -1);
    REQUIRE(v2.y ==  0);
    REQUIRE(v2.z ==  1);
}

TEST_CASE("Vector length and norm", "[sini::Vector]")
{
    vec3 vec{ 1.0f, 0.0f, -1.0f };
    float tol = 1e-10f;

    SECTION("Length") {
        REQUIRE( approxEqual(length(vec), std::sqrt(2.0f), tol) );
    }
    SECTION("2-norm (= length)") {
        REQUIRE( approxEqual(norm(vec), std::sqrt(2.0f), tol) );
    }
    SECTION("p-norm (testing p = 3, 4)") {
        REQUIRE( approxEqual(norm(vec, 3), 0.0f, tol) );
        REQUIRE( approxEqual(norm(vec, 4), std::pow(2.0f, 1.0f/4.0f), tol) );
    }
    SECTION("lengthSquared()") {
        REQUIRE( approxEqual(lengthSquared(vec), 2.0f, tol) );
    }
    SECTION("normPowered()") {
        REQUIRE( approxEqual(normPowered(vec), 2.0f, tol) );
        REQUIRE( approxEqual(normPowered(vec, 3), 0.0f, tol) );
        REQUIRE( approxEqual(normPowered(vec, 4), 2.0f, tol) );
    }
}

TEST_CASE("Vector normalization", "[sini::Vector]")
{
    float tol = 1e-10f;

    SECTION("Floating point vector") {
        vec4 vec = normalize(vec4{ -1.0f, 1.0f, 1.0f, -1.0f }),
             ans{ -0.5f, 0.5f, 0.5f, -0.5f };
        REQUIRE( approxEqual(vec, ans, tol) );
    }
    SECTION("Integer vector") {
        vec4 vec = normalize(vec4i{ -1, 1, 1, -1 }),
             ans{ -0.5f, 0.5f, 0.5f, -0.5f };
        REQUIRE(approxEqual(vec, ans, tol));
    }
}

TEST_CASE("Vector Abs", "[sini::Vector]")
{
    Vector<int, 5> v1{{ -2, 3, -5, -7, 11 }},
                   v2 = abs(v1);

    REQUIRE(v2[0] ==  2);
    REQUIRE(v2[1] ==  3);
    REQUIRE(v2[2] ==  5);
    REQUIRE(v2[3] ==  7);
    REQUIRE(v2[4] == 11);
}

TEST_CASE("Vector min and max element", "[sini::Vector]")
{
    Vector<int, 5> vec{{ -100, 3, 7, -6, 13 }};
    int max = maxElement(vec);
    REQUIRE(max == 13);
    int min = minElement(vec);
    REQUIRE(min == -100);
}
