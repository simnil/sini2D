// Testing of sini/math/Matrix using Catch
#include "catch.hpp"
#include "sini/math/Matrix.hpp"


using namespace sini;

TEST_CASE("2x2 matrix specialization", "[sini::Matrix]") {

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
		const int32_t arr[] = { 4, 3, 2, 1, -1 };
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
		SECTION("Explicit vector constructors") {
			Matrix<int32_t, 2, 2> mat{
				Vector<int32_t,2>{ 2, 3 },
				Vector<int32_t,2>{ 5, 7 }
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
		SECTION("Implicit vector constructors") {
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
}

TEST_CASE("3x3 matrix specialization", "[sini::Matrix]") {

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
		const int arr[] = { -1, 2, -3, 4, -5, 6, -7, 8, -9, 10 };
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
		SECTION("Explicit vector constructors") {
			Matrix<int32_t, 3, 3> mat{
				Vector<int32_t, 3>{ 9, 8, 7 },
				Vector<int32_t, 3>{ 6, 5, 4 },
				Vector<int32_t, 3>{ 3, 2, 1 }
			};
			REQUIRE(mat.a == 9);
			REQUIRE(mat.b == 8);
			REQUIRE(mat.c == 7);
			REQUIRE(mat.d == 6);
			REQUIRE(mat.e == 5);
			REQUIRE(mat.f == 4);
			REQUIRE(mat.g == 3);
			REQUIRE(mat.h == 2);
			REQUIRE(mat.i == 1);
			REQUIRE(mat(0, 0) == 9);
			REQUIRE(mat(0, 1) == 8);
			REQUIRE(mat(0, 2) == 7);
			REQUIRE(mat(1, 0) == 6);
			REQUIRE(mat(1, 1) == 5);
			REQUIRE(mat(1, 2) == 4);
			REQUIRE(mat(2, 0) == 3);
			REQUIRE(mat(2, 1) == 2);
			REQUIRE(mat(2, 2) == 1);
			REQUIRE(mat.row_vectors[0] == vec3i(9, 8, 7));
			REQUIRE(mat.row_vectors[1] == vec3i(6, 5, 4));
			REQUIRE(mat.row_vectors[2] == vec3i(3, 2, 1));
			REQUIRE(mat.column(0) == vec3i(9, 6, 3));
			REQUIRE(mat.column(1) == vec3i(8, 5, 2));
			REQUIRE(mat.column(2) == vec3i(7, 4, 1));
		}
		SECTION("Implicit vector constructors") {
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
}

TEST_CASE("4x4 matrix specialization", "[sini::Matrix]") {

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
		const int32_t arr[] = { 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
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
		SECTION("Explicit vector constructors") {
			Matrix<int32_t, 4, 4> mat{
				Vector<int32_t, 4>{ 2, -2, -2, -2 },
				Vector<int32_t, 4>{ -2, 2, -2, -2 },
				Vector<int32_t, 4>{ -2, -2, 2, -2 },
				Vector<int32_t, 4>{ -2, -2, -2, 2 }
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
		SECTION("Implicit vector constructors") {
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
}

TEST_CASE("General matrix", "[sini::Matrix]") {

}

TEST_CASE("General square matrix", "[sini::Matrix]") {

}

TEST_CASE("Matrix arithmetics", "[sini::Matrix]") {

}

TEST_CASE("Matrix comparisons", "[sini::Matrix]") {

}

TEST_CASE("Matrix-vector conversion", "[sini::Matrix]") {

}

TEST_CASE("Transpose", "[sini::Matrix]") {

}

TEST_CASE("Pow (Matrix)", "[sini::Matrix]") {

}

TEST_CASE("Determinant", "[sini::Matrix]") {

}

TEST_CASE("Adjugate matrix", "[sini::Matrix]") {

}

TEST_CASE("Inverse", "[sini::Matrix]") {

}