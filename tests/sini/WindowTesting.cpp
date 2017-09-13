


#include <GL/glew.h>

#include "sini/sdl/SubsystemInitializer.hpp"
#include "sini/sdl/Window.hpp"
#include "sini/gl/GLContext.hpp"
#include "sini/gl/glutil.hpp"

#include <iostream>

using namespace sini;

int main(int argc, char **argv) {

	std::cout << "Initializing SDL" << std::endl;
	SubsystemInitializer si{ {SubsystemFlags::VIDEO} };
	std::cout << "Creating window" << std::endl;
	Window window{ "test" , {640, 480}, {WindowProperties::OPENGL} };
	gl::GLContext context{ window.win_ptr, 4, 1, gl::GLProfile::CORE };

	float vertices[] = {
		0.0f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};
	glewInit();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	const char* vert_shader = R"glsl(
		#version 420 core
		in vec2 pos;
		void main() {
			gl_Position = vec4(pos, 0.0f, 1.0f);
		}
	)glsl";
	const char* frag_shader = R"glsl(
		#version 420 core
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

	GLint pos_attrib = glGetAttribLocation(shader_program, "pos");
	glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_attrib);

	glBindVertexArray(vbo);

	int w, h;
	SDL_GetWindowSize(window.win_ptr, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapWindow(window.win_ptr);

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			SDL_Delay(10);
		}
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window.win_ptr);
	}

	return 0;
}