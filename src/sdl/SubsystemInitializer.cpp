#include <sini2D/sdl/SubsystemInitializer.hpp>

#include <sini2D/sdl/SdlException.hpp>

#include <iostream>


namespace sini {

namespace {

Uint32 combineSdlFlags(std::initializer_list<SubsystemFlags> flags) noexcept
{
    Uint32 temp = 0;
    for (SubsystemFlags flag : flags)
        temp |= static_cast<Uint32>(flag);
    return temp;
}

} // unnamed namespace


SubsystemInitializer::SubsystemInitializer(std::initializer_list<SubsystemFlags> flags)
{
    const Uint32 SDL_flag = combineSdlFlags(flags);
    if (SDL_Init(SDL_flag) != 0) {
        throw SdlException(std::string("SDL_Init() failed: ")
                           + std::string(SDL_GetError()));
    }
}

SubsystemInitializer::~SubsystemInitializer()
{
    SDL_Quit();
}

void SubsystemInitializer::initSubsystem(std::initializer_list<SubsystemFlags> flags)
{
    const Uint32 SDL_flag = combineSdlFlags(flags);
    if (SDL_InitSubSystem(SDL_flag) != 0) {
        throw SdlException(std::string("SDL_InitSubSystem() failed: ")
                           + std::string(SDL_GetError()));
    }
}

void SubsystemInitializer::quitSubsystem(std::initializer_list<SubsystemFlags> flags) noexcept
{
    const Uint32 SDL_flag = combineSdlFlags(flags);
    SDL_QuitSubSystem(SDL_flag);
}

} // namespace sini
