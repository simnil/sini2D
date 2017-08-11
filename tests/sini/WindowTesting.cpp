
#include <GL/glew.h>

#include "sini/sdl/SubsystemInitializer.hpp"
#include "sini/sdl/Window.hpp"
#include "sini/gl/GLContext.hpp"

#include <iostream>

using namespace sini;

int main(int argc, char **argv) {

	std::cout << "Initializing SDL" << std::endl;
	SubsystemInitializer si{ {SubsystemFlags::VIDEO} };
	std::cout << "Creating window" << std::endl;
	Window window{ "test" , 640, 480, {WindowProperties::OPENGL} };
	gl::GLContext context{ window.win_ptr, 4, 1, gl::GLProfile::CORE };

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window.win_ptr);

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			SDL_Delay(10);
		}
	}

	return 0;
}