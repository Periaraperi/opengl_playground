#include "vertex_array.hpp"
#include "simple_logger.hpp"

#include <glad/glad.h>
#include <utility>

namespace peria {

Vertex_Array::Vertex_Array() noexcept
{ peria::log("Vertex_Array ctor()"); glCreateVertexArrays(1, &id); }

Vertex_Array::~Vertex_Array()
{ peria::log("Vertex_Array ctor()"); glDeleteVertexArrays(1, &id); }

Vertex_Array::Vertex_Array(Vertex_Array&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{}

Vertex_Array& Vertex_Array::operator=(Vertex_Array&& rhs) noexcept
{
    if (&rhs == this) return *this;

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

}

