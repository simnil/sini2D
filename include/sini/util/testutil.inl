// Utilities for testing code with Catch
// Contains specialized utilities for use with things specific to sini, like
// sini::Vector and sini::Matrix
#pragma once

namespace sini::test {

template<uint32_t N>
void requireApproxEqual(const Vector<float,N>& a, const Vector<float,N>& b, float tol)
{
    if (!approxEqual(a, b, tol))
        FAIL("\rwith expansion:\n" << a << " ~ " << b);
}
template<uint32_t N>
void requireApproxEqual(const Vector<double,N>& a, const Vector<double,N>& b, double tol)
{
    if (!approxEqual(a, b, tol))
        FAIL("\rwith expansion:\n" << a << " ~ " << b);
}

template<uint32_t M, uint32_t N>
void requireApproxEqual(const Matrix<float,M,N>& a, const Matrix<float,M,N>& b, float tol)
{
    if (!approxEqual(a, b, tol))
        FAIL("\rwith expansion:\n" << a << " ~ " << b);
}
template<uint32_t M, uint32_t N>
void requireApproxEqual(const Matrix<double,M,N>& a, const Matrix<double,M,N>& b, double tol)
{
    if (!approxEqual(a, b, tol))
        FAIL("\rwith expansion:\n" << a << " ~ " << b);
}

} // namespace sini::test
