#pragma once

#include "sini/math/Vector.hpp"

#include <vector>
#include <initializer_list>


namespace sini {

struct LineSegment;

// A two-dimensional polygon, represented by an array of points in 2D space
struct Polygon {
    std::vector<vec2> vertices;
    std::vector<vec3i> *triangle_mesh = nullptr;

    Polygon() noexcept = delete;
    Polygon(const Polygon& p) noexcept;
    Polygon(Polygon&& p) noexcept;
    Polygon& operator= (const Polygon&) = default;
    ~Polygon() noexcept;

    Polygon(const std::vector<vec2>& vertices) noexcept;
    Polygon(std::vector<vec2>&& vertices) noexcept;
    Polygon(std::initializer_list<vec2> vertices) noexcept;

    std::vector<LineSegment> lines() noexcept;
    bool envelops(vec2 point) noexcept;
    void buildTriangleMesh() noexcept;

private:
    std::vector<vec2i> outerEdgeList() noexcept;
    bool edgeUsedInExistingTriangles(vec2i edge_indices) noexcept;
    bool intersectsOuterEdge(vec3i triangle_indices) noexcept;
    bool intersectsExistingTriangle(vec3i triangle_indices) noexcept;
    bool trianglesIntersect(vec3i vertex_indices1, vec3i vertex_indices2) noexcept;
    bool envelopsAnyVertex(vec3i vertex_indices) noexcept;
    bool hasEdgesOutsidePolygon(vec3i triangle_indices,
                                const std::vector<vec2i>& outer_edges) noexcept;
    void updateOpenAndClosedEdges(std::vector<vec2i>& open_edges,
                                  const std::vector<vec2i>& outer_edges,
                                  vec3i triangle_indices) noexcept;
    bool tryCloseOpenEdges(std::vector<vec2i>& open_edges,
                           std::vector<vec2i>& closed_edges) noexcept;
};

} // namespace sini
