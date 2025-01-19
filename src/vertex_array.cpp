#include "vertex_array.hpp"
#include "simple_logger.hpp"

#include <glad/glad.h>

namespace peria::graphics {
Vertex_Array::Vertex_Array() noexcept
    :offsets(16, 0)
{ peria::log("Vao ctor()"); glCreateVertexArrays(1, &id); }

Vertex_Array::~Vertex_Array()
{ peria::log("Vao dtor()"); glDeleteVertexArrays(1, &id); }

void Vertex_Array::connect_vertex_buffer(u32 vbo, std::size_t stride, u32 binding_index /*=0*/) const noexcept
{ glVertexArrayVertexBuffer(id, binding_index, vbo, 0, stride); }

void Vertex_Array::connect_index_buffer(u32 ibo) const noexcept
{ glVertexArrayElementBuffer(id, ibo); }

void Vertex_Array::bind() const noexcept
{ glBindVertexArray(id); }

u32 Vertex_Array::get_id() const noexcept
{ return id; }

}
