// Testing of sini/math/Matrix using Catch
#include <initializer_list>

#include "catch.hpp"
#include "sini/math/Matrix.hpp"
#include "sini/math/MathUtilities.hpp"


using namespace sini;
template<typename T> using init_list = std::initializer_list<T>;

TEST_CASE("2x2 matrix specialization", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,2,2>) == 4 * sizeof(int) );
    }
    SECTION("Element access") {
        Matrix<int32_t, 2, 2> mat;
        SECTION("() operator") {
            mat(0, 0) = 1;
            mat(0, 1) = 2;
            mat(1, 0) = 3;
            mat(1, 1) = 4;
            REQUIRE(mat.a == 1);
            REQUIRE(mat.b == 2);
            REQUIRE(mat.c == 3);
            REQUIRE(mat.d == 4);
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(1, 0) == 3);
            REQUIRE(mat.at(1, 1) == 4);
            REQUIRE(mat.row_vectors[0] == vec2i(1, 2));
            REQUIRE(mat.row_vectors[1] == vec2i(3, 4));
            REQUIRE(mat.column(0) == vec2i(1, 3));
            REQUIRE(mat.column(1) == vec2i(2, 4));
        }
        SECTION("Data pointer") {
            mat.data()[0] = 1;
            mat.data()[1] = 2;
            mat.data()[2] = 3;
            mat.data()[3] = 4;
            // Row major order
            REQUIRE(mat.a == 1);
            REQUIRE(mat.b == 2);
            REQUIRE(mat.c == 3);
            REQUIRE(mat.d == 4);
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(1, 0) == 3);
            REQUIRE(mat.at(1, 1) == 4);
            REQUIRE(mat.row_vectors[0] == vec2i(1, 2));
            REQUIRE(mat.row_vectors[1] == vec2i(3, 4));
            REQUIRE(mat.column(0) == vec2i(1, 3));
            REQUIRE(mat.column(1) == vec2i(2, 4));
        }
        SECTION("set()") {
            mat.set(0, 0, 1);
            mat.set(0, 1, 2);
            mat.set(1, 0, 3);
            mat.set(1, 1, 4);
            REQUIRE(mat.a == 1);
            REQUIRE(mat.b == 2);
            REQUIRE(mat.row_vectors[0] == vec2i(1, 2));
            REQUIRE(mat.c == 3);
            REQUIRE(mat.d == 4);
            REQUIRE(mat.row_vectors[1] == vec2i(3, 4));
            REQUIRE(mat.column(0) == vec2i(1, 3));
            REQUIRE(mat.column(1) == vec2i(2, 4));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, vec2i(4, 2));
            mat.setColumn(1, vec2i(3, 1));
            REQUIRE(mat.a == 4);
            REQUIRE(mat.b == 3);
            REQUIRE(mat.row_vectors[0] == vec2i(4, 3));
            REQUIRE(mat.c == 2);
            REQUIRE(mat.d == 1);
            REQUIRE(mat.row_vectors[1] == vec2i(2, 1));
            REQUIRE(mat.column(0) == vec2i(4, 2));
            REQUIRE(mat.column(1) == vec2i(3, 1));
        }
    }
    SECTION("Fill constructor") {
        Matrix<int, 2, 2> mat{ 3 };
        REQUIRE(mat.a == 3);
        REQUIRE(mat.b == 3);
        REQUIRE(mat.c == 3);
        REQUIRE(mat.d == 3);
        REQUIRE(mat.at(0, 0) == 3);
        REQUIRE(mat.at(0, 1) == 3);
        REQUIRE(mat.at(1, 0) == 3);
        REQUIRE(mat.at(1, 1) == 3);
        Vector<int, 2> vec{ 3 };
        REQUIRE(mat.row_vectors[0] == vec);
        REQUIRE(mat.row_vectors[1] == vec);
        REQUIRE(mat.column(0) == vec);
        REQUIRE(mat.column(1) == vec);
    }
    SECTION("Array constructor") {
        const int32_t arr[]{ 4, 3, 2, 1, -1 };
        Matrix<int32_t, 2, 2> m1{arr};
        REQUIRE(m1.a == 4);
        REQUIRE(m1.b == 3);
        REQUIRE(m1.c == 2);
        REQUIRE(m1.d == 1);
        REQUIRE(m1.at(0, 0) == 4);
        REQUIRE(m1.at(0, 1) == 3);
        REQUIRE(m1.at(1, 0) == 2);
        REQUIRE(m1.at(1, 1) == 1);
        REQUIRE(m1.row_vectors[0] == vec2i(4, 3));
        REQUIRE(m1.row_vectors[1] == vec2i(2, 1));
        REQUIRE(m1.column(0) == vec2i(4, 2));
        REQUIRE(m1.column(1) == vec2i(3, 1));

        Matrix<int32_t, 2, 2> m2{arr+1};
        REQUIRE(m2.a == 3);
        REQUIRE(m2.b == 2);
        REQUIRE(m2.c == 1);
        REQUIRE(m2.d == -1);
        REQUIRE(m2.at(0, 0) == 3);
        REQUIRE(m2.at(0, 1) == 2);
        REQUIRE(m2.at(1, 0) == 1);
        REQUIRE(m2.at(1, 1) == -1);
        REQUIRE(m2.row_vectors[0] == vec2i(3, 2));
        REQUIRE(m2.row_vectors[1] == vec2i(1, -1));
        REQUIRE(m2.column(0) == vec2i(3, 1));
        REQUIRE(m2.column(1) == vec2i(2, -1));
    }
    SECTION("Initialize from row vectors") {
        Matrix<int, 2, 2> mat{
            { 2, 3 },
            { 5, 7 }
        };
        REQUIRE(mat.a == 2);
        REQUIRE(mat.b == 3);
        REQUIRE(mat.c == 5);
        REQUIRE(mat.d == 7);
        REQUIRE(mat.at(0, 0) == 2);
        REQUIRE(mat.at(0, 1) == 3);
        REQUIRE(mat.at(1, 0) == 5);
        REQUIRE(mat.at(1, 1) == 7);
        REQUIRE(mat.row_vectors[0] == vec2i(2, 3));
        REQUIRE(mat.row_vectors[1] == vec2i(5, 7));
        REQUIRE(mat.column(0) == vec2i(2, 5));
        REQUIRE(mat.column(1) == vec2i(3, 7));
    }
    SECTION("Casting constructor") {
        Matrix<int32_t, 2, 2> mat(mat2{
            { -1.0f, 1.0f },
            { 2.0f, -2.0f }
        });
        REQUIRE(mat.a == -1);
        REQUIRE(mat.b == 1);
        REQUIRE(mat.c == 2);
        REQUIRE(mat.d == -2);
        REQUIRE(mat.at(0, 0) == -1);
        REQUIRE(mat.at(0, 1) == 1);
        REQUIRE(mat.at(1, 0) == 2);
        REQUIRE(mat.at(1, 1) == -2);
        REQUIRE(mat.row_vectors[0] == vec2i(-1, 1));
        REQUIRE(mat.row_vectors[1] == vec2i(2, -2));
        REQUIRE(mat.column(0) == vec2i(-1, 2));
        REQUIRE(mat.column(1) == vec2i(1, -2));
    }
    SECTION("Identity matrix") {
        mat2 id2 = mat2::identity();
        // Row 0
        REQUIRE(id2.e00 == 1.0f);
        REQUIRE(id2.e01 == 0.0f);
        REQUIRE(id2.row_vectors[0] == vec2(1.0f, 0.0f));
        // Row 1
        REQUIRE(id2.e10 == 0.0f);
        REQUIRE(id2.e11 == 1.0f);
        REQUIRE(id2.row_vectors[1] == vec2(0.0f, 1.0f));
        // Columns
        REQUIRE(id2.column(0) == vec2(1.0f, 0.0f));
        REQUIRE(id2.column(1) == vec2(0.0f, 1.0f));
    }
}

