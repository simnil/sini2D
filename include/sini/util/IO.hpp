#pragma once
#ifndef SINI_IO_H
#define SINI_IO_H

#include <fstream>
#include <string>
#include "SDL.h"

//debug
#include <iostream>

namespace sini {

	std::string readTextFile(const char* file_path) noexcept;

	// Returns the path to the executable. Wraps the SDL function
	// SDL_GetBasePath.
	std::string getBasePath() noexcept;

} // namespace sini

#endif // !SINI_IO_H