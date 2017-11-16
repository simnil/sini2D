// SiNi vector class template
//
// This code has been heavily inspired by the Vector.hpp code in sfzCore,
// written by my friend Peter Hillerström (github.com/PetorSFZ).
// The quality of this code does not neccessarily represent the quality of
// sfzCore or any of his work.

#pragma once
#ifndef SINI_MATH_VECTOR_H
#define SINI_MATH_VECTOR_H

#include "sini/CudaCompat.hpp"
#include <cstddef>      // For std::size_t
#include <cstdint>      // For std::int32_t, std::uint32_t
#include <cmath>        // For std::abs, std::sqrt, std::pow
#include <limits>       // For std::numeric_limits<T>
#include <assert.h>
#include <functional>   // For std::hash, for compatibility with unordered_map etc.


namespace sini {

using std::size_t;
using std::uint32_t;
using std::int32_t;


// ---------------
// General vector
// ---------------
template<typename T, uint32_t n>
struct Vector {
    T components[n];

    Vector() noexcept = default;
    Vector(const Vector<T,n>&) noexcept = default;
    Vector<T,n>& operator= (const Vector<T,n>&) noexcept = default;
    ~Vector() noexcept = default;

    SINI_CUDA_COMPAT Vector(T init_val);
    SINI_CUDA_COMPAT explicit Vector(const T* init_array) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Vector(const Vector<T2,n>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return components; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return components; }
    // Element access
    SINI_CUDA_COMPAT T& operator[] (uint32_t index) noexcept;
    SINI_CUDA_COMPAT const T& operator[] (uint32_t index) const noexcept;
};


// ---------------------------------------------------
// Common vector forms, with a bit more functionality
// ---------------------------------------------------
// 2D vector
template<typename T>
struct Vector<T, 2> {
    // Unnamed union makes it possible to get components by writing e.g. vec.x
    // Trick Peter Hillerström (github.com/PetorSFZ) teached me.
    union {
        T components[2];
        struct { T x, y; };
    };

    Vector() noexcept = default;
    Vector(const Vector<T,2>&) noexcept = default;
    Vector<T,2>& operator= (const Vector<T,2>&) noexcept = default;
    ~Vector() noexcept = default;

    SINI_CUDA_COMPAT Vector(T init_val) noexcept;
    SINI_CUDA_COMPAT Vector(T x, T y) noexcept;
    SINI_CUDA_COMPAT explicit Vector(const T* init_arr) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Vector(const Vector<T2,2>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &x; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &x; }
    // Element access
    SINI_CUDA_COMPAT T& operator[] (uint32_t index) noexcept;
    SINI_CUDA_COMPAT const T& operator[] (uint32_t index) const noexcept;
};
// Predefined 2D vectors
using vec2  = Vector<float, 2>;
using vec2d = Vector<double, 2>;
using vec2i = Vector<int32_t, 2>;


// 3D vector
template<typename T>
struct Vector<T, 3> {
    union {
        T components[3];
        struct { T x, y, z; };
        // Makes it possible to get 2D vector by writing vec.xy or vec.yz
        struct { Vector<T,2> xy; T z_; };
        struct { T x_; Vector<T,2> yz; };
    };

    Vector() noexcept = default;
    Vector(const Vector<T,3>&) noexcept = default;
    Vector<T,3>& operator= (const Vector<T,3>&) noexcept = default;
    ~Vector() noexcept = default;

    SINI_CUDA_COMPAT Vector(T init_val) noexcept;
    SINI_CUDA_COMPAT Vector(T x, T y, T z) noexcept;
    SINI_CUDA_COMPAT Vector(Vector<T,2> xy, T z) noexcept;
    SINI_CUDA_COMPAT Vector(T x, Vector<T,2> yz) noexcept;
    SINI_CUDA_COMPAT explicit Vector(const T* init_arr) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Vector(const Vector<T2,3>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &x; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &x; }
    // Element access
    SINI_CUDA_COMPAT T& operator[] (uint32_t index) noexcept;
    SINI_CUDA_COMPAT const T& operator[] (uint32_t index) const noexcept;
};
// Predefined 3D vectors
using vec3  = Vector<float, 3>;
using vec3d = Vector<double, 3>;
using vec3i = Vector<int32_t, 3>;


// 4D vector
template<typename T>
struct Vector<T, 4> {
public:
    union {
        T components[4];
        struct { T x, y, z, w; };
        struct { Vector<T,2> xy, zw; };
        struct { Vector<T,3> xyz; T w_; };
        struct { T x_; Vector<T,3> yzw; };
        struct { T x__; Vector<T,2> yz; T w__; };
    };

    Vector() noexcept = default;
    Vector(const Vector<T,4>&) noexcept = default;
    Vector<T,4>& operator= (const Vector<T,4>&) noexcept = default;
    ~Vector() noexcept = default;

    SINI_CUDA_COMPAT Vector(T init_val) noexcept;
    SINI_CUDA_COMPAT Vector(T x, T y, T z, T w) noexcept;
    SINI_CUDA_COMPAT Vector(Vector<T,2> xy, T z, T w) noexcept;
    SINI_CUDA_COMPAT Vector(T x, T y, Vector<T, 2> zw) noexcept;
    SINI_CUDA_COMPAT Vector(T x, Vector<T, 2> yz, T w) noexcept;
    SINI_CUDA_COMPAT Vector(Vector<T,2> xy, Vector<T,2> zw) noexcept;
    SINI_CUDA_COMPAT Vector(Vector<T,3> xyz, T w) noexcept;
    SINI_CUDA_COMPAT Vector(T x, Vector<T,3> yzw) noexcept;
    SINI_CUDA_COMPAT explicit Vector(const T* init_arr) noexcept;

