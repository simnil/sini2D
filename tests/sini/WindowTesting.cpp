#include <GL/glew.h>

#include "sini/sdl/SubsystemInitializer.hpp"
#include "sini/sdl/Window.hpp"
#include "sini/gl/GLContext.hpp"
#include "sini/gl/glutil.hpp"
#include "sini/gl/Camera.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>

#include <chrono>

using namespace sini;
using gl::GLContext;
using gl::Camera;
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
	GLContext context{ window.win_ptr, 4, 1, gl::GLProfile::CORE };
	Camera camera{ {0, 0}, (float)window.width() / (float)window.height(), 2.0f };

	// Vertices in model space
	float vertices[] = {
		0.0f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};
	glewInit();

	// Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	const char* vert_shader = R"glsl(
		#version 450 core
		in vec2 pos;
		uniform mat2 model_to_world_transf;
		uniform mat3 world_to_cam_transf;
		void main() {
			vec3 cam_pos = world_to_cam_transf * vec3(model_to_world_transf * pos, 1.0f);
			gl_Position = vec4(cam_pos.xy, 0.0f, 1.0f);
		}
	)glsl";
	const char* frag_shader = R"glsl(
		#version 450 core
		out vec4 outColor;
		void main() {
			outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	)glsl";
	
	GLuint shader_program = gl::loadShaderProgram(vert_shader, frag_shader);
	if (shader_program == 0)
		return 0;

	glBindFragDataLocation(shader_program, 0, "outColor");
	bool link_ok = gl::linkShaderProgram(shader_program);
	if (!link_ok) {
		std::cout << "Shader could not be linked" << std::endl;
		return 0;
	}
	glUseProgram(shader_program);

	GLint pos_attrib_loc = glGetAttribLocation(shader_program, "pos");
	glEnableVertexAttribArray(pos_attrib_loc);
	glVertexAttribPointer(pos_attrib_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLint world_to_cam_transf_uniform = glGetUniformLocation(shader_program, "world_to_cam_transf");
	mat3 transf_matrix = camera.worldToCameraViewMatrix();
	glUniformMatrix3fv(world_to_cam_transf_uniform, 1, GL_TRUE, transf_matrix.data());

	GLint model_to_world_location = glGetUniformLocation(shader_program, "model_to_world_transf");
	float theta = 0.1f;
	mat2 model_to_world_matrix = rot_mat(theta);
	glUniformMatrix2fv(model_to_world_location, 1, GL_TRUE, model_to_world_matrix.data());

	vec2i dims = window.dimensions();
	glViewport(0, 0, dims.x, dims.y);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_LINE_STRIP, 0, 3);

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
		camera.width = 2 + 0.6f*cos(x);
		camera.position = vec2{ 0.6f*cos(x)*sin(x), 0.0f };
		// Update camera matrix
		transf_matrix = camera.worldToCameraViewMatrix();
		// GL_TRUE (transpose) since OpenGL expects matrices in column-major order
		glUniformMatrix3fv(world_to_cam_transf_uniform, 1, GL_TRUE, transf_matrix.data());
		// Update triangle orientation (model -> world transformation)
		model_to_world_matrix = rot_mat(x);
		glUniformMatrix2fv(model_to_world_location, 1, GL_TRUE, model_to_world_matrix.data());

		glClearColor(0, 0, 0, 1);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		SDL_GL_SwapWindow(window.win_ptr);

		// Measure frame time
		frame_end = high_res_clock::now();
		frame_time = duration_cast<time_ms>(frame_end - frame_start);
		std::cout << "frame time: "
			/* formatting */ << std::setw(5) << std::left
			<< static_cast<float>(frame_time.count())/1000 << " ms    "
			<< duration_cast<std::chrono::milliseconds>(frame_time).count()
			<< std::endl;
		// 17 ms -> 60 fps,
		// sleep for the remaining time if the frametime was < 17 ms
		SDL_Delay(std::max(
			0,
			static_cast<int32_t>(17 - duration_cast<std::chrono::milliseconds>(frame_time).count())
		));
	}

	return 0;
}