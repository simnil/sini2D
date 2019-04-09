// Basic general math utilities and helper functions
#pragma once

#include <sini2D/CudaCompat.hpp>

#include <cstdint>  // For std::int32_t, std::uint32_t
#include <cmath>    // For std::abs, std::sqrt, std::pow


namespace sini {

// Math util constants
// -----------------------------------------------------------------------------
constexpr double DEFAULT_TOLERANCE_DOUBLE = 1e-5;
constexpr float  DEFAULT_TOLERANCE        = static_cast<float>(DEFAULT_TOLERANCE_DOUBLE);

// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
SINI_CUDA_COMPAT bool approxEqual(float a, float b, float tol = DEFAULT_TOLERANCE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(double a, double b, double tol = DEFAULT_TOLERANCE_DOUBLE) noexcept;

} // namespace sini

#include "MathUtilitiesBase.inl"
