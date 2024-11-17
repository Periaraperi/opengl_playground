#include "graphics.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

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
        {{-0.5f, -0.5f, 0.5f}, SILVER},
        {{-0.5f,  0.5f, 0.5f}, SILVER},
        {{ 0.5f,  0.5f, 0.5f}, SILVER},

        {{-0.5f, -0.5f, 0.5f}, SILVER},
        {{ 0.5f,  0.5f, 0.5f}, SILVER},
        {{ 0.5f, -0.5f, 0.5f}, SILVER},
        
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
        {{ 0.5f, -0.5f, -0.5f}, YELLOW},
        {{ 0.5f,  0.5f, -0.5f}, YELLOW},
        {{ 0.5f,  0.5f,  0.5f}, YELLOW},

        {{ 0.5f, -0.5f, -0.5f}, YELLOW},
        {{ 0.5f,  0.5f,  0.5f}, YELLOW},
        {{ 0.5f, -0.5f,  0.5f}, YELLOW},
        
        // SIDE 5 (BOTTOM)
        {{-0.5f, -0.5f,  0.5f}, CORNFLOWERBLUE},
        {{-0.5f, -0.5f, -0.5f}, CORNFLOWERBLUE},
        {{ 0.5f, -0.5f, -0.5f}, CORNFLOWERBLUE},

        {{-0.5f, -0.5f,  0.5f}, CORNFLOWERBLUE},
        {{ 0.5f, -0.5f, -0.5f}, CORNFLOWERBLUE},
        {{ 0.5f, -0.5f,  0.5f}, CORNFLOWERBLUE},

        // SIDE 6 (TOP)
        {{-0.5f,  0.5f,  0.5f}, CYAN},
        {{-0.5f,  0.5f, -0.5f}, CYAN},
        {{ 0.5f,  0.5f, -0.5f}, CYAN},

        {{-0.5f,  0.5f,  0.5f}, CYAN},
        {{ 0.5f,  0.5f, -0.5f}, CYAN},
        {{ 0.5f,  0.5f,  0.5f}, CYAN}
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

struct Transform {
    float sx, sy, sz; // scale values
    float rot_x, rot_y, rot_z; // angle rotation around _axis
    float x, y, z; // translation values
};

Transform trans_1 {};
Transform trans_2 {};

Matrix4 get_model_mat(const Transform& t) noexcept
{
    return {
        peria::graphics::translate(t.x, t.y, t.z)*
        peria::graphics::rotate(glm::radians(t.rot_x), glm::radians(t.rot_y), glm::radians(t.rot_z))*
        peria::graphics::scale(t.sx, t.sy, t.sz)
    };
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
    //quad_shader->set_mat4("u_mvp", screen_ortho_projection);

    // this was for testing my own projection mat
    quad_shader->set_mat4("u_mvp", peria_ortho_projection);
    
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

//static glm::mat4 view {glm::translate(glm::mat4{1.0f}, glm::vec3{-0.45f, -0.3f, -3.0f})*
//                       glm::rotate(glm::mat4{1.0f}, glm::radians(50.0f), glm::vec3{1.0f, 1.0f, 0.0f})};

static Matrix4 vv {peria::graphics::translate(-0.23f, -0.20f, 0.0f)*
                   peria::graphics::rotate(glm::radians(35.0f), glm::radians(20.0f), glm::radians(30.0f))};

void Graphics::render_cube() noexcept
{
    cube_vao->bind();
    cube_shader->use_shader();
    //auto pp {peria::graphics::get_ortho_projection(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)};
    //auto view {peria::graphics::translate(-0.23f, -0.20f, 0.0f)*
    //           peria::graphics::rotate(glm::radians(35.0f), glm::radians(20.0f), glm::radians(30.0f))};
    auto model1 {get_model_mat(trans_1)};
    cube_shader->set_mat4("u_mvp", peria_ortho_projection*model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //cube_shader->set_mat4("u_mvp", perspective_projection*view);

    auto model2 {get_model_mat(trans_2)};
    cube_shader->set_mat4("u_mvp", peria_ortho_projection*model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Graphics::peria_ortho(float left, float right, float bottom, float top) noexcept
{ peria_ortho_projection = peria::graphics::get_ortho_projection(left, right, bottom, top, 300.0f, -300.0f); }
//{ screen_ortho_projection = glm::ortho(left, right, bottom, top); }

void Graphics::set_perspective_projection(glm::mat4&& projection) noexcept
{ perspective_projection = std::move(projection); }

void Graphics::start_imgui_frame(ImFont* font)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGui::PushFont(font); // must be after ImGui::NewFrame(), at least seems like so xD
}

void Graphics::imgui_render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Graphics::imgui_transforms()
{
    ImGui::Begin("Transforms");
    ImGui::SliderFloat("translate X", &trans_1.x, 0.0f, 800.0f);
    ImGui::SliderFloat("translate Y", &trans_1.y, 0.0f, 600.0f);
    ImGui::SliderFloat("translate Z", &trans_1.z, 0.0f, 1000.0f);
                                                              
    ImGui::SliderFloat("scale X", &trans_1.sx, 1.0f, 500.0f);
    ImGui::SliderFloat("scale Y", &trans_1.sy, 1.0f, 500.0f);
    ImGui::SliderFloat("scale Z", &trans_1.sz, 1.0f, 500.0f);

    ImGui::SliderFloat("rot-angle X", &trans_1.rot_x, -360.0f, 360.0f);
    ImGui::SliderFloat("rot-angle Y", &trans_1.rot_y, -360.0f, 360.0f);
    ImGui::SliderFloat("rot-angle Z", &trans_1.rot_z, -360.0f, 360.0f);

    ImGui::Text("Second");
    ImGui::SliderFloat("tr X", &trans_2.x, 0.0f, 800.0f);
    ImGui::SliderFloat("tr Y", &trans_2.y, 0.0f, 600.0f);
    ImGui::SliderFloat("tr Z", &trans_2.z, 0.0f, 1000.0f);
                                                              
    ImGui::SliderFloat("sc X", &trans_2.sx, 1.0f, 500.0f);
    ImGui::SliderFloat("sc Y", &trans_2.sy, 1.0f, 500.0f);
    ImGui::SliderFloat("sc Z", &trans_2.sz, 1.0f, 500.0f);

    ImGui::SliderFloat("rot X", &trans_2.rot_x, -360.0f, 360.0f);
    ImGui::SliderFloat("rot Y", &trans_2.rot_y, -360.0f, 360.0f);
    ImGui::SliderFloat("rot Z", &trans_2.rot_z, -360.0f, 360.0f);
    ImGui::End();
}

Graphics::~Graphics()
{ peria::log("Graphics shutdown"); }
}
