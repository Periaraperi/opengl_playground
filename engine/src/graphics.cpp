#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>


namespace peria::engine {

std::vector<simple_vertex> init_simple_vbo()
{
    return {
        {{-0.5f, -0.5f}},
        {{-0.5f,  0.5f}},
        {{ 0.5f,  0.5f}},
        {{ 0.5f, -0.5f}},
    };
}

graphics::graphics()
    :vao{std::vector<vertex_array::vertex_attribute>{{2, GL_FLOAT, false, sizeof(simple_vertex)}}}, vbo{init_simple_vbo()}
{ std::cerr << "Graphics initialized\n"; }

graphics::~graphics()
{ std::cerr << "Graphics shutting down\n"; }

void graphics::set_clear_color(const peria_color<float>& color)
{ clear_color = color; }

void graphics::set_viewport(i32 x, i32 y, u32 w, u32 h)
{
    std::cerr << "Viewport set to " << x << " " << y << " " << w << " " << h << '\n';
    glViewport(x, y, w, h);
    // update projection matrix here later when we add it
}

void graphics::clear_buffer()
{ 
    const auto& [r, g, b, a] = clear_color;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void graphics::swap_buffers(SDL_Window* window)
{ SDL_GL_SwapWindow(window); }

}
