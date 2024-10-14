#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

namespace peria::engine {
graphics::graphics()
{
    std::cerr << "Graphics initialized\n";
}

graphics::~graphics()
{
    std::cerr << "Graphics shutting down\n";
}

void graphics::set_clear_color(const peria_color<float>& color)
{ clear_color = color; }

void graphics::clear_buffer()
{ 
    const auto& [r, g, b, a] = clear_color;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void graphics::swap_buffers(SDL_Window* window)
{ SDL_GL_SwapWindow(window); }

}
