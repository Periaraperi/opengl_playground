#pragma once

#include <glm/mat4x4.hpp>
#include "frame_buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "peria_types.hpp"
#include "graphics.hpp"

namespace peria {

// a shadow mapper for directional light and spot lights
class Shadowmapper {
public:
    Shadowmapper(i32 w, i32 h) noexcept;
    ~Shadowmapper() noexcept = default;

    Shadowmapper(const Shadowmapper&) = delete;
    Shadowmapper& operator=(const Shadowmapper&) = delete;

    Shadowmapper(Shadowmapper&& rhs) noexcept = delete;
    Shadowmapper& operator=(Shadowmapper&& rhs) noexcept = delete;

    void execute(auto&& callback, const Shader& shader) const noexcept
    {
        bind_frame_buffer(shadow_fbo);
        set_viewport(0, 0, width, height);
        clear_buffer_depth(shadow_fbo.id, 1.0f);

        shader.set_mat4("u_vp", light_projection*light_view);
        callback(shader);
    }

    void set_light_projection(glm::mat4&& projection) noexcept;
    void set_light_view(glm::mat4&& view) noexcept;

    [[nodiscard]]
    glm::mat4 get_light_vp() noexcept
    { return light_projection*light_view; }

    [[nodiscard]]
    u32 get_shadowmap_id() noexcept
    { return shadowmap.id; }

private:
    i32          width  {};
    i32          height {};
    Frame_Buffer shadow_fbo;
    Texture2D    shadowmap;
    glm::mat4    light_projection;
    glm::mat4    light_view;
};

}
