#include "demo.hpp"

#include <glad/glad.h>
#include <array>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <stb_image.h>

#include "common_models.hpp"
#include "graphics.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "timer.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace {

//
//    // random
//    std::random_device rd = std::random_device();
//    std::mt19937 generator(rd());
//
//    [[nodiscard]]
//    int get_int(int l, int r)
//    {
//        std::uniform_int_distribution<> dist(l, r);
//        return dist(rd);
//    }
//
//    [[nodiscard]]
//    float get_float(float l, float r)
//    {
//        std::uniform_real_distribution<> dist(l, r);
//        return dist(rd);
//    }
//
//
//    [[nodiscard]]
//    glm::mat4 get_model_mat(const Transform& t) noexcept
//    {
//        return {
//            glm::translate(glm::mat4{1.0f}, glm::vec3{t.x, t.y, t.z})*
//            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_x), glm::vec3{1.0f, 0.0f, 0.0f})*
//            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_y), glm::vec3{0.0f, 1.0f, 0.0f})*
//            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_z), glm::vec3{0.0f, 0.0f, 1.0f})*
//            glm::scale(glm::mat4{1.0f}, glm::vec3{t.sx, t.sy, t.sz})
//        };
//    }
//
//    [[nodiscard]]
//    glm::vec3 get_vec3(const std::array<float, 3>& color) noexcept
//    { return {color[0], color[1], color[2]}; }
//}
}

namespace peria::demos {

Textured_Cube::Textured_Cube()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     tex{create_texture2d_from_image("./assets/textures/chitunia.png")},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     shader{"./assets/shaders/a_vertex.glsl", "./assets/shaders/a_fragment.glsl"}
{
    // vao/vbo
    {
        buffer_upload_data<Vertex<Pos3D, TexCoord>>(vbo, cube_data, GL_STATIC_DRAW);
        vao_configure<Pos3D, TexCoord>(vao.id, vbo.id, 0);
    }

    shader.set_int("u_texture", 0);
}

void Textured_Cube::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

void Textured_Cube::update()
{}

void Textured_Cube::render()
{
    clear_buffer_all(0, colors::SILVER, 1.0f, 0);

    bind_vertex_array(vao);
    shader.use_shader();
    
    static float x {0.0f};
    x += Timer::instance()->dt();

    shader.set_mat4("u_mvp", projection*camera.get_view());
    shader.set_float("u_offset", x);
    bind_texture_and_sampler(tex.id, sampler.id);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Textured_Cube::imgui() {}

Kvadebi::Kvadebi()
    :solid_color{create_texture2d_colored(colors::WHITE)},
     tex1{create_texture2d_from_image("./assets/textures/chitunia.png")},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     shader{"./assets/shaders/quad_vertex.glsl", "./assets/shaders/quad_fragment.glsl"}
{
    // vao/vbo
    {
        std::array<Vertex<Pos2D, TexCoord, Color4, Attr<float, 1>>, 4> quad_data1 {{
            {{-0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 0.2f, 0.4f, 1.0f}, {0.0f}},
            {{-0.5f,  0.5f}, {0.0f, 1.0f}, {0.0f, 0.2f, 0.1f, 1.0f}, {0.0f}},
            {{ 0.5f,  0.5f}, {1.0f, 1.0f}, {0.4f, 0.6f, 0.0f, 1.0f}, {0.0f}},
            {{ 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.7f, 0.9f, 1.0f}, {0.0f}}
        }};

        std::array<Vertex<Pos2D, TexCoord, Color4, Attr<float, 1>>, 4> quad_data2 {{
            {{-0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f}},
            {{-0.5f,  0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f}},
            {{ 0.5f,  0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f}},
            {{ 0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f}}
        }};

        std::array<u32, 6> indices {0,1,2, 0,2,3};
        buffer_upload_data<u32>(ibo, indices, GL_STATIC_DRAW);

        buffer_upload_data<Vertex<Pos2D, TexCoord, Color4, Attr<float, 1>>>(vbo1, quad_data1, GL_STATIC_DRAW);
        buffer_upload_data<Vertex<Pos2D, TexCoord, Color4, Attr<float, 1>>>(vbo2, quad_data2, GL_STATIC_DRAW);
        vao_configure<Pos2D, TexCoord, Color4, Attr<float, 1>>(vao1.id, vbo1.id, 0);
        vao_configure<Pos2D, TexCoord, Color4, Attr<float, 1>>(vao2.id, vbo2.id, 0);
        vao_connect_ibo(vao1, ibo);
        vao_connect_ibo(vao2, ibo);
    }

    {
        std::array tex_slots {0, 1, 2, 3, 4, 5, 6, 7};
        shader.set_array("u_textures", 8, tex_slots.data());
    }

    recalculate_projection();
}

void Kvadebi::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y, -1.0f, 1.0f);
}

