#pragma once
namespace sini {

// Line member functions
// =============================================================================
SINI_CUDA_COMPAT Line::Line(vec2 p, vec2 dir) noexcept
    : p(p),
      dir(dir)
{}

SINI_CUDA_COMPAT bool Line::intersects(vec2 point, float tol) noexcept
{
    assert(length(dir) > 0.0f);
    // The closest distance from the line to 'point' is along a line
    // perpendicular to the first one.
    vec2 p_to_point = point - p,
         proj_on_line = dot(dir, p_to_point)/length(dir) * dir,
         line_to_point = p_to_point - proj_on_line;
    // If this distance is shorter than the provided tolerance, the line
    // "intersects" the point
    return lengthSquared(line_to_point) < tol * tol;
}

SINI_CUDA_COMPAT Line& Line::normalizeDir() noexcept
{
    float dir_length = length(dir);
    assert(dir_length > 0.0f);
    if (!approxEqual(dir_length, 1.0f))
        dir = normalize(dir);
    return *this;
}


// LineSegment member functions
// =============================================================================
SINI_CUDA_COMPAT LineSegment::LineSegment(vec2 p1, vec2 p2) noexcept
    : p1(p1),
      p2(p2)
{}

SINI_CUDA_COMPAT Line LineSegment::extrapolate() noexcept
{
    return Line{ p1, normalize(p2-p1) };
}

SINI_CUDA_COMPAT bool LineSegment::intersects(vec2 point, float tol) noexcept
{
    // The closest distance from the line segment to 'point' is along a line
    // perpedicular to the segment.
    vec2 line_dir = normalize(p2 - p1),
         to_point = point - p1;

    // If the point is "before" the line segment "start point", check whether
    // the line segment "start point" is within the provided tolerance from
    // the point
    float dir_to_point_dot = dot(line_dir, to_point);
    float tol_sq = tol * tol;
    if (dir_to_point_dot < 0.0f)
        return lengthSquared(to_point) < tol_sq;

    // If the point is beyond the "end point" of the line segment, check
    // whether the line segment "end point" is within the provided tolerance
    // from the point
    vec2 proj_on_line = dir_to_point_dot * line_dir;
    if (lengthSquared(proj_on_line) > lengthSquared(p2 - p1))
        return lengthSquared(point - p2) < tol_sq;

    vec2 line_to_point = to_point - proj_on_line;
    // If the distance is shorter than the provided tolerance, the line segment
    // "intersects" the point
    return lengthSquared(line_to_point) < tol_sq;
}


// Intersection functions
// =============================================================================
SINI_CUDA_COMPAT bool intersect(Line l1, Line l2) noexcept
{
    // Two infinitely long lines always intersect unless they are parallell,
    // which they are if and only if the system matrix determinant is zero.
    // (x1, y1) + s*(a,b) = (x2, y2) + t*(c, d)
    // --> [a -c] [s] = [x2 - x1]
    //     [b -d] [t]   [y2 - y1]
    // Dealing with finite precision floating point numbers means this will
    // most likely not happen. Therefore, this functionality should probably
    // not be used, but is included for completeness sake
    float a = l1.dir.x,
          b = l1.dir.y,
          c = l2.dir.x,
          d = l2.dir.y;
    return a*d - b*c != 0.0f;
}

SINI_CUDA_COMPAT bool intersect(LineSegment l1, LineSegment l2) noexcept
{
    // Line a -> b intersection with line c -> d
    // a + s(b-a) = c + t(d-c)
    vec2 a = l1.p1,
         b = l1.p2,
         c = l2.p1,
         d = l2.p2;
    float mat_det = (b.x - a.x)*(c.y - d.y) - (c.x - d.x)*(b.y - a.y);

    // There are two reasons why two line segments might not intersect:
    // 1. Lines are parallell, i.e. system matrix determinant is zero
    //    (unsolvable equation)
    if (mat_det == 0.0f) return false;

    // 2. The intersection of the extrapolated lines is beyond one of the end
    //    points of either line segment
    float s = ( (c.x - a.x)*(c.y - d.y) - (c.x - d.x)*(c.y - a.y) ) / mat_det;
    if (s < 0.0f || s > 1.0f) return false;

    float t = ( (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y) ) / mat_det;
    return t >= 0.0f && t <= 1.0f;
}

SINI_CUDA_COMPAT bool intersect(Line l1, LineSegment l2) noexcept
{
    vec2 a = l1.p,
         b = l1.p + l1.dir,
         c = l2.p1,
         d = l2.p2;
    float mat_det = (b.x - a.x)*(c.y - d.y) - (c.x - d.x)*(b.y - a.y);

    // Lines are parallell (unsolvable equation, i.e. no intersection)
    // if expression above is zero
    if (mat_det == 0.0f) return false;

    float t = ( (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y) ) / mat_det;
    return t >= 0.0f && t <= 1.0f;
}

SINI_CUDA_COMPAT IntersectionPoint intersection(Line l1, Line l2) noexcept
{
    IntersectionPoint ip;
    float mat_det = l1.dir.y*l2.dir.x - l1.dir.x*l2.dir.y;

    if (mat_det == 0.0f) {
        ip.intersect = false;
        return std::move(ip);
    }

    ip.intersect = true;
    float s = l2.dir.x*(l2.p.y - l1.p.y) - l2.dir.y*(l2.p.x - l1.p.x) / mat_det;
    ip.intersection_point = l1.p + s*l1.dir;
    return std::move(ip);
}

SINI_CUDA_COMPAT IntersectionPoint intersection(LineSegment l1, LineSegment l2) noexcept
{
    IntersectionPoint ip;
    // Line segment a -> b intersection with line segment c -> d
    // a + s(b-a) = c + t(d-c)
    // 0 < s < 1, 0 < t < 1
    vec2 a = l1.p1,
         b = l1.p2,
         c = l2.p1,
         d = l2.p2;
    float mat_det = (b.x - a.x)*(c.y - d.y) - (c.x - d.x)*(b.y - a.y);

    // There are two reasons why two line segments might not intersect:
    // 1. Lines are parallell, i.e. system matrix determinant is zero
    //    (unsolvable equation)
    if (mat_det == 0.0f) {
        ip.intersect = false;
        return std::move(ip);
    }

    // 2. The intersection of the extrapolated lines is beyond one of the end
    //    points of either line segment
    float s = ( (c.x - a.x)*(c.y - d.y) - (c.x - d.x)*(c.y - a.y) ) / mat_det;
    if (s < 0.0f || s > 1.0f) {
        ip.intersect = false;
        return std::move(ip);
    }
    float t = ( (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y) ) / mat_det;
    if (t < 0.0f || t > 1.0f) {
        ip.intersect = false;
        return std::move(ip);
    }

    ip.intersect = true;
    ip.intersection_point = a + s*(b-a);
    return std::move(ip);
}

SINI_CUDA_COMPAT IntersectionPoint intersection(Line l1, LineSegment l2) noexcept
{
    IntersectionPoint ip;
    // Line segment a -> b intersection with line c + dir
    // a + s(b-a) = c + k*dir = c + t(d-c)
    // 0 < s < 1, t real
    vec2 a = l2.p1,
         b = l2.p2,
         c = l1.p,
         d = l1.p + l1.dir;
    float mat_det = (b.x - a.x)*(c.y - d.y) - (c.x - d.x)*(b.y - a.y);

    // There are two reasons why the line might not intersect the line segment:
    // 1. The line and line segment are parallell, i.e. the system matrix
    //    determinant is zero (unsolvable equation)
    if (mat_det == 0.0f) {
        ip.intersect = false;
        return std::move(ip);
    }

    // 2. The intersection point is beyond one of the end points of the line
    //    segment
    float s = ( (c.x - a.x)*(c.y - d.y) - (c.x - d.x)*(c.y - a.y) ) / mat_det;
    if (s < 0.0f || s > 1.0f) {
        ip.intersect = false;
        return std::move(ip);
    }

    ip.intersect = true;
    ip.intersection_point = a + s*(b-a);
    return std::move(ip);
}

SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(Line l1, Line l2) noexcept
{
    IntersectionDistance id;
    float mat_det = l1.dir.y*l2.dir.x - l1.dir.x*l2.dir.y;

    // The only reason two lines do not intersect is if they are parallell,
    // which they if the system matrix determinant is zero
    if (mat_det == 0.0f) {
        id.intersect = false;
        return std::move(id);
    }

    id.intersect = true;
    id.intersection_distance =
        ( l2.dir.x*(l2.p.y - l1.p.y) - l2.dir.y*(l2.p.x - l1.p.x) ) / mat_det;
    return std::move(id);
}

SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(Line l1, LineSegment l2) noexcept
{
    IntersectionDistance id;
    // Line a + s*dir intersecting with line segment c -> d = c + t(d-c)
    // 0 < t < 1
    vec2 a = l1.p,
         b = l1.p + l1.dir,
         c = l2.p1,
         d = l2.p2;
    float mat_det = (b.x - a.x)*(c.y - d.y) - (c.x - d.x)*(b.y - a.y);

    // There are two reasons why the line might not intersect the line segment
    // 1. The line and line segment are parallell, i.e. the system matrix
    //    determinant is zero (unsolvable equation)
    if (mat_det == 0.0f) {
        id.intersect = false;
        return std::move(id);
    }

    // 2. The intersection point is beyond one of the end points of the line
    //    segment
    float t = ( (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y) ) / mat_det;
    if (t < 0.0f || t > 1.0f) {
        id.intersect = false;
        return std::move(id);
    }

    id.intersect = true;
    id.intersection_distance =
        ( (c.x - a.x)*(c.y - d.y) - (c.x - d.x)*(c.y - a.y) ) / mat_det;
    return std::move(id);
}

SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(LineSegment l1, LineSegment l2) noexcept
{
    IntersectionDistance id;
    // Line segment a -> b intersecting with line segment c -> d
    // a + s(b-a) = c + t(d-c)
    // 0 < s < 1, 0 < t < 1
    vec2 a = l1.p1,
         b = l1.p2,
         c = l2.p1,
         d = l2.p2;
    float mat_det = (b.x - a.x)*(c.y - d.y) - (c.x - d.x)*(b.y - a.y);

    // There are two reasons why the line might not intersect the line segment
    // 1. The line and line segment are parallell, i.e. the system matrix
    //    determinant is zero (unsolvable equation)
    if (mat_det == 0.0f) {
        id.intersect = false;
        return std::move(id);
    }

    // 2. The intersection point is beyond one of the end points of either line
    //    segment
    float s = ( (c.x - a.x)*(c.y - d.y) - (c.x - d.x)*(c.y - a.y) ) / mat_det;
    if (s < 0.0f || s > 1.0f) {
        id.intersect = false;
        return std::move(id);
    }
    float t = ( (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y) ) / mat_det;
    if (t < 0.0f || t > 1.0f) {
        id.intersect = false;
        return std::move(id);
    }

    id.intersect = true;
    id.intersection_distance = s;
    return std::move(id);

}

SINI_CUDA_COMPAT IntersectionDistance intersectionDistance(LineSegment l1, Line l2) noexcept
{
    IntersectionDistance id;
    // Line segment a -> b intersecting with line through c -> d
    // a + s(b-a) = c + k*dir = c + t(d-c)
    // 0 < s < 1, t real
    vec2 a = l1.p1,
         b = l1.p2,
         c = l2.p,
         d = l2.p + l2.dir;
    float mat_det = (b.x - a.x)*(c.y - d.y) - (c.x - d.x)*(b.y - a.y);

    // There are two reasons why the line might not intersect the line segment
    // 1. The line and line segment are parallell, i.e. the system matrix
    //    determinant is zero (unsolvable equation)
    if (mat_det == 0.0f) {
        id.intersect = false;
        return std::move(id);
    }

    // 2. The intersection point is beyond one of the end points of the line
    //    segment
    float s = ( (c.x - a.x)*(c.y - d.y) - (c.x - d.x)*(c.y - a.y) ) / mat_det;
    if (s < 0.0f || s > 1.0f) {
        id.intersect = false;
        return std::move(id);
    }

    id.intersect = true;
    id.intersection_distance = s;
    return std::move(id);
}

} // namespace sini