TEST_CASE("3x3 matrix specialization", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,3,3>) == 9 * sizeof(int) );
    }
    SECTION("Element access") {
        Matrix<int32_t, 3, 3> mat;
        SECTION("() operator") {
            mat(0, 0) = -1; mat(0, 1) = -2; mat(0, 2) = -3;
            mat(1, 0) = -4; mat(1, 1) = -5; mat(1, 2) = -6;
            mat(2, 0) = -7; mat(2, 1) = -8; mat(2, 2) = -9;

            REQUIRE(mat.a == -1);
            REQUIRE(mat.b == -2);
            REQUIRE(mat.c == -3);
            REQUIRE(mat.d == -4);
            REQUIRE(mat.e == -5);
            REQUIRE(mat.f == -6);
            REQUIRE(mat.g == -7);
            REQUIRE(mat.h == -8);
            REQUIRE(mat.i == -9);
            REQUIRE(mat(0, 0) == -1);
            REQUIRE(mat(0, 1) == -2);
            REQUIRE(mat(0, 2) == -3);
            REQUIRE(mat(1, 0) == -4);
            REQUIRE(mat(1, 1) == -5);
            REQUIRE(mat(1, 2) == -6);
            REQUIRE(mat(2, 0) == -7);
            REQUIRE(mat(2, 1) == -8);
            REQUIRE(mat(2, 2) == -9);
            REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
            REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
            REQUIRE(mat.row_vectors[2] == vec3i(-7, -8, -9));
            REQUIRE(mat.column(0) == vec3i(-1, -4, -7));
            REQUIRE(mat.column(1) == vec3i(-2, -5, -8));
            REQUIRE(mat.column(2) == vec3i(-3, -6, -9));
        }
        SECTION("Data pointer") {
            mat.data()[0] = 1; mat.data()[1] = 2; mat.data()[2] = 3;
            mat.data()[3] = 4; mat.data()[4] = 5; mat.data()[5] = 6;
            mat.data()[6] = 7; mat.data()[7] = 8; mat.data()[8] = 9;

            REQUIRE(mat.a == 1);
            REQUIRE(mat.b == 2);
            REQUIRE(mat.c == 3);
            REQUIRE(mat.d == 4);
            REQUIRE(mat.e == 5);
            REQUIRE(mat.f == 6);
            REQUIRE(mat.g == 7);
            REQUIRE(mat.h == 8);
            REQUIRE(mat.i == 9);
            REQUIRE(mat(0, 0) == 1);
            REQUIRE(mat(0, 1) == 2);
            REQUIRE(mat(0, 2) == 3);
            REQUIRE(mat(1, 0) == 4);
            REQUIRE(mat(1, 1) == 5);
            REQUIRE(mat(1, 2) == 6);
            REQUIRE(mat(2, 0) == 7);
            REQUIRE(mat(2, 1) == 8);
            REQUIRE(mat(2, 2) == 9);
            REQUIRE(mat.row_vectors[0] == vec3i(1, 2, 3));
            REQUIRE(mat.row_vectors[1] == vec3i(4, 5, 6));
            REQUIRE(mat.row_vectors[2] == vec3i(7, 8, 9));
            REQUIRE(mat.column(0) == vec3i(1, 4, 7));
            REQUIRE(mat.column(1) == vec3i(2, 5, 8));
            REQUIRE(mat.column(2) == vec3i(3, 6, 9));
        }
        SECTION("set()") {
            mat.set(0, 0, 1);
            mat.set(0, 1, 2);
            mat.set(0, 2, 3);
            mat.set(1, 0, 4);
            mat.set(1, 1, 5);
            mat.set(1, 2, 6);
            mat.set(2, 0, 7);
            mat.set(2, 1, 8);
            mat.set(2, 2, 9);
            REQUIRE(mat.a == 1);
            REQUIRE(mat.b == 2);
            REQUIRE(mat.c == 3);
            REQUIRE(mat.row_vectors[0] == vec3i(1, 2, 3));
            REQUIRE(mat.d == 4);
            REQUIRE(mat.e == 5);
            REQUIRE(mat.f == 6);
            REQUIRE(mat.row_vectors[1] == vec3i(4, 5, 6));
            REQUIRE(mat.g == 7);
            REQUIRE(mat.h == 8);
            REQUIRE(mat.i == 9);
            REQUIRE(mat.row_vectors[2] == vec3i(7, 8, 9));
            REQUIRE(mat.column(0) == vec3i(1, 4, 7));
            REQUIRE(mat.column(1) == vec3i(2, 5, 8));
            REQUIRE(mat.column(2) == vec3i(3, 6, 9));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, vec3i(1, -4, 7));
            mat.setColumn(1, vec3i(-2, 5, -8));
            mat.setColumn(2, vec3i(3, -6, 9));
            REQUIRE(mat.a == 1);
            REQUIRE(mat.b == -2);
            REQUIRE(mat.c == 3);
            REQUIRE(mat.row_vectors[0] == vec3i(1, -2, 3));
            REQUIRE(mat.d == -4);
            REQUIRE(mat.e == 5);
            REQUIRE(mat.f == -6);
            REQUIRE(mat.row_vectors[1] == vec3i(-4, 5, -6));
            REQUIRE(mat.g == 7);
            REQUIRE(mat.h == -8);
            REQUIRE(mat.i == 9);
            REQUIRE(mat.row_vectors[2] == vec3i(7, -8, 9));
            REQUIRE(mat.column(0) == vec3i(1, -4, 7));
            REQUIRE(mat.column(1) == vec3i(-2, 5, -8));
            REQUIRE(mat.column(2) == vec3i(3, -6, 9));
        }
    }
    SECTION("Fill constructor") {
        Matrix<int, 3, 3> mat{ 7 };
        REQUIRE(mat.a == 7);
        REQUIRE(mat.b == 7);
        REQUIRE(mat.c == 7);
        REQUIRE(mat.d == 7);
        REQUIRE(mat.e == 7);
        REQUIRE(mat.f == 7);
        REQUIRE(mat.g == 7);
        REQUIRE(mat.h == 7);
        REQUIRE(mat.i == 7);
        Vector<int, 3> vec{ 7 };
        REQUIRE(mat.row_vectors[0] == vec);
        REQUIRE(mat.row_vectors[1] == vec);
        REQUIRE(mat.row_vectors[2] == vec);
        REQUIRE(mat.column(0) == vec);
        REQUIRE(mat.column(1) == vec);
        REQUIRE(mat.column(2) == vec);
    }
    SECTION("Array constructor") {
        const int arr[]{ -1, 2, -3, 4, -5, 6, -7, 8, -9, 10 };
        Matrix<int, 3, 3> m1{arr};
        REQUIRE(m1.a == -1);
        REQUIRE(m1.b == 2);
        REQUIRE(m1.c == -3);
        REQUIRE(m1.d == 4);
        REQUIRE(m1.e == -5);
        REQUIRE(m1.f == 6);
        REQUIRE(m1.g == -7);
        REQUIRE(m1.h == 8);
        REQUIRE(m1.i == -9);
        REQUIRE(m1(0, 0) == -1);
        REQUIRE(m1(0, 1) == 2);
        REQUIRE(m1(0, 2) == -3);
        REQUIRE(m1(1, 0) == 4);
        REQUIRE(m1(1, 1) == -5);
        REQUIRE(m1(1, 2) == 6);
        REQUIRE(m1(2, 0) == -7);
        REQUIRE(m1(2, 1) == 8);
        REQUIRE(m1(2, 2) == -9);
        REQUIRE(m1.row_vectors[0] == vec3i(-1, 2, -3));
        REQUIRE(m1.row_vectors[1] == vec3i(4, -5, 6));
        REQUIRE(m1.row_vectors[2] == vec3i(-7, 8, -9));
        REQUIRE(m1.column(0) == vec3i(-1, 4, -7));
        REQUIRE(m1.column(1) == vec3i(2, -5, 8));
        REQUIRE(m1.column(2) == vec3i(-3, 6, -9));

        Matrix<int, 3, 3>m2{arr+1};
        REQUIRE(m2.a == 2);
        REQUIRE(m2.b == -3);
        REQUIRE(m2.c == 4);
        REQUIRE(m2.d == -5);
        REQUIRE(m2.e == 6);
        REQUIRE(m2.f == -7);
        REQUIRE(m2.g == 8);
        REQUIRE(m2.h == -9);
        REQUIRE(m2.i == 10);
        REQUIRE(m2(0, 0) == 2);
        REQUIRE(m2(0, 1) == -3);
        REQUIRE(m2(0, 2) == 4);
        REQUIRE(m2(1, 0) == -5);
        REQUIRE(m2(1, 1) == 6);
        REQUIRE(m2(1, 2) == -7);
        REQUIRE(m2(2, 0) == 8);
        REQUIRE(m2(2, 1) == -9);
        REQUIRE(m2(2, 2) == 10);
        REQUIRE(m2.row_vectors[0] == vec3i(2, -3, 4));
        REQUIRE(m2.row_vectors[1] == vec3i(-5, 6, -7));
        REQUIRE(m2.row_vectors[2] == vec3i(8, -9, 10));
        REQUIRE(m2.column(0) == vec3i(2, -5, 8));
        REQUIRE(m2.column(1) == vec3i(-3, 6, -9));
        REQUIRE(m2.column(2) == vec3i(4, -7, 10));
    }
    SECTION("Initialize from row vectors") {
        Matrix<int32_t, 3, 3> mat{
            { -9, -8, -7 },
            { -6, -5, -4 },
            { -3, -2, -1 }
        };
        REQUIRE(mat.a == -9);
        REQUIRE(mat.b == -8);
        REQUIRE(mat.c == -7);
        REQUIRE(mat.d == -6);
        REQUIRE(mat.e == -5);
        REQUIRE(mat.f == -4);
        REQUIRE(mat.g == -3);
        REQUIRE(mat.h == -2);
        REQUIRE(mat.i == -1);
        REQUIRE(mat(0, 0) == -9);
        REQUIRE(mat(0, 1) == -8);
        REQUIRE(mat(0, 2) == -7);
        REQUIRE(mat(1, 0) == -6);
        REQUIRE(mat(1, 1) == -5);
        REQUIRE(mat(1, 2) == -4);
        REQUIRE(mat(2, 0) == -3);
        REQUIRE(mat(2, 1) == -2);
        REQUIRE(mat(2, 2) == -1);
        REQUIRE(mat.row_vectors[0] == vec3i(-9, -8, -7));
        REQUIRE(mat.row_vectors[1] == vec3i(-6, -5, -4));
        REQUIRE(mat.row_vectors[2] == vec3i(-3, -2, -1));
        REQUIRE(mat.column(0) == vec3i(-9, -6, -3));
        REQUIRE(mat.column(1) == vec3i(-8, -5, -2));
        REQUIRE(mat.column(2) == vec3i(-7, -4, -1));
    }
    SECTION("Casting constructor") {
        Matrix<int, 3, 3> mat{ mat3{ -3.0f } };
        REQUIRE(mat.a == -3);
        REQUIRE(mat.b == -3);
        REQUIRE(mat.c == -3);
        REQUIRE(mat.d == -3);
        REQUIRE(mat.e == -3);
        REQUIRE(mat.f == -3);
        REQUIRE(mat.g == -3);
        REQUIRE(mat.h == -3);
        REQUIRE(mat.i == -3);
        REQUIRE(mat(0, 0) == -3);
        REQUIRE(mat(0, 1) == -3);
        REQUIRE(mat(0, 2) == -3);
        REQUIRE(mat(1, 0) == -3);
        REQUIRE(mat(1, 1) == -3);
        REQUIRE(mat(1, 2) == -3);
        REQUIRE(mat(2, 0) == -3);
        REQUIRE(mat(2, 1) == -3);
        REQUIRE(mat(2, 2) == -3);
        Vector<int, 3> vec{ -3 };
        REQUIRE(mat.row_vectors[0] == vec);
        REQUIRE(mat.row_vectors[1] == vec);
        REQUIRE(mat.row_vectors[2] == vec);
        REQUIRE(mat.column(0) == vec);
        REQUIRE(mat.column(1) == vec);
        REQUIRE(mat.column(2) == vec);
    }
    SECTION("Identity matrix") {
        mat3 id3 = mat3::identity();
        // Row 0
        REQUIRE(id3.e00 == 1.0f);
        REQUIRE(id3.e01 == 0.0f);
        REQUIRE(id3.e02 == 0.0f);
        REQUIRE(id3.row_vectors[0] == vec3(1.0f, 0.0, 0.0));
        // Row 1
        REQUIRE(id3.e10 == 0.0f);
        REQUIRE(id3.e11 == 1.0f);
        REQUIRE(id3.e12 == 0.0f);
        REQUIRE(id3.row_vectors[1] == vec3(0.0f, 1.0f, 0.0f));
        // Row 2
        REQUIRE(id3.e20 == 0.0f);
        REQUIRE(id3.e21 == 0.0f);
        REQUIRE(id3.e22 == 1.0f);
        REQUIRE(id3.row_vectors[2] == vec3(0.0f, 0.0f, 1.0f));
        // Columns
        REQUIRE(id3.column(0) == vec3(1.0f, 0.0f, 0.0f));
        REQUIRE(id3.column(1) == vec3(0.0f, 1.0f, 0.0f));
        REQUIRE(id3.column(2) == vec3(0.0f, 0.0f, 1.0f));
    }
}

