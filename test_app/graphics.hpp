#pragma once

#include <glad/glad.h>

#include "peria_types.hpp"
#include "peria_color.hpp"
#include "glm/vec2.hpp"
#include <vector>

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

void populate_vbo(const std::vector<Vertex>& vertex_data) noexcept;

void populate_ibo(const std::vector<u32>& indices) noexcept;

template<typename T>
void populate_named_buffer_object(u32 id, const std::vector<T>& buffer_data) noexcept
{ glNamedBufferData(id, sizeof(T)*buffer_data.size(), buffer_data.data(), GL_STATIC_DRAW); }

void bind_buffer_object(u32 id, i32 type) noexcept;

void bind_vao(u32 id) noexcept;

// direct state access functions
void create_vao(u32* id) noexcept;

void create_buffer_object(u32* id) noexcept;

// regular gen functions
void gen_vao(u32* id) noexcept;

void gen_buffer_object(u32* id) noexcept;

void delete_vao(u32* id) noexcept;

void delete_buffer_object(u32* id) noexcept;

}
