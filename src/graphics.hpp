#pragma once

#include "peria_types.hpp"
#include "peria_color.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <array>

namespace peria {

void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
void set_vsync(bool vsync) noexcept;

// clears frame buffers color attachment, depth, and stencil values.
void clear_named_buffer(u32 fbo,
                        const peria::colors::Color<float>& color,
                        float depth_value,
                        i32 stencil_value) noexcept;

void set_screen_dimensions(i32 w, i32 h) noexcept;

[[nodiscard]]
glm::vec2 get_screen_dimensions() noexcept;

void bind_default_frame_buffer() noexcept;

void bind_texture_and_sampler(u32 texture_id, u32 sampler_id, i32 unit = 0) noexcept;

[[nodiscard]]
std::array<glm::vec2, 4> 
get_texture_coordinates(float x, float y, float w, float h, float atlas_width, float atlas_height) noexcept;

void start_imgui_frame() noexcept;
void imgui_render() noexcept;

}
