#include "math/Matrix.hpp"
#include "math/MatrixUtilities.hpp"

#include <catch.hpp>

#include <initializer_list>


using namespace sini;

void verifyInitialization(mat2i mat, const vec2i (&expected_rows)[2])
{
    REQUIRE(mat.a == expected_rows[0][0]);
    REQUIRE(mat.b == expected_rows[0][1]);
    REQUIRE(mat.c == expected_rows[1][0]);
    REQUIRE(mat.d == expected_rows[1][1]);

    REQUIRE(mat.e00 == expected_rows[0][0]);
    REQUIRE(mat.e01 == expected_rows[0][1]);
    REQUIRE(mat.e10 == expected_rows[1][0]);
    REQUIRE(mat.e11 == expected_rows[1][1]);

    REQUIRE(mat.row_vectors[0] == expected_rows[0]);
    REQUIRE(mat.row_vectors[1] == expected_rows[1]);
}

void verifyInitialization(mat3i mat, const vec3i (&expected_rows)[3])
{
    REQUIRE(mat.a == expected_rows[0][0]);
    REQUIRE(mat.b == expected_rows[0][1]);
    REQUIRE(mat.c == expected_rows[0][2]);
    REQUIRE(mat.d == expected_rows[1][0]);
    REQUIRE(mat.e == expected_rows[1][1]);
    REQUIRE(mat.f == expected_rows[1][2]);
    REQUIRE(mat.g == expected_rows[2][0]);
    REQUIRE(mat.h == expected_rows[2][1]);
    REQUIRE(mat.i == expected_rows[2][2]);

    REQUIRE(mat.e00 == expected_rows[0][0]);
    REQUIRE(mat.e01 == expected_rows[0][1]);
    REQUIRE(mat.e02 == expected_rows[0][2]);
    REQUIRE(mat.e10 == expected_rows[1][0]);
    REQUIRE(mat.e11 == expected_rows[1][1]);
    REQUIRE(mat.e12 == expected_rows[1][2]);
    REQUIRE(mat.e20 == expected_rows[2][0]);
    REQUIRE(mat.e21 == expected_rows[2][1]);
    REQUIRE(mat.e22 == expected_rows[2][2]);

    REQUIRE(mat.row_vectors[0] == expected_rows[0]);
    REQUIRE(mat.row_vectors[1] == expected_rows[1]);
    REQUIRE(mat.row_vectors[2] == expected_rows[2]);
}

void verifyInitialization(mat4i mat, const vec4i (&expected_rows)[4])
{
    REQUIRE(mat.e00 == expected_rows[0][0]);
    REQUIRE(mat.e01 == expected_rows[0][1]);
    REQUIRE(mat.e02 == expected_rows[0][2]);
    REQUIRE(mat.e03 == expected_rows[0][3]);
    REQUIRE(mat.row_vectors[0] == expected_rows[0]);

    REQUIRE(mat.e10 == expected_rows[1][0]);
    REQUIRE(mat.e11 == expected_rows[1][1]);
    REQUIRE(mat.e12 == expected_rows[1][2]);
    REQUIRE(mat.e13 == expected_rows[1][3]);
    REQUIRE(mat.row_vectors[1] == expected_rows[1]);

    REQUIRE(mat.e20 == expected_rows[2][0]);
    REQUIRE(mat.e21 == expected_rows[2][1]);
    REQUIRE(mat.e22 == expected_rows[2][2]);
    REQUIRE(mat.e23 == expected_rows[2][3]);
    REQUIRE(mat.row_vectors[2] == expected_rows[2]);

    REQUIRE(mat.e30 == expected_rows[3][0]);
    REQUIRE(mat.e31 == expected_rows[3][1]);
    REQUIRE(mat.e32 == expected_rows[3][2]);
    REQUIRE(mat.e33 == expected_rows[3][3]);
    REQUIRE(mat.row_vectors[3] == expected_rows[3]);
}

