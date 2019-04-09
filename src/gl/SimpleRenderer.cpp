#include "gl/SimpleRenderer.hpp"

#include "geometry/Polygon.hpp"
#include "gl/glutil.hpp"
#include "sdl/Window.hpp"

#include <array>
#include <vector>
#include <iostream>


namespace sini {

// Simple vertex and fragment shader source
// -----------------------------------------------------------------------------
static const char* simple_vertex_shader_src = R"glsl(
    #version 420 core
    precision highp float;

    uniform mat2 model_to_world_transf;
    uniform mat3 world_to_cam_transf;

    layout(location = 0) in vec2 position;
    layout(location = 1) in vec3 in_color;
    out vec2 texcoord;
    out vec3 color;

    void main() {
        color = in_color;
        vec3 camview_pos = world_to_cam_transf * vec3(model_to_world_transf
            * position, 1.0f);
        texcoord = 0.5f * (camview_pos.xy + vec2(1.0f));
        gl_Position = vec4(camview_pos.xy, 0.0f, 1.0f);
    }
)glsl";
// TODO simple geometry shader
static const char* simple_geometry_shader_src = nullptr;
static const char* simple_fragment_shader_src = R"glsl(
    #version 420 core
    precision highp float;

    uniform float alpha;
    uniform sampler2D backbuffer;

    in vec2 texcoord;
    in vec3 color;
    layout(location = 0) out vec4 fragment_color;

    void main() {
        fragment_color = mix(
            vec4(color, 1.0f), texture(backbuffer, texcoord), 1.0f - alpha);
    }
)glsl";
// Shader for drawing the framebuffer on the screen
// -----------------------------------------------------------------------------
static const char* screen_vertex_shader_src = R"glsl(
    #version 420 core
    precision highp float;

    layout(location = 0) in vec2 position;
    layout(location = 1) in vec2 texture_coord;
    out vec2 texcoord;

    void main() {
        gl_Position = vec4(position, 0.0f, 1.0f);
        texcoord = texture_coord;
    }
)glsl";
static const char* screen_fragment_shader_src = R"glsl(
    #version 420 core
    precision highp float;

    uniform sampler2D framebuffer;
    in vec2 texcoord;
    layout(location = 0) out vec4 fragment_color;

    void main() {
        fragment_color = texture(framebuffer, texcoord);
    }
)glsl";


// Helper functions
// -----------------------------------------------------------------------------
namespace {

std::array<vec2,4> setupRectangleVertices(vec2 bottom_left, vec2 upper_right) noexcept
{
    std::array<vec2, 4> vertices;
    vertices[0] = bottom_left;
    vertices[1] = { upper_right.x, bottom_left.y };
    vertices[2] = upper_right;
    vertices[3] = { bottom_left.x, upper_right.y };

    return vertices;
}

Polygon* createCirclePolygon()
{
    std::vector<vec2> vertices;
    constexpr int n_vertices = 64;
    vertices.reserve(n_vertices);
    constexpr float two_pi = 2.0f * 3.1415926535f;
    for (float angle = 0.0f; angle < two_pi; angle += two_pi / n_vertices)
        vertices.push_back({ std::cos(angle), std::sin(angle) });
    return new Polygon(std::move(vertices));
}

size_t sizeGrowthFunction(size_t current_size, size_t target_minimum_size) noexcept
{
    size_t new_size = current_size;
    while (new_size < target_minimum_size)
        new_size *= 2;
    return new_size;
}

} // anonymous namespace


// Constructors and destructor
// -----------------------------------------------------------------------------
SimpleRenderer::SimpleRenderer(const Window& window, Camera camera)
    : camera(camera),
      window(&window),
      context(window.win_ptr, 4, 2, GLProfile::CORE)
{
    glewInit();
    shader_program = loadShaderProgram(simple_vertex_shader_src,
        simple_geometry_shader_src, simple_fragment_shader_src);
    screen_shader = loadShaderProgram(screen_vertex_shader_src,
        nullptr, screen_fragment_shader_src);

    // Print error message and terminate if failure
    // TODO Print error message if load fails
    if (shader_program == 0) {
        std::cerr << "Simple shader program could not load" << std::endl;
        std::terminate();
    }
    if (screen_shader == 0) {
        std::cerr << "Screen shader program could not load" << std::endl;
        std::terminate();
    }

    setupInternalVertexObjects();
    setupInternalFramebuffer();
}

