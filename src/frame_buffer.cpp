#include "frame_buffer.hpp"

#include "glad/glad.h"

#include <utility>

namespace peria {

Frame_Buffer::Frame_Buffer()
{ glCreateFramebuffers(1, &id); }

Frame_Buffer::~Frame_Buffer()
{ glDeleteFramebuffers(1, &id); id = 0; }

Frame_Buffer::Frame_Buffer(Frame_Buffer&& rhs) noexcept
{
    id = rhs.id;
    rhs.id = 0;
}

Frame_Buffer& Frame_Buffer::operator=(Frame_Buffer&& rhs) noexcept
{
    if (&rhs == this) return *this;
    std::swap(id, rhs.id);
    return *this;
}

}
