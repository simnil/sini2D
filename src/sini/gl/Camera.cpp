#include "sini/gl/Camera.hpp"

namespace sini { namespace gl {

Camera::Camera(vec2 position, float aspect_ratio, float width) noexcept
    : position(position), aspect_ratio(aspect_ratio), width(width)
{}

mat3 Camera::worldToCameraViewMatrix() noexcept
{
    float height = width / aspect_ratio;
    return mat3{
        { 2.0f / width, 0.0f, -2.0f * position.x / width },
        { 0.0f, 2.0f / height, -2.0f * position.y / height},
        { 0.0f, 0.0f, 1.0f }
    };
}

}} // namespace gl, namespace sini
