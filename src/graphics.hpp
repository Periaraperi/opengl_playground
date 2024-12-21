#pragma once

#include "peria_types.hpp"
#include "peria_color.hpp"

namespace peria::graphics {

struct Graphics_Info {
    graphics::colors::Color<float> bg {graphics::colors::BLACK};
    u32 clear_buffer_bit_flags {};
};
void clear_color() noexcept;
void set_clear_color(const peria::graphics::colors::Color<float>& color) noexcept;
void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
void set_clear_buffer_bits(bool clear_color = true, bool clear_depth = false, bool clear_stencil = false) noexcept;
void clear_buffer() noexcept;
void set_vsync(bool vsync) noexcept;

void start_imgui_frame() noexcept;
void imgui_render() noexcept;

}