SimpleRenderer::SimpleRenderer(const Window& window)
    : SimpleRenderer(window, Camera({ 0.0f, 0.0f }, 16.0f / 9.0f, 2.0f))
{}

SimpleRenderer::~SimpleRenderer()
{
    if (circle_polygon)
        delete circle_polygon;

    glDeleteProgram(shader_program);
    glDeleteProgram(screen_shader);

    glDeleteTextures(1, &backbuffer_texture);
    glDeleteTextures(1, &framebuffer_texture);

    glDeleteFramebuffers(1, &backbuffer);
    glDeleteFramebuffers(1, &framebuffer);

    glDeleteBuffers(1, &quad_vertex_buffer);
    glDeleteVertexArrays(1, &quad_vertex_array);

    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
}


// Member functions
// -----------------------------------------------------------------------------
void SimpleRenderer::clear(vec4 clear_color) noexcept
{
    vec2i dim = window->dimensions();

    glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
    glViewport(0, 0, dim.x, dim.y);
    glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, dim.x, dim.y);
    glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SimpleRenderer::drawPolygon(Polygon& polygon, vec3 color, float alpha) noexcept
{
    if (alpha <= 0.0f || polygon.vertices.size() < 3)
        return;
    else if (alpha < 1.0f
             || render_style != DRAW)
    {
        flushRenderQueue(render_style);
        render_style = DRAW;
    }

    const size_t initial_queue_data_size = queued_vertex_data.size();

    for (vec2 vertex : polygon.vertices)
        queued_vertex_data.push_back(
            Vector<float, 5>({ vertex.x, vertex.y, color[0], color[1], color[2] }));

    for (size_t i = 0; i < polygon.vertices.size()-1; ++i) {
        queued_elements.push_back(static_cast<GLuint>(i + initial_queue_data_size));
        queued_elements.push_back(static_cast<GLuint>(i+1 + initial_queue_data_size));
    }
    queued_elements.push_back(static_cast<GLuint>(polygon.vertices.size()-1
                                                  + initial_queue_data_size));
    queued_elements.push_back(static_cast<GLuint>(0 + initial_queue_data_size));

    if (alpha < 1.0f)
        flushRenderQueue(render_style, alpha);
}

void SimpleRenderer::fillPolygon(Polygon& polygon, vec3 color, float alpha)
{
    if (alpha <= 0.0f || polygon.vertices.size() < 3)
        return;
    else if (alpha < 1.0f
             || render_style != FILL)
    {
        flushRenderQueue(render_style);
        render_style = FILL;
    }

    const size_t initial_queue_data_size = queued_vertex_data.size();
    // no reserve, since it causes queued_vertex_data and queued_elements to grow linearly
    // instead of exponentially
    for (vec2 vertex : polygon.vertices)
        queued_vertex_data.push_back(
            Vector<float, 5>({ vertex.x, vertex.y, color[0], color[1], color[2] }));

    if (!polygon.triangle_mesh)
        polygon.buildTriangleMesh();

    for (vec3i index_triplet : *polygon.triangle_mesh)
        for (int idx : index_triplet)
            queued_elements.push_back(static_cast<GLuint>(idx + initial_queue_data_size));

    if (alpha < 1.0f)
        flushRenderQueue(render_style, alpha);
}

void SimpleRenderer::drawPolygonTriangleMesh(Polygon& polygon, vec3 color, float alpha)
{
    if (alpha <= 0.0f || polygon.vertices.size() < 3)
        return;
    else if (alpha < 1.0f
             || render_style != DRAW)
    {
        flushRenderQueue(render_style);
        render_style = DRAW;
    }

    const size_t initial_queue_data_size = queued_vertex_data.size();

    for (vec2 vertex : polygon.vertices)
        queued_vertex_data.push_back(
            Vector<float, 5>({ vertex.x, vertex.y, color[0], color[1], color[2] }));

    if (!polygon.triangle_mesh)
        polygon.buildTriangleMesh();

    for (vec3i vertex : *polygon.triangle_mesh) {
        queued_elements.push_back(static_cast<GLuint>(vertex.x + initial_queue_data_size));
        queued_elements.push_back(static_cast<GLuint>(vertex.y + initial_queue_data_size));
        queued_elements.push_back(static_cast<GLuint>(vertex.y + initial_queue_data_size));
        queued_elements.push_back(static_cast<GLuint>(vertex.z + initial_queue_data_size));
        queued_elements.push_back(static_cast<GLuint>(vertex.z + initial_queue_data_size));
        queued_elements.push_back(static_cast<GLuint>(vertex.x + initial_queue_data_size));
    }

    if (alpha < 1.0f)
        flushRenderQueue(render_style, alpha);
}

