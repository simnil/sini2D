#include <GL/glew.h>

#include "sini/sdl/SubsystemInitializer.hpp"
#include "sini/sdl/Window.hpp"
#include "sini/util/IO.hpp"

#include "sini/gl/GLContext.hpp"
#include "sini/gl/glutil.hpp"
#include "sini/gl/Camera.hpp"
#include "sini/gl/SimpleRenderer.hpp"

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>   // For high_resolution_clock, time_point, duration_cast, duration
#include <thread>   // For this_thread::sleep_for

using namespace sini;
using gl::GLContext;
using gl::Camera;
using gl::SimpleRenderer;
using high_res_clock = std::chrono::high_resolution_clock;
using time_point = high_res_clock::time_point;
using std::chrono::duration_cast;
using time_ms = std::chrono::duration<float, std::milli>;
using time_sec = std::chrono::duration<float>;
using std::cos;
using std::sin;


mat2 rot_mat(float angle)
{
    return mat2{
        { cos(angle), -sin(angle)},
        { sin(angle), cos(angle) }
    };
}

int main(int argc, char **argv)
{
    std::cout << "Initializing SDL...";
    SubsystemInitializer si{ {SubsystemFlags::VIDEO} };
    std::cout << " Done\nCreating window...";
    Window window{ "OpenGL testing" , {640, 480}, {WindowProperties::OPENGL} };
    std::cout << " Done" << std::endl;

    Camera camera{ {0, 0}, (float)window.width() / (float)window.height(), 2.0f };

    std::cout << "Creating simple renderer...";
    SimpleRenderer renderer{ window, camera };
    std::cout << " Done" << std::endl;

    // Vertices in model space
    std::vector<vec2> model_vertices_vec = {
        { 0.0f, 0.4f },
        { -0.2f, 0.1f },
        { 0.1f, -0.2f },
        { 0.5f, 0.0f },
        { 0.25f, 0.25f },
        { 0.4f, 0.0f },
        { 0.0f, 0.0f }
    };

    mat2 model_to_world_matrix = rot_mat(0.1f);
    std::vector<vec2> vertices_vec = model_vertices_vec;
    Polygon model_polygon = model_vertices_vec,
                  polygon = model_vertices_vec;

    std::cout << "Building triangle mesh...\n";
    polygon.buildTriangleMesh();

    std::cout << "Triangle mesh: {";
    std::vector<vec3i> triangle_mesh = *polygon.triangle_mesh;
    for (size_t i = 0; i < triangle_mesh.size()-1; i++)
        std::cout << triangle_mesh[i] << ", ";
    std::cout << triangle_mesh.back() << "}\n";

    SDL_GL_SwapWindow(window.win_ptr);

    bool display_frame_time = true;
    time_point start = high_res_clock::now(), frame_start, frame_end;
    time_ms elapsed_time, frame_time, extra_time;
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c &&
                    e.key.keysym.mod & KMOD_CTRL)
                    quit = true;
            }
        }
        frame_start = high_res_clock::now();
        elapsed_time = frame_start - start;
        // Update camera
        float x = duration_cast<time_sec>(elapsed_time).count() * 0.5f;
        renderer.camera.width = 2 + 0.6f*cos(x);
        renderer.camera.position = vec2{ 0.6f*cos(x)*sin(x), 0.0f };
        // Update triangle
        model_to_world_matrix = rot_mat(x);
        for (size_t i = 0; i < model_vertices_vec.size(); i++) {
            polygon.vertices[i] = model_to_world_matrix * model_vertices_vec[i];
            vertices_vec[i] = model_to_world_matrix * model_vertices_vec[i];
        }

        // Clear and draw frame using SimpleRenderer
        renderer.clear({ vec3{ 1.0f } , 1.0f });
        renderer.fillRectangle({-0.35f, -0.25f}, { 0.35f, 0.25f }, { 0.05f, 0.05f, 0.9f }, 1.0f);
        renderer.fillPolygon(model_polygon, { 0.0f }, 1.0f);
        renderer.fillPolygon(polygon, { 1.0f, 0.0f, 0.0f }, 0.5f);
        renderer.fillCircle({ 0.0f, 0.0f }, 0.1f + 0.05f * cos(2*x), { 0.0f, 1.0f, 0.0f }, 1.0f);
        renderer.drawCircle({ 0.0f, 0.0f }, 0.1f + 0.05f * cos(4*x), 2.0f, { 1.0f }, 1.0f);
        renderer.drawPolygonTriangleMesh(polygon, { 1.0f, 0.0f, 0.0f }, 1.0f);
        // renderer.drawPolygon(polygon, { 1.0f, 0.0f, 0.0f }, 1.0f);

        renderer.updateScreen();

        // Measure frame time
        frame_end = high_res_clock::now();
        frame_time = frame_end - frame_start;
        // 17 ms -> ~60 fps,
        // sleep for the remaining time if the frametime was < 17 ms
        extra_time = time_ms{ 17 } - frame_time;

        if (display_frame_time) {
            std::cout << std::setprecision(3) << std::fixed;
            std::cout << "frame time: " << std::setw(6) << frame_time.count() << " ms   "
                      << "remaining: " << std::setprecision(2) << extra_time.count() << " ms   ";
        }

        extra_time = std::chrono::floor<time_ms>(extra_time);
        if (extra_time.count() > 0) {
            time_point sleep_start = high_res_clock::now();
            std::this_thread::sleep_for(extra_time);
            time_ms sleep_duration = high_res_clock::now() - sleep_start;

            if (display_frame_time)
                std::cout << "slept for: " << sleep_duration.count() << " ms\n";
        }
        else if (display_frame_time)
            std::cout << std::endl;
    }

    return 0;
}
