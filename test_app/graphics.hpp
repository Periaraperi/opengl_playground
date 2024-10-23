#pragma once

#include <glad/glad.h>

#include "peria_types.hpp"
#include "peria_color.hpp"
#include <glm/vec2.hpp>

typedef struct SDL_Window SDL_Window;

namespace graphics {

struct Vertex {
    glm::vec2 pos;
    Color<float> color;
};

void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;

void clear_color(const graphics::Color<float>& color) noexcept;

void clear_buffer() noexcept;

void swap_buffers(SDL_Window* window) noexcept;
}
