#include <GL/glew.h>

#include "sini/sdl/SubsystemInitializer.hpp"
#include "sini/sdl/Window.hpp"
#include "sini/util/IO.hpp"

#include "sini/gl/GLContext.hpp"
#include "sini/gl/glutil.hpp"
#include "sini/gl/Camera.hpp"
#include "sini/gl/SimpleRenderer.hpp"

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include <initializer_list>
#include <chrono>

using namespace sini;
using gl::GLContext;
using gl::Camera;
using gl::SimpleRenderer;
using high_res_clock = std::chrono::high_resolution_clock;
using time_point = high_res_clock::time_point;
using std::chrono::duration_cast;
using time_ms = std::chrono::microseconds;
using time_sec_f = std::chrono::duration<float>;


mat2 rot_mat(float angle) {

	return mat2{
		{ cos(angle), -sin(angle)},
		{ sin(angle), cos(angle) }
	};
}

int main(int argc, char **argv) {

	std::cout << "Initializing SDL" << std::endl;
	SubsystemInitializer si{ {SubsystemFlags::VIDEO} };
	std::cout << "Creating window...";
	Window window{ "OpenGL testing" , {640, 480}, {WindowProperties::OPENGL} };
	std::cout << " done" << std::endl;
	//GLContext context{ window.win_ptr, 4, 2, gl::GLProfile::CORE };
	Camera camera{ {0, 0}, (float)window.width() / (float)window.height(), 2.0f };

	std::cout << "Creating simple renderer...";
	SimpleRenderer renderer{ window, camera };
	std::cout << " Done" << std::endl;

	// Vertices in model space
	std::vector<vec2> model_vertices_vec = {
		{0.0f, 0.5f},
		{-0.5f, -0.5f},
		{0.5f, -0.5f}
	};


	float theta = 0.1f;
	mat2 model_to_world_matrix = rot_mat(theta);
	std::vector<vec2> vertices_vec = std::vector<vec2>(model_vertices_vec.size());

	SDL_GL_SwapWindow(window.win_ptr);

	time_point start = high_res_clock::now();
	time_point frame_start, frame_end;
	time_ms elapsed_time, frame_time;
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}
		frame_start = high_res_clock::now();
		elapsed_time = duration_cast<time_ms>(frame_start - start);
		// Update camera
		float x = duration_cast<time_sec_f>(elapsed_time).count() * 0.5f;
		renderer.camera.width = 2 + 0.6f*cos(x);
		renderer.camera.position = vec2{ 0.6f*cos(x)*sin(x), 0.0f };
		// Update triangle
		model_to_world_matrix = rot_mat(x);
		for (int i = 0; i < model_vertices_vec.size(); i++)
			vertices_vec[i] = model_to_world_matrix * model_vertices_vec[i];
		// Clear and draw frame using SimpleRenderer
		renderer.clear({ vec3{ 1.0f } , 1.0f });
		renderer.drawPolygon(model_vertices_vec, { 0.0f }, 1.0f);
		renderer.drawPolygon(vertices_vec, { 1.0f, 0.0f, 0.0f }, 1.0f);

		SDL_GL_SwapWindow(window.win_ptr);

		// Measure frame time
		frame_end = high_res_clock::now();
		frame_time = duration_cast<time_ms>(frame_end - frame_start);
		std::cout << "frame time: "
			/* formatting */ << std::setw(5) << std::left
			<< static_cast<float>(frame_time.count())/1000 << " ms    "
			<< duration_cast<std::chrono::milliseconds>(frame_time).count()
			<< std::endl;
		// 17 ms -> ~60 fps,
		// sleep for the remaining time if the frametime was < 17 ms
		SDL_Delay(std::max(
			0,
			static_cast<int32_t>(17 - duration_cast<std::chrono::milliseconds>(frame_time).count())
		));
	}

	return 0;
}