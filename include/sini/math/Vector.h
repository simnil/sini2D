// SiNi vector class template
//
// This code has been inspired by or based on the Vector.hpp code in sfzCore,
// written by Peter Hillerström (github.com/PetorSFZ) -- which is superior to this code.
// I've taken to writing very similar code for my own learning purposes, but this is not
// necessarily representative of the code I've gotten inspiration from.
//
// Simon Nilsson (sim.f.nilsson@gmail.com)

#pragma once
#ifndef SINI_MATH_VECTOR_H
#define SINI_MATH_VECTOR_H

#include <cstddef>		// For std::size_t
#include <cstdint>		// For std::in32_t
#include <cmath>		// For std::abs, std::sqrt, std::pow
#include <assert.h>
#include <functional>	// For std::hash, for compatibility with unordered_map etc.


using std::size_t;
using std::int32_t;

namespace sini {

	// General vector
	template<typename T, size_t n>
	struct Vector {
		T components[n];

		Vector() noexcept;
		Vector(T initVal);
		Vector(const Vector<T, n>&) noexcept = default;
		explicit Vector(const T* initArray) noexcept;
		~Vector() noexcept = default;

		template<typename T2>
		Vector(const Vector<T2, n>& other) noexcept;

		Vector<T, n>& operator= (const Vector<T, n>&) noexcept = default;
		T	operator[] (const size_t index) const noexcept;
		T&	operator[] (const size_t index) noexcept;
	};


	// 2D vector
	template<typename T>
	struct Vector<T, 2> {
		// Union makes it possible to get components by writing e.g. vec.x
		// Trick Peter Hillerström (github.com/PetorSFZ) teached me.
		union {
			T components[2];
			struct { T x, y; };
		};

		Vector() noexcept;
		Vector(T initVal) noexcept;
		Vector(T x, T y) noexcept;
		Vector(const Vector<T, 2>&) noexcept = default;
		explicit Vector(const T* initArray) noexcept;
		~Vector() noexcept = default;

		template<typename T2>
		Vector(const Vector<T2, 2>& other) noexcept;

		Vector<T, 2>& operator= (const Vector<T, 2>&) noexcept = default;
		T	operator[] (const size_t index) const noexcept;
		T&	operator[] (const size_t index) noexcept;


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
			struct { Vector<T, 2> xy; T z; };
			struct { T x; Vector<T, 2> yz; };
		};

		Vector() noexcept;
		Vector(T initVal) noexcept;
		Vector(T x, T y, T z) noexcept;
		Vector(const Vector<T, 3>&) noexcept = default;
		explicit Vector(const T* initArray) noexcept;
		~Vector() noexcept = default;

		template<typename T2>
		Vector(const Vector<T2, 3>& other) noexcept;