void SimpleRenderer::drawRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha)
{
    if (alpha <= 0.0f)
        return;
    else if (alpha < 1.0f
             || render_style != DRAW)
    {
        flushRenderQueue(render_style);
        render_style = DRAW;
    }

    const std::array<vec2, 4> vertices = setupRectangleVertices(bottom_left, upper_right);
    const size_t initial_queue_data_size = queued_vertex_data.size();

    for (vec2 vertex : vertices)
        queued_vertex_data.push_back(
            Vector<float, 5>({ vertex.x, vertex.y, color[0], color[1], color[2] }));

    const int indices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
    for (int idx : indices)
        queued_elements.push_back(static_cast<GLuint>(idx + initial_queue_data_size));

    if (alpha < 1.0f)
        flushRenderQueue(render_style, alpha);
}

void SimpleRenderer::fillRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha)
{
    if (alpha <= 0.0f)
        return;
    else if (alpha < 1.0f
             || render_style != FILL)
    {
        flushRenderQueue(render_style);
        render_style = FILL;
    }

    const std::array<vec2, 4> vertices = setupRectangleVertices(bottom_left, upper_right);
    const size_t initial_queue_data_size = queued_vertex_data.size();
    // no reserve, since it causes queued_vertex_data and queued_elements to grow linearly
    // instead of exponentially
    for (vec2 vertex : vertices)
        queued_vertex_data.push_back(
            Vector<float, 5>({ vertex.x, vertex.y, color[0], color[1], color[2] }));

    const int indices[] = { 0, 1, 2, 0, 2, 3 };
    for (int idx : indices)
        queued_elements.push_back(static_cast<GLuint>(idx + initial_queue_data_size));

    if (alpha < 1.0f)
        flushRenderQueue(render_style, alpha);
}

void SimpleRenderer::drawCircle(vec2 center, float radius, vec3 color, float alpha)
{
    Polygon circle{ setupCircle(center, radius) };
    drawPolygon(circle, color, alpha);
}

void SimpleRenderer::fillCircle(vec2 center, float radius, vec3 color, float alpha)
{
    if (!circle_polygon) circle_polygon = createCirclePolygon();
    if (!circle_polygon->triangle_mesh) circle_polygon->buildTriangleMesh();

    Polygon circle{ setupCircle(center, radius) };
    fillPolygon(circle, color, alpha);
}

void SimpleRenderer::updateScreen() noexcept
{
    flushRenderQueue(render_style);
    renderFramebuffer(0);
    SDL_GL_SwapWindow(window->win_ptr);
}


// Private member functions
// -----------------------------------------------------------------------------
void SimpleRenderer::flushRenderQueue(RenderStyle style, float alpha) noexcept
{
    if (queued_elements.size() == 0)
        return;

    const size_t queued_data_size = sizeof(Vector<float,5>) * queued_vertex_data.size(),
              queued_element_size = sizeof(GLuint) * queued_elements.size();

    if (queued_data_size > vertex_buffer_size) growInternalVertexBuffer(queued_data_size);
    if (queued_element_size > element_buffer_size) growInternalElementBuffer(queued_element_size);

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, queued_data_size, queued_vertex_data.data()->data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, queued_element_size, queued_elements.data());

    glUseProgram(shader_program);
    setUniforms(alpha);

    GLenum draw_mode = (style == FILL) ? GL_TRIANGLES : GL_LINES;
    glDrawElements(draw_mode, queued_elements.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    queued_vertex_data.clear();
    queued_elements.clear();

    renderFramebuffer(backbuffer);
    glUseProgram(0);
}

