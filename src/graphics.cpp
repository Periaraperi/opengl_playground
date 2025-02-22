#include "graphics.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp> 

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <array>

namespace peria {

struct Graphics_Info {
    peria::colors::Color<float> bg {colors::BLACK};
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

void bind_texture_and_sampler(u32 texture_id, u32 sampler_id, i32 unit) noexcept
{
    glBindTextureUnit(unit, texture_id);
    glBindSampler(unit, sampler_id);
}

void bind_default_frame_buffer() noexcept
{ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept
{ glViewport(x, y, w, h); }

void clear_named_buffer(u32 fbo,
                        const peria::colors::Color<float>& color,
                        float depth_value,
                        i32 stencil_value) noexcept
{
    const auto& [r, g, b, a] = color;
    std::array<float, 4> cl {r, g, b, a};
    glClearNamedFramebufferfv(fbo, GL_COLOR, 0, cl.data());
    glClearNamedFramebufferfi(fbo, GL_DEPTH_STENCIL, 0, depth_value, stencil_value);
}

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
