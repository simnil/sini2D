// SiNi Matrix class template
//
// NOTE!
// Most functions are written with floating point values in mind, and some do
// therefore not always translate well to integers with undefined or undesired
// behaviour as a consquence. Until (if) this is fixed, consider casting to
// float/double when e.g. computing the inverse of a matrix (very few integer
// matrices have integer matrix inverses!) or other more complicated operations.
#pragma once

// Workaround for clang++/g++
// For some reason the minor and major macros are defined, resulting in a naming
// conflict
#undef minor

#include "CudaCompat.hpp"
#include "math/Vector.hpp"


namespace sini {

// ---------------
// General Matrix
// ---------------
template<typename T, uint32_t M, uint32_t N>
struct Matrix {
    Vector<T, N> row_vectors[M];

    Matrix() noexcept = default;
    Matrix(const Matrix<T,M,N>&) noexcept = default;
    Matrix<T,M,N>& operator= (const Matrix<T,M,N>&) noexcept = default;
    ~Matrix() noexcept = default;

    SINI_CUDA_COMPAT Matrix(T init_val);
    SINI_CUDA_COMPAT explicit Matrix(const T* data_ptr) noexcept;
    SINI_CUDA_COMPAT Matrix(const Vector<T,N> (&init_rows)[M]) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Matrix(const Matrix<T2,M,N>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &row_vectors[0][0]; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &row_vectors[0][0]; }
    // Element access with bounds checking via assert
    SINI_CUDA_COMPAT T& at(uint32_t i, uint32_t j) noexcept;
    SINI_CUDA_COMPAT T  at(uint32_t i, uint32_t j) const noexcept;
    SINI_CUDA_COMPAT Vector<T,M> column(uint32_t j) const noexcept;
    // Element access without bounds checking
    SINI_CUDA_COMPAT T& operator() (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
    SINI_CUDA_COMPAT T  operator() (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

    SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
    SINI_CUDA_COMPAT void setColumn(uint32_t j, const Vector<T,M>& col) noexcept;

    SINI_CUDA_COMPAT Matrix<T,M-1,N-1> submatrix(uint32_t i, uint32_t j) const noexcept;
};


// ----------------------
// General square matrix
// ----------------------
template<typename T, uint32_t N>
struct Matrix<T, N, N> {
    Vector<T, N> row_vectors[N];

    Matrix() noexcept = default;
    Matrix(const Matrix<T,N,N>&) noexcept = default;
    static SINI_CUDA_COMPAT Matrix<T, N, N> identity() noexcept;
    Matrix<T,N,N>& operator= (const Matrix<T,N,N>&) noexcept = default;
    ~Matrix() noexcept = default;

    SINI_CUDA_COMPAT Matrix(T init_val);
    SINI_CUDA_COMPAT explicit Matrix(const T* data_ptr) noexcept;
    SINI_CUDA_COMPAT Matrix(const Vector<T,N> (&init_rows)[N]) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Matrix(const Matrix<T2,N,N>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &row_vectors[0][0]; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &row_vectors[0][0]; }
    // Element access with bounds checking via assert
    SINI_CUDA_COMPAT T& at(uint32_t i, uint32_t j) noexcept;
    SINI_CUDA_COMPAT T  at(uint32_t i, uint32_t j) const noexcept;
    SINI_CUDA_COMPAT Vector<T,N> column(uint32_t j) const noexcept;
    // Element access without bounds checking
    SINI_CUDA_COMPAT T& operator() (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
    SINI_CUDA_COMPAT T  operator() (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

    SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
    SINI_CUDA_COMPAT void setColumn(uint32_t j, const Vector<T,N>& col) noexcept;

    SINI_CUDA_COMPAT Matrix<T,N-1,N-1> submatrix(uint32_t i, uint32_t j) const noexcept;
};


// ---------------------------------------------------
// Common matrix forms, with a bit more functionality
// ---------------------------------------------------
// 2x2 Matrix
template<typename T>
struct Matrix<T, 2, 2> {
    // Nameless union to allow for getting rows/elements both via the
    // array or via e.g. mat.a
    union {
        struct { Vector<T,2> row_vectors[2]; };
        struct {
            T   a, b,
                c, d;
        };
        struct {
            T   e00, e01,
                e10, e11;
        };
    };

    Matrix() noexcept = default;
    Matrix(const Matrix<T,2,2>&) noexcept = default;
    Matrix<T, 2, 2>& operator= (const Matrix<T,2,2>&) noexcept = default;
    ~Matrix() noexcept = default;

    SINI_CUDA_COMPAT Matrix(T init_val) noexcept;
    SINI_CUDA_COMPAT explicit Matrix(const T* data_ptr) noexcept;
    SINI_CUDA_COMPAT Matrix(const Vector<T,2> (&init_rows)[2]) noexcept;
    SINI_CUDA_COMPAT Matrix(Vector<T,2> row1, Vector<T,2> row2) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Matrix(const Matrix<T2,2,2>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &a; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &a; }
    // Element access with bounds checking via assert
    SINI_CUDA_COMPAT T& at(uint32_t i, uint32_t j) noexcept;
    SINI_CUDA_COMPAT T  at(uint32_t i, uint32_t j) const noexcept;
    SINI_CUDA_COMPAT Vector<T,2> column(uint32_t i) const noexcept;
    // Element access without bounds checking
    SINI_CUDA_COMPAT T& operator() (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
    SINI_CUDA_COMPAT T  operator() (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

    SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
    SINI_CUDA_COMPAT void setColumn(uint32_t j, const Vector<T,2>& col) noexcept;

    static SINI_CUDA_COMPAT Matrix<T,2,2> identity() noexcept;
};
// Pre-defined 2x2 matrix types
using mat2 = Matrix<float, 2, 2>;
using mat2d = Matrix<double, 2, 2>;
using mat2i = Matrix<int32_t, 2, 2>;

// 3x3 Matrix
template<typename T>
struct Matrix<T, 3, 3> {
    // Nameless union to allow for getting rows/elements both via the
    // array or via e.g. mat.a
    union {
        struct { Vector<T, 3> row_vectors[3]; };
        struct {
            T   a, b, c,
                d, e, f,
                g, h, i;
        };
        struct {
            T   e00, e01, e02,
                e10, e11, e12,
                e20, e21, e22;
        };
    };

    Matrix() noexcept = default;
    Matrix(const Matrix<T,3,3>&) noexcept = default;
    Matrix<T, 3, 3>& operator= (const Matrix<T,3,3>&) noexcept = default;
    ~Matrix() noexcept = default;

    SINI_CUDA_COMPAT Matrix(T init_val) noexcept;
    SINI_CUDA_COMPAT explicit Matrix(const T* data_ptr) noexcept;
    SINI_CUDA_COMPAT Matrix(const Vector<T,3> (&init_rows)[3]) noexcept;
    SINI_CUDA_COMPAT Matrix(Vector<T,3> row1, Vector<T,3> row2, Vector<T,3> row3) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Matrix(const Matrix<T2,3,3>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &a; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &a; }
    // Element access with bounds checking via assert
    SINI_CUDA_COMPAT T& at(uint32_t i, uint32_t j) noexcept;
    SINI_CUDA_COMPAT T  at(uint32_t i, uint32_t j) const noexcept;
    SINI_CUDA_COMPAT Vector<T,3> column(uint32_t i) const noexcept;
    // Element access without bounds checking
    SINI_CUDA_COMPAT T& operator() (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
    SINI_CUDA_COMPAT T  operator() (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

    SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
    SINI_CUDA_COMPAT void setColumn(uint32_t j, const Vector<T,3>& col) noexcept;

    SINI_CUDA_COMPAT Matrix<T,2,2> submatrix(uint32_t i, uint32_t j) const noexcept;

    static SINI_CUDA_COMPAT Matrix<T,3,3> identity() noexcept;
};
// Pre-defined 3x3 matrices
using mat3 = Matrix<float, 3, 3>;
using mat3d = Matrix<double, 3, 3>;
using mat3i = Matrix<int32_t, 3, 3>;

// 4x4 Matrix
template<typename T>
struct Matrix<T, 4, 4> {
    // Nameless union to allow for getting rows/elements both via the
    // array or via e.g. mat.a
    union {
        struct { Vector<T, 4> row_vectors[4]; };
        struct {
            T   e00, e01, e02, e03,
                e10, e11, e12, e13,
                e20, e21, e22, e23,
                e30, e31, e32, e33;
        };
    };

    Matrix() noexcept = default;
    Matrix(const Matrix<T,4,4>&) noexcept = default;
    Matrix<T, 4, 4>& operator= (const Matrix<T,4,4>&) noexcept = default;
    ~Matrix() noexcept = default;

    SINI_CUDA_COMPAT Matrix(T init_val) noexcept;
    SINI_CUDA_COMPAT explicit Matrix(const T* data_ptr) noexcept;
    SINI_CUDA_COMPAT Matrix(const Vector<T,4> (&init_rows)[4]) noexcept;
    SINI_CUDA_COMPAT Matrix(Vector<T,4> row1, Vector<T,4> row2, Vector<T,4> row3, Vector<T,4> row4) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Matrix(const Matrix<T2, 4, 4>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &e00; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &e00; }
    // Element access with bounds checking via assert
    SINI_CUDA_COMPAT T& at(uint32_t i, uint32_t j) noexcept;
    SINI_CUDA_COMPAT T  at(uint32_t i, uint32_t j) const noexcept;
    SINI_CUDA_COMPAT Vector<T,4> column(uint32_t i) const noexcept;
    // Element access without bounds checking
    SINI_CUDA_COMPAT T& operator() (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
    SINI_CUDA_COMPAT T  operator() (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

    SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
    SINI_CUDA_COMPAT void setColumn(uint32_t j, const Vector<T,4>& col) noexcept;

    SINI_CUDA_COMPAT Matrix<T,3,3> submatrix(uint32_t i, uint32_t j) const noexcept;

    static SINI_CUDA_COMPAT Matrix<T,4,4> identity() noexcept;
};
// Pre-defined 4x4 matrices
using mat4 = Matrix<float, 4, 4>;
using mat4d = Matrix<double, 4, 4>;
using mat4i = Matrix<int32_t, 4, 4>;


// Printing and to string
// -----------------------------------------------------------------------------
template<uint32_t M, uint32_t N>
std::string toString(const Matrix<int32_t,M,N>& mat, bool linebreak = false);
template<uint32_t M, uint32_t N>
std::string toString(const Matrix<float,M,N>& mat, bool linebreak = false);
template<uint32_t M, uint32_t N>
std::string toString(const Matrix<double,M,N>& mat, bool linebreak = false);
template<typename T, uint32_t M, uint32_t N>
std::string toFormattedString(const Matrix<T,M,N>& mat, const char* format, bool linebreak = false);
template<typename T, uint32_t M, uint32_t N>
std::ostream& operator<< (std::ostream& ostream, const Matrix<T,M,N>& mat);


// Math functions (and some other utilities)
// -----------------------------------------------------------------------------

// Conversion between column vectors (sini::Vector) and row vectors
// (sini::Matrix<T,1,N>)
// Separated from transpose since they are separate structs
// Might be changed in the future
template<typename T, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,1,N> toRowVector(const Vector<T,N>& vec) noexcept;
template<typename T, uint32_t N>
SINI_CUDA_COMPAT Vector<T,N> toColumnVector(const Matrix<T,1,N>& mat) noexcept;

// Element-wise multiplication
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> eqElemMult(Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> elemMult(const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;

// Element-wise division
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> eqElemDiv(Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> elemDiv(const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;

// Element-wise exponentiation
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T,M,N>& mat, T exponent) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T,M,N>& mat, T exponent) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T, M, N>& mat, uint32_t exponent) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T, M, N>& mat, uint32_t exponent) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T,M,N>& mat, const Matrix<T,M,N>& exp_mat) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T,M,N>& mat, const Matrix<T,M,N>& exp_mat) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T,M,N>& mat, const Matrix<uint32_t,M,N>& exp_mat) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T,M,N>& mat, const Matrix<uint32_t,M,N>& exp_mat) noexcept;

// Transpose
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,N,M> transpose(const Matrix<T,M,N>& mat) noexcept;

// Power
template<typename T, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,N,N> eqPow(Matrix<T,N,N>& mat, uint32_t exponent) noexcept;
template<typename T, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,N,N> pow(const Matrix<T,N,N>& mat, uint32_t exponent) noexcept;

// Dimensions
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Vector<uint32_t,2> dimensions(const Matrix<T,M,N>&) noexcept
{
    return Vector<uint32_t, 2>(M, N);
}

// Hasher
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT size_t hash(const Matrix<T,M,N>& m) noexcept;

// Determinant of 2x2, 3x3 and 4x4 matrices
// For computing the determinant of an arbitrary matrix size include
// "sini/math/MatrixMath.hpp", which adds more advanced matrix operations
// like LU decomposition etc. (LU decomp. is used for computing larger
// determinants.)
template<typename T>
SINI_CUDA_COMPAT T det(const Matrix<T,2,2>& mat) noexcept;
template<typename T>
SINI_CUDA_COMPAT T det(const Matrix<T,3,3>& mat) noexcept;
template<typename T>
SINI_CUDA_COMPAT T det(const Matrix<T,4,4>& mat) noexcept;
//template<typename T, uint32_t M, uint32_t N>
//SINI_CUDA_COMPAT T det(const Matrix<T,M,N>& mat) noexcept;

// Submatrix determinant (a.k.a. minor) for 3x3, 4x4 and 5x5 matrices
template<typename T>
SINI_CUDA_COMPAT T minor(const Matrix<T,3,3>& mat, uint32_t i, uint32_t j) noexcept;
template<typename T>
SINI_CUDA_COMPAT T minor(const Matrix<T,4,4>& mat, uint32_t i, uint32_t j) noexcept;

// Adjugate matrix
template<typename T>
SINI_CUDA_COMPAT Matrix<T,2,2> adj(const Matrix<T,2,2>& mat) noexcept;
template<typename T>
SINI_CUDA_COMPAT Matrix<T,3,3> adj(const Matrix<T,3,3>& mat) noexcept;
template<typename T>
SINI_CUDA_COMPAT Matrix<T,4,4> adj(const Matrix<T,4,4>& mat) noexcept;

// Element-wise abs
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> abs(const Matrix<T,M,N>& mat) noexcept;

// Inverse of 2x2 and 3x3 matrices
// More general inverse and pseudo-inverse in "sini/math/MatrixMath.h"
template<typename T>
SINI_CUDA_COMPAT Matrix<T,2,2> inverse(const Matrix<T,2,2>& mat) noexcept;
template<typename T>
SINI_CUDA_COMPAT Matrix<T,3,3> inverse(const Matrix<T,3,3>& mat) noexcept;

// Max and min element
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT T maxElement(const Matrix<T,M,N>& mat) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT T minElement(const Matrix<T,M,N>& mat) noexcept;

//TODO add more functions?

// Operators
// -----------------------------------------------------------------------------

// Equality and inequality
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool operator== (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool operator!= (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;

// Addition
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator+= (Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator+ (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;

// Subtraction
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator-= (Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator- (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;

// Negation
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator- (const Matrix<T,M,N>& mat) noexcept;

// Multiplication for general matrices
template<typename T, uint32_t M, uint32_t N, uint32_t O>
SINI_CUDA_COMPAT Matrix<T,M,O> operator* (const Matrix<T,M,N>& left, const Matrix<T,N,O>& right) noexcept;
// Multiplication for square matrices
template<typename T, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,N,N> operator*= (Matrix<T,N,N>& left, const Matrix<T,N,N>& right) noexcept;

// Multiplication with vector (matrix*vector)
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Vector<T,M> operator* (const Matrix<T,M,N>& mat, const Vector<T,N>& vec) noexcept;
// vector*matrix multiplication must be done by converting the vector
// to a row vector, i.e. a 1xN matrix. The reason for this design is
// to only allow vector*matrix multiplication when you really want it
// (and hopefully know what you're doing)

// Multiplication with scalar
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator*= (Matrix<T,M,N>& mat, T scalar) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator* (const Matrix<T,M,N>& mat, T scalar) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator* (T scalar, const Matrix<T,M,N>& mat) noexcept;

// Division with scalar
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator/= (Matrix<T,M,N>& mat, T scalar) noexcept;
template<typename T, uint32_t M, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,M,N> operator/ (const Matrix<T,M,N>& mat, T scalar) noexcept;

//TODO Keep this?
// Operator version of pow
template<typename T, uint32_t N>
SINI_CUDA_COMPAT Matrix<T,N,N> operator^ (const Matrix<T,N,N>& mat, uint32_t exponent);

} // namespace sini

#include "math/Matrix.inl"
