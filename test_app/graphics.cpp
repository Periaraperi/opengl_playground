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

void populate_vbo(const std::vector<Vertex>& vertex_data) noexcept
{ glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW); }

void populate_ibo(const std::vector<u32>& indices) noexcept
{ glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32)*indices.size(), indices.data(), GL_STATIC_DRAW); }

void bind_buffer_object(u32 id, i32 type) noexcept
{ glBindBuffer(type, id); }

void bind_vao(u32 id) noexcept
{ glBindVertexArray(id); }

// direct state access functions
void create_vao(u32* id) noexcept
{ glCreateVertexArrays(1, id); }

void create_buffer_object(u32* id) noexcept
{ glCreateBuffers(1, id); }

// regular gen functions
void gen_vao(u32* id) noexcept
{ glGenVertexArrays(1, id); }

void gen_buffer_object(u32* id) noexcept
{ glGenBuffers(1, id); }

void delete_vao(u32* id) noexcept
{ glDeleteVertexArrays(1, id); }

void delete_buffer_object(u32* id) noexcept
{ glDeleteBuffers(1, id); }

}
