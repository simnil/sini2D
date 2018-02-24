// Basic math utilities and helper functions exclusively for matricesx
#pragma once
#ifndef SINI_MATRIX_UTIL_H
#define SINI_MATRIX_UTIL_H

#include "sini/math/Matrix.hpp"
#include "sini/math/MathUtilitiesBase.hpp"
#include "sini/math/VectorUtilities.hpp"

namespace sini {

using std::int32_t;
using std::uint32_t;

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
#endif // SINI_MATRIX_UTIL_H