void Kvadebi::update()
{}

void Kvadebi::render()
{
    const auto screen_dims {get_screen_dimensions()};
    clear_buffer_all(0, colors::GREY, 1.0f, 0);
    shader.use_shader();

    bind_texture_and_sampler(solid_color.id, sampler.id, 0);
    auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{screen_dims.x*0.5f-150.0f, screen_dims.y*0.5f, 0.0f})*
                glm::scale(glm::mat4{1.0f}, glm::vec3{80.0f, 60.0f, 1.0f})};

    bind_vertex_array(vao1);
    shader.set_mat4("u_mvp", projection*model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    model = glm::translate(glm::mat4{1.0f}, glm::vec3{screen_dims.x*0.5f+150.0f, screen_dims.y*0.5f, 0.0f})*
            glm::scale(glm::mat4{1.0f}, glm::vec3{80.0f, 60.0f, 1.0f});
    bind_vertex_array(vao2);

    bind_texture_and_sampler(tex1.id, sampler.id, 1);
    shader.set_mat4("u_mvp", projection*model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Kvadebi::imgui() {}

Shadows::Shadows()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     shadowmap{create_texture2d(shadow_data.shadowmap_w, shadow_data.shadowmap_h, GL_DEPTH_COMPONENT32F)},
     chiti{create_texture2d_from_image("./assets/textures/chitunia.png")},
     shadow_shader{"./assets/shaders/shadow/shadow_vertex.glsl", "./assets/shaders/shadow/shadow_fragment.glsl"},
     //omni_shadow_shader{"./assets/shaders/shadow/omni_shadow_vertex.glsl", "./assets/shaders/shadow/omni_shadow_fragment.glsl",  "./assets/shaders/shadow/omni_shadow_geometry.glsl"},
     light_shader{"./assets/shaders/lighting/light_vertex.glsl","./assets/shaders/lighting/light_fragment.glsl"},
     shadow_sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER)},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)}
{
    recalculate_projection();

    // Vao / Buffers
    {
        buffer_upload_data(cube_vbo, cube_data_with_norms, GL_STATIC_DRAW);
        vao_configure<Pos3D, Normal, TexCoord>(cube_vao.id, cube_vbo.id, 0);
    }

    // Frame buffers
    {
        glNamedFramebufferTexture(shadow_fbo.id, GL_DEPTH_ATTACHMENT, shadowmap.id, 0);
        glNamedFramebufferReadBuffer(shadow_fbo.id, GL_NONE);
        glNamedFramebufferDrawBuffer(shadow_fbo.id, GL_NONE);
        {
            const auto status {glCheckNamedFramebufferStatus(shadow_fbo.id, GL_FRAMEBUFFER)};
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                peria::log("FrameBuffer with id", shadow_fbo.id, "incomplete\nstatus", status);
            }
        }
        // TODO: add cubemap
    }

    {
        directional_light = {
            {},
            {0.1f, 0.1f, 0.1f},
            {1.0f, 1.0f, 1.0f},
            {0.8f, 0.8f, 0.8f},
            {0.0f, 3.0f, -1.0f}
        };
        const auto& [x, y, z] {directional_light.pos};
        directional_light.direction = {-x, -y, -z};
    }

    {
        // for directional light
        shadow_data.light_projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 20.0f);
        //shadow_data.light_view = glm::lookAt(arr_to_vec3(directional_light.pos), arr_to_vec3(directional_light.pos)+arr_to_vec3(directional_light.direction), {0.0f, 1.0f, 0.0f});
        shadow_data.light_view = glm::lookAt(arr_to_vec3(directional_light.pos), {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    }

    light_shader.set_int("u_texture", 0);
    light_shader.set_int("u_shadowmap", 1);
}

