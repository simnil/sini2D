// sini::SubsystemInitializer initializes SDL subsystems with SDL_Init() when
// constructed and SDL_Quit() on destruction, so the initializer object
// should be alive as long as the specified subsystems are utilized. It can also
// be used to initialize other subsystems after being constructed.

#pragma once

#include "SDL.h"
#include <initializer_list>

namespace sini {


// Wrapper for SDL init flags
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
    // Constructors
    // -------------------------------------------------------------------------
    SubsystemInitializer() noexcept = delete;
    SubsystemInitializer(const SubsystemInitializer&) noexcept = delete;
    SubsystemInitializer& operator= (const SubsystemInitializer&) noexcept = delete;

    SubsystemInitializer(std::initializer_list<SubsystemFlags> flags) noexcept;
    ~SubsystemInitializer() noexcept;

    // Functions
    // -------------------------------------------------------------------------
    void initSubsystem(std::initializer_list<SubsystemFlags> flags) noexcept;
    void quitSubsystem(std::initializer_list<SubsystemFlags> flags) noexcept;
};

} // namespace sini
