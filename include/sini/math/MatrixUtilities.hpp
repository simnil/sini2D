// Basic math utilities and helper functions exclusively for matrices
#pragma once

#include "sini/math/MathUtilitiesBase.hpp"
#include "sini/math/VectorUtilities.hpp"
#include "sini/math/Matrix.hpp"

namespace sini {

// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
template<uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Matrix<float,M,N>& a,
                                  const Matrix<float,M,N>& b,
                                  float tol = DEFAULT_TOLERANCE) noexcept;
template<uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Matrix<double,M,N>& a,
                                  const Matrix<double,M,N>& b,
                                  double tol = DEFAULT_TOLERANCE_DOUBLE) noexcept;

} // namespace sini

#include "sini/math/MatrixUtilities.inl"
