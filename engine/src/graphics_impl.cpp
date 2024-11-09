#include "graphics_impl.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "simple_logger.hpp"

namespace peria::graphics {

namespace buffer_data {
    /*
        1_______2
        |       |
        |   c   |
        |_______|
        0       3
    */
    std::vector<Vertex> default_quad_model {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}, graphics::CYAN}, // left bottom
        {{-0.5f,  0.5f}, {0.0f, 1.0f}, graphics::CYAN}, // left top
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}, graphics::CYAN}, // right top
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}, graphics::CYAN}, // right bottom
    };
    std::vector<u32> default_quad_indices {0,1,2, 0,2,3};
}

Graphics::Graphics(glm::mat4&& projection)
    :screen_ortho_projection{std::move(projection)}
{ 
    peria::log("Graphics init");
    quad_vao = std::make_unique<Vertex_Array>();
    quad_vbo = std::make_unique<Named_Buffer_Object<Vertex>>(buffer_data::default_quad_model);
    quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(buffer_data::default_quad_indices);

    quad_vao->setup_attribute(Attribute<float>{2, false});
    quad_vao->setup_attribute(Attribute<float>{2, false});
    quad_vao->setup_attribute(Attribute<float>{4, false});

    quad_vao->connect_vertex_buffer(quad_vbo->buffer_id(), sizeof(Vertex));
    quad_vao->connect_index_buffer(quad_ibo->buffer_id());

    quad_shader = std::make_unique<Shader>("./assets/quad_vertex.glsl", "./assets/quad_fragment.glsl");
}

void Graphics::set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept
{ glViewport(x, y, w, h); }

void Graphics::clear_color(const graphics::Color<float>& color) noexcept
{ 
    const auto& [r, g, b, a] = color;
    glClearColor(r, g, b, a); 
}

void Graphics::clear_buffer() noexcept
{ glClear(clear_buffer_bit_flags); }

void Graphics::set_clear_buffer_bits(bool clear_color, bool clear_depth, bool clear_stencil) noexcept
{
    u32 flags{};
    if (clear_color)   flags |= GL_COLOR_BUFFER_BIT;
    if (clear_depth)   flags |= GL_DEPTH_BUFFER_BIT;
    if (clear_stencil) flags |= GL_STENCIL_BUFFER_BIT;
    clear_buffer_bit_flags = flags;
}

void Graphics::render_quad(float x, float y, float w, float h, const Color<float>& color) noexcept
{
    quad_shader->use_shader();
    glm::mat4 model {glm::translate(glm::mat4{1.0f}, glm::vec3{x, y, 0.0f})*
                     glm::scale(glm::mat4{1.0f}, glm::vec3{w, h, 1.0f})};
    quad_shader->set_mat4("u_mvp", screen_ortho_projection*model);
    quad_vao->bind();
    glDrawElements(GL_TRIANGLES, buffer_data::default_quad_indices.size(), GL_UNSIGNED_INT, nullptr);
}

Graphics::~Graphics()
{ peria::log("Graphics shutdown"); }
}
