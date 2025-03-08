#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <array>
#include <utility>
#include <vector>

#include "peria_types.hpp"
#include "peria_color.hpp"

#include "sampler.hpp"
#include "simple_logger.hpp"
#include "texture.hpp"
#include "frame_buffer.hpp"

#include "vertex.hpp"
#include "vertex_array.hpp"
#include "named_buffer_object.hpp"

namespace peria {

// some screen/window stuff 
void set_relative_motion(i32 x, i32 y) noexcept;
void set_relative_mouse(bool rel_mouse) noexcept;
void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
void set_vsync(bool vsync) noexcept;
void set_screen_dimensions(i32 w, i32 h) noexcept;
[[nodiscard]] glm::vec2 get_screen_dimensions() noexcept;
[[nodiscard]] bool is_relative_mouse() noexcept;
[[nodiscard]] glm::vec2 get_relative_motion() noexcept;

// entity bindings
void bind_frame_buffer_default() noexcept;
void bind_frame_buffer(const Frame_Buffer& fbo) noexcept;
void bind_frame_buffer(u32 id) noexcept;
void bind_vertex_array(const Vertex_Array& vao) noexcept;
void bind_vertex_array(u32 id) noexcept;
void bind_texture_and_sampler(u32 texture_id, u32 sampler_id, i32 unit = 0) noexcept;

// sets vertex attrib layout for specific vao and binding index and connects vbo to it.
template <typename... Ts>
void vao_configure(u32 vao, u32 vbo, u32 binding_index) noexcept
{
    auto setup = [&](u32 attribute_index, u32 elem_count, std::size_t offset){
        glEnableVertexArrayAttrib(vao, attribute_index);
        glVertexArrayAttribBinding(vao, attribute_index, binding_index);
        glVertexArrayAttribFormat(vao, attribute_index, elem_count, GL_FLOAT, GL_FALSE, offset);
    };

    [&]<std::size_t... I>(std::index_sequence<I...>) {
        std::size_t offset {0};
        (setup(I, Ts::elem_count, std::exchange(offset, offset+Ts::bytes)), ...);
    }(std::make_index_sequence<sizeof...(Ts)>{});

    glVertexArrayVertexBuffer(vao, binding_index, vbo, 0, Vertex<Ts...>::stride);
}

void vao_connect_ibo(const Vertex_Array& vao, const Buffer_Object& ibo) noexcept;

template <typename T, std::size_t N>
void buffer_upload_data(const Buffer_Object& buffer, const std::array<T, N>& data, u32 usage) noexcept
{ 
    if constexpr (peria::is_vertex_v<T>) {
        glNamedBufferData(buffer.id, T::stride*data.size(), data.data(), usage); 
    }
    else {
        glNamedBufferData(buffer.id, sizeof(T)*data.size(), data.data(), usage); 
    }
}

template <typename T>
void buffer_upload_data(const Buffer_Object& buffer, const std::vector<T>& data, u32 usage) noexcept
{ 
    if constexpr (peria::is_vertex_v<T>) {
        glNamedBufferData(buffer.id, T::stride*data.size(), data.data(), usage); 
    }
    else {
        glNamedBufferData(buffer.id, sizeof(T)*data.size(), data.data(), usage); 
    }
}

void buffer_upload_subdata(const Buffer_Object& buffer, i32 buffer_offset, std::size_t data_size, const void* data) noexcept;

// clears frame buffer's color, depth, and stencil values.
void clear_buffer_all(u32 fbo,
                      const peria::colors::Color<float>& color,
                      float depth_value,
                      i32 stencil_value) noexcept;

// clears frame buffer's color.
void clear_buffer_color(u32 fbo, const peria::colors::Color<float>& color) noexcept;

// clears frame buffer's depth attachment.
void clear_buffer_depth(u32 fbo, float depth_value) noexcept;

// creates and allocates texture storage and loads png or jpg image
[[nodiscard]]
Texture2D create_texture2d_from_image(const char* path) noexcept;

// creates and allocates 1x1 texture storage and loads solid color data
[[nodiscard]]
Texture2D create_texture2d_colored(const colors::Color<float>& color) noexcept;

// creates and allocates wxh texture storage with specific format
[[nodiscard]]
Texture2D create_texture2d(i32 w, i32 h, i32 internal_format) noexcept;

// creates and allocates texture storage for cubemap and loads image data into faces.
[[nodiscard]]
Cubemap create_cubemap_from_images(const Cubemap& cubemap, const std::array<const char*, 6>& file_paths) noexcept;

// creates and allocates texture storage for cubemap for depth information.
[[nodiscard]]
Cubemap create_cubemap_depth(i32 width, i32 height) noexcept;

// creates samper with specified params
[[nodiscard]]
Sampler create_sampler(u32 min_filter, u32 mag_filter, u32 wrap_s, u32 wrap_t, u32 wrap_r, 
                       const colors::Color<float>& border_color = colors::WHITE) noexcept;

[[nodiscard]]
std::array<glm::vec2, 4> 
get_texture_coordinates(float x, float y, float w, float h, float atlas_width, float atlas_height) noexcept;

void start_imgui_frame() noexcept;
void imgui_render() noexcept;

}
