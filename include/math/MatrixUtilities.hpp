// Basic math utilities and helper functions exclusively for matrices
#pragma once

#include "math/MathUtilitiesBase.hpp"
#include "math/VectorUtilities.hpp"
#include "math/Matrix.hpp"

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

#include "math/MatrixUtilities.inl"