TEST_CASE("4x4 matrix specialization", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,4,4>) == 16 * sizeof(int) );
    }
    SECTION("Element access") {
        Matrix<int, 4, 4> mat;
        SECTION("() operator") {
            mat(0, 0) = 1;   mat(0, 1) = 2;   mat(0, 2) = 3;   mat(0, 3) = 4;
            mat(1, 0) = -5;  mat(1, 1) = -6;  mat(1, 2) = -7;  mat(1, 3) = -8;
            mat(2, 0) = 9;   mat(2, 1) = 10;  mat(2, 2) = 11;  mat(2, 3) = 12;
            mat(3, 0) = -13; mat(3, 1) = -14; mat(3, 2) = -15; mat(3, 3) = -16;

            // Row 0
            REQUIRE(mat.e00 == 1);
            REQUIRE(mat.e01 == 2);
            REQUIRE(mat.e02 == 3);
            REQUIRE(mat.e03 == 4);
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(0, 2) == 3);
            REQUIRE(mat.at(0, 3) == 4);
            REQUIRE(mat.row_vectors[0] == vec4i(1, 2, 3, 4));
            // Row 1
            REQUIRE(mat.e10 == -5);
            REQUIRE(mat.e11 == -6);
            REQUIRE(mat.e12 == -7);
            REQUIRE(mat.e13 == -8);
            REQUIRE(mat.at(1, 0) == -5);
            REQUIRE(mat.at(1, 1) == -6);
            REQUIRE(mat.at(1, 2) == -7);
            REQUIRE(mat.at(1, 3) == -8);
            REQUIRE(mat.row_vectors[1] == vec4i(-5, -6, -7, -8));
            // Row 2
            REQUIRE(mat.e20 == 9);
            REQUIRE(mat.e21 == 10);
            REQUIRE(mat.e22 == 11);
            REQUIRE(mat.e23 == 12);
            REQUIRE(mat.at(2, 0) == 9);
            REQUIRE(mat.at(2, 1) == 10);
            REQUIRE(mat.at(2, 2) == 11);
            REQUIRE(mat.at(2, 3) == 12);
            REQUIRE(mat.row_vectors[2] == vec4i(9, 10, 11, 12));
            // Row 3
            REQUIRE(mat.e30 == -13);
            REQUIRE(mat.e31 == -14);
            REQUIRE(mat.e32 == -15);
            REQUIRE(mat.e33 == -16);
            REQUIRE(mat.at(3, 0) == -13);
            REQUIRE(mat.at(3, 1) == -14);
            REQUIRE(mat.at(3, 2) == -15);
            REQUIRE(mat.at(3, 3) == -16);
            REQUIRE(mat.row_vectors[3] == vec4i(-13, -14, -15, -16));
            // Columns
            REQUIRE(mat.column(0) == vec4i(1, -5, 9, -13));
            REQUIRE(mat.column(1) == vec4i(2, -6, 10, -14));
            REQUIRE(mat.column(2) == vec4i(3, -7, 11, -15));
            REQUIRE(mat.column(3) == vec4i(4, -8, 12, -16));
        }
        SECTION("Data pointer") {
            int32_t* data = mat.data();
            for (int32_t i = 0; i < 16; i++) data[i] = i;

            // Row 0
            REQUIRE(mat.e00 == 0);
            REQUIRE(mat.e01 == 1);
            REQUIRE(mat.e02 == 2);
            REQUIRE(mat.e03 == 3);
            REQUIRE(mat.row_vectors[0] == vec4i(0, 1, 2, 3));
            // Row 1
            REQUIRE(mat.e10 == 4);
            REQUIRE(mat.e11 == 5);
            REQUIRE(mat.e12 == 6);
            REQUIRE(mat.e13 == 7);
            REQUIRE(mat.row_vectors[1] == vec4i(4, 5, 6, 7));
            // Row 2
            REQUIRE(mat.e20 == 8);
            REQUIRE(mat.e21 == 9);
            REQUIRE(mat.e22 == 10);
            REQUIRE(mat.e23 == 11);
            REQUIRE(mat.row_vectors[2] == vec4i(8, 9, 10, 11));
            // Row 3
            REQUIRE(mat.e30 == 12);
            REQUIRE(mat.e31 == 13);
            REQUIRE(mat.e32 == 14);
            REQUIRE(mat.e33 == 15);
            REQUIRE(mat.row_vectors[3] == vec4i(12, 13, 14, 15));
            // Columns
            REQUIRE(mat.column(0) == vec4i(0, 4, 8, 12));
            REQUIRE(mat.column(1) == vec4i(1, 5, 9, 13));
            REQUIRE(mat.column(2) == vec4i(2, 6, 10, 14));
            REQUIRE(mat.column(3) == vec4i(3, 7, 11, 15));
        }
        SECTION("set()") {
            mat.set(0, 0, 15);
            mat.set(0, 1, 14);
            mat.set(0, 2, 13);
            mat.set(0, 3, 12);
            mat.set(1, 0, 11);
            mat.set(1, 1, 10);
            mat.set(1, 2, 9);
            mat.set(1, 3, 8);
            mat.set(2, 0, 7);
            mat.set(2, 1, 6);
            mat.set(2, 2, 5);
            mat.set(2, 3, 4);
            mat.set(3, 0, 3);
            mat.set(3, 1, 2);
            mat.set(3, 2, 1);
            mat.set(3, 3, 0);
            // Row 0
            REQUIRE(mat.e00 == 15);
            REQUIRE(mat.e01 == 14);
            REQUIRE(mat.e02 == 13);
            REQUIRE(mat.e03 == 12);
            REQUIRE(mat.row_vectors[0] == vec4i(15, 14, 13, 12));
            // Row 1
            REQUIRE(mat.e10 == 11);
            REQUIRE(mat.e11 == 10);
            REQUIRE(mat.e12 == 9);
            REQUIRE(mat.e13 == 8);
            REQUIRE(mat.row_vectors[1] == vec4i(11, 10, 9, 8));
            // Row 2
            REQUIRE(mat.e20 == 7);
            REQUIRE(mat.e21 == 6);
            REQUIRE(mat.e22 == 5);
            REQUIRE(mat.e23 == 4);
            REQUIRE(mat.row_vectors[2] == vec4i(7, 6, 5, 4));
            // Row 3
            REQUIRE(mat.e30 == 3);
            REQUIRE(mat.e31 == 2);
            REQUIRE(mat.e32 == 1);
            REQUIRE(mat.e33 == 0);
            REQUIRE(mat.row_vectors[3] == vec4i(3, 2, 1, 0));
            // Columns
            REQUIRE(mat.column(0) == vec4i(15, 11, 7, 3));
            REQUIRE(mat.column(1) == vec4i(14, 10, 6, 2));
            REQUIRE(mat.column(2) == vec4i(13, 9, 5, 1));
            REQUIRE(mat.column(3) == vec4i(12, 8, 4, 0));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, { 0, 4, 8, 12 });
            mat.setColumn(1, { 1, 5, 9, 13 });
            mat.setColumn(2, { 2, 6, 10, 14 });
            mat.setColumn(3, { 3, 7, 11, 15 });
            // Row 0
            REQUIRE(mat.e00 == 0);
            REQUIRE(mat.e01 == 1);
            REQUIRE(mat.e02 == 2);
            REQUIRE(mat.e03 == 3);
            REQUIRE(mat.row_vectors[0] == vec4i(0, 1, 2, 3));
            // Row 1
            REQUIRE(mat.e10 == 4);
            REQUIRE(mat.e11 == 5);
            REQUIRE(mat.e12 == 6);
            REQUIRE(mat.e13 == 7);
            REQUIRE(mat.row_vectors[1] == vec4i(4, 5, 6, 7));
            // Row 2
            REQUIRE(mat.e20 == 8);
            REQUIRE(mat.e21 == 9);
            REQUIRE(mat.e22 == 10);
            REQUIRE(mat.e23 == 11);
            REQUIRE(mat.row_vectors[2] == vec4i(8, 9, 10, 11));
            // Row 3
            REQUIRE(mat.e30 == 12);
            REQUIRE(mat.e31 == 13);
            REQUIRE(mat.e32 == 14);
            REQUIRE(mat.e33 == 15);
            REQUIRE(mat.row_vectors[3] == vec4i(12, 13, 14, 15));
            // Columns
            REQUIRE(mat.column(0) == vec4i(0, 4, 8, 12));
            REQUIRE(mat.column(1) == vec4i(1, 5, 9, 13));
            REQUIRE(mat.column(2) == vec4i(2, 6, 10, 14));
            REQUIRE(mat.column(3) == vec4i(3, 7, 11, 15));
        }
    }
    SECTION("Fill constructor") {
        Matrix<int, 4, 4> mat{ -1 };
        Vector<int, 4> vec{ -1 };
        // Row 0
        REQUIRE(mat.e00 == -1);
        REQUIRE(mat.e01 == -1);
        REQUIRE(mat.e02 == -1);
        REQUIRE(mat.e03 == -1);
        REQUIRE(mat.row_vectors[0] == vec);
        // Row 1
        REQUIRE(mat.e10 == -1);
        REQUIRE(mat.e11 == -1);
        REQUIRE(mat.e12 == -1);
        REQUIRE(mat.e13 == -1);
        REQUIRE(mat.row_vectors[1] == vec);
        // Row 2
        REQUIRE(mat.e20 == -1);
        REQUIRE(mat.e21 == -1);
        REQUIRE(mat.e22 == -1);
        REQUIRE(mat.e23 == -1);
        REQUIRE(mat.row_vectors[2] == vec);
        // Row 3
        REQUIRE(mat.e30 == -1);
        REQUIRE(mat.e31 == -1);
        REQUIRE(mat.e32 == -1);
        REQUIRE(mat.e33 == -1);
        REQUIRE(mat.row_vectors[3] == vec);
        // Columns
        REQUIRE(mat.column(0) == vec);
        REQUIRE(mat.column(1) == vec);
        REQUIRE(mat.column(2) == vec);
        REQUIRE(mat.column(3) == vec);
    }
    SECTION("Array constructor") {
        const int32_t arr[]{ 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        Matrix<int32_t, 4, 4> m1{arr};
        // Row 0
        REQUIRE(m1.e00 == 16);
        REQUIRE(m1.e01 == 15);
        REQUIRE(m1.e02 == 14);
        REQUIRE(m1.e03 == 13);
        REQUIRE(m1.at(0, 0) == 16);
        REQUIRE(m1.at(0, 1) == 15);
        REQUIRE(m1.at(0, 2) == 14);
        REQUIRE(m1.at(0, 3) == 13);
        REQUIRE(m1.row_vectors[0] == vec4i(16, 15, 14, 13));
        // Row 1
        REQUIRE(m1.e10 == 12);
        REQUIRE(m1.e11 == 11);
        REQUIRE(m1.e12 == 10);
        REQUIRE(m1.e13 == 9);
        REQUIRE(m1.at(1, 0) == 12);
        REQUIRE(m1.at(1, 1) == 11);
        REQUIRE(m1.at(1, 2) == 10);
        REQUIRE(m1.at(1, 3) == 9);
        REQUIRE(m1.row_vectors[1] == vec4i(12, 11, 10, 9));
        // Row 2
        REQUIRE(m1.e20 == 8);
        REQUIRE(m1.e21 == 7);
        REQUIRE(m1.e22 == 6);
        REQUIRE(m1.e23 == 5);
        REQUIRE(m1.at(2, 0) == 8);
        REQUIRE(m1.at(2, 1) == 7);
        REQUIRE(m1.at(2, 2) == 6);
        REQUIRE(m1.at(2, 3) == 5);
        REQUIRE(m1.row_vectors[2] == vec4i(8, 7, 6, 5));
        // Row 3
        REQUIRE(m1.e30 == 4);
        REQUIRE(m1.e31 == 3);
        REQUIRE(m1.e32 == 2);
        REQUIRE(m1.e33 == 1);
        REQUIRE(m1.at(3, 0) == 4);
        REQUIRE(m1.at(3, 1) == 3);
        REQUIRE(m1.at(3, 2) == 2);
        REQUIRE(m1.at(3, 3) == 1);
        REQUIRE(m1.row_vectors[3] == vec4i(4, 3, 2, 1));
        // Columns
        REQUIRE(m1.column(0) == vec4i(16, 12, 8, 4));
        REQUIRE(m1.column(1) == vec4i(15, 11, 7, 3));
        REQUIRE(m1.column(2) == vec4i(14, 10, 6, 2));
        REQUIRE(m1.column(3) == vec4i(13, 9, 5, 1));

        Matrix<int32_t, 4, 4> m2{arr+1};
        // Row 0
        REQUIRE(m2.e00 == 15);
        REQUIRE(m2.e01 == 14);
        REQUIRE(m2.e02 == 13);
        REQUIRE(m2.e03 == 12);
        REQUIRE(m2.at(0, 0) == 15);
        REQUIRE(m2.at(0, 1) == 14);
        REQUIRE(m2.at(0, 2) == 13);
        REQUIRE(m2.at(0, 3) == 12);
        REQUIRE(m2.row_vectors[0] == vec4i(15, 14, 13, 12));
        // Row 1
        REQUIRE(m2.e10 == 11);
        REQUIRE(m2.e11 == 10);
        REQUIRE(m2.e12 == 9);
        REQUIRE(m2.e13 == 8);
        REQUIRE(m2.at(1, 0) == 11);
        REQUIRE(m2.at(1, 1) == 10);
        REQUIRE(m2.at(1, 2) == 9);
        REQUIRE(m2.at(1, 3) == 8);
        REQUIRE(m2.row_vectors[1] == vec4i(11, 10, 9, 8));
        // Row 2
        REQUIRE(m2.e20 == 7);
        REQUIRE(m2.e21 == 6);
        REQUIRE(m2.e22 == 5);
        REQUIRE(m2.e23 == 4);
        REQUIRE(m2.at(2, 0) == 7);
        REQUIRE(m2.at(2, 1) == 6);
        REQUIRE(m2.at(2, 2) == 5);
        REQUIRE(m2.at(2, 3) == 4);
        REQUIRE(m2.row_vectors[2] == vec4i(7, 6, 5, 4));
        // Row 3
        REQUIRE(m2.e30 == 3);
        REQUIRE(m2.e31 == 2);
        REQUIRE(m2.e32 == 1);
        REQUIRE(m2.e33 == 0);
        REQUIRE(m2.at(3, 0) == 3);
        REQUIRE(m2.at(3, 1) == 2);
        REQUIRE(m2.at(3, 2) == 1);
        REQUIRE(m2.at(3, 3) == 0);
        REQUIRE(m2.row_vectors[3] == vec4i(3, 2, 1, 0));
        // Columns
        REQUIRE(m2.column(0) == vec4i(15, 11, 7, 3));
        REQUIRE(m2.column(1) == vec4i(14, 10, 6, 2));
        REQUIRE(m2.column(2) == vec4i(13, 9, 5, 1));
        REQUIRE(m2.column(3) == vec4i(12, 8, 4, 0));
    }
    SECTION("Initialize from row vectors") {
        Matrix<int32_t, 4, 4> mat{
            { 2, -2, -2, -2 },
            { -2, 2, -2, -2 },
            { -2, -2, 2, -2 },
            { -2, -2, -2, 2 }
        };
        // Row 0
        REQUIRE(mat.e00 == 2);
        REQUIRE(mat.e01 == -2);
        REQUIRE(mat.e02 == -2);
        REQUIRE(mat.e03 == -2);
        REQUIRE(mat.at(0, 0) == 2);
        REQUIRE(mat.at(0, 1) == -2);
        REQUIRE(mat.at(0, 2) == -2);
        REQUIRE(mat.at(0, 3) == -2);
        REQUIRE(mat.row_vectors[0] == vec4i(2, -2, -2, -2));
        // Row 1
        REQUIRE(mat.e10 == -2);
        REQUIRE(mat.e11 == 2);
        REQUIRE(mat.e12 == -2);
        REQUIRE(mat.e13 == -2);
        REQUIRE(mat.at(1, 0) == -2);
        REQUIRE(mat.at(1, 1) == 2);
        REQUIRE(mat.at(1, 2) == -2);
        REQUIRE(mat.at(1, 3) == -2);
        REQUIRE(mat.row_vectors[1] == vec4i(-2, 2, -2, -2));
        // Row 2
        REQUIRE(mat.e20 == -2);
        REQUIRE(mat.e21 == -2);
        REQUIRE(mat.e22 == 2);
        REQUIRE(mat.e23 == -2);
        REQUIRE(mat.at(2, 0) == -2);
        REQUIRE(mat.at(2, 1) == -2);
        REQUIRE(mat.at(2, 2) == 2);
        REQUIRE(mat.at(2, 3) == -2);
        REQUIRE(mat.row_vectors[2] == vec4i(-2, -2, 2, -2));
        // Row 3
        REQUIRE(mat.e30 == -2);
        REQUIRE(mat.e31 == -2);
        REQUIRE(mat.e32 == -2);
        REQUIRE(mat.e33 == 2);
        REQUIRE(mat.at(3, 0) == -2);
        REQUIRE(mat.at(3, 1) == -2);
        REQUIRE(mat.at(3, 2) == -2);
        REQUIRE(mat.at(3, 3) == 2);
        REQUIRE(mat.row_vectors[3] == vec4i(-2, -2, -2, 2));
        // Columns
        REQUIRE(mat.column(0) == vec4i(2, -2, -2, -2));
        REQUIRE(mat.column(1) == vec4i(-2, 2, -2, -2));
        REQUIRE(mat.column(2) == vec4i(-2, -2, 2, -2));
        REQUIRE(mat.column(3) == vec4i(-2, -2, -2, 2));
    }
    SECTION("Casting constructor") {
        Matrix<int, 4, 4> mat{ mat4{ 1.0f } };
        Vector<int, 4> vec{ 1 };
        // Row 0
        REQUIRE(mat.e00 == 1);
        REQUIRE(mat.e01 == 1);
        REQUIRE(mat.e02 == 1);
        REQUIRE(mat.e03 == 1);
        REQUIRE(mat.at(0, 0) == 1);
        REQUIRE(mat.at(0, 1) == 1);
        REQUIRE(mat.at(0, 2) == 1);
        REQUIRE(mat.at(0, 3) == 1);
        REQUIRE(mat.row_vectors[0] == vec);
        // Row 1
        REQUIRE(mat.e10 == 1);
        REQUIRE(mat.e11 == 1);
        REQUIRE(mat.e12 == 1);
        REQUIRE(mat.e13 == 1);
        REQUIRE(mat.at(1, 0) == 1);
        REQUIRE(mat.at(1, 1) == 1);
        REQUIRE(mat.at(1, 2) == 1);
        REQUIRE(mat.at(1, 3) == 1);
        REQUIRE(mat.row_vectors[1] == vec);
        // Row 2
        REQUIRE(mat.e20 == 1);
        REQUIRE(mat.e21 == 1);
        REQUIRE(mat.e22 == 1);
        REQUIRE(mat.e23 == 1);
        REQUIRE(mat.at(2, 0) == 1);
        REQUIRE(mat.at(2, 1) == 1);
        REQUIRE(mat.at(2, 2) == 1);
        REQUIRE(mat.at(2, 3) == 1);
        REQUIRE(mat.row_vectors[2] == vec);
        // Row 3
        REQUIRE(mat.e30 == 1);
        REQUIRE(mat.e31 == 1);
        REQUIRE(mat.e32 == 1);
        REQUIRE(mat.e33 == 1);
        REQUIRE(mat.at(3, 0) == 1);
        REQUIRE(mat.at(3, 1) == 1);
        REQUIRE(mat.at(3, 2) == 1);
        REQUIRE(mat.at(3, 3) == 1);
        REQUIRE(mat.row_vectors[3] == vec);
        // Columns
        REQUIRE(mat.column(0) == vec);
        REQUIRE(mat.column(1) == vec);
        REQUIRE(mat.column(2) == vec);
        REQUIRE(mat.column(3) == vec);
    }
    SECTION("Identity matrix") {
        mat4 id4 = mat4::identity();
        // Row 0
        REQUIRE(id4.e00 == 1.0f);
        REQUIRE(id4.e01 == 0.0f);
        REQUIRE(id4.e02 == 0.0f);
        REQUIRE(id4.e03 == 0.0f);
        REQUIRE(id4.row_vectors[0] == vec4(1.0f, 0.0f, 0.0f, 0.0f));
        // Row 1
        REQUIRE(id4.e10 == 0.0f);
        REQUIRE(id4.e11 == 1.0f);
        REQUIRE(id4.e12 == 0.0f);
        REQUIRE(id4.e13 == 0.0f);
        REQUIRE(id4.row_vectors[1] == vec4(0.0f, 1.0f, 0.0f, 0.0f));
        // Row 2
        REQUIRE(id4.e20 == 0.0f);
        REQUIRE(id4.e21 == 0.0f);
        REQUIRE(id4.e22 == 1.0f);
        REQUIRE(id4.e23 == 0.0f);
        REQUIRE(id4.row_vectors[2] == vec4(0.0f, 0.0f, 1.0f, 0.0f));
        // Row 3
        REQUIRE(id4.e30 == 0.0f);
        REQUIRE(id4.e31 == 0.0f);
        REQUIRE(id4.e32 == 0.0f);
        REQUIRE(id4.e33 == 1.0f);
        REQUIRE(id4.row_vectors[3] == vec4(0.0f, 0.0f, 0.0f, 1.0f));
        // Columns
        REQUIRE(id4.column(0) == vec4(1.0f, 0.0f, 0.0f, 0.0f));
        REQUIRE(id4.column(1) == vec4(0.0f, 1.0f, 0.0f, 0.0f));
        REQUIRE(id4.column(2) == vec4(0.0f, 0.0f, 1.0f, 0.0f));
        REQUIRE(id4.column(3) == vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
}

TEST_CASE("General matrix", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int, 1, 5>) == 5*sizeof(int) );
    }
    SECTION("Element access") {
        Matrix<int32_t, 2, 3> mat;
        SECTION("() operator") {
            mat(0, 0) = 1;
            mat(0, 1) = 2;
            mat(0, 2) = 3;
            mat(1, 0) = 4;
            mat(1, 1) = 5;
            mat(1, 2) = 6;
            // Row 0
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(0, 2) == 3);
            REQUIRE(mat.row_vectors[0] == vec3i(1, 2, 3));
            // Row 1
            REQUIRE(mat.at(1, 0) == 4);
            REQUIRE(mat.at(1, 1) == 5);
            REQUIRE(mat.at(1, 2) == 6);
            REQUIRE(mat.row_vectors[1] == vec3i(4, 5, 6));
            // Columns
            REQUIRE(mat.column(0) == vec2i(1, 4));
            REQUIRE(mat.column(1) == vec2i(2, 5));
            REQUIRE(mat.column(2) == vec2i(3, 6));
        }
        SECTION("Data pointer") {
            mat.data()[0] = -1;
            mat.data()[1] = -2;
            mat.data()[2] = -3;
            mat.data()[3] = -4;
            mat.data()[4] = -5;
            mat.data()[5] = -6;
            // Row 0
            REQUIRE(mat.at(0, 0) == -1);
            REQUIRE(mat.at(0, 1) == -2);
            REQUIRE(mat.at(0, 2) == -3);
            REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
            // Row 1
            REQUIRE(mat.at(1, 0) == -4);
            REQUIRE(mat.at(1, 1) == -5);
            REQUIRE(mat.at(1, 2) == -6);
            REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
            // Columns
            REQUIRE(mat.column(0) == vec2i(-1, -4));
            REQUIRE(mat.column(1) == vec2i(-2, -5));
            REQUIRE(mat.column(2) == vec2i(-3, -6));
        }
        SECTION("set()") {
            mat.set(0, 0, -1);
            mat.set(0, 1, -2);
            mat.set(0, 2, -3);
            mat.set(1, 0, -4);
            mat.set(1, 1, -5);
            mat.set(1, 2, -6);
            // Row 0
            REQUIRE(mat.at(0, 0) == -1);
            REQUIRE(mat.at(0, 1) == -2);
            REQUIRE(mat.at(0, 2) == -3);
            REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
            // Row 1
            REQUIRE(mat.at(1, 0) == -4);
            REQUIRE(mat.at(1, 1) == -5);
            REQUIRE(mat.at(1, 2) == -6);
            REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
            // Columns
            REQUIRE(mat.column(0) == vec2i(-1, -4));
            REQUIRE(mat.column(1) == vec2i(-2, -5));
            REQUIRE(mat.column(2) == vec2i(-3, -6));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, vec2i(-1, -4));
            mat.setColumn(1, vec2i(-2, -5));
            mat.setColumn(2, vec2i(-3, -6));
            // Row 0
            REQUIRE(mat.at(0, 0) == -1);
            REQUIRE(mat.at(0, 1) == -2);
            REQUIRE(mat.at(0, 2) == -3);
            REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
            // Row 1
            REQUIRE(mat.at(1, 0) == -4);
            REQUIRE(mat.at(1, 1) == -5);
            REQUIRE(mat.at(1, 2) == -6);
            REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
            // Columns
            REQUIRE(mat.column(0) == vec2i(-1, -4));
            REQUIRE(mat.column(1) == vec2i(-2, -5));
            REQUIRE(mat.column(2) == vec2i(-3, -6));
        }
    }
    SECTION("Fill constructor") {
        Matrix<int32_t, 2, 3> mat{ 3 };
        vec2i vec_2{ 3 };
        vec3i vec_3{ 3 };
        // Row 0
        REQUIRE(mat(0, 0) == 3);
        REQUIRE(mat(0, 1) == 3);
        REQUIRE(mat(0, 2) == 3);
        REQUIRE(mat.row_vectors[0] == vec_3);
        // Row 1
        REQUIRE(mat(1, 0) == 3);
        REQUIRE(mat(1, 1) == 3);
        REQUIRE(mat(1, 2) == 3);
        REQUIRE(mat.row_vectors[1] == vec_3);
        // Columns
        REQUIRE(mat.column(0) == vec_2);
        REQUIRE(mat.column(1) == vec_2);
        REQUIRE(mat.column(2) == vec_2);
    }
    SECTION("Array constructor") {
        const int32_t arr[]{ 0, 1, 2, 3, 4, 5, 6 };
        Matrix<int32_t, 3, 2> m1{arr};
        // Row 0
        REQUIRE(m1(0, 0) == 0);
        REQUIRE(m1(0, 1) == 1);
        REQUIRE(m1.row_vectors[0] == vec2i(0, 1));
        // Row 1
        REQUIRE(m1(1, 0) == 2);
        REQUIRE(m1(1, 1) == 3);
        REQUIRE(m1.row_vectors[1] == vec2i(2, 3));
        // Row 2
        REQUIRE(m1(2, 0) == 4);
        REQUIRE(m1(2, 1) == 5);
        REQUIRE(m1.row_vectors[2] == vec2i(4, 5));
        // Columns
        REQUIRE(m1.column(0) == vec3i(0, 2, 4));
        REQUIRE(m1.column(1) == vec3i(1, 3, 5));

        Matrix<int32_t, 3, 2> m2{arr+1};
        // Row 0
        REQUIRE(m2(0, 0) == 1);
        REQUIRE(m2(0, 1) == 2);
        REQUIRE(m2.row_vectors[0] == vec2i(1, 2));
        // Row 1
        REQUIRE(m2(1, 0) == 3);
        REQUIRE(m2(1, 1) == 4);
        REQUIRE(m2.row_vectors[1] == vec2i(3, 4));
        // Row 2
        REQUIRE(m2(2, 0) == 5);
        REQUIRE(m2(2, 1) == 6);
        REQUIRE(m2.row_vectors[2] == vec2i(5, 6));
        // Columns
        REQUIRE(m2.column(0) == vec3i(1, 3, 5));
        REQUIRE(m2.column(1) == vec3i(2, 4, 6));
    }
    SECTION("Casting constructor") {
        Matrix<int32_t, 2, 3> mat{ Matrix<float, 2, 3>{ 1.0f } };
        vec2i vec_2{ 1 };
        vec3i vec_3{ 1 };
        // Row 0
        REQUIRE(mat(0, 0) == 1);
        REQUIRE(mat(0, 1) == 1);
        REQUIRE(mat.row_vectors[0] == vec_3);
        // Row 1
        REQUIRE(mat(1, 0) == 1);
        REQUIRE(mat(1, 1) == 1);
        REQUIRE(mat.row_vectors[1] == vec_3);
        // Columns
        REQUIRE(mat.column(0) == vec_2);
        REQUIRE(mat.column(1) == vec_2);
        REQUIRE(mat.column(2) == vec_2);
    }
}