		Vector<T, 3>& operator= (const Vector<T, 3>&) noexcept = default;
		T	operator[] (const size_t index) const noexcept;
		T&	operator[] (const size_t index) noexcept;
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
			struct { Vector<T, 2> xy, zw; };
			struct { Vector<T, 3> xyz; T w; };
			struct { T x; Vector<T, 2> yz; T w; };
		};

		Vector() noexcept;
		Vector(T initVal) noexcept;
		Vector(T x, T y, T z, T w) noexcept;
		Vector(const Vector<T, 4>&) noexcept = default;
		explicit Vector(const T* initArray) noexcept;
		~Vector() noexcept = default;

		template<typename T2>
		Vector(const Vector<T2, 4>& other) noexcept;

		Vector<T, 4>& operator= (const Vector<T, 4>&) noexcept = default;
		T	operator[] (const size_t index) const noexcept;
		T&	operator[] (const size_t index) noexcept;
	};


	// Math functions
	// -----------------------------------------------------------------------------

	// Dot/scalar product
	template<size_t n>
	float dot(const Vector<int32_t, n>& v1, const Vector<int32_t, n>& v2);
	template<size_t n>
	float dot(const Vector<float, n>& v1, const Vector<float, n>& v2);
	template<size_t n>
	double dot(const Vector<double, n>& v1, const Vector<double, n>& v2);

	// Cross product
	template<typename T>
	Vector<T, 3> cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2);
	//TODO: higher dimension/general cross product

	// Vector norm
	template<size_t n>
	float norm(const Vector<int32_t, n>& v, int32_t N = 2);
	template<size_t n>
	float norm(const Vector<float, n>& v, int32_t N = 2);
	template<size_t n>
	double norm(const Vector<double, n>& v, int32_t N = 2);
	// The euclidean length is the same as the 2-norm
	template<size_t n>
	float length(const Vector<int32_t, n>& v);
	template<typename T, size_t n>
	T length(const Vector<T, n>& v);

	// Squared norm -- for performance
	template<typename T, size_t n>
	T normPowered(const Vector<T, n>& v, int32_t N = 2);
	template<typename T, size_t n>
	T lengthSquared(const Vector<T, n>& v);

	// Abs
	template<typename T, size_t n>
	Vector<T, n> abs(const Vector<T, n>& v);

	// Returns a normalized version of the vector
	template<size_t n>
	Vector<float, n> normalize(const Vector<int32_t, n>& v);
	template<typename T, size_t n>
	Vector<T, n> normalize(const Vector<T, n>& v);
	template<size_t n>
	Vector<int32_t, n> normalizeInt(const Vector<int32_t, n>& v);

	// Dimension
	template<typename T, size_t n>
	size_t dimension(const Vector<T, n>& v) { return n; }

	template<typename T, size_t n>
	size_t hash(const Vector<T, n>& v) noexcept;

	//TODO: add more math functions


	// Operators
	// -----------------------------------------------------------------------------

	// Equality
	template<typename T, size_t n>
	bool operator== (const Vector<T, n>& left, const Vector<T, n>& right);
	// Inequaltity
	template<typename T, size_t n>
	bool operator!= (const Vector<T, n>& left, const Vector<T, n>& right);
	
	// Addition
	template<typename T, size_t n>
	Vector<T, n>& operator+= (Vector<T, n>& left, const Vector<T, n>& right);
	template<typename T, size_t n>
	Vector<T, n> operator+ (const Vector<T, n>& left, const Vector<T, n>& right);

	// Subtraction
	template<typename T, size_t n>
	Vector<T, n>& operator-= (Vector<T, n>& left, const Vector<T, n>& right);
	template<typename T, size_t n>
	Vector<T, n> operator- (const Vector<T, n>& left, const Vector<T, n>& right);

	// Multiplication with scalar
	template<typename T, size_t n>
	Vector<T, n>& operator*= (Vector<T, n>& vector, T scalar);
	template<typename T, size_t n>
	Vector<T, n> operator* (const Vector<T, n>& vector, T scalar);
	template<typename T, size_t n>
	Vector<T, n> operator* (T scalar, const Vector<T, n>& vector);

	// Pointwise mutliplication
	template<typename T, size_t n>
	Vector<T, n>& operator*= (Vector<T, n>& left, const Vector<T, n>& right);
	template<typename T, size_t n>
	Vector<T, n> operator* (const Vector<T, n>& left, const Vector<T, n>& right);

	// Division with scalar
	template<typename T, size_t n>
	Vector<T, n>& operator/= (Vector<T, n>& vector, T scalar);
	template<typename T, size_t n>
	Vector<T, n> operator/ (const Vector<T, n>& vector, T scalar);

	// Pointwise modulus
	template<typename T, size_t n>
	Vector<T, n>& operator%= (Vector<T, n>& left, const Vector<T, n>& right);
	template<typename T, size_t n>
	Vector<T, n> operator% (const Vector<T, n>& left, const Vector<T, n>& right);
	// Modulus with scalar
	template<typename T, size_t n>
	Vector<T, n>& operator%= (Vector<T, n>& vector, T scalar);
	template<typename T, size_t n>
	Vector<T, n> operator% (const Vector<T, n>& vector, T scalar);

	// Possibly bad practice to use operators differently to the built-in version
	// But I think the chosen operators make sense
	// Operator version of scalar product
	template<size_t n>
	float operator| (const Vector<int32_t, n>& left, const Vector<int32_t, n>& right) { return dot(left, right); }
	template<size_t n>
	float operator| (const Vector<float, n>& left, const Vector<float, n>& right) { return dot(left, right); }
	template<size_t n>
	double operator| (const Vector<double, n>& left, const Vector<double, n>& right) { return dot(left, right); }
	// Operator version of cross product
	template<typename T>
	Vector<T, 3> operator^ (const Vector<T, 3>& v1, const Vector<T, 3>& v2) { return cross(v1, v2); }





}

#include "Vector-defs.h"
#endif