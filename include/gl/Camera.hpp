#pragma once

#include "math/Matrix.hpp"
#include "math/Vector.hpp"


namespace sini {

class Camera {
public:
    vec2 position;
    const float aspect_ratio;   // = width / height
    float width,
          orientation;

    Camera() = delete;
    Camera(vec2 position, float aspect_ratio, float width) noexcept;
    Camera(vec2 position, float aspect_ratio, float width, float orientation) noexcept;
    Camera(const Camera&) noexcept = default;
    ~Camera() noexcept = default;

    mat3 worldToCameraViewMatrix() noexcept;
};

}
