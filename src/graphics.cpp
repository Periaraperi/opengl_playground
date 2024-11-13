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

    // I assume that cube has center on (0,0)
    // and we are looking at negative Z-axis at Side 1
    std::vector<Vertex3d> cube_model {
        // SIDE 1 (near)
        {{-0.5f, -0.5f, 0.5f}, OLIVEDRAB},
        {{-0.5f,  0.5f, 0.5f}, OLIVEDRAB},
        {{ 0.5f,  0.5f, 0.5f}, OLIVEDRAB},

        {{-0.5f, -0.5f, 0.5f}, OLIVEDRAB},
        {{ 0.5f,  0.5f, 0.5f}, OLIVEDRAB},
        {{ 0.5f, -0.5f, 0.5f}, OLIVEDRAB},
        
        // SIDE 2 (far)
        {{-0.5f, -0.5f, -0.5f}, OLIVEDRAB},
        {{-0.5f,  0.5f, -0.5f}, OLIVEDRAB},
        {{ 0.5f,  0.5f, -0.5f}, OLIVEDRAB},

        {{-0.5f, -0.5f, -0.5f}, OLIVEDRAB},
        {{ 0.5f,  0.5f, -0.5f}, OLIVEDRAB},
        {{ 0.5f, -0.5f, -0.5f}, OLIVEDRAB},

        // SIDE 3 (LEFT)
        {{-0.5f, -0.5f, -0.5f}, RED},
        {{-0.5f,  0.5f, -0.5f}, RED},
        {{-0.5f,  0.5f,  0.5f}, RED},

        {{-0.5f, -0.5f, -0.5f}, RED},
        {{-0.5f,  0.5f,  0.5f}, RED},
        {{-0.5f, -0.5f,  0.5f}, RED},

        // SIDE 4 (RIGHT)
        {{ 0.5f, -0.5f, -0.5f}, RED},
        {{ 0.5f,  0.5f, -0.5f}, RED},
        {{ 0.5f,  0.5f,  0.5f}, RED},

        {{ 0.5f, -0.5f, -0.5f}, RED},
        {{ 0.5f,  0.5f,  0.5f}, RED},
        {{ 0.5f, -0.5f,  0.5f}, RED},
        
        // SIDE 5 (BOTTOM)
        {{-0.5f, -0.5f,  0.5f}, CORNFLOWERBLUE},
        {{-0.5f, -0.5f, -0.5f}, CORNFLOWERBLUE},
        {{ 0.5f, -0.5f, -0.5f}, CORNFLOWERBLUE},

        {{-0.5f, -0.5f,  0.5f}, CORNFLOWERBLUE},
        {{ 0.5f, -0.5f, -0.5f}, CORNFLOWERBLUE},
        {{ 0.5f, -0.5f,  0.5f}, CORNFLOWERBLUE},

        // SIDE 6 (TOP)
        {{-0.5f,  0.5f,  0.5f}, CORNFLOWERBLUE},
        {{-0.5f,  0.5f, -0.5f}, CORNFLOWERBLUE},
        {{ 0.5f,  0.5f, -0.5f}, CORNFLOWERBLUE},

        {{-0.5f,  0.5f,  0.5f}, CORNFLOWERBLUE},
        {{ 0.5f,  0.5f, -0.5f}, CORNFLOWERBLUE},
        {{ 0.5f,  0.5f,  0.5f}, CORNFLOWERBLUE}
    };
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

[[nodiscard]]
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
    glEnable(GL_DEPTH_TEST);

    { // setup batch buffers for quads
        batch_quad_vao = std::make_unique<Vertex_Array>();
        batch_quad_vbo = std::make_unique<Named_Buffer_Object<Vertex>>(batch_quad_vbo_size);

        auto quad_count {batch_quad_vbo_size / 4 / sizeof(Vertex)};
        std::vector<u32> indices; indices.reserve(quad_count * 6);
        for (u32 i{}; i<quad_count; ++i) {
            indices.emplace_back(4*i);
            indices.emplace_back(4*i + 1);
            indices.emplace_back(4*i + 2);

            indices.emplace_back(4*i);
            indices.emplace_back(4*i + 2);
            indices.emplace_back(4*i + 3);
        }
        batch_quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

        // pos
        batch_quad_vao->setup_attribute(Attribute<float>{2, false});
        // texture coordinates
        batch_quad_vao->setup_attribute(Attribute<float>{2, false});
        // color
        batch_quad_vao->setup_attribute(Attribute<float>{4, false});
        // texture unit
        batch_quad_vao->setup_attribute(Attribute<float>{1, false});
        batch_quad_vao->connect_vertex_buffer(batch_quad_vbo->buffer_id(), sizeof(Vertex)); batch_quad_vao->connect_index_buffer(batch_quad_ibo->buffer_id());

        draw_quad_command_data.reserve(quad_count);
    }

    quad_shader = std::make_unique<Shader>("./assets/quad_vertex.glsl", "./assets/quad_fragment.glsl");
    {
        std::array<i32, 3> arr{0, 1, 2};
        quad_shader->set_array("u_textures", arr.size(), arr.data());
    }

    { // setup cube vao/vbo
        cube_vao = std::make_unique<Vertex_Array>();
        cube_vbo = std::make_unique<Named_Buffer_Object<Vertex3d>>(buffer_data::cube_model);

        // pos vec3
        cube_vao->setup_attribute(Attribute<float>{3, false});
        // color 
        cube_vao->setup_attribute(Attribute<float>{4, false});
        cube_vao->connect_vertex_buffer(cube_vbo->buffer_id(), sizeof(Vertex3d));
    }

    cube_shader = std::make_unique<Shader>("./assets/cube_vertex.glsl", "./assets/cube_fragment.glsl");

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

