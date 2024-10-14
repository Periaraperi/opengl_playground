#include "initializer.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <stdexcept>

namespace peria::engine::sdl {

initializer::initializer()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error{"SDL_Init failed"};

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0) 
        throw std::runtime_error{"SDL_GL_SetAttribute failed: " + std::string{SDL_GetError()}};
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6) != 0)
        throw std::runtime_error{"SDL_GL_SetAttribute failed: " + std::string{SDL_GetError()}};
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
        throw std::runtime_error{"SDL_GL_SetAttribute failed: " + std::string{SDL_GetError()}};

    std::cerr << "SDL initialized successfully\n";
}

initializer::~initializer()
{
    SDL_Quit();
    std::cerr << "SDL shut down successfully\n";
}

// custom deleters for SDL entities
void window_deleter::operator()(SDL_Window* window) const
{ std::cerr << "Destroying SDL_Window\n"; SDL_DestroyWindow(window); }

void gl_context_deleter::operator()(SDL_GLContext context) const
{ std::cerr << "Destroying SDL_GLContext\n"; SDL_GL_DeleteContext(context); }

}
