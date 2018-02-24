// Basic math utilities and helper functions exclusively for matrices
#pragma once
namespace sini {

// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
// float
template<uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Matrix<float,M,N>& a,
    const Matrix<float,M,N>& b, float tol) noexcept
{
    const float *a_data = a.data(),
        *b_data = b.data();
    for (uint32_t i = 0; i < M*N; i++)
        if (!approxEqual(a_data[i], b_data[i], tol)) return false;
    return true;
}

// double
template<uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Matrix<double,M,N>& a,
    const Matrix<double,M,N>& b, double tol) noexcept
{
    const double *a_data = a.data(),
        *b_data = b.data();
    for (uint32_t i = 0; i < M*N; i++)
        if (!approxEqual(a_data[i], b_data[i], tol)) return false;
    return true;
}

} // namespace sini
