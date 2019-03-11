// Basic math utilities for and helper functions exclusively for vectors
namespace sini {

// Approximate equality, for floating point numbers
// -----------------------------------------------------------------------------
// float
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
SINI_CUDA_COMPAT bool approxEqual(const Vector<float,N>& a,
    const Vector<float,N>& b, float tol) noexcept
{
    for (uint32_t i = 0; i < N; i++)
        if (!approxEqual(a[i], b[i], tol)) return false;
    return true;
}

// double
SINI_CUDA_COMPAT bool approxEqual(vec2d a, vec2d b, double tol) noexcept
{
    return approxEqual(a.x, b.x, tol)
        && approxEqual(a.y, b.y, tol);
}

SINI_CUDA_COMPAT bool approxEqual(vec3d a, vec3d b, double tol) noexcept
{
    return approxEqual(a.x, b.x, tol)
        && approxEqual(a.y, b.y, tol)
        && approxEqual(a.z, b.z, tol);
}

SINI_CUDA_COMPAT bool approxEqual(vec4d a, vec4d b, double tol) noexcept
{
    return approxEqual(a.x, b.x, tol)
        && approxEqual(a.y, b.y, tol)
        && approxEqual(a.z, b.z, tol)
        && approxEqual(a.w, b.w, tol);
}

template<uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Vector<double,N>& a,
    const Vector<double,N>& b, double tol) noexcept
{
    for (uint32_t i = 0; i < N; i++)
        if (!approxEqual(a[i], b[i], tol)) return false;
    return true;
}

} // namespace sini
