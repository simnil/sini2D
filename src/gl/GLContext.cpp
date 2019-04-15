// A class wrapping the creation and deletion of an OpenGL context upon creation
// and destruction, using SDL_GL_<Create/Delete>Context. Therefore the GLContext
// object must be alive as long as OpenGL function calls are made. Usually this
// is the lifetime of the whole application.
#include <sini2D/gl/GLContext.hpp>

#include <sini2D/gl/OpenGlException.hpp>

#include <string>


namespace sini {

GLContext::GLContext(SDL_Window* win_ptr,
                     int gl_major_version, int gl_minor_version,
                     GLProfile gl_profile)
{
    // Set OpenGL context to the desired version
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version) != 0)
        throw OpenGlException(std::string("Failed to set GL context major version: ")
                              + std::string(SDL_GetError()));

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version) != 0)
        throw OpenGlException(std::string("Failed to set GL context minor version: ")
                              + std::string(SDL_GetError()));

    // Set OpenGL to the desired gl_profile
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, static_cast<Uint32>(gl_profile)) != 0)
        throw OpenGlException(std::string("Failed to set GL profile: ")
                              + std::string(SDL_GetError()));

    handle = SDL_GL_CreateContext(win_ptr);
}

GLContext::~GLContext()
{
    SDL_GL_DeleteContext(handle);
}

}
