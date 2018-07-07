#include "sini/gl/SimpleRenderer.hpp"

namespace sini { namespace gl {

// Simple vertex and fragment shader source
// -----------------------------------------------------------------------------
static const char* simple_vertex_shader_src = R"glsl(
    #version 420 core
    precision highp float;

    uniform mat2 model_to_world_transf;
    uniform mat3 world_to_cam_transf;

    layout(location = 0) in vec2 position;

    void main() {
        vec3 camview_pos = world_to_cam_transf * vec3(model_to_world_transf
            * position, 1.0f);
        gl_Position = vec4(camview_pos.xy, 0.0f, 1.0f);
    }
)glsl";
// TODO simple geometry shader
static const char* simple_geometry_shader_src = nullptr;
static const char* simple_fragment_shader_src = R"glsl(
    #version 420 core
    precision highp float;

    uniform vec3 color;
    uniform float alpha; // UNFINISHED

    layout(location = 0) out vec4 fragment_color;

    void main() {
        fragment_color = vec4(color, 1.0f);
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

    uniform sampler2D framebuffer_texture;
    in vec2 texcoord;
    layout(location = 0) out vec4 fragment_color;

    void main() {
        fragment_color = texture(framebuffer_texture, texcoord);
    }
)glsl";


// Helper functions
// -----------------------------------------------------------------------------
namespace {
GLuint setupVertexBuffer(std::vector<vec2> vertices) noexcept
{
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * vertices.size(),
        vertices.data()->data(), GL_STREAM_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    return vertex_buffer;
}

std::vector<vec2> setupRectangleVertices(vec2 bottom_left, vec2 upper_right) noexcept
{
    std::vector<vec2> vertices;
    vertices.push_back(bottom_left);
    vertices.push_back({ upper_right.x, bottom_left.y });
    vertices.push_back(upper_right);
    vertices.push_back({ bottom_left.x, upper_right.y });
    return vertices;
}

Polygon* createCirclePolygon() noexcept
{
    std::vector<vec2> vertices;
    int32_t n_vertices = 64;
    float two_pi = 2.0f * 3.1415926535f;
    for (float angle = 0.0f; angle < two_pi; angle += two_pi / n_vertices)
        vertices.push_back({ std::cos(angle), std::sin(angle) });
    return new Polygon(std::move(vertices));
}
}


// Constructors and destructor
// -----------------------------------------------------------------------------
SimpleRenderer::SimpleRenderer(const Window& window, Camera camera) noexcept
    : camera(camera),
      window(&window),
      context(window.win_ptr, 4, 2, gl::GLProfile::CORE)
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

    setupInternalFramebuffer();

    // TODO? Set up vertex buffer objects for supported geometric shapes
    // with many vertices, for efficiency
}

SimpleRenderer::SimpleRenderer(const Window& window) noexcept
    : SimpleRenderer(window, Camera({ 0.0f, 0.0f }, 16.0f / 9.0f, 2.0f))
{}

SimpleRenderer::~SimpleRenderer() noexcept
{
    if (circle_polygon)
        delete circle_polygon;
    glDeleteTextures(1, &frame_color_buffer);
    glDeleteFramebuffers(1, &frame_buffer);
    glDeleteBuffers(1, &quad_vertex_buffer);
    glDeleteVertexArrays(1, &quad_vertex_array);
}


// Member functions
// -----------------------------------------------------------------------------
void SimpleRenderer::clear(vec4 clear_color) noexcept
{
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    vec2i dim = window->dimensions();
    glViewport(0, 0, dim.x, dim.y);
    glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SimpleRenderer::drawPolygon(Polygon polygon, vec3 color, float alpha) noexcept
{
    drawPolygon(polygon, 1.0f, color, alpha);
}

void SimpleRenderer::drawPolygon(Polygon polygon, float width, vec3 color, float alpha) noexcept
{
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);

    GLuint vertex_buffer = setupVertexBuffer(polygon.vertices);

    glUseProgram(shader_program);
    setUniforms(color, alpha);
    glLineWidth(width);

    glDrawArrays(GL_LINE_LOOP, 0, polygon.vertices.size());

    glLineWidth(1.0f);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_obj);
    glUseProgram(0);
}

void SimpleRenderer::fillPolygon(Polygon polygon, vec3 color, float alpha) noexcept
{
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);

    GLuint vertex_buffer = setupVertexBuffer(polygon.vertices);

    if (!polygon.triangle_mesh) polygon.buildTriangleMesh();
    std::vector<vec3i> triangle_mesh = *polygon.triangle_mesh;
    GLuint element_buffer,
          *elements = new GLuint[3*triangle_mesh.size()];
    for (int i = 0; i < 3*triangle_mesh.size(); i++)
        elements[i] = static_cast<GLuint>(*(triangle_mesh[0].data() + i));

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*triangle_mesh.size()*sizeof(GLuint),
        elements, GL_STREAM_DRAW);
    delete[] elements;

    glUseProgram(shader_program);
    setUniforms(color, alpha);

    glDrawElements(GL_TRIANGLES, 3*triangle_mesh.size(), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_obj);
    glUseProgram(0);
}