TEST_CASE("General square matrix", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int, 5, 5>) == 25*sizeof(int) );
    }
    SECTION("Element access") {
        Matrix<int, 5, 5> mat;
        SECTION("() operator") {
            mat(0, 0) = 1; mat(0, 1) = 2; mat(0, 2) = 3; mat(0, 3) = 4; mat(0, 4) = 5;
            mat(1, 0) = 2; mat(1, 1) = 3; mat(1, 2) = 4; mat(1, 3) = 5; mat(1, 4) = 6;
            mat(2, 0) = 3; mat(2, 1) = 4; mat(2, 2) = 5; mat(2, 3) = 6; mat(2, 4) = 7;
            mat(3, 0) = 4; mat(3, 1) = 5; mat(3, 2) = 6; mat(3, 3) = 7; mat(3, 4) = 8;
            mat(4, 0) = 5; mat(4, 1) = 6; mat(4, 2) = 7; mat(4, 3) = 8; mat(4, 4) = 9;

            // Row 0
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(0, 2) == 3);
            REQUIRE(mat.at(0, 3) == 4);
            REQUIRE(mat.at(0, 4) == 5);
            Vector<int, 5> row0{ init_list<int>{ 1, 2, 3, 4, 5 }.begin() };
            REQUIRE(mat.row_vectors[0] == row0);
            // Row 1
            REQUIRE(mat.at(1, 0) == 2);
            REQUIRE(mat.at(1, 1) == 3);
            REQUIRE(mat.at(1, 2) == 4);
            REQUIRE(mat.at(1, 3) == 5);
            REQUIRE(mat.at(1, 4) == 6);
            Vector<int, 5> row1{ init_list<int>{ 2, 3, 4, 5, 6 }.begin() };
            REQUIRE(mat.row_vectors[1] == row1);
            // Row 2
            REQUIRE(mat.at(2, 0) == 3);
            REQUIRE(mat.at(2, 1) == 4);
            REQUIRE(mat.at(2, 2) == 5);
            REQUIRE(mat.at(2, 3) == 6);
            REQUIRE(mat.at(2, 4) == 7);
            Vector<int, 5> row2{ init_list<int>{ 3, 4, 5, 6, 7 }.begin() };
            REQUIRE(mat.row_vectors[2] == row2);
            // Row 3
            REQUIRE(mat.at(3, 0) == 4);
            REQUIRE(mat.at(3, 1) == 5);
            REQUIRE(mat.at(3, 2) == 6);
            REQUIRE(mat.at(3, 3) == 7);
            REQUIRE(mat.at(3, 4) == 8);
            Vector<int, 5> row3{ init_list<int>{ 4, 5, 6, 7, 8 }.begin() };
            REQUIRE(mat.row_vectors[3] == row3);
            // Row 4
            REQUIRE(mat.at(4, 0) == 5);
            REQUIRE(mat.at(4, 1) == 6);
            REQUIRE(mat.at(4, 2) == 7);
            REQUIRE(mat.at(4, 3) == 8);
            REQUIRE(mat.at(4, 4) == 9);
            Vector<int, 5> row4{ init_list<int>{ 5, 6, 7, 8, 9 }.begin() };
            REQUIRE(mat.row_vectors[4] == row4);
            // Columns
            Vector<int, 5> col0{ init_list<int>{ 1, 2, 3, 4, 5 }.begin() },
                col1{ init_list<int>{ 2, 3, 4, 5, 6 }.begin() },
                col2{ init_list<int>{ 3, 4, 5, 6, 7 }.begin() },
                col3{ init_list<int>{ 4, 5, 6, 7, 8 }.begin() },
                col4{ init_list<int>{ 5, 6, 7, 8, 9 }.begin() };
            REQUIRE(mat.column(0) == col0);
            REQUIRE(mat.column(1) == col1);
            REQUIRE(mat.column(2) == col2);
            REQUIRE(mat.column(3) == col3);
            REQUIRE(mat.column(4) == col4);
        }
        SECTION("Data pointer") {
            int* md = mat.data();
            md[0] = 1; md[1] = 2; md[2] = 3; md[3] = 4; md[4] = 5;
            md[5] = 2; md[6] = 3; md[7] = 4; md[8] = 5; md[9] = 6;
            md[10] = 3; md[11] = 4; md[12] = 5; md[13] = 6; md[14] = 7;
            md[15] = 4; md[16] = 5; md[17] = 6; md[18] = 7; md[19] = 8;
            md[20] = 5; md[21] = 6; md[22] = 7; md[23] = 8; md[24] = 9;

            // Row 0
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(0, 2) == 3);
            REQUIRE(mat.at(0, 3) == 4);
            REQUIRE(mat.at(0, 4) == 5);
            Vector<int, 5> row0{ init_list<int>{ 1, 2, 3, 4, 5 }.begin() };
            REQUIRE(mat.row_vectors[0] == row0);
            // Row 1
            REQUIRE(mat.at(1, 0) == 2);
            REQUIRE(mat.at(1, 1) == 3);
            REQUIRE(mat.at(1, 2) == 4);
            REQUIRE(mat.at(1, 3) == 5);
            REQUIRE(mat.at(1, 4) == 6);
            Vector<int, 5> row1{ init_list<int>{ 2, 3, 4, 5, 6 }.begin() };
            REQUIRE(mat.row_vectors[1] == row1);
            // Row 2
            REQUIRE(mat.at(2, 0) == 3);
            REQUIRE(mat.at(2, 1) == 4);
            REQUIRE(mat.at(2, 2) == 5);
            REQUIRE(mat.at(2, 3) == 6);
            REQUIRE(mat.at(2, 4) == 7);
            Vector<int, 5> row2{ init_list<int>{ 3, 4, 5, 6, 7 }.begin() };
            REQUIRE(mat.row_vectors[2] == row2);
            // Row 3
            REQUIRE(mat.at(3, 0) == 4);
            REQUIRE(mat.at(3, 1) == 5);
            REQUIRE(mat.at(3, 2) == 6);
            REQUIRE(mat.at(3, 3) == 7);
            REQUIRE(mat.at(3, 4) == 8);
            Vector<int, 5> row3{ init_list<int>{ 4, 5, 6, 7, 8 }.begin() };
            REQUIRE(mat.row_vectors[3] == row3);
            // Row 4
            REQUIRE(mat.at(4, 0) == 5);
            REQUIRE(mat.at(4, 1) == 6);
            REQUIRE(mat.at(4, 2) == 7);
            REQUIRE(mat.at(4, 3) == 8);
            REQUIRE(mat.at(4, 4) == 9);
            Vector<int, 5> row4{ init_list<int>{ 5, 6, 7, 8, 9 }.begin() };
            REQUIRE(mat.row_vectors[4] == row4);
            // Columns
            Vector<int, 5> col0{ init_list<int>{ 1, 2, 3, 4, 5 }.begin() },
                col1{ init_list<int>{ 2, 3, 4, 5, 6 }.begin() },
                col2{ init_list<int>{ 3, 4, 5, 6, 7 }.begin() },
                col3{ init_list<int>{ 4, 5, 6, 7, 8 }.begin() },
                col4{ init_list<int>{ 5, 6, 7, 8, 9 }.begin() };
            REQUIRE(mat.column(0) == col0);
            REQUIRE(mat.column(1) == col1);
            REQUIRE(mat.column(2) == col2);
            REQUIRE(mat.column(3) == col3);
            REQUIRE(mat.column(4) == col4);
        }
        SECTION("set()") {
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                    mat.set(i, j, 5*i + j + 1);
            // Row 0
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(0, 2) == 3);
            REQUIRE(mat.at(0, 3) == 4);
            REQUIRE(mat.at(0, 4) == 5);
            Vector<int, 5> row0{ init_list<int>{ 1, 2, 3, 4, 5 }.begin() };
            REQUIRE(mat.row_vectors[0] == row0);
            // Row 1
            REQUIRE(mat.at(1, 0) == 6);
            REQUIRE(mat.at(1, 1) == 7);
            REQUIRE(mat.at(1, 2) == 8);
            REQUIRE(mat.at(1, 3) == 9);
            REQUIRE(mat.at(1, 4) == 10);
            Vector<int, 5> row1{ init_list<int>{ 6, 7, 8, 9, 10 }.begin() };
            REQUIRE(mat.row_vectors[1] == row1);
            // Row 2
            REQUIRE(mat.at(2, 0) == 11);
            REQUIRE(mat.at(2, 1) == 12);
            REQUIRE(mat.at(2, 2) == 13);
            REQUIRE(mat.at(2, 3) == 14);
            REQUIRE(mat.at(2, 4) == 15);
            Vector<int, 5> row2{ init_list<int>{ 11, 12, 13, 14, 15 }.begin() };
            REQUIRE(mat.row_vectors[2] == row2);
            // Row 3
            REQUIRE(mat.at(3, 0) == 16);
            REQUIRE(mat.at(3, 1) == 17);
            REQUIRE(mat.at(3, 2) == 18);
            REQUIRE(mat.at(3, 3) == 19);
            REQUIRE(mat.at(3, 4) == 20);
            Vector<int, 5> row3{ init_list<int>{ 16, 17, 18, 19, 20 }.begin() };
            REQUIRE(mat.row_vectors[3] == row3);
            // Row 4
            REQUIRE(mat.at(4, 0) == 21);
            REQUIRE(mat.at(4, 1) == 22);
            REQUIRE(mat.at(4, 2) == 23);
            REQUIRE(mat.at(4, 3) == 24);
            REQUIRE(mat.at(4, 4) == 25);
            Vector<int, 5> row4{ init_list<int>{ 21, 22, 23, 24, 25 }.begin() };
            REQUIRE(mat.row_vectors[4] == row4);
            // Columns
            Vector<int, 5> col0{ init_list<int>{ 1, 6, 11, 16, 21 }.begin() },
                col1{ init_list<int>{ 2, 7, 12, 17, 22 }.begin() },
                col2{ init_list<int>{ 3, 8, 13, 18, 23 }.begin() },
                col3{ init_list<int>{ 4, 9, 14, 19, 24 }.begin() },
                col4{ init_list<int>{ 5, 10, 15, 20, 25 }.begin() };
            REQUIRE(mat.column(0) == col0);
            REQUIRE(mat.column(1) == col1);
            REQUIRE(mat.column(2) == col2);
            REQUIRE(mat.column(3) == col3);
            REQUIRE(mat.column(4) == col4);
        }
        SECTION("setColumn()") {
            Vector<int, 5> col0{ init_list<int>{ 1, 6, 11, 16, 21 }.begin() },
                col1{ init_list<int>{ 2, 7, 12, 17, 22 }.begin() },
                col2{ init_list<int>{ 3, 8, 13, 18, 23 }.begin() },
                col3{ init_list<int>{ 4, 9, 14, 19, 24 }.begin() },
                col4{ init_list<int>{ 5, 10, 15, 20, 25 }.begin() };
            mat.setColumn(0, col0);
            mat.setColumn(1, col1);
            mat.setColumn(2, col2);
            mat.setColumn(3, col3);
            mat.setColumn(4, col4);
            // Row 0
            REQUIRE(mat.at(0, 0) == 1);
            REQUIRE(mat.at(0, 1) == 2);
            REQUIRE(mat.at(0, 2) == 3);
            REQUIRE(mat.at(0, 3) == 4);
            REQUIRE(mat.at(0, 4) == 5);
            Vector<int, 5> row0{ init_list<int>{ 1, 2, 3, 4, 5}.begin() };
            REQUIRE(mat.row_vectors[0] == row0);
            REQUIRE(mat.column(0) == col0);
            // Row 1
            REQUIRE(mat.at(1, 0) == 6);
            REQUIRE(mat.at(1, 1) == 7);
            REQUIRE(mat.at(1, 2) == 8);
            REQUIRE(mat.at(1, 3) == 9);
            REQUIRE(mat.at(1, 4) == 10);
            Vector<int, 5> row1{ init_list<int>{6, 7, 8, 9, 10}.begin() };
            REQUIRE(mat.row_vectors[1] == row1);
            REQUIRE(mat.column(1) == col1);
            // Row 2
            REQUIRE(mat.at(2, 0) == 11);
            REQUIRE(mat.at(2, 1) == 12);
            REQUIRE(mat.at(2, 2) == 13);
            REQUIRE(mat.at(2, 3) == 14);
            REQUIRE(mat.at(2, 4) == 15);
            Vector<int, 5> row2{ init_list<int>{ 11, 12, 13, 14, 15}.begin() };
            REQUIRE(mat.row_vectors[2] == row2);
            REQUIRE(mat.column(2) == col2);
            // Row 3
            REQUIRE(mat.at(3, 0) == 16);
            REQUIRE(mat.at(3, 1) == 17);
            REQUIRE(mat.at(3, 2) == 18);
            REQUIRE(mat.at(3, 3) == 19);
            REQUIRE(mat.at(3, 4) == 20);
            Vector<int, 5> row3{ init_list<int>{ 16, 17, 18, 19, 20}.begin() };
            REQUIRE(mat.row_vectors[3] == row3);
            REQUIRE(mat.column(3) == col3);
            // Row 4
            REQUIRE(mat.at(4, 0) == 21);
            REQUIRE(mat.at(4, 1) == 22);
            REQUIRE(mat.at(4, 2) == 23);
            REQUIRE(mat.at(4, 3) == 24);
            REQUIRE(mat.at(4, 4) == 25);
            Vector<int, 5> row4{ init_list<int>{ 21, 22, 24, 25}.begin() };
            REQUIRE(mat.row_vectors[3] == row3);
            REQUIRE(mat.column(4) == col4);
        }
    }
    SECTION("Fill constructor") {
        Matrix<int, 5, 5> mat{ -1 };
        Vector<int, 5> vec{ -1 };
        // Row 0
        REQUIRE(mat.at(0, 0) == -1);
        REQUIRE(mat.at(0, 1) == -1);
        REQUIRE(mat.at(0, 2) == -1);
        REQUIRE(mat.at(0, 3) == -1);
        REQUIRE(mat.at(0, 4) == -1);
        REQUIRE(mat.row_vectors[0] == vec);
        // Row 1
        REQUIRE(mat.at(1, 0) == -1);
        REQUIRE(mat.at(1, 1) == -1);
        REQUIRE(mat.at(1, 2) == -1);
        REQUIRE(mat.at(1, 3) == -1);
        REQUIRE(mat.at(1, 4) == -1);
        REQUIRE(mat.row_vectors[1] == vec);
        // Row 2
        REQUIRE(mat.at(2, 0) == -1);
        REQUIRE(mat.at(2, 1) == -1);
        REQUIRE(mat.at(2, 2) == -1);
        REQUIRE(mat.at(2, 3) == -1);
        REQUIRE(mat.at(2, 4) == -1);
        REQUIRE(mat.row_vectors[2] == vec);
        // Row 3
        REQUIRE(mat.at(3, 0) == -1);
        REQUIRE(mat.at(3, 1) == -1);
        REQUIRE(mat.at(3, 2) == -1);
        REQUIRE(mat.at(3, 3) == -1);
        REQUIRE(mat.at(3, 4) == -1);
        REQUIRE(mat.row_vectors[3] == vec);
        // Row 4
        REQUIRE(mat.at(4, 0) == -1);
        REQUIRE(mat.at(4, 1) == -1);
        REQUIRE(mat.at(4, 2) == -1);
        REQUIRE(mat.at(4, 3) == -1);
        REQUIRE(mat.at(4, 4) == -1);
        REQUIRE(mat.row_vectors[4] == vec);
        // Columns
        REQUIRE(mat.column(0) == vec);
        REQUIRE(mat.column(1) == vec);
        REQUIRE(mat.column(2) == vec);
        REQUIRE(mat.column(3) == vec);
        REQUIRE(mat.column(4) == vec);
    }
    SECTION("Array constructor") {
        const int arr[] = {
            0, 1, 2, 3, 4,
            5, 6, 7, 8, 9,
            10, 11, 12, 13, 14,
            15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25
        };
        Matrix<int, 5, 5> m1{arr};
        // Row 0
        REQUIRE(m1.at(0, 0) == 0);
        REQUIRE(m1.at(0, 1) == 1);
        REQUIRE(m1.at(0, 2) == 2);
        REQUIRE(m1.at(0, 3) == 3);
        REQUIRE(m1.at(0, 4) == 4);
        Vector<int, 5> row0{ arr };
        REQUIRE(m1.row_vectors[0] == row0);
        // Row 1
        REQUIRE(m1.at(1, 0) == 5);
        REQUIRE(m1.at(1, 1) == 6);
        REQUIRE(m1.at(1, 2) == 7);
        REQUIRE(m1.at(1, 3) == 8);
        REQUIRE(m1.at(1, 4) == 9);
        Vector<int, 5> row1{ arr + 5 };
        REQUIRE(m1.row_vectors[1] == row1);
        // Row 2
        REQUIRE(m1.at(2, 0) == 10);
        REQUIRE(m1.at(2, 1) == 11);
        REQUIRE(m1.at(2, 2) == 12);
        REQUIRE(m1.at(2, 3) == 13);
        REQUIRE(m1.at(2, 4) == 14);
        Vector<int, 5> row2{ arr + 10 };
        REQUIRE(m1.row_vectors[2] == row2);
        // Row 3
        REQUIRE(m1.at(3, 0) == 15);
        REQUIRE(m1.at(3, 1) == 16);
        REQUIRE(m1.at(3, 2) == 17);
        REQUIRE(m1.at(3, 3) == 18);
        REQUIRE(m1.at(3, 4) == 19);
        Vector<int, 5> row3{ arr + 15 };
        REQUIRE(m1.row_vectors[3] == row3);
        // Row 4
        REQUIRE(m1.at(4, 0) == 20);
        REQUIRE(m1.at(4, 1) == 21);
        REQUIRE(m1.at(4, 2) == 22);
        REQUIRE(m1.at(4, 3) == 23);
        REQUIRE(m1.at(4, 4) == 24);
        Vector<int, 5> row4{ arr + 20 };
        REQUIRE(m1.row_vectors[4] == row4);
        // Columns
        Vector<int, 5> col0{ init_list<int>{ 0, 5, 10, 15, 20 }.begin() },
            col1{ init_list<int>{ 1, 6, 11, 16, 21 }.begin() },
            col2{ init_list<int>{ 2, 7, 12, 17, 22 }.begin() },
            col3{ init_list<int>{ 3, 8, 13, 18, 23 }.begin() },
            col4{ init_list<int>{ 4, 9, 14, 19, 24 }.begin() };
        REQUIRE(m1.column(0) == col0);
        REQUIRE(m1.column(1) == col1);
        REQUIRE(m1.column(2) == col2);
        REQUIRE(m1.column(3) == col3);
        REQUIRE(m1.column(4) == col4);

        Matrix<int, 5, 5> m2{arr+1};
        // Row 0
        REQUIRE(m2.at(0, 0) == 1);
        REQUIRE(m2.at(0, 1) == 2);
        REQUIRE(m2.at(0, 2) == 3);
        REQUIRE(m2.at(0, 3) == 4);
        REQUIRE(m2.at(0, 4) == 5);
        row0 = Vector<int, 5>{ arr + 1 };
        REQUIRE(m2.row_vectors[0] == row0);
        // Row 1
        REQUIRE(m2.at(1, 0) == 6);
        REQUIRE(m2.at(1, 1) == 7);
        REQUIRE(m2.at(1, 2) == 8);
        REQUIRE(m2.at(1, 3) == 9);
        REQUIRE(m2.at(1, 4) == 10);
        row1 = Vector<int, 5>{ arr + 6 };
        REQUIRE(m2.row_vectors[1] == row1);
        // Row 2
        REQUIRE(m2.at(2, 0) == 11);
        REQUIRE(m2.at(2, 1) == 12);
        REQUIRE(m2.at(2, 2) == 13);
        REQUIRE(m2.at(2, 3) == 14);
        REQUIRE(m2.at(2, 4) == 15);
        row2 = Vector<int, 5>{ arr + 11 };
        REQUIRE(m2.row_vectors[2] == row2);
        // Row 3
        REQUIRE(m2.at(3, 0) == 16);
        REQUIRE(m2.at(3, 1) == 17);
        REQUIRE(m2.at(3, 2) == 18);
        REQUIRE(m2.at(3, 3) == 19);
        REQUIRE(m2.at(3, 4) == 20);
        row3 = Vector<int, 5>{ arr + 16 };
        REQUIRE(m2.row_vectors[3] == row3);
        // Row 4
        REQUIRE(m2.at(4, 0) == 21);
        REQUIRE(m2.at(4, 1) == 22);
        REQUIRE(m2.at(4, 2) == 23);
        REQUIRE(m2.at(4, 3) == 24);
        REQUIRE(m2.at(4, 4) == 25);
        row4 = Vector<int, 5>{ arr + 21 };
        REQUIRE(m2.row_vectors[4] == row4);
    }
    SECTION("Casting constructors") {
        Matrix<int, 5, 5> mat{ Matrix<float, 5, 5>(-3.0f) };
        Vector<int, 5> vec{ -3 };
        // Row 0
        REQUIRE(mat.at(0, 0) == -3);
        REQUIRE(mat.at(0, 1) == -3);
        REQUIRE(mat.at(0, 2) == -3);
        REQUIRE(mat.at(0, 3) == -3);
        REQUIRE(mat.at(0, 4) == -3);
        REQUIRE(mat.row_vectors[0] == vec);
        // Row 1
        REQUIRE(mat.at(1, 0) == -3);
        REQUIRE(mat.at(1, 1) == -3);
        REQUIRE(mat.at(1, 2) == -3);
        REQUIRE(mat.at(1, 3) == -3);
        REQUIRE(mat.at(1, 4) == -3);
        REQUIRE(mat.row_vectors[1] == vec);
        // Row 2
        REQUIRE(mat.at(2, 0) == -3);
        REQUIRE(mat.at(2, 1) == -3);
        REQUIRE(mat.at(2, 2) == -3);
        REQUIRE(mat.at(2, 3) == -3);
        REQUIRE(mat.at(2, 4) == -3);
        REQUIRE(mat.row_vectors[2] == vec);
        // Row 3
        REQUIRE(mat.at(3, 0) == -3);
        REQUIRE(mat.at(3, 1) == -3);
        REQUIRE(mat.at(3, 2) == -3);
        REQUIRE(mat.at(3, 3) == -3);
        REQUIRE(mat.at(3, 4) == -3);
        REQUIRE(mat.row_vectors[3] == vec);
        // Row 4
        REQUIRE(mat.at(4, 0) == -3);
        REQUIRE(mat.at(4, 1) == -3);
        REQUIRE(mat.at(4, 2) == -3);
        REQUIRE(mat.at(4, 3) == -3);
        REQUIRE(mat.at(4, 4) == -3);
        REQUIRE(mat.row_vectors[4] == vec);
    }
    SECTION("Identity matrix") {
        Matrix<float, 5, 5> id5 = Matrix<float, 5, 5>::identity();
        // Row 0
        REQUIRE(id5.at(0, 0) == 1.0f);
        REQUIRE(id5.at(0, 1) == 0.0f);
        REQUIRE(id5.at(0, 2) == 0.0f);
        REQUIRE(id5.at(0, 3) == 0.0f);
        REQUIRE(id5.at(0, 4) == 0.0f);
        Vector<float, 5> row0{ 0.0f }; row0[0] = 1.0f;
        REQUIRE(id5.row_vectors[0] == row0);
        // Row 1
        REQUIRE(id5.at(1, 0) == 0.0f);
        REQUIRE(id5.at(1, 1) == 1.0f);
        REQUIRE(id5.at(1, 2) == 0.0f);
        REQUIRE(id5.at(1, 3) == 0.0f);
        REQUIRE(id5.at(1, 4) == 0.0f);
        Vector<float, 5> row1{ 0.0f }; row1[1] = 1.0f;
        REQUIRE(id5.row_vectors[1] == row1);
        // Row 2
        REQUIRE(id5.at(2, 0) == 0.0f);
        REQUIRE(id5.at(2, 1) == 0.0f);
        REQUIRE(id5.at(2, 2) == 1.0f);
        REQUIRE(id5.at(2, 3) == 0.0f);
        REQUIRE(id5.at(2, 4) == 0.0f);
        Vector<float, 5> row2{ 0.0f }; row2[2] = 1.0f;
        REQUIRE(id5.row_vectors[2] == row2);
        // Row 3
        REQUIRE(id5.at(3, 0) == 0.0f);
        REQUIRE(id5.at(3, 1) == 0.0f);
        REQUIRE(id5.at(3, 2) == 0.0f);
        REQUIRE(id5.at(3, 3) == 1.0f);
        REQUIRE(id5.at(3, 4) == 0.0f);
        Vector<float, 5> row3{ 0.0f }; row3[3] = 1.0f;
        REQUIRE(id5.row_vectors[3] == row3);
        // Row 4
        REQUIRE(id5.at(4, 0) == 0.0f);
        REQUIRE(id5.at(4, 1) == 0.0f);
        REQUIRE(id5.at(4, 2) == 0.0f);
        REQUIRE(id5.at(4, 3) == 0.0f);
        REQUIRE(id5.at(4, 4) == 1.0f);
        Vector<float, 5> row4{ 0.0f }; row4[4] = 1.0f;
        REQUIRE(id5.row_vectors[4] == row4);
        // Columns
        // Comparing with row0, row1, etc. since identity matrices are 
        // symmetric and it's less to write for vectors of dimension > 4
        REQUIRE(id5.column(0) == row0);
        REQUIRE(id5.column(1) == row1);
        REQUIRE(id5.column(2) == row2);
        REQUIRE(id5.column(3) == row3);
        REQUIRE(id5.column(4) == row4);
    }
}

