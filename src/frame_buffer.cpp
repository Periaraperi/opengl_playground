#include "frame_buffer.hpp"

#include "glad/glad.h"

#include "simple_logger.hpp"
#include <utility>

namespace peria::graphics {

Frame_Buffer::Frame_Buffer(i32 width_, i32 height_)
{
    peria::log("Frame_Buffer ctor()");
    glCreateFramebuffers(1, &id);
    color_attachment_texture = std::make_unique<Texture>(width_, height_);
    glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0, color_attachment_texture->texture_id(), 0);

    depth_stencil_buffer = std::make_unique<Render_Buffer>(width_, height_);
    glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer->render_buffer_id());

    if (glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        peria::log("ERROR: Frame Buffer is not complete!");
    }
}

Frame_Buffer::Frame_Buffer(Frame_Buffer&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{ peria::log("Move constructing Frame_Buffer"); }

Frame_Buffer& Frame_Buffer::operator=(Frame_Buffer&& rhs) noexcept
{
    if (&rhs == this) return *this;
    peria::log("Move assigning Frame_Buffer"); 

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

Frame_Buffer::~Frame_Buffer()
{ 
    peria::log("Frame_Buffer dtor()");
    glDeleteFramebuffers(1, &id); 
}

void Frame_Buffer::bind() const noexcept
{ glBindFramebuffer(GL_FRAMEBUFFER, id); }

glm::vec2 Frame_Buffer::dimensions() const noexcept
{ return color_attachment_texture->dimensions(); }

u32 Frame_Buffer::texture_id() const noexcept
{ return color_attachment_texture->texture_id(); }

u32 Frame_Buffer::fbo_id() const noexcept
{ return id; }

Texture* Frame_Buffer::texture() const noexcept
{ return color_attachment_texture.get(); }

}
