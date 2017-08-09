
#include "sini/sdl/SubsystemInitializer.hpp"
#include "sini/sdl/Window.hpp"
#include <iostream>

using namespace sini;

int main(int argc, char **argv) {

	std::cout << "Initializing SDL" << std::endl;
	SubsystemInitializer si{ {SubsystemFlags::VIDEO} };
	std::cout << "Creating window" << std::endl;
	Window window{ "test" };

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