TEST_CASE("Matrix arithmetics", "[sini::Matrix]")
{
    mat2i m1{ { 4, 2 }, { 16, 32 } },
        m2{ { -1, 3 }, { 4, -8 } };
    // Check m1
    REQUIRE(m1(0, 0) == 4);
    REQUIRE(m1(0, 1) == 2);
    REQUIRE(m1(1, 0) == 16);
    REQUIRE(m1(1, 1) == 32);
    // Check m2
    REQUIRE(m2(0, 0) == -1);
    REQUIRE(m2(0, 1) == 3);
    REQUIRE(m2(1, 0) == 4);
    REQUIRE(m2(1, 1) == -8);

    SECTION("Addition") {
        mat2i m3 = m1 + m2;
        REQUIRE(m3(0, 0) == 3);
        REQUIRE(m3(0, 1) == 5);
        REQUIRE(m3(1, 0) == 20);
        REQUIRE(m3(1, 1) == 24);
        // Check integrity of m1
        REQUIRE(m1(0, 0) == 4);
        REQUIRE(m1(0, 1) == 2);
        REQUIRE(m1(1, 0) == 16);
        REQUIRE(m1(1, 1) == 32);
        // Check integrity of m2
        REQUIRE(m2(0, 0) == -1);
        REQUIRE(m2(0, 1) == 3);
        REQUIRE(m2(1, 0) == 4);
        REQUIRE(m2(1, 1) == -8);
    }
    SECTION("Addition assignment") {
        m1 += m2;
        REQUIRE(m1(0, 0) == 3);
        REQUIRE(m1(0, 1) == 5);
        REQUIRE(m1(1, 0) == 20);
        REQUIRE(m1(1, 1) == 24);
        // Check integrity of m2
        REQUIRE(m2(0, 0) == -1);
        REQUIRE(m2(0, 1) == 3);
        REQUIRE(m2(1, 0) == 4);
        REQUIRE(m2(1, 1) == -8);
    }
    SECTION("Subtraction") {
        mat2i m3 = m1 - m2;
        REQUIRE(m3(0, 0) == 5);
        REQUIRE(m3(0, 1) == -1);
        REQUIRE(m3(1, 0) == 12);
        REQUIRE(m3(1, 1) == 40);
        // Check integrity of m1
        REQUIRE(m1(0, 0) == 4);
        REQUIRE(m1(0, 1) == 2);
        REQUIRE(m1(1, 0) == 16);
        REQUIRE(m1(1, 1) == 32);
        // Check integrity of m2
        REQUIRE(m2(0, 0) == -1);
        REQUIRE(m2(0, 1) == 3);
        REQUIRE(m2(1, 0) == 4);
        REQUIRE(m2(1, 1) == -8);
    }
    SECTION("Subtraction assignment") {
        m1 -= m2;
        REQUIRE(m1(0, 0) == 5);
        REQUIRE(m1(0, 1) == -1);
        REQUIRE(m1(1, 0) == 12);
        REQUIRE(m1(1, 1) == 40);
        // Check integrity of m2
        REQUIRE(m2(0, 0) == -1);
        REQUIRE(m2(0, 1) == 3);
        REQUIRE(m2(1, 0) == 4);
        REQUIRE(m2(1, 1) == -8);
    }
    SECTION("Negation") {
        mat2i m3 = -m2;
        REQUIRE(m3(0, 0) == 1);
        REQUIRE(m3(0, 1) == -3);
        REQUIRE(m3(1, 0) == -4);
        REQUIRE(m3(1, 1) == 8);
        // Check integrity of m2
        REQUIRE(m2(0, 0) == -1);
        REQUIRE(m2(0, 1) == 3);
        REQUIRE(m2(1, 0) == 4);
        REQUIRE(m2(1, 1) == -8);
    }
    SECTION("Multiplication with scalar") {
        mat2i m3 = 2 * m2;
        REQUIRE(m3(0, 0) == -2);
        REQUIRE(m3(0, 1) == 6);
        REQUIRE(m3(1, 0) == 8);
        REQUIRE(m3(1, 1) == -16);
        // Check integrity of m2
        REQUIRE(m2(0, 0) == -1);
        REQUIRE(m2(0, 1) == 3);
        REQUIRE(m2(1, 0) == 4);
        REQUIRE(m2(1, 1) == -8);
    }
    SECTION("Scalar multiplication assignment") {
        m2 *= -2;
        REQUIRE(m2(0, 0) == 2);
        REQUIRE(m2(0, 1) == -6);
        REQUIRE(m2(1, 0) == -8);
        REQUIRE(m2(1, 1) == 16);
    }
    SECTION("Division with scalar") {
        mat2i mat{ { 3, 6 }, { 9, 12 } };
        mat2i mat2 = mat / 3;
        REQUIRE(mat2(0, 0) == 1);
        REQUIRE(mat2(0, 1) == 2);
        REQUIRE(mat2(1, 0) == 3);
        REQUIRE(mat2(1, 1) == 4);
        // Check integrity of mat
        REQUIRE(mat(0, 0) == 3);
        REQUIRE(mat(0, 1) == 6);
        REQUIRE(mat(1, 0) == 9);
        REQUIRE(mat(1, 1) == 12);
    }
    SECTION("Scalar division assignment") {
        mat2i mat{ { 3, 6 }, { 9, 12 } };
        mat /= 3;
        REQUIRE(mat(0, 0) == 1);
        REQUIRE(mat(0, 1) == 2);
        REQUIRE(mat(1, 0) == 3);
        REQUIRE(mat(1, 1) == 4);
    }
    SECTION("Element-wise multiplication") {
        mat2i mat1{ { 1, -2 }, { -3, 4 } },
            mat2{ { 3, 5 }, { 7, 11 } };
        // Check mat1
        REQUIRE(mat1(0, 0) == 1);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == -3);
        REQUIRE(mat1(1, 1) == 4);
        // Check mat2
        REQUIRE(mat2(0, 0) == 3);
        REQUIRE(mat2(0, 1) == 5);
        REQUIRE(mat2(1, 0) == 7);
        REQUIRE(mat2(1, 1) == 11);

        mat2i mat3 = elemMult(mat1, mat2);
        REQUIRE(mat3(0, 0) == 3);
        REQUIRE(mat3(0, 1) == -10);
        REQUIRE(mat3(1, 0) == -21);
        REQUIRE(mat3(1, 1) == 44);

        // Check integrity of mat1
        REQUIRE(mat1(0, 0) == 1);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == -3);
        REQUIRE(mat1(1, 1) == 4);
        // Check integrity of mat2
        REQUIRE(mat2(0, 0) == 3);
        REQUIRE(mat2(0, 1) == 5);
        REQUIRE(mat2(1, 0) == 7);
        REQUIRE(mat2(1, 1) == 11);
    }
    SECTION("Element-wise multiplication assignment") {
        mat2i mat1{ { 1, -2 },{ -3, 4 } },
            mat2{ { 3, 5 },{ 7, 11 } };
        eqElemMult(mat1, mat2);
        REQUIRE(mat1(0, 0) == 3);
        REQUIRE(mat1(0, 1) == -10);
        REQUIRE(mat1(1, 0) == -21);
        REQUIRE(mat1(1, 1) == 44);
        // Check integrity of mat2
        REQUIRE(mat2(0, 0) == 3);
        REQUIRE(mat2(0, 1) == 5);
        REQUIRE(mat2(1, 0) == 7);
        REQUIRE(mat2(1, 1) == 11);
    }
    SECTION("Element-wise division") {
        mat2i mat1{ { 16, 8 }, { 32, 256 } },
            mat2{ { 4, 2 }, { 16, 32 } };
        // Check mat1
        REQUIRE(mat1(0, 0) == 16);
        REQUIRE(mat1(0, 1) == 8);
        REQUIRE(mat1(1, 0) == 32);
        REQUIRE(mat1(1, 1) == 256);
        // Check mat2
        REQUIRE(mat2(0, 0) == 4);
        REQUIRE(mat2(0, 1) == 2);
        REQUIRE(mat2(1, 0) == 16);
        REQUIRE(mat2(1, 1) == 32);

        mat2i mat3 = elemDiv(mat1, mat2);
        REQUIRE(mat3(0, 0) == 4);
        REQUIRE(mat3(0, 1) == 4);
        REQUIRE(mat3(1, 0) == 2);
        REQUIRE(mat3(1, 1) == 8);
        // Check integrity of mat1
        REQUIRE(mat1(0, 0) == 16);
        REQUIRE(mat1(0, 1) == 8);
        REQUIRE(mat1(1, 0) == 32);
        REQUIRE(mat1(1, 1) == 256);
        // Check integrity of mat2
        REQUIRE(mat2(0, 0) == 4);
        REQUIRE(mat2(0, 1) == 2);
        REQUIRE(mat2(1, 0) == 16);
        REQUIRE(mat2(1, 1) == 32);
    }
    SECTION("Element-wise division assignment") {
        mat2i mat1{ { 16, 8 },{ 32, 256 } },
            mat2{ { 4, 2 },{ 16, 32 } };
        // Check mat1
        REQUIRE(mat1(0, 0) == 16);
        REQUIRE(mat1(0, 1) == 8);
        REQUIRE(mat1(1, 0) == 32);
        REQUIRE(mat1(1, 1) == 256);
        // Check mat2
        REQUIRE(mat2(0, 0) == 4);
        REQUIRE(mat2(0, 1) == 2);
        REQUIRE(mat2(1, 0) == 16);
        REQUIRE(mat2(1, 1) == 32);

        eqElemDiv(mat1, mat2);
        REQUIRE(mat1(0, 0) == 4);
        REQUIRE(mat1(0, 1) == 4);
        REQUIRE(mat1(1, 0) == 2);
        REQUIRE(mat1(1, 1) == 8);
        // Check integrity of mat2
        REQUIRE(mat2(0, 0) == 4);
        REQUIRE(mat2(0, 1) == 2);
        REQUIRE(mat2(1, 0) == 16);
        REQUIRE(mat2(1, 1) == 32);
    }
    SECTION("Matrix multiplication") {
        mat2i id = mat2i::identity(),
            mat1{ { 4, -2 }, { 1, 3 } };
        Matrix<int32_t, 2, 3> mat2{ init_list<int32_t>{
            2, 3, 1,
            1, 5, 7
        }.begin() };
        // Check mat1
        REQUIRE(mat1(0, 0) == 4);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == 1);
        REQUIRE(mat1(1, 1) == 3);
        // Check mat2
        REQUIRE(mat2(0, 0) == 2);
        REQUIRE(mat2(0, 1) == 3);
        REQUIRE(mat2(0, 2) == 1);
        REQUIRE(mat2(1, 0) == 1);
        REQUIRE(mat2(1, 1) == 5);
        REQUIRE(mat2(1, 2) == 7);

        // Multiplication with the identity matrix shouldn't change anything,
        // whether multiplied from left or right
        mat2i mat3 = mat1 * id;
        REQUIRE(mat1(0, 0) == 4);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == 1);
        REQUIRE(mat1(1, 1) == 3);
        mat3 = id * mat1;
        REQUIRE(mat1(0, 0) == 4);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == 1);
        REQUIRE(mat1(1, 1) == 3);

        // More general matrix multiplication (2x2 x 2x3 -> 2x3)
        auto mat4 = mat1 * mat2;
        REQUIRE(mat4(0, 0) == 6);
        REQUIRE(mat4(0, 1) == 2);
        REQUIRE(mat4(0, 2) == -10);
        REQUIRE(mat4(1, 0) == 5);
        REQUIRE(mat4(1, 1) == 18);
        REQUIRE(mat4(1, 2) == 22);
    }
    SECTION("Matrix multiplication assignment") {
        mat2i id = mat2i::identity(),
            mat1{ { 4, -2 },{ 1, 3 } },
            mat2{ { 2, 3 }, { 5, 7 } };
        // Check mat1
        REQUIRE(mat1(0, 0) == 4);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == 1);
        REQUIRE(mat1(1, 1) == 3);
        // Check mat2
        REQUIRE(mat2(0, 0) == 2);
        REQUIRE(mat2(0, 1) == 3);
        REQUIRE(mat2(1, 0) == 5);
        REQUIRE(mat2(1, 1) == 7);

        mat1 *= id;
        REQUIRE(mat1(0, 0) == 4);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == 1);
        REQUIRE(mat1(1, 1) == 3);

        mat1 *= mat2;
        REQUIRE(mat1(0, 0) == -2);
        REQUIRE(mat1(0, 1) == -2);
        REQUIRE(mat1(1, 0) == 17);
        REQUIRE(mat1(1, 1) == 24);
    }
    SECTION("Matrix-vector multiplication") {
        Matrix<int32_t, 2, 3> mat{ init_list<int32_t>{
            1, -2, 3,
            -1, 0, 0
        }.begin() };
        vec3i vec{ 3, 2, 1 };
        // Check mat
        REQUIRE(mat(0, 0) == 1);
        REQUIRE(mat(0, 1) == -2);
        REQUIRE(mat(0, 2) == 3);
        REQUIRE(mat(1, 0) == -1);
        REQUIRE(mat(1, 1) == 0);
        REQUIRE(mat(1, 2) == 0);
        // Check vec
        REQUIRE(vec[0] == 3);
        REQUIRE(vec[1] == 2);
        REQUIRE(vec[2] == 1);

        vec2i result = mat * vec;
        REQUIRE(result[0] == 2);
        REQUIRE(result[1] == -3);
    }
}