void Shadows::update()
{
    shadow_data.light_view = glm::lookAt(arr_to_vec3(directional_light.pos), {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    const auto& [x, y, z] {directional_light.pos};
    directional_light.direction = {-x, -y, -z};
}

void Shadows::render()
{
    // shadow pass
    {
        bind_frame_buffer(shadow_fbo);
        set_viewport(0, 0, shadow_data.shadowmap_w, shadow_data.shadowmap_h);
        clear_buffer_depth(shadow_fbo.id, 1.0f);

        bind_vertex_array(cube_vao);
        shadow_shader.use_shader();

        const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, -0.5f, 0.0f))*
                          glm::scale(glm::mat4{1.0f}, glm::vec3(50.0f, 0.1f, 50.0f))};
        shadow_data.light_view = glm::lookAt(arr_to_vec3(directional_light.pos), arr_to_vec3(directional_light.pos)+arr_to_vec3(directional_light.direction), {0.0f, 1.0f, 0.0f});

        shadow_shader.set_mat4("u_vp", shadow_data.light_projection*shadow_data.light_view);
        shadow_shader.set_mat4("u_model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shadow_shader.set_mat4("u_model", glm::mat4{1.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // light pass
    {
        bind_frame_buffer_default();
        const auto screen_dims {get_screen_dimensions()};
        set_viewport(0, 0, screen_dims.x, screen_dims.y);
        clear_buffer_all(0, colors::GREY, 1.0f, 0);

        bind_vertex_array(cube_vao);
        light_shader.use_shader();

        light_shader.set_mat4("u_vp", projection*camera.get_view());
        light_shader.set_mat4("u_light_vp", shadow_data.light_projection*shadow_data.light_view);

        light_shader.set_float("u_min_shadow_bias", shadow_data.min_bias);
        light_shader.set_float("u_max_shadow_bias", shadow_data.max_bias);

        light_shader.set_vec3("u_camera_pos", camera.get_pos());
        bind_texture_and_sampler(chiti.id, sampler.id);
        bind_texture_and_sampler(shadowmap.id, shadow_sampler.id, 1);

        light_shader.set_vec3("u_directional_light.direction", arr_to_vec3(directional_light.direction));
        light_shader.set_vec3("u_directional_light.pos",       arr_to_vec3(directional_light.pos));
        light_shader.set_vec3("u_directional_light.ambient",   arr_to_vec3(directional_light.ambient));
        light_shader.set_vec3("u_directional_light.diffuse",   arr_to_vec3(directional_light.diffuse));
        light_shader.set_vec3("u_directional_light.specular",  arr_to_vec3(directional_light.specular));

        const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, -0.5f, 0.0f))*
                          glm::scale(glm::mat4{1.0f}, glm::vec3(50.0f, 0.1f, 50.0f))};
        light_shader.set_mat4("u_model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        light_shader.set_mat4("u_model", glm::mat4{1.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Shadows::imgui()
{
    ImGui::SliderFloat3("DirLight dir", directional_light.direction.data(), -1.0f, 1.0f);
    ImGui::SliderFloat3("DirLight pos", directional_light.pos.data(), -50.0f, 50.0f);

    ImGui::SliderFloat("min bias", &shadow_data.min_bias, 0.0f, 1.0f);
    ImGui::SliderFloat("max bias", &shadow_data.max_bias, 0.0f, 1.0f);
}

void Shadows::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}


}
