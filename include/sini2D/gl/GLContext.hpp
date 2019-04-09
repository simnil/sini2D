// A class wrapping the creation and deletion of an OpenGL context upon creation
// and destruction, using SDL_GL_<Create/Delete>Context. Therefore the GLContext
// object must be alive as long as OpenGL function calls are made. Usually this
// is the lifetime of the whole application.
#pragma once

#include <SDL.h>


namespace sini {

// Wrapper for SDL_GLprofile
// https://wiki.libsdl.org/SDL_GLprofile
enum class GLProfile : Uint32 {
    CORE            = SDL_GL_CONTEXT_PROFILE_CORE,
    COMPATIBILITY   = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
    ES              = SDL_GL_CONTEXT_PROFILE_ES
};


class GLContext {
public:
    SDL_GLContext handle;

    GLContext() noexcept = delete;
    GLContext(const GLContext&) noexcept = delete;
    GLContext& operator= (const GLContext&) noexcept = delete;
    GLContext(SDL_Window* win_ptr,
              int gl_major_version, int gl_minor_version,
              GLProfile profile) noexcept;

    ~GLContext() noexcept;
};

}