TEST_CASE("Matrix comparisons", "[sini::Matrix]")
{
    mat2i mat1{ { 1, 2 }, { 3, 4 } },
        mat2{ { -1, -2 }, {-3, -4 } },
        mat3{ { 1, 2 }, { 3, 5 } };
    REQUIRE(mat1 == mat1);
    REQUIRE(mat1 != mat2);
    REQUIRE(mat1 == -mat2);
    REQUIRE(mat1 != mat3);
}

TEST_CASE("Matrix-vector conversion", "[sini::Matrix]")
{
    SECTION("Column to row vector (vector to 1xN matrix)") {
        vec3i vec{ 3, 5, 7 };
        auto mat = toRowVector(vec);
        REQUIRE(mat(0, 0) == 3);
        REQUIRE(mat(0, 1) == 5);
        REQUIRE(mat(0, 2) == 7);
    }
    SECTION("Row to column vector (1xN matrix to vector)") {
        Matrix<int32_t, 1, 3> mat{ init_list<int32_t>{ 1, -2, 3 }.begin() };
        vec3i vec = toColumnVector(mat);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == -2);
        REQUIRE(vec[2] == 3);
    }
}

TEST_CASE("Transpose", "[sini::Matrix]")
{
    mat3i mat{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    mat = transpose(mat);
    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(0, 1) == 4);
    REQUIRE(mat(0, 2) == 7);
    REQUIRE(mat(1, 0) == 2);
    REQUIRE(mat(1, 1) == 5);
    REQUIRE(mat(1, 2) == 8);
    REQUIRE(mat(2, 0) == 3);
    REQUIRE(mat(2, 1) == 6);
    REQUIRE(mat(2, 2) == 9);
}

