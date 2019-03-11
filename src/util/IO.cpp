#include "util/IO.hpp"

#include <SDL.h>

#include <fstream>


namespace sini {

std::string readTextFile(const char* file_path) noexcept
{
    std::ifstream file{ file_path };
    std::string file_str{ std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>() };

    return file_str;
}

std::string getBasePath() noexcept
{
    static const std::string base_path = []()->std::string {
        char *sdl_base_path = SDL_GetBasePath();
        std::string base_path{ sdl_base_path };
        SDL_free(sdl_base_path);
        return base_path;
    }();
    return base_path;
}

} // namespace sini
