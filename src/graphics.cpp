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
    /* for 2D I used this convention. for 3D I use the same. I.E. clockwise order - back-facing triangles for OpenGL convention
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

    // tex coords are all the same
    // I use clockwise/back-face triangles
    std::vector<Vertex3d_Textured> cube_model_textured {
        // near
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        
        // far
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},

        // left
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},

        // right
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        
        // bottom
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},

        // top
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}
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

Transform trans_1 {1, 1, 1, 0, 0, 0, 0, 0, -3.0f};
Transform trans_2 {150, 150, 150, 0, 0, 0, 450, 300, 0};

Matrix4 get_model_mat(const Transform& t) noexcept
{
    return {
        peria::graphics::translate(t.x, t.y, t.z)*
        peria::graphics::rotate(glm::radians(t.rot_x), glm::radians(t.rot_y), glm::radians(t.rot_z))*
        peria::graphics::scale(t.sx, t.sy, t.sz)
    };
}

glm::mat4 get_model_mat2(const Transform& t) noexcept
{
    return {
        glm::translate(glm::mat4{1.0f}, glm::vec3{t.x, t.y, t.z})*
        glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_x), glm::vec3{1.0f, 0.0f, 0.0f})*
        glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_y), glm::vec3{0.0f, 1.0f, 0.0f})*
        glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_z), glm::vec3{0.0f, 0.0f, 1.0f})*
        glm::scale(glm::mat4{1.0f}, glm::vec3{t.sx, t.sy, t.sz})
    };
}

}

Graphics::Graphics(glm::mat4&& projection)
    :screen_ortho_projection{std::move(projection)},
     peria_perspective_projection{peria::graphics::get_perspective_projection(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f)}
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

    { // setup textured cube vao/vbo
        cube_vao_textured = std::make_unique<Vertex_Array>();
        cube_vbo_textured = std::make_unique<Named_Buffer_Object<Vertex3d_Textured>>(buffer_data::cube_model_textured);

        // pos vec3
        cube_vao_textured->setup_attribute(Attribute<float>{3, false});
        // color 
        cube_vao_textured->setup_attribute(Attribute<float>{2, false});
        cube_vao_textured->connect_vertex_buffer(cube_vbo_textured->buffer_id(), sizeof(Vertex3d_Textured));
    }

    cube_shader = std::make_unique<Shader>("./assets/cube_vertex.glsl", "./assets/cube_fragment.glsl");
    cube_shader_textured = std::make_unique<Shader>("./assets/cube_vertex_textured.glsl", "./assets/cube_fragment_textured.glsl");
    cube_shader_textured->set_int("u_texture", 0);

    white_texture = std::make_unique<Texture>(1, 1, Color<float>::to_u8_color(WHITE));
    texture_atlas = std::make_unique<Texture>("./assets/mushrooms_sheet.png");
    chiti = std::make_unique<Texture>("./assets/chitunia.png");

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
    // quad_shader->set_mat4("u_mvp", peria_ortho_projection);
    
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

static auto FOV {45.0f};

void Graphics::render_cube() noexcept
{
    cube_vao->bind();
    cube_shader->use_shader();

    const auto view {peria::graphics::translate(trans_1.x, trans_1.y, trans_1.z)};
    peria_perspective(FOV, 800.0f/600.0f, 0.1f, 100.0f);

    cube_shader->set_mat4("u_mvp", peria_perspective_projection*view);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Graphics::render_cube_textured() noexcept
{
    cube_vao_textured->bind();
    cube_shader_textured->use_shader();
    bind_texture_and_sampler(chiti, sampler1, 0);

    //const auto view {glm::translate(glm::mat4{1.0f}, glm::vec3{trans_1.x, trans_1.y, trans_1.z})};
    const auto model {peria::graphics::rotate(glm::radians(trans_1.rot_x), glm::radians(trans_1.rot_y), glm::radians(trans_1.rot_z))};
    const auto view {peria::graphics::translate(trans_1.x, trans_1.y, trans_1.z)};

    cube_shader_textured->set_mat4("u_mvp", peria_perspective_projection*view*model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Graphics::peria_ortho(float left, float right, float bottom, float top) noexcept
//{ peria_ortho_projection = peria::graphics::get_ortho_projection(left, right, bottom, top, -300.0f, 300.0f); }
//{ peria_ortho_projection = peria::graphics::get_ortho_projection(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f); }
//{ peria_ortho_projection = peria::graphics::get_ortho_projection(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f); }
{ screen_ortho_projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f); }

void Graphics::peria_perspective(float fov_y, float aspect_ratio, float near, float far) noexcept
// { perspective_projection = glm::perspective(glm::radians(fov_y), aspect_ratio, near, far); }
{ peria_perspective_projection = peria::graphics::get_perspective_projection(glm::radians(fov_y), aspect_ratio, near, far); }

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
    ImGui::InputFloat("translate X", &trans_1.x, 0.05f);
    ImGui::InputFloat("translate Y", &trans_1.y, 0.05f);
    ImGui::InputFloat("translate Z", &trans_1.z, 0.05f);

    ImGui::InputFloat("rot-angle X", &trans_1.rot_x, 2.0f);
    ImGui::InputFloat("rot-angle Y", &trans_1.rot_y, 2.0f);
    ImGui::InputFloat("rot-angle Z", &trans_1.rot_z, 2.0f);

    /*
    ImGui::InputFloat("translate X", &trans_1.x, 0.1f);
    ImGui::InputFloat("translate Y", &trans_1.y, 0.1f);
    ImGui::InputFloat("translate Z", &trans_1.z, 0.1f);
                                                              
    ImGui::InputFloat("scale X", &trans_1.sx, 0.1f);
    ImGui::InputFloat("scale Y", &trans_1.sy, 0.1f);
    ImGui::InputFloat("scale Z", &trans_1.sz, 0.1f);


    ImGui::Text("Second");
    ImGui::InputFloat("tr X", &trans_2.x, 0.1f);
    ImGui::InputFloat("tr Y", &trans_2.y, 0.1f);
    ImGui::InputFloat("tr Z", &trans_2.z, 0.1f);

    ImGui::InputFloat("sc X", &trans_2.sx, 0.1f);
    ImGui::InputFloat("sc Y", &trans_2.sy, 0.1f);
    ImGui::InputFloat("sc Z", &trans_2.sz, 0.1f);

    ImGui::SliderFloat("rot X", &trans_2.rot_x, -360.0f, 360.0f);
    ImGui::SliderFloat("rot Y", &trans_2.rot_y, -360.0f, 360.0f);
    ImGui::SliderFloat("rot Z", &trans_2.rot_z, -360.0f, 360.0f);
    */
    ImGui::End();
}

Graphics::~Graphics()
{ peria::log("Graphics shutdown"); }
}
