#pragma once

#include <sini2D/CudaCompat.hpp>
#include <sini2D/math/Vector.hpp>
#include <sini2D/math/MathUtilities.hpp>


namespace sini {

// Declare Line and LineSegment to make full declaration order irrelevant
struct LineSegment;
struct Line;

struct IntersectionPoint {
    bool intersect;
    vec2 intersection_point;
};

struct IntersectionDistance {
    bool intersect;
    float intersection_distance;
};

// A two-dimensional, infinitely long line. Any point, x, on the line can be
// described by x = p + k * dir, where k is a real number.
struct Line {
    vec2 p, dir;

    Line() noexcept = default;
    Line(const Line&) noexcept = default;
    Line& operator= (const Line&) noexcept = default;
    ~Line() noexcept = default;

    SINI_CUDA_COMPAT Line(vec2 p, vec2 dir) noexcept;

    SINI_CUDA_COMPAT Line& normalizeDir() noexcept;

    SINI_CUDA_COMPAT bool intersects(vec2 point, float tol = DEFAULT_TOLERANCE) const noexcept;
    SINI_CUDA_COMPAT bool intersectsAlongDirection(vec2 point, float tol = DEFAULT_TOLERANCE) const noexcept;
    SINI_CUDA_COMPAT bool intersectsAlongDirection(Line line) const noexcept;
    SINI_CUDA_COMPAT bool intersectsAlongDirection(LineSegment line) const noexcept;

};

// A two-dimensional line from p1 to p2
struct LineSegment {
    vec2 p1, p2;

    LineSegment() noexcept = default;
    LineSegment(const LineSegment&) noexcept = default;
    LineSegment& operator= (const LineSegment&) noexcept = default;
    ~LineSegment() noexcept = default;

    SINI_CUDA_COMPAT LineSegment(vec2 p1, vec2 p2) noexcept;

    SINI_CUDA_COMPAT Line extrapolate() const noexcept;
    SINI_CUDA_COMPAT bool intersects(vec2 point, float tol = DEFAULT_TOLERANCE) const noexcept;

};

// Intersection functions
// ----------------------
SINI_CUDA_COMPAT bool intersect(Line l1, Line l2) noexcept;
SINI_CUDA_COMPAT bool intersect(LineSegment l1, LineSegment l2) noexcept;
SINI_CUDA_COMPAT bool intersect(Line l1, LineSegment l2) noexcept;
SINI_CUDA_COMPAT bool intersect(LineSegment l1, Line l2) noexcept { return intersect(l2, l1); }

SINI_CUDA_COMPAT IntersectionPoint intersection(Line l1, Line l2) noexcept;
SINI_CUDA_COMPAT IntersectionPoint intersection(LineSegment l1, LineSegment l2) noexcept;
SINI_CUDA_COMPAT IntersectionPoint intersection(Line l1, LineSegment l2) noexcept;
SINI_CUDA_COMPAT IntersectionPoint intersection(LineSegment l1, Line l2) noexcept { return intersection(l2, l1); }

// Obtain the distance from the "start" point, 'p' (or 'p1'), to the
// intersection point in units of direction vector length (or length
// of the line segment)
SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(Line l1, Line l2) noexcept;
SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(Line l1, LineSegment l2) noexcept;
SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(LineSegment l1, LineSegment l2) noexcept;
SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(LineSegment l1, Line l2) noexcept;

// Comparison operators
// --------------------
SINI_CUDA_COMPAT bool operator== (Line l1, Line l2) noexcept;
SINI_CUDA_COMPAT bool operator== (LineSegment l1, LineSegment l2) noexcept;

SINI_CUDA_COMPAT bool approxEquivalent(Line l1, Line l2, float tol = DEFAULT_TOLERANCE) noexcept;

} // namespace sini

#include "Line.inl"
