// Convenience wrapper for constructing and destructing a window using
// SDL2. Accepts property flags, wrapped in an enum class, which are passed on
// to the window creation. Also wraps other window-related flags as enum
// classes.
#pragma once

#include <sini2D/math/Vector.hpp>

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
    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;

    Window(const char* title);
    Window(const char* title, vec2i size);
    Window(const char* title, vec2i size, std::initializer_list<WindowProperties> flags);
    Window(const char* title, vec2i pos, vec2i size, std::initializer_list<WindowProperties> flags);

    ~Window();

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

std::vector<vec2i> getAvailableResolutions();

} // namespace sini
