// sini::Window is basically a wrapper for SDL_Window, handling a few of the
// details "under the hood" as well as providing a wrapper for SDL_WindowFlags
//
// sini::Window is clearly similar to and has taken inspiration from
// sfz::sdl::Window from sfzCore and SkipIfZeroCommon by Peter Hillerström,
// (https://github.com/SkipIfZero). However, I don't see the point in going
// out of my way to find other solutions when I like the way he's done it. This
// does not necessarily represent the quality of Peter's work.

#pragma once
#ifndef SINI_WINDOW_H
#define SINI_WINDOW_H

#include "SDL.h"
#include <initializer_list>
#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>            // For std::sort

#include "sini/math/Vector.hpp"

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

// Fullscreen modes
enum class FullscreenMode : uint8_t {
    OFF = 0,
    WINDOWED = 1,
    EXCLUSIVE = 2
};

// Vsync modes
enum class VSync : uint8_t {
    OFF = 0,
    ON = 1,
    LATE_SWAP_TEAR = 2
};


class Window {
public:
    SDL_Window* const win_ptr;

    // Constructors
    // -------------------------------------------------------------------------
    Window() = delete;
    Window(const Window&) noexcept = delete;
    Window& operator= (const Window&) noexcept = delete;

    Window(const char* title) noexcept;
    Window(const char* title, vec2i size) noexcept;
    Window(const char* title, vec2i size, std::initializer_list<WindowProperties> flags) noexcept;
    Window(const char* title, vec2i pos, vec2i size, std::initializer_list<WindowProperties> flags) noexcept;

    ~Window() noexcept;

    // Functions
    // -------------------------------------------------------------------------
    // Getters
    int32_t width() const noexcept;
    int32_t height() const noexcept;
    vec2i dimensions() const noexcept;
    int32_t drawWidth() const noexcept;
    int32_t drawHeight() const noexcept;
    vec2i drawingDimensions() const noexcept;
    // Setters
    void setSize(int width, int height) noexcept;
    void setFullscreen(FullscreenMode mode, int display_index) noexcept;
    void setVSync(VSync mode) noexcept;
};

// Other functions
std::vector<vec2i> getAvailableResolutions() noexcept;

} // namespace sini
#endif // SINI_WINDOW_H
