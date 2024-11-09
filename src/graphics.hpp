#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <memory>

#include "peria_types.hpp"
#include "peria_color.hpp"
#include "vertex_array.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"

typedef struct SDL_Window SDL_Window;

namespace peria::graphics {

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texture_coordinates;
    Color<float> color;
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
    
    void render_quad(float x, float y, float w, float h, const Color<float>& color) noexcept;

private:
    u32 clear_buffer_bit_flags {};

    glm::mat4 screen_ortho_projection;
    
    std::unique_ptr<Vertex_Array> quad_vao;
    std::unique_ptr<Named_Buffer_Object<Vertex>> quad_vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> quad_ibo;
    std::unique_ptr<Shader> quad_shader;
};
}