void SimpleRenderer::drawPolygonTriangleMesh(Polygon polygon, vec3 color, float alpha) noexcept
{
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);

    GLuint vertex_buffer = setupVertexBuffer(polygon.vertices);

    if (!polygon.triangle_mesh) polygon.buildTriangleMesh();
    std::vector<vec3i> triangle_mesh = *polygon.triangle_mesh;
    GLuint element_buffer;
    uint32_t n_elements = 6 * triangle_mesh.size();
    GLuint* elements = new GLuint[n_elements];
    for (int i = 0; i < triangle_mesh.size(); i++) {
        elements[6*i]     = triangle_mesh[i].x;
        elements[6*i + 1] = triangle_mesh[i].y;
        elements[6*i + 2] = triangle_mesh[i].y;
        elements[6*i + 3] = triangle_mesh[i].z;
        elements[6*i + 4] = triangle_mesh[i].z;
        elements[6*i + 5] = triangle_mesh[i].x;
    }

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_elements*sizeof(GLuint), elements,
        GL_STREAM_DRAW);
    delete[] elements;

    glUseProgram(shader_program);
    setUniforms(color, alpha);

    glDrawElements(GL_LINES, n_elements, GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_obj);
    glUseProgram(0);
}

void SimpleRenderer::drawRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha) noexcept
{
    drawRectangle(bottom_left, upper_right, 1.0f, color, alpha);
}

void SimpleRenderer::drawRectangle(vec2 bottom_left, vec2 upper_right, float width, vec3 color, float alpha) noexcept
{
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);

    std::vector<vec2> vertices = setupRectangleVertices(bottom_left, upper_right);
    GLuint vertex_buffer = setupVertexBuffer(vertices);

    glUseProgram(shader_program);
    setUniforms(color, alpha);
    glLineWidth(width);

    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());

    glLineWidth(1.0f);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_obj);
    glUseProgram(0);
}

void SimpleRenderer::fillRectangle(vec2 bottom_left, vec2 upper_right, vec3 color, float alpha) noexcept
{
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);

    std::vector<vec2> vertices = setupRectangleVertices(bottom_left, upper_right);
    GLuint vertex_buffer = setupVertexBuffer(vertices);
    GLuint elements[] = { 0, 1, 2, 0, 2, 3 };

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
        GL_STREAM_DRAW);

    glUseProgram(shader_program);
    setUniforms(color, alpha);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_obj);
    glUseProgram(0);
}

void SimpleRenderer::drawCircle(vec2 center, float radius, vec3 color, float alpha) noexcept
{
    drawCircle(center, radius, 1.0f, color, alpha);
}

void SimpleRenderer::drawCircle(vec2 center, float radius, float width, vec3 color, float alpha) noexcept
{
    Polygon circle = setupCircle(center, radius);
    drawPolygon(circle, width, color, alpha);
}

void SimpleRenderer::fillCircle(vec2 center, float radius, vec3 color, float alpha) noexcept
{
    if (!circle_polygon) circle_polygon = createCirclePolygon();
    if (!circle_polygon->triangle_mesh) circle_polygon->buildTriangleMesh();

    Polygon circle = setupCircle(center, radius);
    fillPolygon(circle, color, alpha);
}

void SimpleRenderer::updateScreen() noexcept
{
    renderFramebuffer();
    SDL_GL_SwapWindow(window->win_ptr);
}


// Private member functions
// -----------------------------------------------------------------------------
void SimpleRenderer::setUniforms(vec3 color, float alpha) noexcept
{
    int color_loc = glGetUniformLocation(shader_program, "color");
    glUniform3fv(color_loc, 1, color.data());
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
    // Generate framebuffer
    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

    // Generate color buffer
    glGenTextures(1, &frame_color_buffer);
    glBindTexture(GL_TEXTURE_2D, frame_color_buffer);
    vec2i dims = window->dimensions();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dims.x, dims.y, 0,
        GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        frame_color_buffer, 0);

    setupQuadVertexArray();

    glUseProgram(screen_shader);
    glUniform1i(glGetUniformLocation(screen_shader, "framebuffer_texture"), 0);
    glUseProgram(0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete" << std::endl;
        std::terminate();
    }
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

Polygon SimpleRenderer::setupCircle(vec2 offset, float radius) noexcept
{
    if (!circle_polygon) circle_polygon = createCirclePolygon();
    Polygon new_circle = Polygon(*circle_polygon);
    for (int32_t i = 0; i < new_circle.vertices.size(); i++) {
        new_circle.vertices[i] *= radius;
        new_circle.vertices[i] += offset;
    }
    return new_circle;
}

void SimpleRenderer::renderFramebuffer() noexcept
{
    // Render to default framebuffer (screen)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(screen_shader);
    glBindVertexArray(quad_vertex_array);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, frame_color_buffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);

    // Rebind internal framebuffer for future rendering
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    glBindTexture(GL_TEXTURE_2D, frame_color_buffer);
}


}} // namespace gl, namespace sini
