// Basic math utilities and helper functions

#pragma once
#ifndef SINI_MATH_UTILITIES_H
#define SINI_MATH_UTILITIES_H

#include <cstdint>  // For std::int32_t, std::uint32_t
#include <cmath>    // For std::abs, std::sqrt, std::pow

#include "sini/CudaCompat.hpp"
#include "sini/math/Vector.hpp"
#include "sini/math/Matrix.hpp"

namespace sini {

using std::int32_t;
using std::uint32_t;


// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
constexpr float DEFAULT_TOLERANCE = 1e-5f;
// Scalars
SINI_CUDA_COMPAT bool approxEqual(float a, float b, float tol = DEFAULT_TOLERANCE) noexcept;
// Vectors
SINI_CUDA_COMPAT bool approxEqual(vec2 a, vec2 b, float tol = DEFAULT_TOLERANCE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(vec3 a, vec3 b, float tol = DEFAULT_TOLERANCE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(vec4 a, vec4 b, float tol = DEFAULT_TOLERANCE) noexcept;
template<uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Vector<float,N>& a, const Vector<float,N>& b, float tol = DEFAULT_TOLERANCE) noexcept;
// Matrices
template<uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Matrix<float,M,N>& a, const Matrix<float,M,N>& b, float tol = DEFAULT_TOLERANCE) noexcept;

} // namespace sini

#include "sini/math/MathUtilities.inl"
#endif // SINI_MATH_UTILITIES_H
