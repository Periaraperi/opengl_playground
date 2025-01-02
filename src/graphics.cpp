#include "graphics.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp> 

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <array>

#include "sampler.hpp"
#include "texture.hpp"

namespace peria::graphics {

struct Graphics_Info {
    graphics::colors::Color<float> bg {graphics::colors::BLACK};
    u32 clear_buffer_bit_flags {};
    i32 screen_width  {800};
    i32 screen_height {600};
} graphics_info;

std::array<glm::vec2, 4> 
get_texture_coordinates(float x, float y, float w, float h, float atlas_width, float atlas_height) noexcept
{
    return {{
        {x/atlas_width,     y/atlas_height    },
        {x/atlas_width,     (y+h)/atlas_height},
        {(x+w)/atlas_width, (y+h)/atlas_height},
        {(x+w)/atlas_width, y/atlas_height    }
    }};
}

void bind_texture_and_sampler(const peria::graphics::Texture* const texture, 
                              const peria::graphics::Sampler* const sampler, u32 unit) noexcept
{
    texture->bind(unit);
    sampler->bind(unit);
}


void clear_color() noexcept
{
    const auto& [r, g, b, a] = graphics_info.bg;
    glClearColor(r, g, b, a); 
}

void set_clear_color(const peria::graphics::colors::Color<float>& color) noexcept
{ graphics_info.bg = color; }

void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept
{ glViewport(x, y, w, h); }

void set_clear_buffer_bits(bool clear_color, bool clear_depth, bool clear_stencil) noexcept
{
    u32 flags {};
    if (clear_color)   flags |= GL_COLOR_BUFFER_BIT;
    if (clear_depth)   flags |= GL_DEPTH_BUFFER_BIT;
    if (clear_stencil) flags |= GL_STENCIL_BUFFER_BIT;
    graphics_info.clear_buffer_bit_flags = flags;
}

void clear_buffer() noexcept
{ glClear(graphics_info.clear_buffer_bit_flags); }

void set_vsync(bool vsync) noexcept
{ (vsync) ? SDL_GL_SetSwapInterval(1) : SDL_GL_SetSwapInterval(0); }

void set_screen_dimensions(i32 w, i32 h) noexcept
{
    graphics_info.screen_width = w;
    graphics_info.screen_height = h;
}

glm::vec2 get_screen_dimensions() noexcept
{ return {graphics_info.screen_width, graphics_info.screen_height}; }

void start_imgui_frame() noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void imgui_render() noexcept
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace peria::graphics
