#pragma once

#include <string>

namespace sini {

std::string readTextFile(const char* file_path) noexcept;

// Returns the path to the executable. Wraps the SDL function
// SDL_GetBasePath.
std::string getBasePath() noexcept;

} // namespace sini
