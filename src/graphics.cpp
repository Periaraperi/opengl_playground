#include "graphics.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <array>

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

    std::vector<Vertex_Combined> default_quad_model_combined {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}, graphics::WHITE, 0},
        {{-0.5f,  0.5f}, {0.0f, 2.0f}, graphics::WHITE, 0},
        {{ 0.5f,  0.5f}, {2.0f, 2.0f}, graphics::WHITE, 0},
        {{ 0.5f, -0.5f}, {2.0f, 0.0f}, graphics::WHITE, 0}
    };

    std::vector<u32> default_quad_indices {0,1,2, 0,2,3};
}

namespace {

void bind_texture_and_sampler(const std::unique_ptr<Texture>& texture, 
                              const std::unique_ptr<Sampler>& sampler, u32 unit = 0) noexcept
{
    texture->bind(unit);
    sampler->bind(unit);
}

void bind_texture_and_sampler(const Texture* const texture, 
                              const Sampler* const sampler, u32 unit = 0) noexcept
{
    texture->bind(unit);
    sampler->bind(unit);
}

std::array<glm::vec2, 4> get_texture_coordinates(float x, float y, float w, float h, float atlas_width, float atlas_height) noexcept
{
    return {{
        {x/atlas_width,     y/atlas_height    },
        {x/atlas_width,     (y+h)/atlas_height},
        {(x+w)/atlas_width, (y+h)/atlas_height},
        {(x+w)/atlas_width, y/atlas_height    }
    }};
}

}

Graphics::Graphics(glm::mat4&& projection)
    :screen_ortho_projection{std::move(projection)}
{ 
    peria::log("Graphics init");
    SDL_GL_SetSwapInterval(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    quad_vao_combined = std::make_unique<Vertex_Array>();
    quad_vbo_combined = std::make_unique<Named_Buffer_Object<Vertex_Combined>>(buffer_data::default_quad_model_combined);
    quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(buffer_data::default_quad_indices);

    quad_vao_combined->setup_attribute(Attribute<float>{2, false});
    quad_vao_combined->setup_attribute(Attribute<float>{2, false});
    quad_vao_combined->setup_attribute(Attribute<float>{4, false});
    quad_vao_combined->setup_attribute(Attribute<float>{1, false});

    quad_vao_combined->connect_vertex_buffer(quad_vbo_combined->buffer_id(), sizeof(Vertex_Combined));
    quad_vao_combined->connect_index_buffer(quad_ibo->buffer_id());

    quad_shader = std::make_unique<Shader>("./assets/quad_vertex.glsl", "./assets/quad_fragment.glsl");
    {
        std::array<i32, 3> arr{0, 1, 2};
        quad_shader->set_array("u_textures", arr.size(), arr.data());
    }

    white_texture = std::make_unique<Texture>(1, 1, Color<float>::to_u8_color(WHITE));
    texture_atlas = std::make_unique<Texture>("./assets/mushrooms_sheet.png");

    sampler1 = std::make_unique<Sampler>(0);
    sampler2 = std::make_unique<Sampler>(1);

    bind_texture_and_sampler(white_texture, sampler2, 0);
    bind_texture_and_sampler(texture_atlas, sampler1, 1);
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

void Graphics::set_vsync(bool vsync) noexcept
{ (vsync) ? SDL_GL_SetSwapInterval(1) : SDL_GL_SetSwapInterval(0); }

void Graphics::set_clear_buffer_bits(bool clear_color, bool clear_depth, bool clear_stencil) noexcept
{
    u32 flags{};
    if (clear_color)   flags |= GL_COLOR_BUFFER_BIT;
    if (clear_depth)   flags |= GL_DEPTH_BUFFER_BIT;
    if (clear_stencil) flags |= GL_STENCIL_BUFFER_BIT;
    clear_buffer_bit_flags = flags;
}

void Graphics::draw_colored_quad(float x, float y, float w, float h, const Color<float>& color) noexcept
{
    auto tmp_vao {std::make_unique<Vertex_Array>()};
    std::vector<Vertex_Combined> v {
        {{x,   y  }, {0.0f, 0.0f}, color, 0},
        {{x,   y+h}, {0.0f, 1.0f}, color, 0},
        {{x+w, y+h}, {1.0f, 1.0f}, color, 0},
        {{x+w, y  }, {1.0f, 0.0f}, color, 0}
    };
    auto tmp_vbo {std::make_unique<Named_Buffer_Object<Vertex_Combined>>(v)};

    tmp_vao->setup_attribute(Attribute<float>{2, false});
    tmp_vao->setup_attribute(Attribute<float>{2, false});
    tmp_vao->setup_attribute(Attribute<float>{4, false});
    tmp_vao->setup_attribute(Attribute<float>{1, false});

    tmp_vao->connect_vertex_buffer(tmp_vbo->buffer_id(), sizeof(Vertex_Combined));
    tmp_vao->connect_index_buffer(quad_ibo->buffer_id());

    tmp_vao->bind();
    quad_shader->use_shader();
    bind_texture_and_sampler(white_texture, sampler1, 0);
    quad_shader->set_mat4("u_mvp", screen_ortho_projection);
    glDrawElements(GL_TRIANGLES, buffer_data::default_quad_indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Graphics::draw_textured_quad(float x, float y, float w, float h, float ax, float ay, float aw, float ah) noexcept
{
    auto tmp_vao {std::make_unique<Vertex_Array>()};
    auto texture_dimensions {texture_atlas->dimensions()};
    auto coords {get_texture_coordinates(ax, ay, aw, ah, texture_dimensions.x, texture_dimensions.y)};
    std::vector<Vertex_Combined> v {
        {{x,   y  }, coords[0], WHITE, 1},
        {{x,   y+h}, coords[1], WHITE, 1},
        {{x+w, y+h}, coords[2], WHITE, 1},
        {{x+w, y  }, coords[3], WHITE, 1}
    };
    auto tmp_vbo {std::make_unique<Named_Buffer_Object<Vertex_Combined>>(v)};

    tmp_vao->setup_attribute(Attribute<float>{2, false});
    tmp_vao->setup_attribute(Attribute<float>{2, false});
    tmp_vao->setup_attribute(Attribute<float>{4, false});
    tmp_vao->setup_attribute(Attribute<float>{1, false});

    tmp_vao->connect_vertex_buffer(tmp_vbo->buffer_id(), sizeof(Vertex_Combined));
    tmp_vao->connect_index_buffer(quad_ibo->buffer_id());

    tmp_vao->bind();
    quad_shader->use_shader();
    quad_shader->set_mat4("u_mvp", screen_ortho_projection);
    glDrawElements(GL_TRIANGLES, buffer_data::default_quad_indices.size(), GL_UNSIGNED_INT, nullptr);
}

Graphics::~Graphics()
{ peria::log("Graphics shutdown"); }
}
