#pragma once

#include "peria_types.hpp"
#include "peria_color.hpp"
#include "texture_cubemap.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <array>

namespace peria::graphics {
class Sampler;
class Texture;

struct Quad {
    float x, y;
    float w, h;
};

void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
void set_vsync(bool vsync) noexcept;

// clears frame buffers color attachment, depth, and stencil values.
void clear_named_buffer(u32 fbo,
                        const peria::graphics::colors::Color<float>& color,
                        float depth_value,
                        i32 stencil_value) noexcept;

void set_screen_dimensions(i32 w, i32 h) noexcept;

[[nodiscard]]
glm::vec2 get_screen_dimensions() noexcept;

void bind_texture_and_sampler(const peria::graphics::Texture* const texture, 
                              const peria::graphics::Sampler* const sampler, u32 unit = 0) noexcept;

void bind_texture_and_sampler(const peria::graphics::Texture_Cubemap* const texture, 
                              const peria::graphics::Sampler* const sampler, u32 unit = 0) noexcept;

void bind_default_frame_buffer() noexcept;

[[nodiscard]]
std::array<glm::vec2, 4> 
get_texture_coordinates(float x, float y, float w, float h, float atlas_width, float atlas_height) noexcept;

void start_imgui_frame() noexcept;
void imgui_render() noexcept;

}
