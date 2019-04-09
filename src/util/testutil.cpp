// Utilities for testing code with Catch
// Contains specialized utilities for use with things specific to sini, like
// sini::Vector and sini::Matrix
#include <sini2D/util/testutil.hpp>

namespace sini::test {

void requireApproxEqual(float a, float b, float tol)
{
    if (!approxEqual(a, b, tol))
        FAIL("\rwith expansion:\n" << a << " ~ " << b);
}

void requireApproxEqual(double a, double b, double tol)
{
    if (!approxEqual(a, b, tol))
        FAIL("\rwith expansion:\n" << a << " ~ " << b);
}


} // namespace sini::test