TEST_CASE("Pow (Matrix)", "[sini::Matrix]")
{
    mat3i mat{
        {1, 0, 3},
        {-2, 1, 0},
        {0, 0, 2}
    };
    // Or check individual elements? To make independent from matrix comparisons?
    REQUIRE(pow(mat, 0) == mat3i::identity());
    REQUIRE(pow(mat, 1) == mat);
    REQUIRE(pow(mat, 2) == mat*mat);
    REQUIRE(pow(mat, 3) == mat*mat*mat);
}

TEST_CASE("Determinant", "[sini::Matrix]")
{
    // This could probably do with some better testing, but I'm leaving that
    // for the future and hope that this is enough for now
    // TODO

    // Correctness for 2x2 matrix
    mat2i m1{ {1, 2}, {3, -4} };
    REQUIRE(det(m1) == -10);
    // Correctness for 3x3 matrix
    // This matrix should have the same determinant
    mat3i m2{
        {1, 0, 0},
        {0, 1, 2},
        {0, 3, -4}
    };
    REQUIRE(det(m2) == -10);
    REQUIRE(det(m2) == det(m1));
    // Correctness for 4x4 matrix
    mat4i m3{
        {-1, 2, 3, 4},
        {5, -6, 7, 8},
        {9, 10, -11, 12},
        {13, 14, 15, -16}
    };
    // According to WolframAlpha.com the determinant is -36 416
    REQUIRE(det(m3) == -36416);

    mat3i m4{
        {0, 1, 2},
        {3, 4, -5}, 
        {0, 2, 4} // linearly dependent of the other (the first) rows
    };
    REQUIRE(det(m4) == 0);
}

