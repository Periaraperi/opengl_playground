#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <memory>

#include "peria_types.hpp"
#include "peria_color.hpp"
#include "sampler.hpp"
#include "vertex_array.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"

typedef struct SDL_Window SDL_Window;

namespace peria::graphics {

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texture_coordinates;
    Color<float> color;
};

struct Vertex_Combined {
    glm::vec2 pos;
    glm::vec2 texture_coordinates;
    Color<float> color;
    float texture_unit;
};

class Graphics {
public:
    Graphics(glm::mat4&& projection);

    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    Graphics(Graphics&&) noexcept = default;
    Graphics& operator=(Graphics&&) noexcept = default;

    ~Graphics();

    void clear_color(const Color<float>& color) noexcept;
    void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
    void set_clear_buffer_bits(bool clear_color = true, bool clear_depth = false, bool clear_stencil = false) noexcept;
    void clear_buffer() noexcept;
    void set_vsync(bool vsync) noexcept;
    
    void draw_colored_quad(float x, float y, float w, float h, const Color<float>& color) noexcept;
    void draw_textured_quad(float x, float y, float w, float h, float ax, float ay, float aw, float ah) noexcept;

private:
    u32 clear_buffer_bit_flags {};
    glm::mat4 screen_ortho_projection;
    
    // buffers and vaos
    std::unique_ptr<Vertex_Array> quad_vao_combined;
    std::unique_ptr<Named_Buffer_Object<Vertex_Combined>> quad_vbo_combined;
    std::unique_ptr<Named_Buffer_Object<u32>> quad_ibo;

    // shaders
    std::unique_ptr<Shader> quad_shader;

    // textures
    std::unique_ptr<Texture> white_texture;
    std::unique_ptr<Texture> texture_atlas;

    // samplers
    std::unique_ptr<Sampler> sampler1;
    std::unique_ptr<Sampler> sampler2;
};
}
