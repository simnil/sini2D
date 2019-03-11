// Basic general math utilities and helper functions
namespace sini {

// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
SINI_CUDA_COMPAT bool approxEqual(float a, float b, float tol) noexcept
{
    return (a <= b + tol) && (a >= b - tol);
}

SINI_CUDA_COMPAT bool approxEqual(double a, double b, double tol) noexcept
{
    return (a <= b + tol) && (a >= b - tol);
}

} // namespace sini
