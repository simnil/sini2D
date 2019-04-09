#pragma once

#include <string>

namespace sini {

std::string readTextFile(const char* file_path);

// Returns the path to the executable. Wraps the SDL function
// SDL_GetBasePath.
std::string getBasePath();

} // namespace sini
