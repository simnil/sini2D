#pragma once
#ifndef SINI_LINE_H
#define SINI_LINE_H

#include "sini/CudaCompat.hpp"
#include "sini/math/Vector.hpp"

namespace sini {

struct IntersectionPoint {
    bool intersect;
    vec2 intersection_point;
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

    SINI_CUDA_COMPAT bool intersects(vec2 point) noexcept;

};

// A two-dimensional line from p1 to p2
struct LineSegment {
    vec2 p1, p2;

    LineSegment() noexcept = default;
    LineSegment(const LineSegment&) noexcept = default;
    LineSegment& operator= (const LineSegment&) noexcept = default;
    ~LineSegment() noexcept = default;

    SINI_CUDA_COMPAT LineSegment(vec2 p1, vec2 p2) noexcept;

    SINI_CUDA_COMPAT Line extrapolate() noexcept;
    SINI_CUDA_COMPAT bool intersects(vec2 point) noexcept;

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
SINI_CUDA_COMPAT IntersectionPoint intersection(LineSegment l1, Line l2) { return intersection(l2, l1); }

} // namespace sini
#endif // SINI_LINE_H
