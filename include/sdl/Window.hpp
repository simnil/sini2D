// sini::Window is basically a wrapper for SDL_Window, handling a few of the
// details "under the hood" as well as providing a wrapper for SDL_WindowFlags
//
// sini::Window is clearly similar to and has taken inspiration from
// sfz::sdl::Window from sfzCore and SkipIfZeroCommon by Peter Hillerström,
// (https://github.com/SkipIfZero). However, I don't see the point in going
// out of my way to find other solutions when I like the way he's done it. This
// does not necessarily represent the quality of Peter's work.
#pragma once

#include "math/Vector.hpp"

#include <SDL.h>

#include <initializer_list>
#include <vector>


namespace sini {

// Wrapper for SDL_WindowFlags
// https://wiki.libsdl.org/SDL_WindowFlags
enum class WindowProperties : Uint32 {
    FULLSCREEN          = SDL_WINDOW_FULLSCREEN,
    FULLSCREEN_DESKTOP  = SDL_WINDOW_FULLSCREEN_DESKTOP,
    OPENGL              = SDL_WINDOW_OPENGL,
    SHOWN               = SDL_WINDOW_SHOWN,
    HIDDEN              = SDL_WINDOW_HIDDEN,
    BORDERLESS          = SDL_WINDOW_BORDERLESS,
    RESIZABLE           = SDL_WINDOW_RESIZABLE,
    MINIMIZED           = SDL_WINDOW_MINIMIZED,
    MAXIMIZED           = SDL_WINDOW_MAXIMIZED,
    INPUT_GRABBED       = SDL_WINDOW_INPUT_GRABBED,
    INPUT_FOCUS         = SDL_WINDOW_INPUT_FOCUS,
    HIGH_DPI            = SDL_WINDOW_ALLOW_HIGHDPI,
    MOUSE_CAPTURE       = SDL_WINDOW_MOUSE_CAPTURE
};

enum class FullscreenMode : Uint32 {
    OFF       = 0,
    WINDOWED  = SDL_WINDOW_FULLSCREEN_DESKTOP,
    EXCLUSIVE = SDL_WINDOW_FULLSCREEN
};

enum class VSync : int {
    OFF      =  0,
    ON       =  1,
    ADAPTIVE = -1
};


class Window {
public:
    SDL_Window* const win_ptr;

    Window() = delete;
    Window(const Window&) noexcept = delete;
    Window& operator= (const Window&) noexcept = delete;

    Window(const char* title) noexcept;
    Window(const char* title, vec2i size) noexcept;
    Window(const char* title, vec2i size, std::initializer_list<WindowProperties> flags) noexcept;
    Window(const char* title, vec2i pos, vec2i size, std::initializer_list<WindowProperties> flags) noexcept;

    ~Window() noexcept;

    int32_t width() const noexcept;
    int32_t height() const noexcept;
    vec2i dimensions() const noexcept;
    int32_t drawWidth() const noexcept;
    int32_t drawHeight() const noexcept;
    vec2i drawingDimensions() const noexcept;

    void setSize(int width, int height) noexcept;
    // The second argument, display_index, is only relevant if using
    // FullscreenMode::EXCLUSIVE and is silently ignored otherwise.
    // display_index = -1 (the default value) means the current display
    void setFullscreen(FullscreenMode mode, int display_index = -1) noexcept;
    void setVSync(VSync mode) noexcept;
};

// Other functions
std::vector<vec2i> getAvailableResolutions();

} // namespace sini
