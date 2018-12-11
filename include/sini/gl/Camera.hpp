#pragma once

#include "sini/math/Vector.hpp"
#include "sini/math/Matrix.hpp"

namespace sini::gl {


class Camera {
public:
    // Members
    // -------------------------------------------------------------------------
    vec2 position;
    const float aspect_ratio;   // = width / height
    float width,
          orientation;

    // Constructor
    // -------------------------------------------------------------------------
    Camera() = delete;
    Camera(vec2 position, float aspect_ratio, float width) noexcept;
    Camera(vec2 position, float aspect_ratio, float width, float orientation) noexcept;
    Camera(const Camera&) noexcept = default;
    ~Camera() noexcept = default;

    // Functions
    // -------------------------------------------------------------------------
    mat3 worldToCameraViewMatrix() noexcept;
};

} // namespace sini::gl
