// Basic general math utilities and helper functions
#pragma once
#ifndef SINI_MATH_UTILITIES_BASE_H
#define SINI_MATH_UTILITIES_BASE_H

#include <cstdint>  // For std::int32_t, std::uint32_t
#include <cmath>    // For std::abs, std::sqrt, std::pow

#include "sini/CudaCompat.hpp"

namespace sini {

using std::int32_t;
using std::uint32_t;

// Math util constants
// -----------------------------------------------------------------------------
constexpr float  DEFAULT_TOLERANCE        = 1e-5f;
constexpr double DEFAULT_TOLERANCE_DOUBLE = static_cast<double>(DEFAULT_TOLERANCE);

// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
SINI_CUDA_COMPAT bool approxEqual(float a, float b, float tol = DEFAULT_TOLERANCE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(double a, double b, double tol = DEFAULT_TOLERANCE_DOUBLE) noexcept;

} // namespace sini

#include "sini/math/MathUtilitiesBase.inl"
#endif // SINI_MATH_UTILITIES_BASE_H
