#pragma once
#ifndef SINI_POLYGON_H
#define SINI_POLYGON_H

#include "sini/CudaCompat.hpp"
#include "sini/math/Vector.hpp"
#include "sini/geometry/Line.hpp"
#include <vector>
#include <initializer_list>


namespace sini {

// A two-dimensional polygon, represented by an array of points in 2D space
struct Polygon {
    std::vector<vec2> vertices;
    // TODO
    // dynamically allocated triangle mesh data

    Polygon() noexcept = delete;
    Polygon(const Polygon&) = default;
    Polygon(Polygon&&) = default;
    Polygon& operator= (const Polygon&) = default;
    ~Polygon() noexcept = default;

    Polygon(const std::vector<vec2>& vertices) noexcept;
    Polygon(std::vector<vec2>&& vertices) noexcept;
    Polygon(std::initializer_list<vec2> vertices) noexcept;

    std::vector<LineSegment> lines() noexcept;
    bool envelops(vec2 point) noexcept;
    // TODO
    // void buildTriangleMesh() noexcept;
};

} // namespace sini
#endif // SINI_POLYGON_H
