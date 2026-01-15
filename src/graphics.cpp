#include "graphics.hpp"
#include "simple_logger.hpp"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stb_image.h>
#include <glm/glm.hpp> 

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include <array>

namespace peria {

struct Graphics_Info {
    peria::colors::Color<float> bg {colors::BLACK};
    i32 screen_width    {800};
    i32 screen_height   {600};
    bool relative_mouse {true};
    i32 mouse_x_rel     {};
    i32 mouse_y_rel     {};
} graphics_info;

void set_relative_motion(i32 x, i32 y) noexcept
{
    graphics_info.mouse_x_rel = x;
    graphics_info.mouse_y_rel = y;
}

glm::vec2 get_relative_motion() noexcept
{ return {graphics_info.mouse_x_rel, graphics_info.mouse_y_rel}; }

void set_relative_mouse(SDL_Window* window, bool rel_mouse) noexcept
{
    graphics_info.relative_mouse = rel_mouse;
    SDL_SetWindowRelativeMouseMode(window, graphics_info.relative_mouse);
}

bool is_relative_mouse() noexcept
{ return graphics_info.relative_mouse; }

void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept
{ glViewport(x, y, w, h); }

void set_vsync(bool vsync) noexcept
{ (vsync) ? SDL_GL_SetSwapInterval(1) : SDL_GL_SetSwapInterval(0); }

void set_screen_dimensions(i32 w, i32 h) noexcept
{
    graphics_info.screen_width = w;
    graphics_info.screen_height = h;
}

glm::vec2 get_screen_dimensions() noexcept
{ return {graphics_info.screen_width, graphics_info.screen_height}; }

void bind_frame_buffer_default() noexcept
{ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void bind_frame_buffer(const Frame_Buffer& fbo) noexcept
{ glBindFramebuffer(GL_FRAMEBUFFER, fbo.id); }

void bind_frame_buffer(u32 id) noexcept
{ glBindFramebuffer(GL_FRAMEBUFFER, id); }

void bind_vertex_array(const Vertex_Array& vao) noexcept
{ glBindVertexArray(vao.id); }

void bind_vertex_array(u32 id) noexcept
{ glBindVertexArray(id); }

void bind_texture_and_sampler(u32 texture_id, u32 sampler_id, i32 unit) noexcept
{
    glBindTextureUnit(unit, texture_id);
    glBindSampler(unit, sampler_id);
}

void vao_connect_ibo(const Vertex_Array& vao, const Buffer_Object& ibo) noexcept
{ glVertexArrayElementBuffer(vao.id, ibo.id); }

void buffer_allocate_data(const Buffer_Object& buffer, std::size_t bytes, u32 usage) noexcept
{ glNamedBufferData(buffer.id, bytes, nullptr, usage); }

void buffer_upload_subdata(const Buffer_Object& buffer, i32 buffer_offset, std::size_t data_size, const void* data) noexcept
{ glNamedBufferSubData(buffer.id, buffer_offset, data_size, data); }

void clear_buffer_all(u32 fbo,
                      const peria::colors::Color<float>& color,
                      float depth_value,
                      i32 stencil_value) noexcept
{
    const auto& [r, g, b, a] = color;
    std::array<float, 4> cl {r, g, b, a};
    glClearNamedFramebufferfv(fbo, GL_COLOR, 0, cl.data());
    glClearNamedFramebufferfi(fbo, GL_DEPTH_STENCIL, 0, depth_value, stencil_value);
}

void clear_buffer_color(u32 fbo, const peria::colors::Color<float>& color) noexcept
{ 
    const auto& [r, g, b, a] = color;
    std::array<float, 4> cl {r, g, b, a};
    glClearNamedFramebufferfv(fbo, GL_COLOR, 0, cl.data()); 
}

void clear_buffer_depth(u32 fbo, float depth_value) noexcept
{ glClearNamedFramebufferfv(fbo, GL_DEPTH, 0, &depth_value); }

Texture2D create_texture2d_from_image(const char* path, bool flip /* = true*/) noexcept
{
    stbi_set_flip_vertically_on_load(flip);

    i32 width, height, channel_count;
    u8* data {stbi_load(path, &width, &height, &channel_count, 0)};

    if (data == nullptr) {
        peria::log("failed to load res: ", path);
    }


    i32 internal_format {channel_count == 4 ? GL_RGBA8 : GL_RGB8};
    i32 format          {channel_count == 4 ? GL_RGBA : GL_RGB};

    Texture2D texture;
    glTextureStorage2D(texture.id, 1, internal_format, width, height);
    glTextureSubImage2D(texture.id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(texture.id);

    stbi_image_free(data); 
    data = nullptr;

    return texture;
}

Texture2D create_texture2d_from_image(const char* path, i32& width, i32& height, bool flip /* = true*/) noexcept
{
    stbi_set_flip_vertically_on_load(flip);

    i32 channel_count;
    u8* data {stbi_load(path, &width, &height, &channel_count, 0)};

    if (data == nullptr) {
        width = 0;
        height = 0;
        peria::log("failed to load res: ", path);
    }

    i32 internal_format {channel_count == 4 ? GL_RGBA8 : GL_RGB8};
    i32 format          {channel_count == 4 ? GL_RGBA : GL_RGB};

    Texture2D texture;
    glTextureStorage2D(texture.id, 1, internal_format, width, height);
    glTextureSubImage2D(texture.id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(texture.id);

    stbi_image_free(data); 
    data = nullptr;

    return texture;
}

Texture2D create_texture2d_from_image_srgb(const char* path, bool flip /* = true*/) noexcept
{
    stbi_set_flip_vertically_on_load(flip);

    i32 width, height, channel_count;
    u8* data {stbi_load(path, &width, &height, &channel_count, 0)};

    if (data == nullptr) {
        peria::log("failed to load res: ", path);
    }

    i32 internal_format {channel_count == 4 ? GL_SRGB8_ALPHA8 : GL_SRGB8};
    i32 format          {channel_count == 4 ? GL_RGBA : GL_RGB};

    Texture2D texture;
    glTextureStorage2D(texture.id, 1, internal_format, width, height);
    glTextureSubImage2D(texture.id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(texture.id);

    stbi_image_free(data); 
    data = nullptr;

    return texture;
}

Texture2D create_texture2d_colored(const colors::Color<float>& color) noexcept
{
    const auto c {colors::Color<float>::to_u8_color(color)};
    std::array<u8, 3> data {c.r, c.g, c.b};

    i32 width{1}, height{1};
    Texture2D texture;
    glTextureStorage2D(texture.id, 1, GL_RGB8, width, height);
    glTextureSubImage2D(texture.id, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glGenerateTextureMipmap(texture.id);
    return texture;
}

Texture2D create_texture2d(i32 w, i32 h, i32 internal_format) noexcept
{
    Texture2D texture;
    glTextureStorage2D(texture.id, 1, internal_format, w, h);
    return texture;
}

Texture2D create_texture2d_multisample(i32 w, i32 h, i32 internal_format, i32 samples) noexcept
{
    Texture2D texture{true};
    glTextureStorage2DMultisample(texture.id, samples, internal_format, w, h, GL_TRUE);
    return texture;
}

Cubemap create_cubemap_from_images(const Cubemap& cubemap, const std::array<const char*, 6>& file_paths) noexcept
{ return {}; }

Cubemap create_cubemap_depth(i32 width, i32 height) noexcept
{ return {}; }

Sampler create_sampler(u32 min_filter, u32 mag_filter, u32 wrap_s, u32 wrap_t, u32 wrap_r, const colors::Color<float>& border_color) noexcept
{
    Sampler sampler;
    glSamplerParameteri(sampler.id, GL_TEXTURE_MIN_FILTER, min_filter);
    glSamplerParameteri(sampler.id, GL_TEXTURE_MAG_FILTER, mag_filter);
    glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_S, wrap_s);
    glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_T, wrap_t);
    glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_R, wrap_r);
    if (wrap_s == GL_CLAMP_TO_BORDER || wrap_t == GL_CLAMP_TO_BORDER || wrap_r == GL_CLAMP_TO_BORDER) {
        const auto& [r, g, b, _] {border_color};
        const std::array clr {r, g, b};
        glSamplerParameterfv(sampler.id, GL_TEXTURE_BORDER_COLOR, clr.data());
    }
    return sampler;
}

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

void start_imgui_frame() noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void imgui_render() noexcept
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace peria::graphics
