#include "sini/geometry/Polygon.hpp"

namespace sini {

// Helper functions
// =============================================================================
namespace {
bool isOdd(int32_t integer)
{
    return integer % 2 == 1;
}

// TODO put in general util file (templated)
bool inList(vec2i element, std::vector<vec2i> list)
{
    return std::find(list.begin(), list.end(), element) != list.end();
}
bool inList(vec3i element, std::vector<vec3i> list)
{
    return std::find(list.begin(), list.end(), element) != list.end();
}

vec3i sorted(vec3i v) noexcept
{
    std::sort(&v.x, &v.z+1);
    return v;
}
vec2i sorted(vec2i v) noexcept
{
    return { minElement(v), maxElement(v) };
}
}


// Constructors and destructor
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

Polygon::Polygon(const Polygon& p) noexcept
    : vertices(p.vertices)
{
    if (p.triangle_mesh)
        triangle_mesh = new std::vector<vec3i>(*p.triangle_mesh);
}

Polygon::~Polygon() noexcept
{
    if (triangle_mesh) delete triangle_mesh;
}


// Member functions
// =============================================================================
std::vector<LineSegment> Polygon::lines() noexcept
{
    std::vector<LineSegment> lines;
    for (int32_t i = 0; i < vertices.size(); i++)
        lines.push_back(LineSegment(vertices[i], vertices[(i+1) % vertices.size()]));

    return lines;
}

bool Polygon::envelops(vec2 point) noexcept
{
    std::vector<LineSegment> lines_ = lines();
    float pi = 3.1415926535f;
    int32_t n_intersections;
    for (float dir_angle = 0.0f; dir_angle < 2*pi; dir_angle += pi / 5.0f) {
        Line point_to_inf{ point, { std::cos(dir_angle), std::sin(dir_angle) }};
        int32_t n_vertex_intersections = 0;
        n_intersections = 0;

        for (uint32_t i = 0; i < lines_.size(); i++) {
            if (point_to_inf.intersectsAlongDirection(lines_[i])) n_intersections++;
            if (point_to_inf.intersectsAlongDirection(vertices[i])) n_vertex_intersections++;
        }

        if (n_vertex_intersections != 0) continue;
        else break;
    }
    return isOdd(n_intersections);
}

void Polygon::buildTriangleMesh() noexcept
{
    if (vertices.size() < 3) return;
    if (!triangle_mesh)
        triangle_mesh = new std::vector<vec3i>();
    else
        triangle_mesh->clear();

    if (vertices.size() == 3) {
        *triangle_mesh = { vec3i(0, 1, 2) };
        return;
    }

    std::vector<vec2i> open_edges,
                       outer_edges  = outerEdgeList(),
                       closed_edges = outer_edges;
    for (size_t i = 0; i < outer_edges.size()-1; i++) {
        vec2i current_edge = outer_edges[i];
        if (edgeUsedInExistingTriangles(current_edge)) continue;

        for (int32_t k = (current_edge.x+1) % vertices.size(); k != current_edge.x;
             k = (k+1) % vertices.size()) {
            if (k == current_edge.y) continue;

            vec3i current_triangle = sorted({ current_edge, k });
            if (inList(current_triangle, *triangle_mesh)
                || intersectsOuterEdge(current_triangle)
                || intersectsExistingTriangle(current_triangle)
                || envelopsAnyVertex(current_triangle)
                || hasEdgesOutsidePolygon(current_triangle, outer_edges)) continue;

            triangle_mesh->push_back(current_triangle);
            updateOpenAndClosedEdges(open_edges, closed_edges, current_triangle);
            if (tryCloseOpenEdges(open_edges, closed_edges)) break;
            else triangle_mesh->pop_back();
        }
    }
}

// Private member functions
// =============================================================================
std::vector<vec2i> Polygon::outerEdgeList() noexcept
{
    std::vector<vec2i> edges;
    for (int32_t i = 0; i < vertices.size()-1; i++)
        edges.push_back({ i, i+1 });
    edges.push_back(vec2i( 0, vertices.size()-1 ));
    return std::move(edges);
}

bool Polygon::edgeUsedInExistingTriangles(vec2i edge_indices) noexcept
{
    for (int32_t i = 0; i < triangle_mesh->size(); i++) {
        vec3i current_triangle = (*triangle_mesh)[i];
        if (edge_indices == current_triangle.xy
            || edge_indices == current_triangle.yz
            || edge_indices == vec2i(current_triangle.x, current_triangle.z))
            return true;
    }
    return false;
}

bool Polygon::intersectsOuterEdge(vec3i triangle_indices) noexcept
{
    std::vector<LineSegment> outer_edges = lines();
    for (int32_t i = 0; i < 3; i++) {
        vec2i edge_indices = { triangle_indices[i], triangle_indices[(i+1)%3] };
        LineSegment triangle_edge = { vertices[edge_indices.x], vertices[edge_indices.y] };

        for (int32_t j = 0; j < outer_edges.size(); j++) {
            vec2i vertex_indices = vec2i( j, (j+1) % vertices.size() );
            if (edge_indices.x == vertex_indices.x
                || edge_indices.y == vertex_indices.y
                || edge_indices.x == vertex_indices.y
                || edge_indices.y == vertex_indices.x) continue;

            if (intersect(triangle_edge, outer_edges[j])) return true;
        }
    }
    return false;
}

bool Polygon::intersectsExistingTriangle(vec3i triangle_indices) noexcept
{
    for (int32_t i = 0; i < triangle_mesh->size(); i++)
        if (trianglesIntersect(triangle_indices, (*triangle_mesh)[i]))
            return true;
    return false;
}

bool Polygon::trianglesIntersect(vec3i vertex_indices1, vec3i vertex_indices2) noexcept
{
    for (int32_t j = 0; j < 3; j++) {
        vec2i indices1 = { vertex_indices1[j], vertex_indices1[(j+1)%3] };
        LineSegment edge1 = { vertices[indices1.x],
                              vertices[indices1.y] };
        for (int32_t k = 0; k < 3; k++) {
            vec2i indices2 = { vertex_indices2[k], vertex_indices2[(k+1)%3] };
            if (indices2.x == indices1.x
                || indices2.x == indices1.y
                || indices2.y == indices1.x
                || indices2.y == indices1.y)
                continue;

            LineSegment edge2 = { vertices[indices2.x], vertices[indices2.y] };
            if (intersect(edge1, edge2)) return true;
        }
    }
    return false;
}

bool Polygon::envelopsAnyVertex(vec3i vertex_indices) noexcept
{
    Polygon triangle = { vertices[vertex_indices[0]],
                         vertices[vertex_indices[1]],
                         vertices[vertex_indices[2]] };
    for (int32_t i = 0; i < vertices.size(); i++) {
        if (i == vertex_indices.x || i == vertex_indices.y || i == vertex_indices.z )
            continue;

        if (triangle.envelops(vertices[i]))
            return true;
    }
    return false;
}

bool Polygon::hasEdgesOutsidePolygon(vec3i vertex_indices, const std::vector<vec2i>& outer_edges) noexcept
{
    for (int32_t i = 0; i < 3; i++) {
        vec2i edge = sorted(vec2i{ vertex_indices[i], vertex_indices[(i+1)%3] });
        if (inList(edge, outer_edges)) continue;

        vec2 edge_mid_point = (vertices[edge.x] + vertices[edge.y]) / 2.0f;
        if (!(this->envelops(edge_mid_point))) return true;
    }
    return false;
}

bool Polygon::tryCloseOpenEdges(std::vector<vec2i>& open_edges, std::vector<vec2i>& closed_edges) noexcept
{
    std::vector<vec2i> initial_open_edges   = open_edges,
                       initial_closed_edges = closed_edges;
    int32_t n_triangles_added = 0;
    while (!open_edges.empty()) {
        vec2i current_open_edge = open_edges.back();
        closed_edges.push_back(current_open_edge);
        open_edges.pop_back();

        for (int32_t k = (current_open_edge.x+1) % vertices.size(); ; k = (k+1) % vertices.size()) {
            if (k == current_open_edge.y) continue;
            if (k == current_open_edge.x) {
                // All possibilities tried, revert changes and report failure
                for (int32_t i = 0; i < n_triangles_added; i++)
                    triangle_mesh->pop_back();
                open_edges   = initial_open_edges;
                closed_edges = initial_closed_edges;
                return false;
            }

            vec3i current_triangle = sorted({ current_open_edge, k });
            if (inList(current_triangle, *triangle_mesh)
                || intersectsOuterEdge(current_triangle)
                || intersectsExistingTriangle(current_triangle)
                || envelopsAnyVertex(current_triangle)) continue;

            triangle_mesh->push_back(current_triangle);
            n_triangles_added++;
            updateOpenAndClosedEdges(open_edges, closed_edges, current_triangle);
            break;
        }
    }
    return true;
}

void Polygon::updateOpenAndClosedEdges(std::vector<vec2i>& open_edges,
    const std::vector<vec2i>& closed_edges, vec3i triangle_indices) noexcept
{
    vec2i edges[] = { sorted(triangle_indices.xy),
                      sorted(triangle_indices.yz),
                      sorted(vec2i{ triangle_indices.z, triangle_indices.x }) };

    for (int32_t i = 0; i < 3; i++)
        if (!inList(edges[i], closed_edges)
            && !inList(edges[i], open_edges))
            open_edges.push_back(edges[i]);
}


} // namespace sini
