// Utilities for testing code with Catch
// Contains specialized utilities for use with things specific to sini, like
// sini::Vector and sini::Matrix
#pragma once
#ifndef SINI_TEST_UTILS_H
#define SINI_TEST_UTILS_H

#include "catch.hpp"
#include "sini/math/Vector.hpp"
#include "sini/math/Matrix.hpp"
#include "sini/math/MathUtilities.hpp"

namespace sini::test {

#define REQUIRE_APPROX_EQUAL(a, ...)                \
    INFO("\r" << __FILE__ << "(" << __LINE__        \
        << "):\nREQUIRE_APPROX_EQUAL(" #a ", "      \
        #__VA_ARGS__ ")");                          \
    sini::test::requireApproxEqual(a, __VA_ARGS__)


// Scalars
// -----------------------------------------------------------------------------
void requireApproxEqual(float a, float b, float tol = DEFAULT_TOLERANCE);
void requireApproxEqual(double a, double b, double tol = DEFAULT_TOLERANCE_DOUBLE);


// Vectors
// -----------------------------------------------------------------------------
template<uint32_t N>
void requireApproxEqual(const Vector<float,N>& a, const Vector<float,N>& b,
    float tol = DEFAULT_TOLERANCE);
template <uint32_t N>
void requireApproxEqual(const Vector<double,N>& a, const Vector<double,N>& b,
    double tol = DEFAULT_TOLERANCE_DOUBLE);


// Matrices
// -----------------------------------------------------------------------------
template<uint32_t M, uint32_t N>
void requireApproxEqual(const Matrix<float,M,N>& a, const Matrix<float,M,N>& b,
    float tol = DEFAULT_TOLERANCE);
template<uint32_t M, uint32_t N>
void requireApproxEqual(const Matrix<double,M,N>& a, const Matrix<double,M,N>& b,
    double tol = DEFAULT_TOLERANCE_DOUBLE);

} // namespace sini::test

#include "sini/util/testutil.inl"
#endif // SINI_TEST_UTILS_H
