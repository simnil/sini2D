// Conveniece wrapper around SDL2 initialization and exiting on creation and
// destruction. SDL2 initialization flags are wrapped in an enum class, which
// can be passed during construction or at a later time. Throws a
// sini::SdlException if SDL2 initialization fails, so the main function can
// crash gracefully.
#pragma once

#include <SDL.h>

#include <initializer_list>


namespace sini {

// SDL init flags
// https://wiki.libsdl.org/SDL_Init
enum class SubsystemFlags : Uint32 {
    TIMER           = SDL_INIT_TIMER,
    AUDIO           = SDL_INIT_AUDIO,
    VIDEO           = SDL_INIT_VIDEO,
    JOYSTICK        = SDL_INIT_JOYSTICK,
    HAPTIC          = SDL_INIT_HAPTIC,
    GAME_CONTROLLER = SDL_INIT_GAMECONTROLLER,
    EVENTS          = SDL_INIT_EVENTS,
    EVERYTHING      = SDL_INIT_EVERYTHING,
    NO_PARACHUTE    = SDL_INIT_NOPARACHUTE
};


class SubsystemInitializer {
public:
    SubsystemInitializer() = delete;
    SubsystemInitializer(const SubsystemInitializer&) = delete;
    SubsystemInitializer& operator= (const SubsystemInitializer&) = delete;

    SubsystemInitializer(std::initializer_list<SubsystemFlags> flags);
    ~SubsystemInitializer();

    void initSubsystem(std::initializer_list<SubsystemFlags> flags);
    void quitSubsystem(std::initializer_list<SubsystemFlags> flags) noexcept;
};

} // namespace sini
