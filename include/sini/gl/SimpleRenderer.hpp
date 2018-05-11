#pragma once
#ifndef SINI_SIMPLE_RENDERER_H
#define SINI_SIMPLE_RENDERER_H

#include <vector>

#include "sini/geometry/Polygon.hpp"

#include "sini/gl/glutil.hpp"
#include "sini/gl/GLContext.hpp"
#include "sini/gl/Camera.hpp"
#include "sini/sdl/Window.hpp"

namespace sini { namespace gl {


class SimpleRenderer {
public:
    Camera camera;

    SimpleRenderer(const Window& window) noexcept;
    SimpleRenderer(const Window& window, Camera camera) noexcept;
    SimpleRenderer() noexcept = delete;
    ~SimpleRenderer() noexcept = default;

    void clear(vec4 clear_color = vec4(0.0f, 0.0f, 0.0f, 1.0f)) noexcept;

    // All polygon draw calls can also be called with std::vector<vec2> instead
    // of polygon as input
    void drawPolygon(Polygon polygon, vec3 color, float alpha) noexcept;
    void drawPolygon(Polygon polygon, float width, vec3 color, float alpha) noexcept;
    void drawPolygonTriangleMesh(Polygon polygon, vec3 color, float alpha) noexcept;
    void fillPolygon(Polygon polygon, vec3 color, float alpha) noexcept;

    void drawRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha) noexcept;
    void drawRectangle(vec2 bottom_left, vec2 upper_right, float width, vec3 color, float alpha) noexcept;
    void fillRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha) noexcept;

    // TODO
    // void drawCircle(vec2 position, float radius, vec3 color, float alpha) noexcept;
    // void drawCircle(vec2 position, float radius, vec3 color, float alpha) noexcept;
    // void fillCircle(vec2 position, float radius, vec3 color, float alpha) noexcept;

    // Update the screen to make drawings visible (swap back and front
    // buffer). Wraps the SDL function SDL_GL_SwapWindow().
    void updateScreen() noexcept;

private:
    const Window* const window;
    const GLContext context;
    GLuint shader_program;

    void setUniforms(vec3 color, float alpha) noexcept;
};

}} // namespace gl, namespace sini
#endif // SINI_SIMPLE_RENDERER_H
