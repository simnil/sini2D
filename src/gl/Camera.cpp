#include <sini2D/gl/Camera.hpp>

#include <cmath>        // For std::sin, std::cos

using std::cos;
using std::sin;


namespace sini {

Camera::Camera(vec2 position, float aspect_ratio, float width) noexcept
    : Camera(position, aspect_ratio, width, 0.0f)
{}

Camera::Camera(vec2 position, float aspect_ratio, float width, float orientation) noexcept
    : position(position), aspect_ratio(aspect_ratio), width(width),
      orientation(orientation)
{}

mat3 Camera::worldToCameraViewMatrix() noexcept
{
    float height = width / aspect_ratio,
            _cos = cos(orientation),
            _sin = sin(orientation);
    return mat3{
        { 2.0f * _cos / width,  -2.0f * _sin / width,  -2.0f * (position.x*_cos - position.y*_sin) / width},
        { 2.0f * _sin / height,  2.0f * _cos / height, -2.0f * (position.x*_sin + position.y*_cos) / height},
        { 0.0f, 0.0f, 1.0f }
    };
}

}
