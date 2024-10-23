#include "graphics.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>

namespace graphics {

void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept
{ glViewport(x, y, w, h); }

void clear_color(const graphics::Color<float>& color) noexcept
{ 
    const auto& [r, g, b, a] = color;
    glClearColor(r, g, b, a); 
}

void clear_buffer() noexcept
{ glClear(GL_COLOR_BUFFER_BIT); }

void swap_buffers(SDL_Window* window) noexcept
{ SDL_GL_SwapWindow(window); }
}
