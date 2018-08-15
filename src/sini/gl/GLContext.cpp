// A class wrapping the creation and deletion of an OpenGL context upon creation
// and destruction, using SDL_GL_<Create/Delete>Context. Therefore the GLContext
// object must be alive as long as OpenGL function calls are made. Usually this
// is the lifetime of the whole application.

#include <iostream>
#include <exception>

#include "sini/gl/GLContext.hpp"

namespace sini::gl {

// Constructor
GLContext::GLContext(SDL_Window* win_ptr, int gl_major_version, int gl_minor_version, GLProfile gl_profile) noexcept
{
    // Set OpenGL context to the desired version
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version) != 0) {
        std::cerr << "Failed to set GL context major version: "
            << SDL_GetError() << std::endl;
        std::terminate();
    }
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version) != 0) {
        std::cerr << "Failed to set GL context minor version: "
            << SDL_GetError() << std::endl;
        std::terminate();
    }

    // Set OpenGL to the desired gl_profile
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, static_cast<Uint32>(gl_profile)) != 0) {
        std::cerr << "Failed to set GL profile: "
            << SDL_GetError() << std::endl;
        std::terminate();
    }

    handle = SDL_GL_CreateContext(win_ptr);
}

// Destructor
GLContext::~GLContext() noexcept
{
    SDL_GL_DeleteContext(handle);
}

} // namespace sini::gl