TEST_CASE("Adjugate matrix", "[sini::Matrix]")
{
    // Correctness tests
    REQUIRE(adj(mat2i::identity()) == mat2i::identity());
    // 2x2 matrix
    mat2i m1{
        {1, 2},
        {3, 4}
    }, m1_adj{
        {4, -3},
        {-2, 1}
    };
    REQUIRE(adj(m1) == m1_adj);
    // 3x3 matrix
    mat3i m2{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    }, m2_adj{ // According to WolframAlpha.com
        {-3, 6, -3},
        {6, -12, 6},
        {-3, 6, -3}
    };
    REQUIRE(adj(m2) == m2_adj);
    // 4x4 matrix
    mat4i m3{
        {0, -1, 2, 3},
        {-4, 5, -6, 7},
        {8, -9, 10, -11},
        {12, 13, -14, 15}
    }, m3_adj{ // According to WolframAlpha.com
        {0, 24, 48, 24},
        {96, -1200, -672, 48},
        {192, -888, -480, 24},
        {96, 192, 96, 0}
    };
    REQUIRE(adj(m3) == m3_adj);
}

TEST_CASE("Inverse matrix", "[sini::Matrix]")
{
    // Correctness tests
    // This could probably use better testing as well, but I hope that a few
    // cases are enough to catch any bugs in the code
    float tol = 1e-10f;
    // Make sure that identity matrices are their own inverse
    REQUIRE( approxEqual(inverse(mat2::identity()), mat2::identity(), tol) );
    REQUIRE( approxEqual(inverse(mat3::identity()), mat3::identity(), tol) );
    REQUIRE( approxEqual(inverse(mat4::identity()), mat4::identity(), tol) );
    // More general 2x2 matrix
    mat2 m1{
        { 1.0f, -1.0f },
        { 2.0f, 2.0f }
    }, m1_inv{
        { 0.5f, 0.25f },
        { -0.5f, 0.25f }
    };
    REQUIRE( approxEqual(inverse(m1), m1_inv, tol) );
    // More general 3x3 matrix
    mat3 m2{
        { 1.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f, 2.0f },
        { -1.0f, 2.0f, 3.0f }
    }, m2_inv{ // According to WolframAlpha.com
        { 0.5f, 1.0f, -0.5f },
        { 1.0f, -1.0f, 1.0f },
        { -0.5f, 1.0f, -0.5f }
    };
    REQUIRE( approxEqual(inverse(m2), m2_inv, tol) );
}

TEST_CASE("Matrix Abs", "[sini::Matrix]")
{
    mat3i mat{
        { 1, -2, -3 },
        { -4, 5, 6 },
        { 7, 8, 9 }
    };
    mat = abs(mat);
    REQUIRE(mat(0, 0) == 1);
    REQUIRE(mat(0, 1) == 2);
    REQUIRE(mat(0, 2) == 3);
    REQUIRE(mat(1, 0) == 4);
    REQUIRE(mat(1, 1) == 5);
    REQUIRE(mat(1, 2) == 6);
    REQUIRE(mat(2, 0) == 7);
    REQUIRE(mat(2, 1) == 8);
    REQUIRE(mat(2, 2) == 9);
}

TEST_CASE("Matrix max and min element", "[sini::Matrix]")
{
    mat3i mat{
        { 1, 2, -3 },
        { -16, 8, 0 },
        { 3, 5, 7 }
    };
    int32_t max = maxElement(mat);
    REQUIRE(max == 8);
    int32_t min = minElement(mat);
    REQUIRE(min == -16);
}
