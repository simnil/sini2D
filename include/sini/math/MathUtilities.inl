// Basic math utilities and helper functions

#pragma once
namespace sini {

// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
// Scalars
SINI_CUDA_COMPAT bool approxEqual(float a, float b, float tol) noexcept
{
    return (a <= b + tol) && (a >= b - tol);
}

// Vectors
SINI_CUDA_COMPAT bool approxEqual(vec2 a, vec2 b, float tol) noexcept
{
    return approxEqual(a.x, b.x, tol)
        && approxEqual(a.y, b.y, tol);
}
SINI_CUDA_COMPAT bool approxEqual(vec3 a, vec3 b, float tol) noexcept
{
    return approxEqual(a.x, b.x, tol)
        && approxEqual(a.y, b.y, tol)
        && approxEqual(a.z, b.z, tol);
}
SINI_CUDA_COMPAT bool approxEqual(vec4 a, vec4 b, float tol) noexcept
{
    return approxEqual(a.x, b.x, tol)
        && approxEqual(a.y, b.y, tol)
        && approxEqual(a.z, b.z, tol)
        && approxEqual(a.w, b.w, tol);
}
template<uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Vector<float,N>& a, const Vector<float,N>& b, float tol) noexcept
{
    for (uint32_t i = 0; i < N; i++)
        if (!approxEqual(a[i], b[i], tol)) return false;
    return true;
}

// Matrices
template<uint32_t M, uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Matrix<float,M,N>& a, const Matrix<float,M,N>& b, float tol) noexcept
{
    const float *a_data = a.data(),
        *b_data = b.data();
    for (uint32_t i = 0; i < M*N; i++)
        if (!approxEqual(a_data[i], b_data[i], tol)) return false;
    return true;
}

} // namespace sini
