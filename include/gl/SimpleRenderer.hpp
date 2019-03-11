#pragma once

#include "gl/Camera.hpp"
#include "gl/GLContext.hpp"
#include "math/Vector.hpp"

#include <GL/glew.h>

#include <vector>


namespace sini {

class Window;
class GLContext;
struct Polygon;


class SimpleRenderer {
public:
    Camera camera;

    SimpleRenderer(const Window& window);
    SimpleRenderer(const Window& window, Camera camera);
    SimpleRenderer() noexcept = delete;
    ~SimpleRenderer();

    void clear(vec4 clear_color = vec4(0.0f, 0.0f, 0.0f, 1.0f)) noexcept;

    // All polygon draw calls can also be called with std::vector<vec2> instead
    // of polygon as input
    void drawPolygon(Polygon polygon, vec3 color, float alpha) noexcept;
    void drawPolygon(Polygon polygon, float width, vec3 color, float alpha) noexcept;
    void drawPolygonTriangleMesh(Polygon polygon, vec3 color, float alpha);
    void fillPolygon(Polygon polygon, vec3 color, float alpha);

    void drawRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha);
    void drawRectangle(vec2 bottom_left, vec2 upper_right, float width, vec3 color, float alpha);
    void fillRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha);

    void drawCircle(vec2 position, float radius, vec3 color, float alpha);
    void drawCircle(vec2 position, float radius, float width, vec3 color, float alpha);
    void fillCircle(vec2 position, float radius, vec3 color, float alpha);

    // Update the screen to make drawings visible. Actual rendering is not
    // guaranteed until updateScreen is called.
    void updateScreen() noexcept;

private:
    const Window* const window;
    const GLContext context;
    Polygon* circle_polygon = nullptr;
    std::vector<Vector<float,5>> queued_vertex_data;
    std::vector<GLuint> queued_elements;
    GLuint shader_program,
           screen_shader,
           framebuffer,
           framebuffer_texture,
           backbuffer,
           backbuffer_texture,
           quad_vertex_array,
           quad_vertex_buffer,
           vertex_array,
           vertex_buffer,
           element_buffer;
    size_t vertex_buffer_size = 8*1024*1024,  // (initial) size in bytes
           element_buffer_size = 8*1024*1024; // (initial) size in bytes

    void flushRenderQueue(float alpha = 1.0f) noexcept;
    void setUniforms(float alpha) noexcept;
    void setupInternalFramebuffer() noexcept;
    void setupInternalVertexObjects() noexcept;
    void setupQuadVertexArray() noexcept;
    Polygon setupCircle(vec2 offset, float radius);
    void growInternalVertexBuffer(size_t minimum_capacity) noexcept;
    void growInternalElementBuffer(size_t minimum_capacity) noexcept;
    // Render 'framebuffer' to another frame buffer, target = 0 being the screen
    void renderFramebuffer(GLuint target_framebuffer = 0) noexcept;
};

} // namespace sini
