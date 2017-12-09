#include "sini/geometry/Line.hpp"

namespace sini {

// Line member functions
// =============================================================================
SINI_CUDA_COMPAT Line::Line(vec2 p, vec2 sir) noexcept
    : p(p), dir(dir)
{}

SINI_CUDA_COMPAT bool Line::intersects(vec2 point) noexcept
{
    // TODO
}


// LineSegment member functions
// =============================================================================
SINI_CUDA_COMPAT LineSegment::LineSegment(vec2 p1, vec2 p2) noexcept
    : p1(p1), p2(p2)
{}

SINI_CUDA_COMPAT LineSegment::extrapolate() noexcept
{
    // TODO
}

SINI_CUDA_COMPAT bool LineSegment::intersects(vec2 point) noexcept
{
    // TODO
}


// Intersection functions
// =============================================================================
SINI_CUDA_COMPAT bool intersect(Line l1, Line l2) noexcept
{
    // TODO
}

SINI_CUDA_COMPAT bool intersect(LineSegment l1, LineSegment l2) noexcept
{
    // TODO
}

SINI_CUDA_COMPAT bool intersect(Line l1, LineSegment l2) noexcept
{
    // TODO
}

SINI_CUDA_COMPAT IntersectionPoint intersection(Line l1, Line l2) noexcept
{
    // TODO
}

SINI_CUDA_COMPAT IntersectionPoint intersection(LineSegment l1, LineSegment l2) noexcept
{
    // TODO
}

SINI_CUDA_COMPAT IntersectionPoint intersection(Line l1, LineSegment l2) noexcept
{
    // TODO
}

} // namespace sini
