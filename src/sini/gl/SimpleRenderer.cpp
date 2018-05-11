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
    //layout(location = 1) in vec3 color;

    void main() {
        vec3 camview_pos = world_to_cam_transf * vec3(model_to_world_transf
            * position, 1.0f);
        gl_Position = vec4(camview_pos.xy, 0.0f, 1.0f);
        //out_color = vec4(color, alpha);
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
}


// Constructors
// -----------------------------------------------------------------------------
SimpleRenderer::SimpleRenderer(const Window& window, Camera camera) noexcept
    : camera(camera),
      window(&window),
      context(window.win_ptr, 4, 2, gl::GLProfile::CORE)
{
    glewInit();
    shader_program = loadShaderProgram(simple_vertex_shader_src,
        simple_geometry_shader_src, simple_fragment_shader_src);
    // Print error message and terminate if failure
    // TODO Print error message if load fails
    if (shader_program == 0) {
        std::cerr << "Simple shader program could not load" << std::endl;
        std::terminate();
    }

    // TODO? Set up vertex buffer objects for supported geometric shapes
    // with many vertices, for efficiency
}

SimpleRenderer::SimpleRenderer(const Window& window) noexcept
    : SimpleRenderer(window, Camera({ 0.0f, 0.0f }, 16.0f / 9.0f, 2.0f))
{}


// Member functions
// -----------------------------------------------------------------------------
void SimpleRenderer::clear(vec4 clear_color) noexcept
{
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

    glDrawElements(GL_TRIANGLES, sizeof(elements), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_obj);
    glUseProgram(0);
}

void SimpleRenderer::updateScreen() noexcept
{
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


}} // namespace gl, namespace sini