TEST_CASE("2x2 matrix specialization", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,2,2>) == 4 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        mat2i mat{ 3 };
        vec2i expected[]{
            { 3, 3 },
            { 3, 3 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[]{ 4, 3, 2, 1, -1 };
        mat2i m1{ arr };
        vec2i expected[]{
            { 4, 3 },
            { 2, 1 }
        };
        verifyInitialization(m1, expected);

        mat2i m2{ arr + 1 };
        expected[0] = { 3,  2 };
        expected[1] = { 1, -1 };
        verifyInitialization(m2, expected);
    }
    SECTION("Vector array constructor") {
        mat2i mat{{
            { 1, 2 },
            { 3, 4 }
        }};
        vec2i expected[]{
            { 1, 2 },
            { 3, 4 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Initialize from row vectors") {
       mat2i mat{
            { 2, 3 },
            { 5, 7 }
        };
       vec2i expected[]{
           { 2, 3 },
           { 5, 7 }
       };
       verifyInitialization(mat, expected);
    }
    SECTION("Casting constructor") {
        mat2i mat(mat2{
            { -1.0f,  1.0f },
            {  2.0f, -2.0f }
        });
        vec2i expected[]{
            { -1,  1 },
            {  2, -2 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Element access") {
        mat2i mat;
        SECTION("at()") {
            SECTION("getting values") {
                mat = {
                    { 1, 2 },
                    { 3, 4 }
                };

                REQUIRE(mat.at(0, 0) == 1);
                REQUIRE(mat.at(0, 1) == 2);
                REQUIRE(mat.at(1, 0) == 3);
                REQUIRE(mat.at(1, 1) == 4);
            }
            SECTION("setting values") {
                mat.at(0, 0) = 4;
                mat.at(0, 1) = 3;
                mat.at(1, 0) = 2;
                mat.at(1, 1) = 1;

                REQUIRE(mat.row_vectors[0] == vec2i(4, 3));
                REQUIRE(mat.row_vectors[1] == vec2i(2, 1));
            }
        }
        SECTION("() operator") {
            SECTION("getting values") {
                mat = {
                    { 1, 2 },
                    { 3, 4 }
                };

                REQUIRE(mat(0, 0) == 1);
                REQUIRE(mat(0, 1) == 2);
                REQUIRE(mat(1, 0) == 3);
                REQUIRE(mat(1, 1) == 4);
            }
            SECTION("setting values") {
                mat(0, 0) = 4;
                mat(0, 1) = 3;
                mat(1, 0) = 2;
                mat(1, 1) = 1;

                REQUIRE(mat.row_vectors[0] == vec2i(4, 3));
                REQUIRE(mat.row_vectors[1] == vec2i(2, 1));
            }
        }
        SECTION("Data pointer") {
            mat.data()[0] = 1;
            mat.data()[1] = 2;
            mat.data()[2] = 3;
            mat.data()[3] = 4;

            REQUIRE(mat.row_vectors[0] == vec2i(1, 2));
            REQUIRE(mat.row_vectors[1] == vec2i(3, 4));
        }
        SECTION("set()") {
            mat.set(0, 0, 1);
            mat.set(0, 1, 2);
            mat.set(1, 0, 3);
            mat.set(1, 1, 4);

            REQUIRE(mat.row_vectors[0] == vec2i(1, 2));
            REQUIRE(mat.row_vectors[1] == vec2i(3, 4));
        }
        SECTION("column()") {
            mat = {
                { 1, 2 },
                { 3, 4 }
            };

            REQUIRE(mat.column(0) == vec2i(1, 3));
            REQUIRE(mat.column(1) == vec2i(2, 4));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, vec2i(4, 2));
            mat.setColumn(1, vec2i(3, 1));

            REQUIRE(mat.row_vectors[0] == vec2i(4, 3));
            REQUIRE(mat.row_vectors[1] == vec2i(2, 1));
        }
    }
    SECTION("Identity matrix") {
        mat2 id = mat2::identity();

        REQUIRE(id.row_vectors[0] == vec2(1.0f, 0.0f));
        REQUIRE(id.row_vectors[1] == vec2(0.0f, 1.0f));
    }
}

TEST_CASE("3x3 matrix specialization", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,3,3>) == 9 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        mat3i mat{ 7 };
        vec3i expected[]{
            { 7, 7, 7 },
            { 7, 7, 7 },
            { 7, 7, 7 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[]{ -1, 2, -3, 4, -5, 6, -7, 8, -9, 10 };
        mat3i m1{ arr };
        vec3i expected[]{
            { -1,  2, -3 },
            {  4, -5,  6 },
            { -7,  8, -9 }
        };
        verifyInitialization(m1, expected);

        mat3i m2{ arr + 1 };
        expected[0] = {  2, -3,  4 };
        expected[1] = { -5,  6, -7 };
        expected[2] = {  8, -9, 10 };
        verifyInitialization(m2, expected);
    }
    SECTION("Vector array constructor") {
        mat3i mat{{
            { 0, 1, 2 },
            { 3, 4, 5 },
            { 6, 7, 8 }
        }};
        vec3i expected[]{
            { 0, 1, 2 },
            { 3, 4, 5 },
            { 6, 7, 8 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Initialize from row vectors") {
        mat3i mat{
            { -9, -8, -7 },
            { -6, -5, -4 },
            { -3, -2, -1 }
        };
        vec3i expected[]{
            { -9, -8, -7 },
            { -6, -5, -4 },
            { -3, -2, -1 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Casting constructor") {
        mat3i mat{ mat3{ -3.0f } };
        vec3i expected[]{
            { -3, -3, -3 },
            { -3, -3, -3 },
            { -3, -3, -3 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Element access") {
        mat3i mat;
        SECTION("at()") {
            SECTION("getting values") {
                mat = {
                    { 1, 2, 3 },
                    { 4, 5, 6 },
                    { 7, 8, 9 }
                };

                REQUIRE(mat.at(0, 0) == 1);
                REQUIRE(mat.at(0, 1) == 2);
                REQUIRE(mat.at(0, 2) == 3);
                REQUIRE(mat.at(1, 0) == 4);
                REQUIRE(mat.at(1, 1) == 5);
                REQUIRE(mat.at(1, 2) == 6);
                REQUIRE(mat.at(2, 0) == 7);
                REQUIRE(mat.at(2, 1) == 8);
                REQUIRE(mat.at(2, 2) == 9);
            }
            SECTION("setting values") {
                mat.at(0, 0) = 1; mat.at(0, 1) = 2; mat.at(0, 2) = 3;
                mat.at(1, 0) = 4; mat.at(1, 1) = 5; mat.at(1, 2) = 6;
                mat.at(2, 0) = 7; mat.at(2, 1) = 8; mat.at(2, 2) = 9;

                REQUIRE(mat.row_vectors[0] == vec3i(1, 2, 3));
                REQUIRE(mat.row_vectors[1] == vec3i(4, 5, 6));
                REQUIRE(mat.row_vectors[2] == vec3i(7, 8, 9));
            }
        }
        SECTION("() operator") {
            SECTION("getting values") {
                mat = {
                    { -1, -2, -3 },
                    { -4, -5, -6 },
                    { -7, -8, -9 }
                };

                REQUIRE(mat(0, 0) == -1);
                REQUIRE(mat(0, 1) == -2);
                REQUIRE(mat(0, 2) == -3);
                REQUIRE(mat(1, 0) == -4);
                REQUIRE(mat(1, 1) == -5);
                REQUIRE(mat(1, 2) == -6);
                REQUIRE(mat(2, 0) == -7);
                REQUIRE(mat(2, 1) == -8);
                REQUIRE(mat(2, 2) == -9);
            }
            SECTION("setting values") {
                mat(0, 0) = -1; mat(0, 1) = -2; mat(0, 2) = -3;
                mat(1, 0) = -4; mat(1, 1) = -5; mat(1, 2) = -6;
                mat(2, 0) = -7; mat(2, 1) = -8; mat(2, 2) = -9;

                REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
                REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
                REQUIRE(mat.row_vectors[2] == vec3i(-7, -8, -9));
            }
        }
        SECTION("Data pointer") {
            mat.data()[0] = 1; mat.data()[1] = 2; mat.data()[2] = 3;
            mat.data()[3] = 4; mat.data()[4] = 5; mat.data()[5] = 6;
            mat.data()[6] = 7; mat.data()[7] = 8; mat.data()[8] = 9;

            REQUIRE(mat.row_vectors[0] == vec3i(1, 2, 3));
            REQUIRE(mat.row_vectors[1] == vec3i(4, 5, 6));
            REQUIRE(mat.row_vectors[2] == vec3i(7, 8, 9));
        }
        SECTION("set()") {
            mat.set(0, 0, 0); mat.set(0, 1, 1); mat.set(0, 2, 2);
            mat.set(1, 0, 3); mat.set(1, 1, 4); mat.set(1, 2, 5);
            mat.set(2, 0, 6); mat.set(2, 1, 7); mat.set(2, 2, 8);

            REQUIRE(mat.row_vectors[0] == vec3i(0, 1, 2));
            REQUIRE(mat.row_vectors[1] == vec3i(3, 4, 5));
            REQUIRE(mat.row_vectors[2] == vec3i(6, 7, 8));
        }
        SECTION("column()") {
            mat = {
                { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 9 }
            };

            REQUIRE(mat.column(0) == vec3i(1, 4, 7));
            REQUIRE(mat.column(1) == vec3i(2, 5, 8));
            REQUIRE(mat.column(2) == vec3i(3, 6, 9));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, vec3i( 1, -4,  7));
            mat.setColumn(1, vec3i(-2,  5, -8));
            mat.setColumn(2, vec3i( 3, -6,  9));

            REQUIRE(mat.row_vectors[0] == vec3i( 1, -2,  3));
            REQUIRE(mat.row_vectors[1] == vec3i(-4,  5, -6));
            REQUIRE(mat.row_vectors[2] == vec3i( 7, -8,  9));
        }
    }
    SECTION("Identity matrix") {
        mat3 id = mat3::identity();

        REQUIRE(id.row_vectors[0] == vec3(1.0f, 0.0f, 0.0f));
        REQUIRE(id.row_vectors[1] == vec3(0.0f, 1.0f, 0.0f));
        REQUIRE(id.row_vectors[2] == vec3(0.0f, 0.0f, 1.0f));
    }
}

TEST_CASE("4x4 matrix specialization", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,4,4>) == 16 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        mat4i mat{ -1 };
        vec4i expected[]{
            { -1, -1, -1, -1 },
            { -1, -1, -1, -1 },
            { -1, -1, -1, -1 },
            { -1, -1, -1, -1 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[]{ 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        mat4i m1{ arr };
        vec4i expected[]{
            { 16, 15, 14, 13 },
            { 12, 11, 10,  9 },
            {  8,  7,  6,  5 },
            {  4,  3,  2,  1 }
        };
        verifyInitialization(m1, expected);

        mat4i m2{ arr + 1 };
        expected[0] = { 15, 14, 13, 12 };
        expected[1] = { 11, 10,  9,  8 };
        expected[2] = {  7,  6,  5,  4 };
        expected[3] = {  3,  2,  1,  0 };
        verifyInitialization(m2, expected);
    }
    SECTION("Vector array constructor") {
        mat4i mat{{
            {  0,  1,  2,  3 },
            {  4,  5,  6,  7 },
            {  8,  9, 10, 11 },
            { 12, 13, 14, 15 }
        }};
        vec4i expected[]{
            {  0,  1,  2,  3 },
            {  4,  5,  6,  7 },
            {  8,  9, 10, 11 },
            { 12, 13, 14, 15 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Initialize from row vectors") {
        mat4i mat{
            {  2, -2, -2, -2 },
            { -2,  2, -2, -2 },
            { -2, -2,  2, -2 },
            { -2, -2, -2,  2 }
        };
        vec4i expected[]{
            {  2, -2, -2, -2 },
            { -2,  2, -2, -2 },
            { -2, -2,  2, -2 },
            { -2, -2, -2,  2 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Casting constructor") {
        mat4i mat{ mat4{ 1.0f } };
        vec4i expected[]{
            { 1, 1, 1, 1 },
            { 1, 1, 1, 1 },
            { 1, 1, 1, 1 },
            { 1, 1, 1, 1 }
        };
        verifyInitialization(mat, expected);
    }
    SECTION("Element access") {
        mat4i mat;
        SECTION("at()") {
            SECTION("getting values") {
                mat = {
                    {   1,   2,   3,   4 },
                    {  -5,  -6,  -7,  -8 },
                    {   9,  10,  11,  12 },
                    { -13, -14, -15, -16 }
                };

                REQUIRE(mat.at(0, 0) ==   1);
                REQUIRE(mat.at(0, 1) ==   2);
                REQUIRE(mat.at(0, 2) ==   3);
                REQUIRE(mat.at(0, 3) ==   4);
                REQUIRE(mat.at(1, 0) ==  -5);
                REQUIRE(mat.at(1, 1) ==  -6);
                REQUIRE(mat.at(1, 2) ==  -7);
                REQUIRE(mat.at(1, 3) ==  -8);
                REQUIRE(mat.at(2, 0) ==   9);
                REQUIRE(mat.at(2, 1) ==  10);
                REQUIRE(mat.at(2, 2) ==  11);
                REQUIRE(mat.at(2, 3) ==  12);
                REQUIRE(mat.at(3, 0) == -13);
                REQUIRE(mat.at(3, 1) == -14);
                REQUIRE(mat.at(3, 2) == -15);
                REQUIRE(mat.at(3, 3) == -16);
            }
            SECTION("setting values") {
                mat.at(0, 0) =  -1; mat.at(0, 1) =  -2; mat.at(0, 2) =  -3; mat.at(0, 3) =  -4;
                mat.at(1, 0) =   5; mat.at(1, 1) =   6; mat.at(1, 2) =   7; mat.at(1, 3) =   8;
                mat.at(2, 0) =  -9; mat.at(2, 1) = -10; mat.at(2, 2) = -11; mat.at(2, 3) = -12;
                mat.at(3, 0) =  13; mat.at(3, 1) =  14; mat.at(3, 2) =  15; mat.at(3, 3) =  16;

                REQUIRE(mat.row_vectors[0] == vec4i( -1,  -2,  -3,  -4));
                REQUIRE(mat.row_vectors[1] == vec4i(  5,   6,   7,   8));
                REQUIRE(mat.row_vectors[2] == vec4i( -9, -10, -11, -12));
                REQUIRE(mat.row_vectors[3] == vec4i( 13,  14,  15,  16));
            }
        }
        SECTION("() operator") {
            SECTION("getting values") {
                mat = {
                    {  -1,  2,  -3,  4 },
                    {  -5,  6,  -7,  8 },
                    {  -9, 10, -11, 12 },
                    { -13, 14, -15, 16 }
                };

                REQUIRE(mat(0, 0) ==  -1);
                REQUIRE(mat(0, 1) ==   2);
                REQUIRE(mat(0, 2) ==  -3);
                REQUIRE(mat(0, 3) ==   4);
                REQUIRE(mat(1, 0) ==  -5);
                REQUIRE(mat(1, 1) ==   6);
                REQUIRE(mat(1, 2) ==  -7);
                REQUIRE(mat(1, 3) ==   8);
                REQUIRE(mat(2, 0) ==  -9);
                REQUIRE(mat(2, 1) ==  10);
                REQUIRE(mat(2, 2) == -11);
                REQUIRE(mat(2, 3) ==  12);
                REQUIRE(mat(3, 0) == -13);
                REQUIRE(mat(3, 1) ==  14);
                REQUIRE(mat(3, 2) == -15);
                REQUIRE(mat(3, 3) ==  16);
            }
            SECTION("setting values") {
                mat(0, 0) =   1; mat(0, 1) =   2; mat(0, 2) =   3; mat(0, 3) =   4;
                mat(1, 0) =  -5; mat(1, 1) =  -6; mat(1, 2) =  -7; mat(1, 3) =  -8;
                mat(2, 0) =   9; mat(2, 1) =  10; mat(2, 2) =  11; mat(2, 3) =  12;
                mat(3, 0) = -13; mat(3, 1) = -14; mat(3, 2) = -15; mat(3, 3) = -16;

                REQUIRE(mat.row_vectors[0] == vec4i(  1,   2,   3,   4));
                REQUIRE(mat.row_vectors[1] == vec4i( -5,  -6,  -7,  -8));
                REQUIRE(mat.row_vectors[2] == vec4i(  9,  10,  11,  12));
                REQUIRE(mat.row_vectors[3] == vec4i(-13, -14, -15, -16));
            }
        }
        SECTION("Data pointer") {
            int32_t* data = mat.data();
            for (int32_t i = 0; i < 16; i++) data[i] = i;

            for (int32_t i = 0; i < 4; i++)
                REQUIRE(mat.row_vectors[i] == vec4i(4*i, 4*i + 1, 4*i + 2, 4*i + 3));
        }
        SECTION("set()") {
            for (int32_t i = 0; i < 16; i++) mat.set(i / 4, i % 4, 15-i);

            for (int32_t i = 0; i < 4; i++)
                REQUIRE(mat.row_vectors[i] == vec4i(15 - 4*i, 14 - 4*i, 13 - 4*i, 12 - 4*i));
        }
        SECTION("column()") {
            mat = {
                { 16, 15, 14, 13 },
                { 12, 11, 10,  9 },
                {  8,  7,  6,  5 },
                {  4,  3,  2,  1 }
            };

            REQUIRE(mat.column(0) == vec4i(16, 12, 8, 4));
            REQUIRE(mat.column(1) == vec4i(15, 11, 7, 3));
            REQUIRE(mat.column(2) == vec4i(14, 10, 6, 2));
            REQUIRE(mat.column(3) == vec4i(13,  9, 5, 1));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, { 0, 4,  8, 12 });
            mat.setColumn(1, { 1, 5,  9, 13 });
            mat.setColumn(2, { 2, 6, 10, 14 });
            mat.setColumn(3, { 3, 7, 11, 15 });

            for (int i = 0; i < 4; i++)
                REQUIRE(mat.row_vectors[i] == vec4i(4*i, 4*i + 1, 4*i + 2, 4*i + 3));
        }
    }
    SECTION("Identity matrix") {
        mat4 id = mat4::identity();
        vec4 expected_rows[]{
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };

        for (int i = 0; i < 4; i++)
            REQUIRE(id.row_vectors[i] == expected_rows[i]);
    }
}

TEST_CASE("General matrix", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,1,5>) == 5 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        Matrix<int32_t, 2, 3> mat{ 3 };
        vec3i vec{ 3 };

        REQUIRE(mat.row_vectors[0] == vec);
        REQUIRE(mat.row_vectors[1] == vec);
    }
    SECTION("Data pointer constructor") {
        const int32_t arr[]{ 0, 1, 2, 3, 4, 5, 6 };
        Matrix<int32_t, 3, 2> m1{ arr };

        REQUIRE(m1.row_vectors[0] == vec2i(0, 1));
        REQUIRE(m1.row_vectors[1] == vec2i(2, 3));
        REQUIRE(m1.row_vectors[2] == vec2i(4, 5));

        Matrix<int32_t, 3, 2> m2{ arr + 1 };
        REQUIRE(m2.row_vectors[0] == vec2i(1, 2));
        REQUIRE(m2.row_vectors[1] == vec2i(3, 4));
        REQUIRE(m2.row_vectors[2] == vec2i(5, 6));
    }
    SECTION("Vector array constructor") {
        Matrix<int32_t, 2, 3> mat{{
            { 1, 2, 3 },
            { 4, 5, 6 }
        }};

        REQUIRE(mat.row_vectors[0] == vec3i(1, 2, 3));
        REQUIRE(mat.row_vectors[1] == vec3i(4, 5, 6));
    }
    SECTION("Casting constructor") {
        Matrix<int32_t, 2, 3> mat{ Matrix<float, 2, 3>{ 1.0f } };
        vec3i vec_3{ 1 };

        REQUIRE(mat.row_vectors[0] == vec_3);
        REQUIRE(mat.row_vectors[1] == vec_3);
    }
    SECTION("Element access") {
        Matrix<int32_t, 2, 3> mat;
        SECTION("at()") {
            SECTION("getting values") {
                const int32_t arr[]{ 1, 2, 3, 4, 5, 6 };
                mat = Matrix<int32_t, 2, 3>{arr};

                REQUIRE(mat.at(0, 0) == 1);
                REQUIRE(mat.at(0, 1) == 2);
                REQUIRE(mat.at(0, 2) == 3);
                REQUIRE(mat.at(1, 0) == 4);
                REQUIRE(mat.at(1, 1) == 5);
                REQUIRE(mat.at(1, 2) == 6);
            }
            SECTION("setting values") {
                mat.at(0, 0) = 6; mat.at(0, 1) = 5; mat.at(0, 2) = 4;
                mat.at(1, 0) = 3; mat.at(1, 1) = 2; mat.at(1, 2) = 1;

                REQUIRE(mat.row_vectors[0] == vec3i(6, 5, 4));
                REQUIRE(mat.row_vectors[1] == vec3i(3, 2, 1));
            }
        }
        SECTION("() operator") {
            SECTION("getting values") {
                const int32_t arr[]{ 1, 2, 3, 4, 5, 6 };
                mat = Matrix<int32_t, 2, 3>{arr};

                REQUIRE(mat(0, 0) == 1);
                REQUIRE(mat(0, 1) == 2);
                REQUIRE(mat(0, 2) == 3);
                REQUIRE(mat(1, 0) == 4);
                REQUIRE(mat(1, 1) == 5);
                REQUIRE(mat(1, 2) == 6);
            }
            SECTION("setting values") {
                mat(0, 0) = 1; mat(0, 1) = 2; mat(0, 2) = 3;
                mat(1, 0) = 4; mat(1, 1) = 5; mat(1, 2) = 6;

                REQUIRE(mat.row_vectors[0] == vec3i(1, 2, 3));
                REQUIRE(mat.row_vectors[1] == vec3i(4, 5, 6));
            }
        }
        SECTION("Data pointer") {
            mat.data()[0] = -1;
            mat.data()[1] = -2;
            mat.data()[2] = -3;
            mat.data()[3] = -4;
            mat.data()[4] = -5;
            mat.data()[5] = -6;

            REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
            REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
        }
        SECTION("set()") {
            mat.set(0, 0, -1); mat.set(0, 1, -2); mat.set(0, 2, -3);
            mat.set(1, 0, -4); mat.set(1, 1, -5); mat.set(1, 2, -6);

            REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
            REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
        }
        SECTION("column()") {
            const int32_t arr[]{ 6, 5, 4, 3, 2, 1 };
            mat = Matrix<int32_t, 2, 3>{arr};

            REQUIRE(mat.column(0) == vec2i(6, 3));
            REQUIRE(mat.column(1) == vec2i(5, 2));
            REQUIRE(mat.column(2) == vec2i(4, 1));
        }
        SECTION("setColumn()") {
            mat.setColumn(0, vec2i(-1, -4));
            mat.setColumn(1, vec2i(-2, -5));
            mat.setColumn(2, vec2i(-3, -6));

            REQUIRE(mat.row_vectors[0] == vec3i(-1, -2, -3));
            REQUIRE(mat.row_vectors[1] == vec3i(-4, -5, -6));
        }
    }
}

TEST_CASE("General square matrix", "[sini::Matrix]")
{
    SECTION("Memory allocation") {
        REQUIRE( sizeof(Matrix<int,5,5>) == 25 * sizeof(int) );
    }
    SECTION("Fill constructor") {
        Matrix<int, 5, 5> mat{ -1 };
        Vector<int, 5> vec{ -1 };

        for (int i = 0; i < 5; i++)
            REQUIRE(mat.row_vectors[i] == vec);
    }
    SECTION("Data pointer constructor") {
        const int arr[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
        Matrix<int, 5, 5> m1{ arr };
        Vector<int, 5> expected_rows[]{
            Vector<int, 5>{ arr },
            Vector<int, 5>{ arr +  5 },
            Vector<int, 5>{ arr + 10 },
            Vector<int, 5>{ arr + 15 },
            Vector<int, 5>{ arr + 20 }
        };

        for (int i = 0; i < 5; i++)
            REQUIRE(m1.row_vectors[i] == expected_rows[i]);

        m1 = Matrix<int, 5, 5>{ arr + 1 };
        for (int i = 0; i < 5; i++) {
            expected_rows[i] = Vector<int, 5>{ arr + (5*i + 1) };
            REQUIRE(m1.row_vectors[i] == expected_rows[i]);
        }
    }
    SECTION("Vector array constructor") {
        Matrix<int, 5, 5> mat{{
            {{  0,  1,  2,  3,  4 }},
            {{  5,  6,  7,  8,  9 }},
            {{ 10, 11, 12, 13, 14 }},
            {{ 15, 16, 17, 18, 19 }},
            {{ 20, 21, 22, 23, 24 }}
        }};
        Vector<int, 5> expected_rows[]{
            {{  0,  1,  2,  3,  4 }},
            {{  5,  6,  7,  8,  9 }},
            {{ 10, 11, 12, 13, 14 }},
            {{ 15, 16, 17, 18, 19 }},
            {{ 20, 21, 22, 23, 24 }}
        };

        for (int i = 0; i < 5; i++)
            REQUIRE(mat.row_vectors[i] == expected_rows[i]);
    }
    SECTION("Casting constructors") {
        Matrix<int, 5, 5> mat{ Matrix<float, 5, 5>(-3.0f) };
        Vector<int, 5> vec{ -3 };

        for (int i = 0; i < 5; i++)
            REQUIRE(mat.row_vectors[i] == vec);
    }
    SECTION("Element access") {
        Matrix<int, 5, 5> mat;
        SECTION("() operator") {
            mat(0, 0) = 1; mat(0, 1) = 2; mat(0, 2) = 3; mat(0, 3) = 4; mat(0, 4) = 5;
            mat(1, 0) = 2; mat(1, 1) = 3; mat(1, 2) = 4; mat(1, 3) = 5; mat(1, 4) = 6;
            mat(2, 0) = 3; mat(2, 1) = 4; mat(2, 2) = 5; mat(2, 3) = 6; mat(2, 4) = 7;
            mat(3, 0) = 4; mat(3, 1) = 5; mat(3, 2) = 6; mat(3, 3) = 7; mat(3, 4) = 8;
            mat(4, 0) = 5; mat(4, 1) = 6; mat(4, 2) = 7; mat(4, 3) = 8; mat(4, 4) = 9;

            Vector<int, 5> expected_rows[]{
                {{ 1, 2, 3, 4, 5 }},
                {{ 2, 3, 4, 5, 6 }},
                {{ 3, 4, 5, 6, 7 }},
                {{ 4, 5, 6, 7, 8 }},
                {{ 5, 6, 7, 8, 9 }}
            };

            for (int i = 0; i < 5; i++)
                REQUIRE(mat.row_vectors[i] == expected_rows[i]);
        }
        SECTION("Data pointer") {
            int* md = mat.data();
            md[0]  = 1; md[1]  = 2; md[2]  = 3; md[3]  = 4; md[4]  = 5;
            md[5]  = 2; md[6]  = 3; md[7]  = 4; md[8]  = 5; md[9]  = 6;
            md[10] = 3; md[11] = 4; md[12] = 5; md[13] = 6; md[14] = 7;
            md[15] = 4; md[16] = 5; md[17] = 6; md[18] = 7; md[19] = 8;
            md[20] = 5; md[21] = 6; md[22] = 7; md[23] = 8; md[24] = 9;

            Vector<int, 5> expected_rows[]{
                {{ 1, 2, 3, 4, 5 }},
                {{ 2, 3, 4, 5, 6 }},
                {{ 3, 4, 5, 6, 7 }},
                {{ 4, 5, 6, 7, 8 }},
                {{ 5, 6, 7, 8, 9 }}
            };

            for (int i = 0; i < 5; i++)
                REQUIRE(mat.row_vectors[i] == expected_rows[i]);
        }
        SECTION("set()") {
            for (int i = 0; i < 25; i++)
                mat.set(i / 5, i % 5, i + 1);

            Vector<int, 5> expected_rows[]{
                {{  1,  2,  3,  4,  5 }},
                {{  6,  7,  8,  9, 10 }},
                {{ 11, 12, 13, 14, 15 }},
                {{ 16, 17, 18, 19, 20 }},
                {{ 21, 22, 23, 24, 25 }}
            };

            for (int i = 0; i < 5; i++)
                REQUIRE(mat.row_vectors[i] == expected_rows[i]);
        }
        SECTION("column()") {
            int* md = mat.data();
            for (int i = 0; i < 25; i++) md[i] = i;

            Vector<int, 5> expected_cols[]{
                {{ 0, 5, 10, 15, 20 }},
                {{ 1, 6, 11, 16, 21 }},
                {{ 2, 7, 12, 17, 22 }},
                {{ 3, 8, 13, 18, 23 }},
                {{ 4, 9, 14, 19, 24 }}
            };

            for (int i = 0; i < 5; i++)
                REQUIRE(mat.column(i) == expected_cols[i]);
        }
        SECTION("setColumn()") {
            Vector<int, 5> cols[]{
                {{ 1,  6, 11, 16, 21 }},
                {{ 2,  7, 12, 17, 22 }},
                {{ 3,  8, 13, 18, 23 }},
                {{ 4,  9, 14, 19, 24 }},
                {{ 5, 10, 15, 20, 25 }}
            };
            for (int i = 0; i < 5; i++)
                mat.setColumn(i, cols[i]);

            Vector<int, 5> expected_rows[]{
                {{  1,  2,  3,  4,  5 }},
                {{  6,  7,  8,  9, 10 }},
                {{ 11, 12, 13, 14, 15 }},
                {{ 16, 17, 18, 19, 20 }},
                {{ 21, 22, 23, 24, 25 }}
            };
            for (int i = 0; i < 5; i++)
                REQUIRE(mat.row_vectors[i] == expected_rows[i]);
        }
    }
    SECTION("Identity matrix") {
        Matrix<float, 5, 5> id = Matrix<float, 5, 5>::identity();
        Vector<float, 5> expected_rows[]{
            {{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }},
            {{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }},
            {{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }},
            {{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }},
            {{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }}
        };

        for (int i = 0; i < 5; i++)
            REQUIRE(id.row_vectors[i] == expected_rows[i]);
    }
}

TEST_CASE("Matrix comparisons", "[sini::Matrix]")
{
    mat2i mat1{
        { 1, 2 },
        { 3, 4 }
    }, mat2{
        { -1, -2 },
        { -3, -4 }
    }, mat3{
        { 1, 2 },
        { 3, 5 }
    };

    REQUIRE(mat1 == mat1);
    REQUIRE(mat1 != mat2);
    REQUIRE(mat1 == -mat2);
    REQUIRE(mat1 != mat3);
}

TEST_CASE("Matrix arithmetics", "[sini::Matrix]")
{
    mat2i m1{
        {  4,  2 },
        { 16, 32 }
    }, m2{
        { -1,  3 },
        {  4, -8 }
    };

    SECTION("Addition") {
        mat2i m3 = m1 + m2;

        REQUIRE(m3 == mat2i({  3,  5 },
                            { 20, 24 }));
        REQUIRE(m1 == mat2i({  4,  2 },
                            { 16, 32 }));
        REQUIRE(m2 == mat2i({ -1,  3 },
                            {  4, -8 }));
    }
    SECTION("Addition assignment") {
        m1 += m2;

        REQUIRE(m1 == mat2i({  3,  5 },
                            { 20, 24 }));
        REQUIRE(m2 == mat2i({ -1,  3 },
                            {  4, -8 }));
    }
    SECTION("Subtraction") {
        mat2i m3 = m1 - m2;

        REQUIRE(m3 == mat2i({  5, -1 },
                            { 12, 40 }));
        REQUIRE(m1 == mat2i({  4,  2 },
                            { 16, 32 }));
        REQUIRE(m2 == mat2i({ -1,  3 },
                            {  4, -8 }));
    }
    SECTION("Subtraction assignment") {
        m1 -= m2;

        REQUIRE(m1 == mat2i({  5, -1 },
                            { 12, 40 }));
        REQUIRE(m2 == mat2i({ -1,  3 },
                            { 4, -8 }));
    }
    SECTION("Negation") {
        mat2i m3 = -m2;

        REQUIRE(m3 == mat2i({  1, -3 },
                            { -4,  8 }));
        REQUIRE(m2 == mat2i({ -1,  3 },
                            {  4, -8 }));
    }
    SECTION("Multiplication with scalar") {
        mat2i m3 = 2 * m2;

        REQUIRE(m3 == mat2i({ -2,   6 },
                            {  8, -16 }));
        REQUIRE(m2 == mat2i({ -1,  3 },
                            {  4, -8 }));
    }
    SECTION("Scalar multiplication assignment") {
        m2 *= -2;

        REQUIRE(m2 == mat2i({  2, -6 },
                            { -8, 16 }));
    }
    SECTION("Division with scalar") {
        mat2i mat{
            { 3,  6 },
            { 9, 12 }
        };
        mat2i mat2 = mat / 3;

        REQUIRE(mat2 == mat2i({ 1, 2 },
                              { 3, 4 }));
        REQUIRE(mat == mat2i({ 3, 6 },
                             { 9, 12 }));
    }
    SECTION("Scalar division assignment") {
        mat2i mat{
            { 3,  6 },
            { 9, 12 }
        };
        mat /= 3;

        REQUIRE(mat == mat2i({ 1, 2 },
                             { 3, 4 }));
    }
    SECTION("Element-wise multiplication") {
        mat2i mat1{
            {  1, -2 },
            { -3,  4 }
        }, mat2{
            { 3,  5 },
            { 7, 11 }
        };
        mat2i mat3 = elemMult(mat1, mat2);

        REQUIRE(mat3 == mat2i({   3, -10 },
                              { -21,  44 }));
        REQUIRE(mat1 == mat2i({  1, -2 },
                              { -3,  4 }));
        REQUIRE(mat2 == mat2i({ 3,  5 },
                              { 7, 11 }));
    }
    SECTION("Element-wise multiplication assignment") {
        mat2i mat1{
            {  1, -2 },
            { -3,  4 }
        }, mat2{
            { 3,  5 },
            { 7, 11 }
        };
        eqElemMult(mat1, mat2);

        REQUIRE(mat1 == mat2i({   3, -10 },
                              { -21,  44 }));
        REQUIRE(mat2 == mat2i({ 3,  5 },
                              { 7, 11 }));
    }
    SECTION("Element-wise division") {
        mat2i mat1{
            { 16,   8 },
            { 32, 256 }
        }, mat2{
            {  4,  2 },
            { 16, 32 }
        };
        mat2i mat3 = elemDiv(mat1, mat2);

        REQUIRE(mat3 == mat2i({ 4, 4 },
                              { 2, 8 }));
        REQUIRE(mat1 == mat2i({ 16,   8 },
                              { 32, 256 }));
    }
    SECTION("Element-wise division assignment") {
        mat2i mat1{
            { 16,   8 },
            { 32, 256 }
        }, mat2{
            {  4,  2 },
            { 16, 32 }
        };
        eqElemDiv(mat1, mat2);

        REQUIRE(mat1 == mat2i({ 4, 4 },
                              { 2, 8 }));
        REQUIRE(mat2 == mat2i({  4,  2 },
                              { 16, 32 }));
    }
    SECTION("Matrix multiplication") {
        mat2i id = mat2i::identity();
        mat2i mat1{
            { 4, -2 },
            { 1,  3 }
        };
        Matrix<int32_t, 2, 3> mat2{{
            { 2, 3, 1 },
            { 1, 5, 7 }
        }};

        // Multiplication with the identity matrix shouldn't change anything,
        // whether multiplied from left or right
        mat2i orig_mat1 = mat1,
              mat3      = mat1 * id;
        REQUIRE(mat3 == orig_mat1);

        mat3 = id * mat1;
        REQUIRE(mat3 == orig_mat1);

        // More general matrix multiplication (2x2 x 2x3 -> 2x3)
        auto mat4 = mat1 * mat2;
        Matrix<int32_t,2,3> expected{{{ 6,  2, -10 },
                                      { 5, 18,  22 }}};
        REQUIRE(mat4 == expected);
    }
    SECTION("Matrix multiplication assignment") {
        mat2i id = mat2i::identity();
        mat2i mat1{
            { 4, -2 },
            { 1,  3 }
        }, mat2{
            { 2, 3 },
            { 5, 7 }
        };

        mat1 *= id;
        REQUIRE(mat1 == mat2i({ 4, -2 },
                              { 1,  3 }));

        mat1 *= mat2;
        REQUIRE(mat1 == mat2i({ -2, -2 },
                              { 17, 24 }));
    }
    SECTION("Matrix-vector multiplication") {
        Matrix<int32_t, 2, 3> mat{{
            {  1, -2, 3 },
            { -1,  0, 0 }
        }};
        vec3i vec{ 3, 2, 1 };
        vec2i result = mat * vec;

        REQUIRE(result == vec2i(2, -3));
    }
}

TEST_CASE("Matrix-vector conversion", "[sini::Matrix]")
{
    SECTION("Column to row vector (vector to 1xN matrix)") {
        vec3i vec{ 3, 5, 7 };
        Matrix<int32_t, 1, 3> row_vec{ vec.data() };

        REQUIRE(toRowVector(vec) == row_vec);
    }
    SECTION("Row to column vector (1xN matrix to vector)") {
        Matrix<int32_t, 1, 3> mat{{{ 1, -2, 3 }}};
        vec3i col_vec{ mat.data() };

        REQUIRE(toColumnVector(mat) == col_vec);
    }
}

TEST_CASE("Transpose", "[sini::Matrix]")
{
    mat3i mat{
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    }, mat_transp{
        { 1, 4, 7 },
        { 2, 5, 8 },
        { 3, 6, 9 }};

    REQUIRE(transpose(mat) == mat_transp);
}

TEST_CASE("Pow (Matrix)", "[sini::Matrix]")
{
    mat3i mat{
        {  1, 0, 3 },
        { -2, 1, 0 },
        {  0, 0, 2 }
    };

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
    SECTION("2x2 matrix") {
        mat2i m1{
            { 1,  2 },
            { 3, -4 }
        };
        REQUIRE(det(m1) == -10);
    }
    SECTION("3x3 matrix") {
        mat3i m2{
            { 1, 0,  0 },
            { 0, 1,  2 },
            { 0, 3, -4 }
        };
        REQUIRE(det(m2) == -10);

        mat3i m4{
            { 0, 1,  2 },
            { 3, 4, -5 },
            { 0, 2,  4 } // linearly dependent of the other (the first) rows
        };
        REQUIRE(det(m4) == 0);
    }
    SECTION("4x4 matrix") {
        mat4i m3{
            { -1,  2,   3,   4 },
            {  5, -6,   7,   8 },
            {  9, 10, -11,  12 },
            { 13, 14,  15, -16 }
        };
        // According to WolframAlpha.com the determinant is -36 416
        REQUIRE(det(m3) == -36416);
    }
}

TEST_CASE("Adjugate matrix", "[sini::Matrix]")
{
    SECTION("Identity matrix") {
        REQUIRE(adj(mat2i::identity()) == mat2i::identity());
    }
    SECTION("2x2 matrix") {
        mat2i m1{
            { 1, 2 },
            { 3, 4 }
        }, m1_adj{
            {  4, -3 },
            { -2,  1 }
        };
        REQUIRE(adj(m1) == m1_adj);
    }
    SECTION("3x3 matrix") {
        mat3i m2{
            { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 }
        }, m2_adj{ // According to WolframAlpha.com
            { -3,   6, -3 },
            {  6, -12,  6 },
            { -3,   6, -3 }
        };
        REQUIRE(adj(m2) == m2_adj);
    }
    SECTION("4x4 matrix") {
        mat4i m3{
            {  0, -1,   2,   3 },
            { -4,  5,  -6,   7 },
            {  8, -9,  10, -11 },
            { 12, 13, -14,  15 }
        }, m3_adj{ // According to WolframAlpha.com
            {   0,    24,   48, 24 },
            {  96, -1200, -672, 48 },
            { 192,  -888, -480, 24 },
            {  96,   192,   96,  0 }
        };
        REQUIRE(adj(m3) == m3_adj);
    }
}

TEST_CASE("Inverse matrix", "[sini::Matrix]")
{
    // Correctness tests
    // This could probably use better testing as well, but I hope that a few
    // cases are enough to catch any bugs in the code
    float tol = 1e-10f;
    SECTION("Identity matrix") {
        // Make sure that identity matrices are their own inverse
        REQUIRE( approxEqual(inverse(mat2::identity()), mat2::identity(), tol) );
        REQUIRE( approxEqual(inverse(mat3::identity()), mat3::identity(), tol) );
        REQUIRE( approxEqual(inverse(mat4::identity()), mat4::identity(), tol) );
    }
    SECTION("2x2 matrix") {
        mat2 m1{
            { 1.0f, -1.0f },
            { 2.0f,  2.0f }
        }, m1_inv{
            {  0.5f, 0.25f },
            { -0.5f, 0.25f }
        };
        REQUIRE( approxEqual(inverse(m1), m1_inv, tol) );
    }
    SECTION("3x3 matrix") {
        mat3 m2{
            {  1.0f, 0.0f, -1.0f },
            {  0.0f, 1.0f,  2.0f },
            { -1.0f, 2.0f,  3.0f }
        }, m2_inv{ // According to WolframAlpha.com
            {  0.5f,  1.0f, -0.5f },
            {  1.0f, -1.0f,  1.0f },
            { -0.5f,  1.0f, -0.5f }
        };
        REQUIRE( approxEqual(inverse(m2), m2_inv, tol) );
    }
}

TEST_CASE("Matrix Abs", "[sini::Matrix]")
{
    mat3i mat{
        {  1, -2, -3 },
        { -4,  5,  6 },
        {  7,  8,  9 }
    }, mat_abs{
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };
    REQUIRE(abs(mat) == mat_abs);
}

TEST_CASE("Matrix max and min element", "[sini::Matrix]")
{
    mat3i mat{
        {   1, 2, -3 },
        { -16, 8,  0 },
        {   3, 5,  7 }
    };
    int32_t max = maxElement(mat);
    REQUIRE(max == 8);
    int32_t min = minElement(mat);
    REQUIRE(min == -16);
}

TEST_CASE("Matrix to string", "[sini::Matrix]")
{
    mat2i int_mat{
        { 2, 1 },
        { 4, 3 }
    };
    mat2 float_mat{
        { 1.12f, 2.23f },
        { 3.34f, 4.456f }
    };
    SECTION("To default string") {
        REQUIRE(toString(int_mat) == "[[2, 1], [4, 3]]");
        REQUIRE(toString(int_mat, true) == "[[2, 1],\n [4, 3]]");

        REQUIRE(toString(float_mat) == "[[1.12, 2.23], [3.34, 4.456]]");
        REQUIRE(toString(float_mat, true) == "[[1.12, 2.23],\n [3.34, 4.456]]");
    }
    SECTION("To formatted string") {
        REQUIRE(toFormattedString(float_mat, "%.1f") == "[[1.1, 2.2], [3.3, 4.5]]");
        REQUIRE(toFormattedString(float_mat, "%.1f", true) == "[[1.1, 2.2],\n [3.3, 4.5]]");
    }
}

TEST_CASE("Matrix is standard layout and trivial", "[sini::Matrix]")
{
    REQUIRE(std::is_standard_layout<mat2>::value);
    REQUIRE(std::is_standard_layout<mat3>::value);
    REQUIRE(std::is_standard_layout<mat4>::value);
    bool mat5_std_layout = std::is_standard_layout<Matrix<float,5,5>>::value,
       mat2x5_std_layout = std::is_standard_layout<Matrix<float,2,5>>::value;
    REQUIRE(mat5_std_layout);
    REQUIRE(mat2x5_std_layout);

    REQUIRE(std::is_trivial<mat2>::value);
    REQUIRE(std::is_trivial<mat3>::value);
    REQUIRE(std::is_trivial<mat4>::value);
    bool mat5_trivial = std::is_trivial<Matrix<float,5,5>>::value,
       mat2x5_trivial = std::is_trivial<Matrix<float,2,5>>::value;
    REQUIRE(mat5_trivial);
    REQUIRE(mat2x5_trivial);
}
