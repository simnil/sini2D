#pragma once

#include <sini2D/math/Vector.hpp>

#include <vector>
#include <initializer_list>


namespace sini {

struct LineSegment;

// A two-dimensional polygon, represented by an array of points in 2D space
struct Polygon {
    std::vector<vec2> vertices;
    std::vector<vec3i> *triangle_mesh = nullptr;

    Polygon() noexcept = delete;
    Polygon(const Polygon& p);
    Polygon(Polygon&& p) noexcept;
    Polygon& operator= (const Polygon&) = default;
    ~Polygon() noexcept;

    Polygon(std::vector<vec2> vertices) noexcept;
    Polygon(std::initializer_list<vec2> vertices);

    std::vector<LineSegment> lines();
    bool envelops(vec2 point);
    void buildTriangleMesh();

private:
    std::vector<vec2i> outerEdgeList();
    bool edgeUsedInExistingTriangles(vec2i edge_indices) noexcept;
    bool intersectsOuterEdge(vec3i triangle_indices);
    bool intersectsExistingTriangle(vec3i triangle_indices) noexcept;
    bool trianglesIntersect(vec3i vertex_indices1, vec3i vertex_indices2) noexcept;
    bool envelopsAnyVertex(vec3i vertex_indices);
    bool hasEdgesOutsidePolygon(vec3i triangle_indices,
                                const std::vector<vec2i>& outer_edges);
    void updateOpenAndClosedEdges(std::vector<vec2i>& open_edges,
                                  const std::vector<vec2i>& outer_edges,
                                  vec3i triangle_indices);
    bool tryCloseOpenEdges(std::vector<vec2i>& open_edges,
                           std::vector<vec2i>& closed_edges);
};

} // namespace sini
