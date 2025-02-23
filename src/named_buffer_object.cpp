#include "named_buffer_object.hpp"

#include <utility>
#include "simple_logger.hpp"

namespace peria {

Buffer_Object::Buffer_Object()
{ peria::log("Buffer_Object ctor()"); glCreateBuffers(1, &id); }

Buffer_Object::~Buffer_Object()
{ peria::log("Buffer_Object dtor()"); glDeleteBuffers(1, &id); }

Buffer_Object::Buffer_Object(Buffer_Object&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{}

Buffer_Object& Buffer_Object::operator=(Buffer_Object&& rhs) noexcept
{
    if (&rhs == this) return *this;

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

}
