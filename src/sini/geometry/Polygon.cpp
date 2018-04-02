#include "sini/geometry/Polygon.hpp"

namespace sini {

namespace {
bool isOdd(int32_t integer)
{
    return integer % 2 == 1;
}
}


// Constructors
// =============================================================================
Polygon::Polygon(const std::vector<vec2>& vertices) noexcept
    : vertices(vertices)
{}

Polygon::Polygon(std::vector<vec2>&& vertices) noexcept
    : vertices(std::move(vertices))
{}

Polygon::Polygon(std::initializer_list<vec2> vertices) noexcept
    : vertices(vertices)
{}


// Member functions
// =============================================================================
std::vector<LineSegment> Polygon::lines() noexcept
{
    std::vector<LineSegment> lines;
    for (int i = 0; i < vertices.size(); i++) {
        int next_idx_w_wrap = (i+1) % vertices.size();
        lines.push_back(LineSegment(vertices[i], vertices[next_idx_w_wrap]));
    }
    return lines;
}

bool Polygon::envelops(vec2 point) noexcept
{
    std::vector<LineSegment> lines_ = lines();
    Line point_to_inf{ point, { 1.0f, 0.0f }};
    int32_t n_intersections = 0,
      n_vertex_intersections = 0;

    for (uint32_t i = 0; i < lines_.size(); i++) {
        if (point_to_inf.intersectsAlongDirection(lines_[i])) n_intersections++;
        if (point_to_inf.intersectsAlongDirection(vertices[i])) n_vertex_intersections++;
    }
    int32_t n_line_intersections = n_intersections - n_vertex_intersections;

    return isOdd(n_line_intersections);
}

} // namespace sini
