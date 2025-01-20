#include "render_buffer.hpp"

#include <glad/glad.h>

#include <utility>
#include "simple_logger.hpp"

namespace peria::graphics {

Render_Buffer::Render_Buffer(i32 width_, i32 height_)
{
    peria::log("Render_Buffer ctor()"); 
    glCreateRenderbuffers(1, &id);
    glNamedRenderbufferStorage(id, GL_DEPTH24_STENCIL8, width_, height_);
}

Render_Buffer::Render_Buffer(i32 width_, i32 height_, i32 samples)
{
    peria::log("Render_Buffer multisampled ctor()"); 
    glCreateRenderbuffers(1, &id);
    glNamedRenderbufferStorageMultisample(id, samples, GL_DEPTH24_STENCIL8, width_, height_);
}

Render_Buffer::Render_Buffer(Render_Buffer&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{ peria::log("Move constructing Render_Buffer"); }

Render_Buffer& Render_Buffer::operator=(Render_Buffer&& rhs) noexcept
{
    if (&rhs == this) return *this;
    peria::log("Move assigning Render_Buffer"); 

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

Render_Buffer::~Render_Buffer()
{
    peria::log("Render_Buffer dtor()"); 
    glDeleteRenderbuffers(1, &id);
}

u32 Render_Buffer::render_buffer_id() const noexcept
{ return id; }

}
