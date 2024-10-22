#include "vertex_array.hpp"

#include <glad/glad.h>

#include <iostream>

namespace graphics {
Vertex_Array::Vertex_Array() noexcept
{ std::cerr << "Vao ctor()\n"; glCreateVertexArrays(1, &id); }

Vertex_Array::~Vertex_Array()
{ std::cerr << "Vao dtor()\n"; glDeleteVertexArrays(1, &id); }

void Vertex_Array::connect_vertex_buffer(u32 vbo, std::size_t stride) noexcept
{ glVertexArrayVertexBuffer(id, 0, vbo, 0, stride); }

void Vertex_Array::connect_index_buffer(u32 ibo) noexcept
{ glVertexArrayElementBuffer(id, ibo); }

void Vertex_Array::bind() noexcept
{ glBindVertexArray(id); }

}
