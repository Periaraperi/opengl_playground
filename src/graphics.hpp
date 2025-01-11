#pragma once

#include "peria_types.hpp"
#include "peria_color.hpp"

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

void clear_color() noexcept;
void set_clear_color(const peria::graphics::colors::Color<float>& color) noexcept;
void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
void set_clear_buffer_bits(bool clear_color = true, bool clear_depth = false, bool clear_stencil = false) noexcept;
void clear_buffer() noexcept;
void set_vsync(bool vsync) noexcept;

void set_screen_dimensions(i32 w, i32 h) noexcept;

[[nodiscard]]
glm::vec2 get_screen_dimensions() noexcept;

void bind_texture_and_sampler(const peria::graphics::Texture* const texture, 
                              const peria::graphics::Sampler* const sampler, u32 unit = 0) noexcept;

void bind_default_frame_buffer() noexcept;

[[nodiscard]]
std::array<glm::vec2, 4> 
get_texture_coordinates(float x, float y, float w, float h, float atlas_width, float atlas_height) noexcept;

void draw_colored_quad(const Quad& quad, const colors::Color<float>& color) noexcept;
void draw_textured_quad(const Quad& quad, const Quad& texture_region) noexcept;

void render_quads(const glm::mat4& projection) noexcept;

void start_imgui_frame() noexcept;
void imgui_render() noexcept;

}