void SimpleRenderer::setUniforms(float alpha) noexcept
{
    int alpha_loc = glGetUniformLocation(shader_program, "alpha");
    glUniform1f(alpha_loc, alpha);
    int model_to_world_loc = glGetUniformLocation(shader_program, "model_to_world_transf");
    mat2 id_mat = mat2::identity();
    // GL_TRUE (transpose) since OpenGL expects matrices in column-major order
    glUniformMatrix2fv(model_to_world_loc, 1, GL_TRUE, id_mat.data());

    int world_to_cam_loc = glGetUniformLocation(shader_program, "world_to_cam_transf");
    mat3 transf_matrix = camera.worldToCameraViewMatrix();
    glUniformMatrix3fv(world_to_cam_loc, 1, GL_TRUE, transf_matrix.data());
}

void SimpleRenderer::setupInternalFramebuffer() noexcept
{
    // Generate frame and color buffers
    glGenFramebuffers(1, &framebuffer);
    glGenFramebuffers(1, &backbuffer);
    glGenTextures(1, &framebuffer_texture);
    glGenTextures(1, &backbuffer_texture);
    vec2i dims = window->dimensions();

    // Create textures
    // -------------------------------------------
    // Framebuffer
    glActiveTexture(GL_TEXTURE0);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dims.x, dims.y, 0,
        GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bind texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        framebuffer_texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete" << std::endl;
        std::terminate();
    }

    // Backbuffer
    glActiveTexture(GL_TEXTURE1);
    glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
    glBindTexture(GL_TEXTURE_2D, backbuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dims.x, dims.y, 0,
        GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bind texture to backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        backbuffer_texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Backbuffer not complete" << std::endl;
        std::terminate();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    setupQuadVertexArray();

    // Bind textures to shaders
    glUseProgram(screen_shader);
    glUniform1i(glGetUniformLocation(screen_shader, "framebuffer"), 0);
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "backbuffer"), 1);
    glUseProgram(0);
}

void SimpleRenderer::setupInternalVertexObjects() noexcept
{
    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_buffer);

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_size, NULL, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
}

void SimpleRenderer::growInternalVertexBuffer(size_t minimum_capacity) noexcept
{
    glBindVertexArray(vertex_array);
    const size_t new_size = sizeGrowthFunction(vertex_buffer_size, minimum_capacity);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, new_size, NULL, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));

    vertex_buffer_size = new_size;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SimpleRenderer::growInternalElementBuffer(size_t minimum_capacity) noexcept
{
    glBindVertexArray(vertex_array);
    const size_t new_size = sizeGrowthFunction(element_buffer_size, minimum_capacity);
    glBindBuffer(GL_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ARRAY_BUFFER, new_size, NULL, GL_STREAM_DRAW);

    element_buffer_size = new_size;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SimpleRenderer::setupQuadVertexArray() noexcept
{
    glGenVertexArrays(1, &quad_vertex_array);
    glGenBuffers(1, &quad_vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer);
    //                        position      texture_coord
    float quad_vertices[] = { -1.0f,  1.0f, 0.0f, 1.0f,
                              -1.0f, -1.0f, 0.0f, 0.0f,
                               1.0f, -1.0f, 1.0f, 0.0f,
                               1.0f, -1.0f, 1.0f, 0.0f,
                               1.0f,  1.0f, 1.0f, 1.0f,
                              -1.0f,  1.0f, 0.0f, 1.0f };
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices,
        GL_STATIC_DRAW);

    glBindVertexArray(quad_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),
        (void*)(2*sizeof(float)));
}

Polygon SimpleRenderer::setupCircle(vec2 offset, float radius)
{
    if (!circle_polygon) circle_polygon = createCirclePolygon();
    Polygon new_circle{ *circle_polygon };
    for (vec2& vertex : new_circle.vertices) {
        vertex *= radius;
        vertex += offset;
    }
    return new_circle;
}

void SimpleRenderer::renderFramebuffer(GLuint target_framebuffer) noexcept
{
    // Render to target framebuffer (0 being the screen)
    glBindFramebuffer(GL_FRAMEBUFFER, target_framebuffer);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(screen_shader);
    glBindVertexArray(quad_vertex_array);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);

    // Rebind internal framebuffer for future rendering
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
}


} // namespace sini