    template<typename T2>
    SINI_CUDA_COMPAT Vector(const Vector<T2,4>& other) noexcept;

    // Pointer to data
    SINI_CUDA_COMPAT T* data() noexcept { return &x; }
    SINI_CUDA_COMPAT const T* data() const noexcept { return &x; }
    // Element access
    SINI_CUDA_COMPAT T& operator[] (uint32_t index) noexcept;
    SINI_CUDA_COMPAT const T& operator[] (uint32_t index) const noexcept;
};
// Pre-defined 4D vectors
using vec4 = Vector<float, 4>;
using vec4d = Vector<double, 4>;
using vec4i = Vector<int32_t, 4>;


// Math functions
// -----------------------------------------------------------------------------

// Dot/scalar product
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T dot(const Vector<T,n>& v1, const Vector<T,n>& v2) noexcept;

// Cross product
template<typename T>
SINI_CUDA_COMPAT Vector<T,3> cross(Vector<T,3> v1, Vector<T,3> v2) noexcept;
//TODO: higher dimension/general cross product

// Vector norm
template<uint32_t n>
SINI_CUDA_COMPAT float norm(const Vector<int32_t,n>& v, int32_t N = 2) noexcept;
template<uint32_t n>
SINI_CUDA_COMPAT float norm(const Vector<float,n>& v, int32_t N = 2) noexcept;
template<uint32_t n>
SINI_CUDA_COMPAT double norm(const Vector<double,n>& v, int32_t N = 2) noexcept;
// The euclidean length is the same as the 2-norm
template<uint32_t n>
SINI_CUDA_COMPAT float length(const Vector<int32_t,n>& v) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T length(const Vector<T,n>& v) noexcept;

// Squared norm -- for performance
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T normPowered(const Vector<T,n>& v, int32_t N = 2) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T lengthSquared(const Vector<T,n>& v) noexcept;

// Abs
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> abs(const Vector<T,n>& v) noexcept;

// Returns a normalized version of the vector
// Remove integer version? It's better to cast to float and use normalize
// on the casted vector
template<uint32_t n>
SINI_CUDA_COMPAT Vector<float,n> normalize(const Vector<int32_t,n>& v);
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> normalize(const Vector<T,n>& v);

// Dimension
template<typename T, uint32_t n>
SINI_CUDA_COMPAT uint32_t dimension(const Vector<T,n>& v) noexcept { return n; }

// Max and min element
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T maxElement(const Vector<T,n>& vec) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T minElement(const Vector<T,n>& vec) noexcept;

// Hash
template<typename T, uint32_t n>
SINI_CUDA_COMPAT size_t hash(const Vector<T,n>& v) noexcept;

//TODO: add more math functions


// Operators
// -----------------------------------------------------------------------------

// Equality
template<typename T, uint32_t n>
SINI_CUDA_COMPAT bool operator== (const Vector<T,n>& left, const Vector<T,n>& right) noexcept;
// Inequaltity
template<typename T, uint32_t n>
SINI_CUDA_COMPAT bool operator!= (const Vector<T,n>& left, const Vector<T,n>& right) noexcept;

// Addition
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T, n>& operator+= (Vector<T,n>& left, const Vector<T,n>& right) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator+ (const Vector<T,n>& left, const Vector<T,n>& right) noexcept;

// Subtraction
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator-= (Vector<T,n>& left, const Vector<T,n>& right) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator- (const Vector<T,n>& left, const Vector<T,n>& right) noexcept;

// Negation
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator- (const Vector<T,n>& vector) noexcept;

// Multiplication with scalar
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator*= (Vector<T,n>& vector, T scalar) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator* (const Vector<T,n>& vector, T scalar) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator* (T scalar, const Vector<T,n>& vector) noexcept;

// Element-wise multiplication
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator*= (Vector<T,n>& left, const Vector<T,n>& right) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator* (const Vector<T,n>& left, const Vector<T,n>& right) noexcept;

// Division with scalar
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator/= (Vector<T,n>& vector, T scalar) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator/ (const Vector<T,n>& vector, T scalar) noexcept;

// Element-wise division
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator/= (Vector<T,n>& left, const Vector<T,n>& right) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator/ (const Vector<T,n>& left, const Vector<T,n>& right) noexcept;

// Modulus with scalar
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T, n>& operator%= (Vector<T, n>& vector, T scalar) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T, n> operator% (const Vector<T, n>& vector, T scalar) noexcept;

// Element-wise modulus
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator%= (Vector<T,n>& left, const Vector<T,n>& right) noexcept;
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator% (const Vector<T,n>& left, const Vector<T,n>& right) noexcept;

// Possibly bad practice to use operators differently to the built-in version
// But I think the chosen operators make sense
// Operator version of scalar product
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T operator| (const Vector<T,n>& left, const Vector<T,n>& right) noexcept { return dot(left, right); }
// Operator version of cross product
template<typename T>
SINI_CUDA_COMPAT Vector<T,3> operator^ (const Vector<T,3>& v1, const Vector<T,3>& v2) noexcept { return cross(v1, v2); }

} // namespace sini

#include "sini/math/Vector.inl"
#endif // !SINI_VECTOR_H