void Graphics::set_batch_quad_count(i32 quad_count) noexcept
{
    batch_quad_vbo_size = quad_count * 4 * sizeof(Vertex);

    // reacreate vbo and ibo
    batch_quad_vbo = std::make_unique<Named_Buffer_Object<Vertex>>(batch_quad_vbo_size);

    std::vector<u32> indices; indices.reserve(quad_count * 6);
    for (u32 i{}; i<static_cast<u32>(quad_count); ++i) {
        indices.emplace_back(4*i);
        indices.emplace_back(4*i + 1);
        indices.emplace_back(4*i + 2);

        indices.emplace_back(4*i);
        indices.emplace_back(4*i + 2);
        indices.emplace_back(4*i + 3);
    }
    batch_quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

    batch_quad_vao->connect_vertex_buffer(batch_quad_vbo->buffer_id(), sizeof(Vertex));
    batch_quad_vao->connect_index_buffer(batch_quad_ibo->buffer_id());

    draw_quad_command_data.clear();
}

void Graphics::draw_colored_quad(const Quad& quad, const Color<float>& color) noexcept
{
    const auto& [x, y, w, h] {quad};
    draw_quad_command_data.emplace_back(Vertex{{x,   y  }, {0.0f, 0.0f}, color, 0});
    draw_quad_command_data.emplace_back(Vertex{{x,   y+h}, {0.0f, 1.0f}, color, 0});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y+h}, {1.0f, 1.0f}, color, 0});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y  }, {1.0f, 0.0f}, color, 0});
}

void Graphics::draw_textured_quad(const Quad& quad, const Quad& texture_region) noexcept
{
    const auto& [x, y, w, h] {quad};
    const auto& [ax, ay, aw, ah] {texture_region};

    const auto texture_dimensions {texture_atlas->dimensions()};
    const auto coords {get_texture_coordinates(ax, ay, aw, ah, texture_dimensions.x, texture_dimensions.y)};

    draw_quad_command_data.emplace_back(Vertex{{x,   y  }, coords[0], WHITE, 1});
    draw_quad_command_data.emplace_back(Vertex{{x,   y+h}, coords[1], WHITE, 1});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y+h}, coords[2], WHITE, 1});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y  }, coords[3], WHITE, 1});
}

void Graphics::render() noexcept
{
    if (draw_quad_command_data.empty()) return;

    const auto max_quad_count {batch_quad_vbo_size / 4 / sizeof(Vertex)};
    auto current_quad_count {draw_quad_command_data.size() / 4};
    std::size_t offset {};

    batch_quad_vao->bind();
    quad_shader->use_shader();
    quad_shader->set_mat4("u_mvp", screen_ortho_projection);

    // this was for testing my own projection mat
    //quad_shader->set_mat4("u_mvp", peria_ortho_projection);
    
    while (current_quad_count > 0) {
        i32 c {}; // count of rects for each batch
        if (current_quad_count >= max_quad_count) {
            c = max_quad_count;
        }
        else { // smaller remaining batch
            c = current_quad_count;
        }

        batch_quad_vbo->set_sub_data(0, c*4*sizeof(Vertex), draw_quad_command_data.data() + offset);
        glDrawElements(GL_TRIANGLES, c*6, GL_UNSIGNED_INT, nullptr);
        offset += 4*c;
        current_quad_count -= c;
    }

    draw_quad_command_data.clear();
}

static glm::mat4 view {glm::translate(glm::mat4{1.0f}, glm::vec3{-0.45f, -0.3f, -3.0f})*
                       glm::rotate(glm::mat4{1.0f}, glm::radians(50.0f), glm::vec3{1.0f, 1.0f, 0.0f})};

void Graphics::render_cube() noexcept
{
    cube_vao->bind();
    cube_shader->use_shader();
    cube_shader->set_mat4("u_mvp", perspective_projection*view);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Graphics::peria_ortho(float left, float right, float bottom, float top) noexcept
{ peria_ortho_projection = Ortho_Projection_Matrix{left, right, bottom, top}; }

void Graphics::set_perspective_projection(glm::mat4&& projection) noexcept
{ perspective_projection = std::move(projection); }

Graphics::~Graphics()
{ peria::log("Graphics shutdown"); }
}
