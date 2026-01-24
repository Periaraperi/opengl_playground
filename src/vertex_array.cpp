#include "vertex_array.hpp"
#include "simple_logger.hpp"

#include <glad/glad.h>
#include <utility>

namespace peria {

Vertex_Array::Vertex_Array() noexcept
{ peria::log("Vertex_Array ctor()"); glCreateVertexArrays(1, &id); }

Vertex_Array::~Vertex_Array()
{ peria::log("Vertex_Array ctor()"); glDeleteVertexArrays(1, &id); id = 0; }

Vertex_Array::Vertex_Array(Vertex_Array&& rhs) noexcept
{
    id = rhs.id;
    rhs.id = 0;
}

Vertex_Array& Vertex_Array::operator=(Vertex_Array&& rhs) noexcept
{
    if (&rhs == this) return *this;
    std::swap(id, rhs.id);
    return *this;
}

}

