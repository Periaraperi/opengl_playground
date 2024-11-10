#include "graphics.hpp"

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
        {{-0.5f, -0.5f}, {0.0f, 0.0f}, graphics::WHITE},
        {{-0.5f,  0.5f}, {0.0f, 2.0f}, graphics::WHITE},
        {{ 0.5f,  0.5f}, {2.0f, 2.0f}, graphics::WHITE},
        {{ 0.5f, -0.5f}, {2.0f, 0.0f}, graphics::WHITE}
    };
    std::vector<u32> default_quad_indices {0,1,2, 0,2,3};
}

Graphics::Graphics(glm::mat4&& projection)
    :screen_ortho_projection{std::move(projection)}
{ 
    peria::log("Graphics init");
    SDL_GL_SetSwapInterval(0);

    quad_vao = std::make_unique<Vertex_Array>();
    quad_vbo = std::make_unique<Named_Buffer_Object<Vertex>>(buffer_data::default_quad_model);
    quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(buffer_data::default_quad_indices);

    quad_vao->setup_attribute(Attribute<float>{2, false});
    quad_vao->setup_attribute(Attribute<float>{2, false});
    quad_vao->setup_attribute(Attribute<float>{4, false});

    quad_vao->connect_vertex_buffer(quad_vbo->buffer_id(), sizeof(Vertex));
    quad_vao->connect_index_buffer(quad_ibo->buffer_id());

    quad_shader = std::make_unique<Shader>("./assets/quad_vertex.glsl", "./assets/quad_fragment.glsl");
    quad_shader->set_int("u_texture", 0);

    //texture = std::make_unique<Texture>(1, 1, Color<float>::to_u8_color(WHITE));
    texture2 = std::make_unique<Texture>("./assets/shaco1.jpg");

    sampler1 = std::make_unique<Sampler>();
    sampler2 = std::make_unique<Sampler>(1);
}

void bind_texture_and_sampler(const std::unique_ptr<Texture>& texture, 
                              const std::unique_ptr<Sampler>& sampler, u32 unit = 0) noexcept
{
    texture->bind(unit);
    sampler->bind(unit);
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

void Graphics::render_textured_quad() noexcept
{
    quad_shader->use_shader();
    quad_vao->bind();
    glm::mat model1 {glm::translate(glm::mat4{1.0f}, glm::vec3{-0.5f, 0.0f, 0.0f})*
                     glm::scale(glm::mat4{1.0f}, glm::vec3{0.75f, 0.75f, 1.0f})};
    glm::mat model2 {glm::translate(glm::mat4{1.0f}, glm::vec3{0.5f, 0.0f, 0.0f})*
                     glm::scale(glm::mat4{1.0f}, glm::vec3{0.75f, 0.75f, 1.0f})};
    quad_shader->set_mat4("u_mvp", model1);
    texture->bind();

    glDrawElements(GL_TRIANGLES, buffer_data::default_quad_indices.size(), GL_UNSIGNED_INT, nullptr);

    quad_shader->set_mat4("u_mvp", model2);
    texture2->bind();
    glDrawElements(GL_TRIANGLES, buffer_data::default_quad_indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Graphics::render2() noexcept
{
    quad_shader->use_shader();
    quad_vao->bind();
    glm::mat model1 {glm::translate(glm::mat4{1.0f}, glm::vec3{-0.5f, 0.0f, 0.0f})*
                     glm::scale(glm::mat4{1.0f}, glm::vec3{0.75f, 0.75f, 1.0f})};
    glm::mat model2 {glm::translate(glm::mat4{1.0f}, glm::vec3{0.5f, 0.0f, 0.0f})*
                     glm::scale(glm::mat4{1.0f}, glm::vec3{0.75f, 0.75f, 1.0f})};
    quad_shader->set_mat4("u_mvp", model1);
    bind_texture_and_sampler(texture2, sampler1);
    glDrawElements(GL_TRIANGLES, buffer_data::default_quad_indices.size(), GL_UNSIGNED_INT, nullptr);

    quad_shader->set_mat4("u_mvp", model2);
    bind_texture_and_sampler(texture2, sampler2);
    glDrawElements(GL_TRIANGLES, buffer_data::default_quad_indices.size(), GL_UNSIGNED_INT, nullptr);
}

Graphics::~Graphics()
{ peria::log("Graphics shutdown"); }
}
