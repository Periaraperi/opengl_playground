#include "shadowmapper.hpp"
#include "graphics.hpp"

namespace peria {

Shadowmapper::Shadowmapper(i32 w, i32 h) noexcept
    :width{w}, height{h},
     shadowmap{create_texture2d(width, height, GL_DEPTH_COMPONENT32F)}
{
    glNamedFramebufferTexture(shadow_fbo.id, GL_DEPTH_ATTACHMENT, shadowmap.id, 0);
    glNamedFramebufferReadBuffer(shadow_fbo.id, GL_NONE);
    glNamedFramebufferDrawBuffer(shadow_fbo.id, GL_NONE);

    {
        const auto status {glCheckNamedFramebufferStatus(shadow_fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", shadow_fbo.id, "incomplete\nstatus", status);
        }
    }

}

Shadowmapper::Shadowmapper(Shadowmapper&& rhs) noexcept
    :width{std::exchange(rhs.width, 0)},
     height{std::exchange(rhs.height, 0)},
     shadow_fbo{std::move(rhs.shadow_fbo)},
     shadowmap{std::move(rhs.shadowmap)},
     light_projection{std::move(rhs.light_projection)},
     light_view{std::move(rhs.light_view)}
{ peria::log("Shadowmapper move ctor()"); }

Shadowmapper& Shadowmapper::operator=(Shadowmapper&& rhs) noexcept
{
    if (&rhs == this) return *this;

    width = std::exchange(rhs.width, 0);
    height = std::exchange(rhs.height, 0);
    shadow_fbo = std::move(rhs.shadow_fbo);
    shadowmap = std::move(rhs.shadowmap);
    light_projection = std::move(rhs.light_projection);
    light_view = std::move(rhs.light_view);
    
    return *this;
}

void Shadowmapper::set_light_projection(glm::mat4&& projection) noexcept
{
    light_projection = std::move(projection);
}

void Shadowmapper::set_light_view(glm::mat4&& view) noexcept
{
    light_view = std::move(view);
}

}
