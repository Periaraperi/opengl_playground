#include "frame_buffer.hpp"

#include "glad/glad.h"

#include <utility>

namespace peria {

Frame_Buffer::Frame_Buffer()
{ glCreateFramebuffers(1, &id); }

Frame_Buffer::~Frame_Buffer()
{ glDeleteFramebuffers(1, &id); }

Frame_Buffer::Frame_Buffer(Frame_Buffer&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{}

Frame_Buffer& Frame_Buffer::operator=(Frame_Buffer&& rhs) noexcept
{
    if (&rhs == this) return *this;

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

}
