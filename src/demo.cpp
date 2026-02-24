#include "demo.hpp"
#include <glad/glad.h> 
#include <array>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <random>
#include <stb_image.h>

#include "common_models.hpp"
#include "graphics.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "timer.hpp"
#include "input_manager.hpp"
#include "mesh.hpp"
#include <imgui.h> 
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

namespace {
    constexpr i32 MAX_PLS  {256};
    constexpr i32 MAX_SPLS {32};

    [[nodiscard]]
    bool sphere_vs_line(const glm::vec3& sc, float r,
                        const glm::vec3& p, const glm::vec3& d)
    {
        const auto d_norm {glm::normalize(d)};
        const float A {1.0f};
        const float B {2.0f*(d_norm.x*(p.x - sc.x) + d_norm.y*(p.y - sc.y) + d_norm.z*(p.z - sc.z))};
        const float C {(p.x - sc.x)*(p.x - sc.x) + (p.y - sc.y)*(p.y - sc.y) + (p.z - sc.z)*(p.z - sc.z) - r*r};
        const float Disc {B*B - 4.0f*A*C};
        if (Disc < 0.0f) {
            return false;
        }
        return true;
    }


    // random
    std::random_device rd = std::random_device();
    std::mt19937 generator(rd());

    [[nodiscard]]
    int get_int(int l, int r)
    {
        std::uniform_int_distribution<> dist(l, r);
        return dist(rd);
    }

    [[nodiscard]]
    float get_float(float l, float r)
    {
        std::uniform_real_distribution<> dist(l, r);
        return dist(rd);
    }


  [[nodiscard]]
  glm::mat4 get_model_mat(const peria::Transform& t) noexcept
  {
      return {
          glm::translate(glm::mat4{1.0f}, glm::vec3{t.pos[0], t.pos[1], t.pos[2]})*
          glm::rotate(glm::mat4{1.0f}, glm::radians(t.angle[0]), glm::vec3{1.0f, 0.0f, 0.0f})*
          glm::rotate(glm::mat4{1.0f}, glm::radians(t.angle[1]), glm::vec3{0.0f, 1.0f, 0.0f})*
          glm::rotate(glm::mat4{1.0f}, glm::radians(t.angle[2]), glm::vec3{0.0f, 0.0f, 1.0f})*
          glm::scale(glm::mat4{1.0f}, glm::vec3{t.scale[0], t.scale[1], t.scale[2]})
      };
  }

    [[nodiscard]]
    glm::vec3 get_vec3(const std::array<float, 3>& color) noexcept
    { return {color[0], color[1], color[2]}; }
}



namespace peria::demos {

Textured_Cube::Textured_Cube()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     tex{create_texture2d_from_image("./assets/textures/chitunia.png")},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     shader{"./assets/shaders/a_vertex.glsl", "./assets/shaders/a_fragment.glsl""./assets/shaders/a_vertex.glsl", "./assets/shaders/a_fragment.glsl"}
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
{
    if (is_relative_mouse()) {
        camera.update();
    }
}

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

    {
        const auto screen_dims {get_screen_dimensions()};
        t1 = {
            {screen_dims.x*0.5f-100.0f, screen_dims.y*0.5f, 0.0f},
            {200.0f, 200.0f, 1.0f},
            0.0f
        };

        t2 = {
            {screen_dims.x*0.5f+150.0f, screen_dims.y*0.5f, 0.0f},
            {80.0f, 80.0f, 1.0f},
            0.0f
        };
    }
}

void Kvadebi::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y, -1.0f, 1.0f);
}

void Kvadebi::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
    t1.angle += speed*Timer::instance()->dt();
    t2.angle += speed*Timer::instance()->dt();
}

void Kvadebi::render()
{
    clear_buffer_all(0, colors::GREY, 1.0f, 0);
    shader.use_shader();

    bind_texture_and_sampler(solid_color.id, sampler.id, 0);
    auto model {glm::translate(glm::mat4{1.0f}, arr_to_vec3(t1.pos))*
                glm::rotate(glm::mat4{1.0f}, glm::radians(t1.angle), glm::vec3{0.0f, 0.0f, 1.0f})*
                glm::scale(glm::mat4{1.0f}, arr_to_vec3(t1.scale))};

    bind_vertex_array(vao1);
    shader.set_mat4("u_mvp", projection*model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    const auto screen_dims {get_screen_dimensions()};

    model = glm::translate(glm::mat4{1.0f}, arr_to_vec3(t2.pos))*
            //glm::translate(glm::mat4{1.0f}, glm::vec3{t2.scale[0]*0.5f, t2.scale[1]*0.5f, 0.0f})*
            glm::translate(glm::mat4{1.0f}, glm::vec3{screen_dims.x*0.5f, screen_dims.y*0.5f, 0.0f})*
            glm::rotate(glm::mat4{1.0f}, glm::radians(t2.angle), glm::vec3{0.0f, 0.0f, 1.0f})*
            glm::translate(glm::mat4{1.0f}, -glm::vec3{screen_dims.x*0.5f, screen_dims.y*0.5f, 0.0f})*
            //glm::translate(glm::mat4{1.0f}, -glm::vec3{t2.scale[0]*0.5f, t2.scale[1]*0.5f, 0.0f})*
            glm::scale(glm::mat4{1.0f}, arr_to_vec3(t2.scale));
    bind_vertex_array(vao2);

    bind_texture_and_sampler(tex1.id, sampler.id, 1);
    shader.set_mat4("u_mvp", projection*model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

void Kvadebi::imgui() 
{
    ImGui::SliderFloat("speed", &speed, 0.0f, 100.0f);
    ImGui::InputFloat3("t1.scale", t1.scale.data());

}

Shadows::Shadows()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     shadowmap{create_texture2d(shadow_data.shadowmap_w, shadow_data.shadowmap_h, GL_DEPTH_COMPONENT32F)},
     picking_depth{create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F)},
     picking_color{create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGB32UI)},
     chiti{create_texture2d_from_image("./assets/textures/chitunia.png")},
     monkey_color{create_texture2d_colored(colors::CYAN)},
     uv_sphere_color{create_texture2d_colored(colors::OLIVE)},
     ico_sphere_color{create_texture2d_colored(colors::TOMATO)},
     shadow_shader{"./assets/shaders/shadow/shadow_vertex.glsl", "./assets/shaders/shadow/shadow_fragment.glsl"},
     //omni_shadow_shader{"./assets/shaders/shadow/omni_shadow_vertex.glsl", "./assets/shaders/shadow/omni_shadow_fragment.glsl",  "./assets/shaders/shadow/omni_shadow_geometry.glsl"},
     light_shader{"./assets/shaders/lighting/light_vertex.glsl","./assets/shaders/lighting/light_fragment.glsl"},
     colored_obj_shader{"./assets/shaders/colored_object_vertex.glsl","./assets/shaders/colored_object_fragment.glsl"},
     line_shader{"./assets/shaders/line_vertex.glsl","./assets/shaders/line_fragment.glsl"},
     picking_shader{"./assets/shaders/picking_vertex.glsl","./assets/shaders/picking_fragment.glsl"},
     shadow_sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER)},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     monkey{"./assets/models/monkey/suzanne.obj"},
     uv_sphere{"./assets/models/uv_sphere/uv_sphere.obj"},
     ico_sphere{"./assets/models/ico_sphere/ico_sphere.obj"},
     shadowmapper{2048, 2048}
{
    recalculate_projection();

    // Vao / Buffers
    {
        buffer_upload_data<Vertex<Pos3D, Normal, TexCoord>>(cube_vbo, cube_data_with_norms, GL_STATIC_DRAW);
        vao_configure<Pos3D, Normal, TexCoord>(cube_vao.id, cube_vbo.id, 0);

        const std::array<Vertex<Pos3D>, 2> line_data {{
            {{-0.5f, 0.0f, 0.0f}},
            {{ 0.5f, 0.0f, 0.0f}}
        }};
        buffer_upload_data(line_vbo, line_data, GL_STATIC_DRAW);
        vao_configure<Pos3D>(line_vao.id, line_vbo.id, 0);
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

        glNamedFramebufferTexture(picking_fbo.id, GL_COLOR_ATTACHMENT0, picking_color.id, 0);
        glNamedFramebufferTexture(picking_fbo.id, GL_DEPTH_ATTACHMENT,  picking_depth.id, 0);
        {
            const auto status {glCheckNamedFramebufferStatus(picking_fbo.id, GL_FRAMEBUFFER)};
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                peria::log("FrameBuffer with id", picking_fbo.id, "incomplete\nstatus", status);
            }
        }

    }

    // light values
    {
        light_data.directional_light = {
            {},
            {0.1f, 0.1f, 0.1f},
            {1.0f, 1.0f, 1.0f},
            {0.8f, 0.8f, 0.8f},
            {0.0f, 3.0f, -1.0f}
        };
        const auto& [x, y, z] {light_data.directional_light.pos};
        light_data.directional_light.direction = {-x, -y, -z};

        light_data.spot_lights[0] = {
            {2.4f, 8.5f, -5.6f},
            {-0.4f, -1.0f, 0.7f},
            {0.0f, 0.05f, 0.05f},
            {1.0f, 1.0f, 1.0f},
            {0.9f, 0.6f, 0.8f},
            40.0f,
            48.0f
        };
        ++light_data.active_spot_lights;
    }

    {
        // for directional light
        //shadow_data.light_projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 20.0f);
        //shadow_data.light_view = glm::lookAt(arr_to_vec3(light_data.directional_light.pos), {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

        shadowmapper.set_light_projection(glm::perspective(glm::radians(light_data.spot_lights[0].inner_angle*2.0f), static_cast<float>(shadow_data.shadowmap_w)/shadow_data.shadowmap_h, 0.1f, 20.0f));
        shadowmapper.set_light_view(glm::lookAt(arr_to_vec3(light_data.spot_lights[0].pos), arr_to_vec3(light_data.spot_lights[0].direction), {0.0f, 1.0f, 0.0f}));
    }

    light_shader.set_int("u_texture", 0);
    light_shader.set_int("u_shadowmap", 1);
}

void Shadows::update()
{
    //shadow_data.light_view = glm::lookAt(arr_to_vec3(light_data.directional_light.pos), {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    //const auto& [x, y, z] {light_data.directional_light.pos};
    //light_data.directional_light.direction = {-x, -y, -z};

    if (is_relative_mouse()) {
        camera.update();
    }
    shadow_data.light_view = glm::lookAt(arr_to_vec3(light_data.spot_lights[0].pos), arr_to_vec3(light_data.spot_lights[0].direction), {0.0f, 1.0f, 0.0f});

    const auto screen_dims {get_screen_dimensions()};
    auto [mx, my] {Input_Manager::instance()->get_mouse()};
    my = screen_dims.y - my;
    
    peria::log("mouse pos:", mx, my);

    const auto mx_ndc {2.0f*mx / screen_dims.x - 1.0f};
    const auto my_ndc {2.0f*my / screen_dims.y - 1.0f};
    peria::log("mouse ndc", mx_ndc, my_ndc);

    glm::vec4 clip {mx_ndc, my_ndc, -1.0f, 1.0f};

    glm::vec4 eye_space {glm::inverse(projection)*clip};
    peria::log("eye space", eye_space.x, eye_space.y, eye_space.z, eye_space.w);
    
    eye_space.w = 0.0f;

    glm::vec4 world_space {glm::normalize(glm::inverse(camera.get_view())*eye_space)};

    peria::log("world space", world_space.x, world_space.y, world_space.z, world_space.w);
}

void Shadows::draw_scene(const Shader& shader)
{
    bind_vertex_array(cube_vao);
    shader.use_shader();

    auto model {glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, -0.5f, 0.0f))*
                glm::scale(glm::mat4{1.0f}, glm::vec3(50.0f, 0.1f, 50.0f))};

    shader.set_mat4("u_model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader.set_mat4("u_model", glm::mat4{1.0f});
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, -2.5f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(2.0f, 2.0f, 1.0f));
    shader.set_mat4("u_model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(-2.0f, 2.0f, 1.0f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.set_mat4("u_model", model);
    {
        const auto& meshes {monkey.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(2.0f, 1.0f, 2.0f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(2.0f, 1.0f, 1.0f));
    shader.set_mat4("u_model", model);
    bind_texture_and_sampler(uv_sphere_color.id, sampler.id);
    {
        const auto& meshes {uv_sphere.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(2.0f, 3.0f, -3.0f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.set_mat4("u_model", model);
    bind_texture_and_sampler(ico_sphere_color.id, sampler.id);
    {
        const auto& meshes {ico_sphere.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

void Shadows::draw_scene_for_picking()
{
    bind_frame_buffer(picking_fbo);
    clear_buffer_all(picking_fbo.id, colors::BLACK, 1.0f, 0);
    picking_shader.use_shader();
    picking_shader.set_mat4("u_vp", projection*camera.get_view());

    bind_vertex_array(cube_vao);
    picking_shader.use_shader();

    u32 object_id {1};

    auto model {glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, -0.5f, 0.0f))*
                glm::scale(glm::mat4{1.0f}, glm::vec3(50.0f, 0.1f, 50.0f))};

    picking_shader.set_mat4("u_model", model);
    picking_shader.set_uint("u_object_id", object_id); ++object_id;
    picking_shader.set_uint("u_draw_id", 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    picking_shader.set_mat4("u_model", glm::mat4{1.0f});
    picking_shader.set_uint("u_object_id", object_id); ++object_id;
    picking_shader.set_uint("u_draw_id", 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, -2.5f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(2.0f, 2.0f, 1.0f));
    picking_shader.set_mat4("u_model", model);
    picking_shader.set_uint("u_object_id", object_id); ++object_id;
    picking_shader.set_uint("u_draw_id", 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(-2.0f, 2.0f, 1.0f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(1.0f, 1.0f, 1.0f));
    picking_shader.set_mat4("u_model", model);
    picking_shader.set_uint("u_object_id", object_id); ++object_id;
    {
        const auto& meshes {monkey.get_meshes()};
        i32 draw_id {};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            picking_shader.set_uint("u_draw_id", draw_id); ++draw_id;
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(2.0f, 1.0f, 2.0f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(2.0f, 1.0f, 1.0f));
    picking_shader.set_mat4("u_model", model);
    picking_shader.set_uint("u_object_id", object_id); ++object_id;
    {
        const auto& meshes {uv_sphere.get_meshes()};
        i32 draw_id {};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            picking_shader.set_uint("u_draw_id", draw_id); ++draw_id;
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

    model = glm::translate(glm::mat4{1.0f}, glm::vec3(2.0f, 3.0f, -3.0f))*
            glm::scale(glm::mat4{1.0f}, glm::vec3(1.0f, 1.0f, 1.0f));
    picking_shader.set_mat4("u_model", model);
    picking_shader.set_uint("u_object_id", object_id); ++object_id;
    {
        const auto& meshes {ico_sphere.get_meshes()};
        i32 draw_id {};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            picking_shader.set_uint("u_draw_id", draw_id); ++draw_id;
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

void Shadows::render()
{
    if (!is_relative_mouse()) {
        draw_scene_for_picking();
    }

    // shadow pass
    if (toggle_shadows) {
        /*
           for each shadowmapper:
               execute shadowmapping with draw_scene callback;
               
               shadowmapper.execute([](){});

         * */

        shadowmapper.execute([&](const Shader& shader){
            bind_vertex_array(cube_vao);
            shader.use_shader();

            auto model {glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, -0.5f, 0.0f))*
                        glm::scale(glm::mat4{1.0f}, glm::vec3(50.0f, 0.1f, 50.0f))};

            shader.set_mat4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            shader.set_mat4("u_model", glm::mat4{1.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, -2.5f))*
                    glm::scale(glm::mat4{1.0f}, glm::vec3(2.0f, 2.0f, 1.0f));
            shader.set_mat4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(glm::mat4{1.0f}, glm::vec3(-2.0f, 2.0f, 1.0f))*
                    glm::scale(glm::mat4{1.0f}, glm::vec3(1.0f, 1.0f, 1.0f));
            shader.set_mat4("u_model", model);
            {
                const auto& meshes {monkey.get_meshes()};
                for (const auto& mesh:meshes) {
                    bind_vertex_array(mesh.vao_id());
                    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
                }
            }

            model = glm::translate(glm::mat4{1.0f}, glm::vec3(2.0f, 1.0f, 2.0f))*
                    glm::scale(glm::mat4{1.0f}, glm::vec3(2.0f, 1.0f, 1.0f));
            shader.set_mat4("u_model", model);
            bind_texture_and_sampler(uv_sphere_color.id, sampler.id);
            {
                const auto& meshes {uv_sphere.get_meshes()};
                for (const auto& mesh:meshes) {
                    bind_vertex_array(mesh.vao_id());
                    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
                }
            }

            model = glm::translate(glm::mat4{1.0f}, glm::vec3(2.0f, 3.0f, -3.0f))*
                    glm::scale(glm::mat4{1.0f}, glm::vec3(1.0f, 1.0f, 1.0f));
            shader.set_mat4("u_model", model);
            bind_texture_and_sampler(ico_sphere_color.id, sampler.id);
            {
                const auto& meshes {ico_sphere.get_meshes()};
                for (const auto& mesh:meshes) {
                    bind_vertex_array(mesh.vao_id());
                    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
                }
            }
                }, shadow_shader);


        //bind_frame_buffer(shadow_fbo);
        //set_viewport(0, 0, shadow_data.shadowmap_w, shadow_data.shadowmap_h);
        //clear_buffer_depth(shadow_fbo.id, 1.0f);

        //shadow_shader.set_mat4("u_vp", shadow_data.light_projection*shadow_data.light_view);
        //draw_scene(shadow_shader);
    }

    // light pass
    {
        bind_frame_buffer_default();
        const auto screen_dims {get_screen_dimensions()};
        set_viewport(0, 0, screen_dims.x, screen_dims.y);
        clear_buffer_all(0, colors::GREY, 1.0f, 0);

        i32 selected_object {-1};

        if (!is_relative_mouse()) {
            auto [mx, my] {Input_Manager::instance()->get_mouse()};
            my = get_screen_dimensions().y - my;
            std::array<u32, 3> pixels {};
            glGetTextureSubImage(picking_color.id, 0, mx, my, 0, 1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, sizeof(u32)*3, pixels.data());
            if (pixels[0] != 0) {
                selected_object = static_cast<i32>(pixels[0] - 1);
            }
        }
        peria::log(selected_object);

        light_shader.set_mat4("u_vp", projection*camera.get_view());
        //light_shader.set_mat4("u_light_vp", shadow_data.light_projection*shadow_data.light_view);
        light_shader.set_mat4("u_light_vp", shadowmapper.get_light_vp());
        light_shader.set_vec3("u_camera_pos", camera.get_pos());

        light_shader.set_float("u_min_shadow_bias",        min_bias);
        light_shader.set_float("u_max_shadow_bias",        max_bias);
        light_shader.set_int("u_toggle_shadows",           toggle_shadows);
        light_shader.set_int("u_toggle_directional_light", light_data.toggle_directional_light);
        light_shader.set_int("u_toggle_spot_lights",       light_data.toggle_spot_lights);

        bind_texture_and_sampler(chiti.id, sampler.id);
        //bind_texture_and_sampler(shadowmap.id, shadow_sampler.id, 1);
        bind_texture_and_sampler(shadowmapper.get_shadowmap_id(), shadow_sampler.id, 1);

        // dir light values
        {
            light_shader.set_vec3("u_directional_light.direction", arr_to_vec3(light_data.directional_light.direction));
            light_shader.set_vec3("u_directional_light.pos",       arr_to_vec3(light_data.directional_light.pos));
            light_shader.set_vec3("u_directional_light.ambient",   arr_to_vec3(light_data.directional_light.ambient));
            light_shader.set_vec3("u_directional_light.diffuse",   arr_to_vec3(light_data.directional_light.diffuse));
            light_shader.set_vec3("u_directional_light.specular",  arr_to_vec3(light_data.directional_light.specular));
        }

        // spot light values
        {
            light_shader.set_vec3("u_spot_lights[0].direction",    arr_to_vec3(light_data.spot_lights[0].direction));
            light_shader.set_vec3("u_spot_lights[0].pos",          arr_to_vec3(light_data.spot_lights[0].pos));
            light_shader.set_vec3("u_spot_lights[0].ambient",      arr_to_vec3(light_data.spot_lights[0].ambient));
            light_shader.set_vec3("u_spot_lights[0].diffuse",      arr_to_vec3(light_data.spot_lights[0].diffuse));
            light_shader.set_vec3("u_spot_lights[0].specular",     arr_to_vec3(light_data.spot_lights[0].specular));

            const auto cos_inner_angle {std::cos(glm::radians(light_data.spot_lights[0].inner_angle))};
            const auto cos_outer_angle {std::cos(glm::radians(light_data.spot_lights[0].outer_angle))};
            light_shader.set_float("u_spot_lights[0].cos_inner_angle", cos_inner_angle);
            light_shader.set_float("u_spot_lights[0].cos_outer_angle", cos_outer_angle);
        }

        draw_scene(light_shader);
    }

    bind_vertex_array(cube_vao);
    // light object visualization (these don't take part in shadows)
    {
        colored_obj_shader.use_shader();
        const auto [x, y, z] = light_data.spot_lights[0].pos;
        auto obj_model {glm::translate(glm::mat4{1.0f}, glm::vec3(x, y, z))*
                        glm::scale(glm::mat4{1.0f}, glm::vec3(0.25f, 0.25f, 0.25f))};
        colored_obj_shader.set_mat4("u_vp", projection*camera.get_view());
        colored_obj_shader.set_mat4("u_model", obj_model);
        colored_obj_shader.set_vec3("u_color", arr_to_vec3(light_data.spot_lights[0].diffuse));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Shadows::imgui()
{
    ImGui::InputFloat3("campos", campos.data());
    ImGui::InputFloat3("camviewdir", camviewdir.data());
    ImGui::Text("LIGHTS");

    ImGui::SliderFloat3("DirLight dir", light_data.directional_light.direction.data(), -1.0f, 1.0f);
    ImGui::SliderFloat3("DirLight pos", light_data.directional_light.pos.data(), -50.0f, 50.0f);

    ImGui::SliderFloat3("SpotLight dir", light_data.spot_lights[0].direction.data(), -1.0f, 1.0f);
    ImGui::SliderFloat3("SpotLight pos", light_data.spot_lights[0].pos.data(), -50.0f, 50.0f);
    ImGui::SliderFloat("SpotLight inner_angle", &light_data.spot_lights[0].inner_angle, 0.0f, 90.0f);
    ImGui::SliderFloat("SpotLight outer_angle", &light_data.spot_lights[0].outer_angle, 0.0f, 90.0f);

    ImGui::SliderFloat("min bias", &min_bias, 0.0f, 1.0f);
    ImGui::SliderFloat("max bias", &max_bias, 0.0f, 1.0f);

    ImGui::Checkbox("Shadows",    &toggle_shadows);
    ImGui::Checkbox("Dir-light",  &light_data.toggle_directional_light);
    ImGui::Checkbox("Spot-light", &light_data.toggle_spot_lights);
}

void Shadows::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Transformations::Transformations()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     camera2{{4.0f, 1.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     chiti{create_texture2d_from_image("./assets/textures/chitunia.png")},
     colored_obj_shader{"./assets/shaders/colored_object_vertex.glsl","./assets/shaders/colored_object_fragment.glsl"},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)}
{
    recalculate_projection();

    // Vao / Buffers
    {
        buffer_upload_data(cube_vbo, cube_data, GL_STATIC_DRAW);
        vao_configure<Pos3D, TexCoord>(cube_vao.id, cube_vbo.id, 0);
    }

    bind_frame_buffer_default();
    const auto screen_dims {get_screen_dimensions()};
    set_viewport(0, 0, screen_dims.x, screen_dims.y);
    rotating_cube_position = {3.0f, 0.0f, 0.0f};
}

void Transformations::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
    recalculate_projection();
    angle += 115.0f*Timer::instance()->dt();
}

void Transformations::render()
{
    clear_buffer_all(0, colors::GREY, 1.0f, 0);
    bind_vertex_array(cube_vao);

    colored_obj_shader.use_shader();
    if (use_main_camera) {
        colored_obj_shader.set_mat4("u_vp", projection*camera.get_view());
    }
    else {
        colored_obj_shader.set_mat4("u_vp", projection*camera2.get_view());
    }

    auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, 0.0f})*
                glm::scale(glm::mat4{1.0f}, glm::vec3{1.0f, 1.0f, 1.0f})};
    colored_obj_shader.set_mat4("u_model", model);
    colored_obj_shader.set_vec3("u_color", glm::vec3{1.0f, 1.0f, 0.0f});
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 2.0f, 0.0f})*
            glm::scale(glm::mat4{1.0f}, glm::vec3{0.7f, 0.9f, 0.4f});
    colored_obj_shader.set_mat4("u_model", model);
    colored_obj_shader.set_vec3("u_color", glm::vec3{1.0f, 0.0f, 1.0f});
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4{1.0f}, camera.get_pos()+glm::vec3{0.0f, 0.0f, 0.2f})*
            glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f, 0.2f, 0.2f});
    colored_obj_shader.set_mat4("u_model", model);
    colored_obj_shader.set_vec3("u_color", glm::vec3{1.0f, 0.0f, 0.0f});
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4{1.0f}, camera2.get_pos()+glm::vec3{0.0f, 0.0f, 0.2f})*
            glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f, 0.2f, 0.2f});
    colored_obj_shader.set_mat4("u_model", model);
    colored_obj_shader.set_vec3("u_color", glm::vec3{0.0f, 1.0f, 0.0f});
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw rotating thingy here
    {
        model = glm::translate(glm::mat4{1.0f}, rotating_cube_position)*
                glm::rotate(glm::mat4{1.0f}, glm::radians(angle), glm::normalize(glm::vec3{0.0f, 0.0f, 1.0f}))*
                glm::translate(glm::mat4{1.0f}, -rotating_cube_position)*
                glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f, 0.2f, 0.2f});
        colored_obj_shader.set_mat4("u_model", model);
        colored_obj_shader.set_vec3("u_color", glm::vec3{0.4f, 0.6f, 0.4f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Transformations::imgui()
{
    if (ImGui::Button("toggle camera")) {
        use_main_camera = !use_main_camera;
    }

    if (ImGui::Button("do_ortho")) {
        do_ortho = !do_ortho;
        recalculate_projection();
    }
    ImGui::InputFloat3("scl", scale.data());
}

void Transformations::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    if (do_ortho) {
        projection = glm::ortho(-scale[0], scale[0], -scale[1], scale[1], -scale[2], scale[2]);
    }
    else {
        projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
    }
}

Modelebi::Modelebi()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     monkey{"./assets/models/monkey/suzanne.obj"},
     uv_sphere{"./assets/models/uv_sphere/uv_sphere.obj"},
     tree{"./assets/models/vcdilob/vcdilob.obj"},
     shader{"./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl"},
     model_shader{"./assets/shaders/basic_model_vertex.glsl", "./assets/shaders/basic_model_fragment.glsl"},
     sampler{create_sampler(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     texture{create_texture2d_from_image("./assets/models/vcdilob/color_palette.png")}
{
    recalculate_projection();
    shader.set_int("u_texture", 0);
}

void Modelebi::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
}

void Modelebi::render()
{
    clear_buffer_all(0, colors::GREY, 1.0f, 0);
    
    model_shader.use_shader();
    model_shader.set_mat4("u_vp", projection*camera.get_view());
    model_shader.set_vec3("u_model_color", {1.0f, 0.5f, 0.2f});
    model_shader.set_mat4("u_model", glm::mat4{1.0f});

    {
        const auto& meshes {monkey.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

    {
        const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{3.0f, 0.0f, 0.0f})};
        model_shader.set_mat4("u_model", model);
        const auto& meshes {uv_sphere.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

    {
        const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{7.0f, 0.0f, 0.0f})};
        shader.set_mat4("u_model", model);
        shader.set_mat4("u_vp", projection*camera.get_view());
        shader.use_shader();
        bind_texture_and_sampler(texture.id, sampler.id, 0); // texture is loaded manually not during model loading
        const auto& meshes {tree.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

void Modelebi::imgui()
{
}

void Modelebi::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Lines::Lines()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     line_shader{"./assets/shaders/line_vertex.glsl", "./assets/shaders/line_fragment.glsl"},
     model_shader{"./assets/shaders/basic_model_vertex.glsl","./assets/shaders/basic_model_fragment.glsl"},
     uv_sphere{"./assets/models/uv_sphere/uv_sphere.obj"}
{
    {
        buffer_upload_data(vbo, line_data, GL_DYNAMIC_DRAW);
        vao_configure<Pos3D, Color4>(vao.id, vbo.id, 0);
    }

    sphere = {
        {1, 1, 0},
        0.7f
    };
}

void Lines::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
}

void Lines::render()
{
    clear_buffer_all(0, colors::TAN, 1.0f, 0);
    bind_vertex_array(vao);
    line_shader.use_shader();
    line_shader.set_mat4("u_vp", projection*camera.get_view());
    glLineWidth(5);

    glm::vec3 sphere_color {0.1f, 0.5f, 0.2f};

    for (const auto& l:lines) {
        const auto p1 {arr_to_vec3(l.p)};
        const auto d {glm::normalize(arr_to_vec3(l.dir))};
        const auto p2 {p1 + l.t*d};

        if (sphere_vs_line(arr_to_vec3(sphere.pos), sphere.radius, p1, d)) {
            sphere_color = {0.0f, 0.0f, 0.0f};
        }

        line_data[0] = Vertex<Pos3D, Color4> {{p1.x, p1.y, p1.z}, {l.color[0], l.color[1], l.color[2], 1.0f}};
        line_data[1] = Vertex<Pos3D, Color4> {{p2.x, p2.y, p2.z}, {l.color[0], l.color[1], l.color[2], 1.0f}};
        buffer_upload_subdata(vbo, 0, line_data.size()*Vertex<Pos3D, Color4>::stride, line_data.data());
        glDrawArrays(GL_LINES, 0, 2);
    }
    glLineWidth(1);
    
    model_shader.use_shader();
    model_shader.set_mat4("u_vp", projection*camera.get_view());
    model_shader.set_vec3("u_model_color", sphere_color);
    model_shader.set_mat4("u_model", glm::translate(glm::mat4{1.0f}, arr_to_vec3(sphere.pos))*
                                     glm::scale(glm::mat4{1.0f}, {sphere.radius, sphere.radius, sphere.radius}));

    {
        const auto& meshes {uv_sphere.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

}

void Lines::imgui()
{
    {
        ImGui::Text("NEW LINE");
        ImGui::InputFloat3("P", line.p.data());
        ImGui::InputFloat3("dir", line.dir.data());
        ImGui::SliderFloat("scalar", &line.t, 0.0f, 10.0f);
        ImGui::ColorPicker3("LineColor", line.color.data());
        if (ImGui::Button("Add line")) {
            lines.emplace_back(line);
        }
    }
    {
        ImGui::Text("LINES");
        i32 i{};
        for (auto& l:lines) {
            ImGui::InputFloat3(("p"+std::to_string(i)).c_str(), l.p.data());
            ImGui::InputFloat3(("dir"+std::to_string(i)).c_str(), l.dir.data());
            ImGui::SliderFloat(("t"+std::to_string(i)).c_str(), &l.t, 0.0f, 10.0f);
            ImGui::ColorPicker3(("color"+std::to_string(i)).c_str(), l.color.data());
            ++i;
        }
    }
}

void Lines::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
    //projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}

Mouse_Moving_Basic::Mouse_Moving_Basic()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     model_shader{"./assets/shaders/basic_model_vertex.glsl","./assets/shaders/basic_model_fragment.glsl"},
     uv_sphere{"./assets/models/uv_sphere/uv_sphere.obj"}
{
    sphere = {
        {0.0f, 0.0f, 0.0f},
        0.8f
    };
}

void Mouse_Moving_Basic::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }

    const auto screen_dims {get_screen_dimensions()};
    auto [mx, my] {Input_Manager::instance()->get_mouse()};
    my = screen_dims.y - my;
    
    const auto mx_ndc {2.0f*mx / screen_dims.x - 1.0f};
    const auto my_ndc {2.0f*my / screen_dims.y - 1.0f};

    glm::vec4 clip {mx_ndc, my_ndc, -1.0f, 1.0f};
    glm::vec4 eye_space {glm::inverse(projection)*clip};
    eye_space.w = 0.0f;

    glm::vec4 world_space {glm::normalize(glm::inverse(camera.get_view())*eye_space)};
    mouse_world = {world_space.x, world_space.y, world_space.z};

    const auto im {Input_Manager::instance()};
    if (im->mouse_down(Mouse_Button::LEFT) &&
        !is_relative_mouse() &&
        sphere_vs_line(arr_to_vec3(sphere.pos), sphere.radius, camera.get_pos(), mouse_world)) {
        const auto sensitivity {0.005f};

        const auto rel_motion {get_relative_motion()};

        if (im->key_down(SDL_SCANCODE_LSHIFT) && im->key_down(SDL_SCANCODE_X)) {
            if (rel_motion.x > 0) {
                sphere.pos[0] += sensitivity;
            }
            else {
                sphere.pos[0] -= sensitivity;
            }
        }
        if (im->key_down(SDL_SCANCODE_LSHIFT) && im->key_down(SDL_SCANCODE_Y)) {
            if (rel_motion.y > 0) {
                sphere.pos[1] += sensitivity;
            }
            else {
                sphere.pos[1] -= sensitivity;
            }
        }
        if (im->key_down(SDL_SCANCODE_LSHIFT) && im->key_down(SDL_SCANCODE_Z)) {
            if (rel_motion.x > 0) {
                sphere.pos[2] += sensitivity;
            }
            else {
                sphere.pos[2] -= sensitivity;
            }
        }
    }


}

void Mouse_Moving_Basic::render()
{
    clear_buffer_all(0, colors::TAN, 1.0f, 0);

    glm::vec3 sphere_color {0.1f, 0.5f, 0.2f};

    if (!is_relative_mouse() && sphere_vs_line(arr_to_vec3(sphere.pos), sphere.radius, camera.get_pos(), mouse_world)) {
        sphere_color = {0.0f, 0.0f, 0.0f};
    }
    
    model_shader.use_shader();
    model_shader.set_mat4("u_vp", projection*camera.get_view());
    model_shader.set_vec3("u_model_color", sphere_color);
    model_shader.set_mat4("u_model", glm::translate(glm::mat4{1.0f}, arr_to_vec3(sphere.pos))*
                                     glm::scale(glm::mat4{1.0f}, {sphere.radius, sphere.radius, sphere.radius}));

    {
        const auto& meshes {uv_sphere.get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

}

void Mouse_Moving_Basic::imgui()
{
    ImGui::SliderFloat3("p", sphere.pos.data(), -10.0f, 10.0f);
    ImGui::SliderFloat("r", &sphere.radius, -10.0f, 10.0f);
}

void Mouse_Moving_Basic::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Mouse_Picking::Mouse_Picking()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     model_shader{"./assets/shaders/basic_model_vertex.glsl","./assets/shaders/basic_model_fragment.glsl"},
     picking_shader{"./assets/shaders/picking_vertex.glsl","./assets/shaders/picking_fragment.glsl"},
     //uv_sphere{"./assets/models/uv_sphere/uv_sphere.obj"},
     uv_sphere{"./assets/models/suzanne/suzanne.obj"},
     picking_texture{create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGB32UI)},
     picking_depth_texture{create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F)},
     sampler{create_sampler(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT)}
{

    glNamedFramebufferTexture(picking_fbo.id, GL_COLOR_ATTACHMENT0, picking_texture.id, 0);
    glNamedFramebufferTexture(picking_fbo.id, GL_DEPTH_ATTACHMENT,  picking_depth_texture.id, 0);
    {
        const auto status {glCheckNamedFramebufferStatus(picking_fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", picking_fbo.id, "incomplete\nstatus", status);
        }
    }

    sphere = {
        {0.0f, 0.0f, 0.0f},
        0.2f
    };
}

void Mouse_Picking::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
}

void Mouse_Picking::render()
{
    std::array modelebi {
        glm::translate(glm::mat4{1.0f}, arr_to_vec3(sphere.pos))*
        glm::scale(glm::mat4{1.0f}, {sphere.radius, sphere.radius, sphere.radius}),
        
        glm::translate(glm::mat4{1.0f}, arr_to_vec3(sphere.pos) + glm::vec3{-3.0f, 0.0f, 0.0f})*
        glm::scale(glm::mat4{1.0f}, {sphere.radius*4.0f, sphere.radius*4.0f, sphere.radius*4.0f}),

        glm::translate(glm::mat4{1.0f}, arr_to_vec3(sphere.pos) + glm::vec3{0.0f, 1.5f, 0.0f})*
        glm::scale(glm::mat4{1.0f}, {sphere.radius*2.0f, sphere.radius*2.0f, sphere.radius*2.0f})
    };
    std::array colors {
        glm::vec3{0.1f, 0.5f, 0.2f},
        glm::vec3{0.5f, 0.5f, 0.7f},
        glm::vec3{0.5f, 0.0f, 0.7f}
    };

    // picking phase
    {
        bind_frame_buffer(picking_fbo);
        clear_buffer_all(picking_fbo.id, colors::BLACK, 1.0f, 0);

        picking_shader.use_shader();

        for (std::size_t i{}; i<modelebi.size(); ++i) {
            picking_shader.set_mat4("u_vp", projection*camera.get_view());
            picking_shader.set_mat4("u_model", modelebi[i]);
            picking_shader.set_uint("u_object_id", i+1);
            const auto& meshes {uv_sphere.get_meshes()};
            u32 draw_call_id {};
            for (const auto& mesh:meshes) {
                bind_vertex_array(mesh.vao_id());
                picking_shader.set_uint("u_draw_id", draw_call_id);
                glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
            }
        }
    }

    {
        bind_frame_buffer_default();
        clear_buffer_all(0, colors::TAN, 1.0f, 0);
        
        model_shader.use_shader();
        model_shader.set_mat4("u_vp", projection*camera.get_view());

        i32 selected_object {-1};

        if (!is_relative_mouse()) {
            // get unique color value that we assigned to each drawn object under mouse pos.
            auto [mx, my] {Input_Manager::instance()->get_mouse()};
            auto dims {get_screen_dimensions()};
            my = dims.y - my;
            std::array<u32, 3> pixels {};
            if (mx >= 0 && mx < dims.x && my >= 0 && my < dims.y) {
                glGetTextureSubImage(picking_texture.id, 0, mx, my, 0, 1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, sizeof(u32)*3, pixels.data());
                if (pixels[0] != 0) {
                    selected_object = static_cast<i32>(pixels[0] - 1);
                }
            }
            else {
                peria::log("mouse pointer is outside");
            }
        }

        for (std::size_t i{}; i<modelebi.size(); ++i) {
            if (selected_object >= 0 && i == static_cast<std::size_t>(selected_object)) {
                model_shader.set_vec3("u_model_color", {0.1f, 0.0f, 0.1f});
            }
            else {
                model_shader.set_vec3("u_model_color", colors[i]);
            }
            model_shader.set_mat4("u_model", modelebi[i]);
            const auto& meshes {uv_sphere.get_meshes()};
            for (const auto& mesh:meshes) {
                bind_vertex_array(mesh.vao_id());
                glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
            }
        }

    }
}

void Mouse_Picking::imgui()
{
    ImGui::SliderFloat3("p", sphere.pos.data(), -10.0f, 10.0f);
    ImGui::SliderFloat("r", &sphere.radius, -10.0f, 10.0f);
}

void Mouse_Picking::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Many_Shadows::Many_Shadows()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     models{{Model{"./assets/models/uv_sphere/uv_sphere.obj"}, Model{"./assets/models/ico_sphere/ico_sphere.obj"}, Model{"./assets/models/monkey/suzanne.obj"}}},
     tree{"./assets/models/tree/tree.obj"},
     tree_texture{create_texture2d_from_image("./assets/models/tree/tree_texture.png", false)},
     shadow_sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER)},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     colors{{create_texture2d_colored(colors::CORAL), create_texture2d_colored(colors::GREENYELLOW), create_texture2d_colored(colors::TEAL), create_texture2d_colored(colors::KHAKI)}},
     light_shader{"./assets/shaders/lighting/lv.glsl", "./assets/shaders/lighting/lf.glsl"},
     shadow_shader{"./assets/shaders/shadow/sv.glsl", "./assets/shaders/shadow/sf.glsl"},
     model_shader{"./assets/shaders/basic_model_vertex.glsl", "./assets/shaders/basic_model_fragment.glsl"},
     line_shader{"./assets/shaders/line_vertex.glsl", "./assets/shaders/line_fragment.glsl"},
     dir_light_shadowmapper{2048*4, 2048*4}
{
    {
        buffer_upload_data<Vertex<Pos3D, Normal, TexCoord>>(cube_vbo, cube_data_with_norms, GL_STATIC_DRAW);
        vao_configure<Pos3D, Normal, TexCoord>(cube_vao.id, cube_vbo.id, 0);

        std::array<Vertex<Pos3D, Color4>, 2> line_data {{
            {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
            {{0.0f, 9.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
        }};
        buffer_upload_data<Vertex<Pos3D, Color4>>(line_vbo, line_data, GL_DYNAMIC_DRAW);
        vao_configure<Pos3D, Color4>(line_vao.id, line_vbo.id, 0);
    }
    
    //lights
    {
        dir_light = {
            {-1.0f, -0.5f, 0.2f},
            {0.01f, 0.02f, 0.3f},
            {1.0f, 1.0f, 0.5f},
            {1.0f, 1.0f, 0.8f},
            {5.0f, 10.0f, -3.0f}
        };

        spls = {
            Spot_Light{{-3.0f,  5.0f, 0.0f},
                       {0.0f, -1.0f, 0.0f},
                       {0.1f,  0.1f, 0.1f},
                       {0.2f,  0.8f, 1.0f},
                       {1.0f,  1.0f, 1.0f},
                        45.0f, 55.0f}
        };

        //spls = {
        //    Spot_Light{{0.0f,  5.0f, 0.0f},
        //               {0.0f, -1.0f, 0.0f},
        //               {0.1f,  0.1f, 0.1f},
        //               {1.0f,  0.6f, 1.0f},
        //               {1.0f,  1.0f, 1.0f},
        //                45.0f, 55.0f},
        //    Spot_Light{{-16.0f, 5.0f, 0.0f},
        //               { 0.0f, -1.0f, 0.0f},
        //               { 0.1f,  0.1f, 0.1f},
        //               { 0.1f,  0.9f, 1.0f},
        //               { 0.5f,  0.6f, 1.0f},
        //                 25.0f, 35.0f},
        //    Spot_Light{{16.0f, 5.0f, 0.0f},
        //               {0.0f, -1.0f, 0.0f},
        //               {0.1f,  0.1f, 0.1f},
        //               {0.7f,  0.0f, 0.2f},
        //               {0.5f,  1.0f, 0.0f},
        //                15.0f, 30.0f}
        //};

        pls = {
            Point_Light{
                {2.0f, 3.0f, 1.0f},
                {0.001f, 0.001f, 0.001f},
                {1.0f, 1.0f, 0.0f},
                {0.9f, 0.8f, 0.9f}
                // attn values = default
            }
        };

        // UBOs
        {
            Ubo_Lights lights {
                to_ubo_directional_light(dir_light),
                {
                    to_ubo_spot_light(spls[0])
                },
                {
                    to_ubo_point_light(pls[0])
                },
                {1,
                0}
            };
            
            // configuration
            {
                std::size_t offset {};
                buffer_allocate_data(lights_ubo, sizeof(Ubo_Lights), GL_DYNAMIC_DRAW);

                buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
                offset += sizeof(Ubo_Directional_Light);

                buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
                offset += sizeof(Ubo_Spot_Light)*32;

                buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
                offset += sizeof(Ubo_Point_Light)*32;

                buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());

                glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_ubo.id);
            }

            {
                std::size_t offset {};
                buffer_allocate_data(shadows_ubo, sizeof(glm::mat4)*33 + 16, GL_DYNAMIC_DRAW); // 16 = spl_count + 12byte

                buffer_upload_subdata(shadows_ubo, offset, sizeof(glm::mat4), &dir_light_shadowmapper.get_light_vp()[0]);
                offset += sizeof(glm::mat4)*33;

                i32 xxx {};
                buffer_upload_subdata(shadows_ubo, offset, sizeof(i32), &xxx);

                glBindBufferBase(GL_UNIFORM_BUFFER, 1, shadows_ubo.id);
            }
        }
    }

    dir_light_shadowmapper.set_light_projection(glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 40.0f));
    dir_light_shadowmapper.set_light_view(glm::lookAt(arr_to_vec3(dir_light.pos), arr_to_vec3(dir_light.pos)+arr_to_vec3(dir_light.direction), {0.0f, 1.0f, 0.0f}));

    spl_shadowmappers = {
        Shadowmapper{2048, 2048}
    };
    spl_shadowmappers[0].set_light_projection(glm::perspective(glm::radians(2.0f*spls[0].outer_angle), 1.0f, 0.1f, 20.0f));
    spl_shadowmappers[0].set_light_view(glm::lookAt(arr_to_vec3(spls[0].pos), arr_to_vec3(spls[0].pos)+arr_to_vec3(spls[0].direction), {0.0f, 1.0f, 0.0f}));

    light_shader.set_int("u_texture", 0);
    light_shader.set_int("u_shadowmap[0]", 1);
    light_shader.set_int("u_shadowmap[1]", 2);
}

void Many_Shadows::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }

    if (Input_Manager::instance()->key_pressed(SDL_SCANCODE_F2)) {
        render_global_axis = !render_global_axis;
    }
    
    {
        dir_light_shadowmapper.set_light_projection(glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 40.0f));
        dir_light_shadowmapper.set_light_view(glm::lookAt(arr_to_vec3(dir_light.pos), arr_to_vec3(dir_light.pos)+arr_to_vec3(dir_light.direction), {0.0f, 1.0f, 0.0f}));

        spl_shadowmappers[0].set_light_projection(glm::perspective(glm::radians(2.0f*spls[0].outer_angle), 1.0f, 0.1f, 20.0f));
        spl_shadowmappers[0].set_light_view(glm::lookAt(arr_to_vec3(spls[0].pos), arr_to_vec3(spls[0].pos)+arr_to_vec3(spls[0].direction), {0.0f, 1.0f, 0.0f}));
    }

    Ubo_Lights lights {
        to_ubo_directional_light(dir_light),
        {
            to_ubo_spot_light(spls[0])
        },
        {
            //to_ubo_point_light(pls[0])
        },
        {1, 0}
    };
    
    {
        std::size_t offset {};

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
        offset += sizeof(Ubo_Directional_Light);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
        offset += sizeof(Ubo_Spot_Light)*32;

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
        offset += sizeof(Ubo_Point_Light)*32;

        buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());
    }

    {
        std::size_t offset {};

        buffer_upload_subdata(shadows_ubo, offset, sizeof(glm::mat4), &dir_light_shadowmapper.get_light_vp()[0]);
        offset += sizeof(glm::mat4);
        buffer_upload_subdata(shadows_ubo, offset, sizeof(glm::mat4), &spl_shadowmappers[0].get_light_vp()[0]);
        offset += sizeof(glm::mat4)*32;

        i32 spl_count {static_cast<i32>(spl_shadowmappers.size())};
        buffer_upload_subdata(shadows_ubo, offset, sizeof(i32), &spl_count);

        glBindBufferBase(GL_UNIFORM_BUFFER, 1, shadows_ubo.id);
    }
}

void Many_Shadows::render()
{
    std::array modelebis_transformebi {
        glm::translate(glm::mat4{1.0f}, {1.5f, 0.5f, 1.0f})*
        glm::scale(glm::mat4{1.0f}, {0.5f, 0.5f, 0.5f}),
        
        glm::translate(glm::mat4{1.0f}, {1.5f, 0.35f, 2.5f})*
        glm::scale(glm::mat4{1.0f}, {0.8f, 0.8f, 0.8f}),

        glm::translate(glm::mat4{1.0f}, {-1.5f, 2.0f, 0.3f})*
        glm::scale(glm::mat4{1.0f}, {1.0f, 1.0f, 1.0f})
    };

    auto draw_scene = [&](const Shader& shader) {
        shader.use_shader();
        // big floor
        {
            bind_vertex_array(cube_vao);
            bind_texture_and_sampler(colors[0].id, sampler.id, 0);
            shader.set_mat4("u_model", glm::translate(glm::mat4{1.0f}, {0.0f, -0.5f, 0.0f})*
                                       glm::scale(glm::mat4{1.0f}, {100.0f, 0.2f, 100.0f}));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        { // models with solid color
            std::size_t MN {models.size()};
            for (std::size_t i{}; i<MN; ++i) {
                shader.set_mat4("u_model", modelebis_transformebi[i]);
                const auto& meshes {models[i].get_meshes()};
                for (const auto& mesh:meshes) {
                    bind_vertex_array(mesh.vao_id());
                    bind_texture_and_sampler(colors[i+1].id, sampler.id, 0); // i = 0 is floor color
                    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
                }
            }
        }

        {
            const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{7.0f, 0.0f, 0.0f})};
            shader.set_mat4("u_model", model);
            bind_texture_and_sampler(tree_texture.id, sampler.id, 0); // texture is loaded manually not during model loading
            const auto& meshes {tree.get_meshes()};
            for (const auto& mesh:meshes) {
                bind_vertex_array(mesh.vao_id());
                glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
            }
        }
    };

    bind_texture_and_sampler(dir_light_shadowmapper.get_shadowmap_id(), shadow_sampler.id, 1);
    dir_light_shadowmapper.execute(draw_scene, shadow_shader);

    bind_texture_and_sampler(spl_shadowmappers[0].get_shadowmap_id(), shadow_sampler.id, 2);
    spl_shadowmappers[0].execute(draw_scene, shadow_shader);

    // Light pass
    {
        bind_frame_buffer_default();
        auto screen_dims {get_screen_dimensions()};
        set_viewport(0, 0, screen_dims.x, screen_dims.y);
        clear_buffer_all(0, colors::GREY, 1.0f, 0);
        
        light_shader.set_mat4("u_vp", projection*camera.get_view());
        light_shader.set_vec3("u_camera_pos", camera.get_pos());

        draw_scene(light_shader);
    }
    
    {
        // temp shit right here kek.
        struct Line {
            glm::vec3 a;
            glm::vec3 b;
        };
        std::vector<Line> lines; // stores directions of lights

        bind_vertex_array(cube_vao);
        model_shader.use_shader();
        model_shader.set_mat4("u_vp", projection*camera.get_view());
        model_shader.set_vec3("u_model_color", {1.0f, 1.0f, 1.0f});
        for (const auto& spl:spls) {
            const auto model {glm::translate(glm::mat4{1.0f}, arr_to_vec3(spl.pos))*
                              glm::scale(glm::mat4{1.0f}, glm::vec3{0.1f, 0.1f, 0.1f})};
            model_shader.set_mat4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            Line line;
            line.a = arr_to_vec3(spl.pos);
            line.b = line.a + 2.0f*glm::normalize(arr_to_vec3(spl.direction));
            lines.push_back(line);
        }

        {
            const auto model {glm::translate(glm::mat4{1.0f}, arr_to_vec3(dir_light.pos))*
                              glm::scale(glm::mat4{1.0f}, glm::vec3{0.1f, 0.1f, 0.1f})};
            model_shader.set_mat4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // lines for orthographic light view frustum
        std::vector<Line> tmp_local_axis(3);
        {
            const auto P     {arr_to_vec3(dir_light.pos)};
            const auto dir   {glm::normalize(arr_to_vec3(dir_light.direction))}; // forward light dir
            const auto right {glm::normalize(glm::cross(dir, glm::vec3{0.0f, 1.0f, 0.0f}))};
            const auto up    {glm::cross(right, dir)};

            tmp_local_axis[0] = {P, P+1.0f*dir};
            tmp_local_axis[1] = {P, P+1.0f*up};
            tmp_local_axis[2] = {P, P+1.0f*right};

            const auto near_left_up    {P+0.1f*dir + 20.0f*up - 20.0f*right};
            const auto near_right_up   {near_left_up  + 40.0f*right};
            const auto near_left_down  {near_left_up  - 40.0f*up};
            const auto near_right_down {near_right_up - 40.0f*up};

            const auto far_left_up     {near_left_up + 20.0f*dir};
            const auto far_right_up    {far_left_up  + 40.0f*right};
            const auto far_left_down   {far_left_up  - 40.0f*up};
            const auto far_right_down  {far_right_up - 40.0f*up};

            lines.push_back({near_left_up,   near_right_up});
            lines.push_back({near_left_up,   near_left_down});
            lines.push_back({near_right_up,  near_right_down});
            lines.push_back({near_left_down, near_right_down});

            lines.push_back({far_left_up,   far_right_up});
            lines.push_back({far_left_up,   far_left_down});
            lines.push_back({far_right_up,  far_right_down});
            lines.push_back({far_left_down, far_right_down});

            lines.push_back({near_left_up,    far_left_up});
            lines.push_back({near_right_up,   far_right_up});
            lines.push_back({near_left_down,  far_left_down});
            lines.push_back({near_right_down, far_right_down});
        }

        { // tmp shit for debug
            bind_vertex_array(line_vao);
            line_shader.use_shader();
            line_shader.set_mat4("u_vp", projection*camera.get_view());
            glLineWidth(2);
            for (const auto& [a, b]:lines) {
                std::array<Vertex<Pos3D, Color4>, 2> line_data {{
                    {{a.x, a.y, a.z}, {1.0f, 0.7f, 1.0f, 1.0f}},
                    {{b.x, b.y, b.z}, {1.0f, 0.7f, 1.0f, 1.0f}}
                }};
                buffer_upload_subdata(line_vbo, 0, Vertex<Pos3D, Color4>::stride*2, line_data.data());
                glDrawArrays(GL_LINES, 0, 2);
            }

            {
                const auto& [fa, fb] {tmp_local_axis[0]};
                const auto& [ua, ub] {tmp_local_axis[1]};
                const auto& [ra, rb] {tmp_local_axis[2]};

                std::array<Vertex<Pos3D, Color4>, 2> line_data {{
                    {{fa.x, fa.y, fa.z}, {0.0f, 0.0f, 1.0f, 1.0f}},
                    {{fb.x, fb.y, fb.z}, {0.0f, 0.0f, 1.0f, 1.0f}}
                }};
                buffer_upload_subdata(line_vbo, 0, Vertex<Pos3D, Color4>::stride*2, line_data.data());
                glDrawArrays(GL_LINES, 0, 2);

                line_data[0] = {{ua.x, ua.y, ua.z}, {0.0f, 1.0f, 0.0f, 1.0f}};
                line_data[1] = {{ub.x, ub.y, ub.z}, {0.0f, 1.0f, 0.0f, 1.0f}};
                buffer_upload_subdata(line_vbo, 0, Vertex<Pos3D, Color4>::stride*2, line_data.data());
                glDrawArrays(GL_LINES, 0, 2);

                line_data[0] = {{ra.x, ra.y, ra.z}, {1.0f, 0.0f, 0.0f, 1.0f}};
                line_data[1] = {{rb.x, rb.y, rb.z}, {1.0f, 0.0f, 0.0f, 1.0f}};
                buffer_upload_subdata(line_vbo, 0, Vertex<Pos3D, Color4>::stride*2, line_data.data());
                glDrawArrays(GL_LINES, 0, 2);
            }

            if (render_global_axis) {
                Line X {{0.0f, 0.0f, 0.0f}, {1000.0f, 0.0f,    0.0f   }};
                Line Y {{0.0f, 0.0f, 0.0f}, {0.0f,    1000.0f, 0.0f   }};
                Line Z {{0.0f, 0.0f, 0.0f}, {0.0f,    0.0f,    1000.0f}};
                std::array line_colors {glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}};
                u32 i{};
                for (const auto& [a, b]:{X, Y, Z}) {
                    std::array<Vertex<Pos3D, Color4>, 2> line_data {{
                        {{a.x, a.y, a.z}, {line_colors[i].r, line_colors[i].g, line_colors[i].b, 1.0f}},
                        {{b.x, b.y, b.z}, {line_colors[i].r, line_colors[i].g, line_colors[i].b, 1.0f}}
                    }};
                    buffer_upload_subdata(line_vbo, 0, Vertex<Pos3D, Color4>::stride*2, line_data.data());
                    glDrawArrays(GL_LINES, 0, 2);
                    ++i;
                }
            }
            glLineWidth(1);
        }
    }

}

void Many_Shadows::imgui()
{
    ImGui::SliderFloat3("DirLight dir",          dir_light.direction.data(), -1.0f, 1.0f);
    ImGui::ColorPicker3("DirLight ambient",      dir_light.ambient.data());
    ImGui::ColorPicker3("DirLight diffuse",      dir_light.diffuse.data());
    ImGui::ColorPicker3("DirLight specular",     dir_light.specular.data());
    ImGui::SliderFloat3("DirLight pos",          dir_light.pos.data(), -50.0f, 50.0f);

    for (std::size_t i{}; i<spls.size(); ++i) {
        const auto name {"SL["+std::to_string(i)+"]"};
        auto& spl{spls[i]};
        ImGui::SliderFloat3((name+" dir").c_str(),          spl.direction.data(), -1.0f, 1.0f);
        ImGui::SliderFloat3((name+" pos").c_str(),          spl.pos.data(), -50.0f, 50.0f);
        ImGui::ColorPicker3((name+" ambient").c_str(),      spl.ambient.data());
        ImGui::ColorPicker3((name+" diffuse").c_str(),      spl.diffuse.data());
        ImGui::ColorPicker3((name+" specular").c_str(),     spl.specular.data());
        ImGui::SliderFloat( (name+" inner_angle").c_str(), &spl.inner_angle, 0.0f, 90.0f);
        ImGui::SliderFloat( (name+" outer_angle").c_str(), &spl.outer_angle, 0.0f, 90.0f);
    }

    //for (std::size_t i{}; i<pls.size(); ++i) {
    //    const auto name {"PL["+std::to_string(i)+"]"};
    //    auto& pl{pls[i]};
    //    ImGui::SliderFloat3((name+" pos").c_str(),          pl.pos.data(), -50.0f, 50.0f);
    //    ImGui::ColorPicker3((name+" ambient").c_str(),      pl.ambient.data());
    //    ImGui::ColorPicker3((name+" diffuse").c_str(),      pl.diffuse.data());
    //    ImGui::ColorPicker3((name+" specular").c_str(),     pl.specular.data());
    //    ImGui::SliderFloat( (name+" constant").c_str(),    &pl.constant, 0.0f, 1.0f);
    //    ImGui::SliderFloat( (name+" linear").c_str(),      &pl.linear, 0.0f, 1.0f);
    //    ImGui::SliderFloat( (name+" quadtratic").c_str(),  &pl.quadratic, 0.0f, 1.0f);
    //}
}

void Many_Shadows::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Normal_Mapping::Normal_Mapping()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     shader{"./assets/shaders/normal_vertex.glsl", "./assets/shaders/normal_fragment.glsl"},
     shader2{"./assets/shaders/normal_vertex2.glsl", "./assets/shaders/normal_fragment2.glsl"},
     model_shader{"./assets/shaders/basic_model_vertex.glsl","./assets/shaders/basic_model_fragment.glsl"},
     wall{create_texture2d_from_image("./assets/textures/brickwall.jpg")},
     wall_normal{create_texture2d_from_image("./assets/textures/brickwall_normal.jpg")},
     wall_normal2{create_texture2d_from_image("./assets/textures/brickwall_normal.jpg", false)},
     wall_sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)}
{
    {
        glm::vec3 p1 {-0.5f, -0.5f, 0.0f}; // lower left
        glm::vec3 p2 { 0.5f,  0.5f, 0.0f}; // upper right
        glm::vec3 p3 {-0.5f,  0.5f, 0.0f}; // upper left
        glm::vec3 p4 { 0.5f, -0.5f, 0.0f}; // lower right

        glm::vec2 uv1 {0.0f, 0.0f}; // lower left
        glm::vec2 uv2 {1.0f, 1.0f}; // upper right
        glm::vec2 uv3 {0.0f, 1.0f}; // upper left
        glm::vec2 uv4 {1.0f, 0.0f}; // lower right
        
        auto calc_tanget_bitangent = [](const glm::vec3& a, const glm::vec3& b, const glm::vec2& dt_uv1, const glm::vec2& dt_uv2) {
            const float f {1.0f / (dt_uv1.x*dt_uv2.y - dt_uv1.y*dt_uv2.x)};

            glm::vec3 tanget {
                f*(dt_uv2.y*a.x - dt_uv1.y*b.x),
                f*(dt_uv2.y*a.y - dt_uv1.y*b.y),
                f*(dt_uv2.y*a.z - dt_uv1.y*b.z)
            };

            glm::vec3 bitanget {
                f*(-dt_uv2.x*a.x + dt_uv1.x*b.x),
                f*(-dt_uv2.x*a.y + dt_uv1.x*b.y),
                f*(-dt_uv2.x*a.z + dt_uv1.x*b.z)
            };
            return std::make_pair(tanget, bitanget);
        };

        const auto [t1, bt1] {calc_tanget_bitangent(p2-p1, p3-p1, uv2-uv1, uv3-uv1)};
        const auto [t2, bt2] {calc_tanget_bitangent(p2-p1, p4-p1, uv2-uv1, uv4-uv1)};

        std::array<Vertex<Pos3D, Normal, TexCoord, Attr<float, 3>, Attr<float, 3>>, 6> plane_data {{
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {t1.x, t1.y, t1.z}, {bt1.x, bt1.y, bt1.z}},
            {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {t1.x, t1.y, t1.z}, {bt1.x, bt1.y, bt1.z}},
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {t1.x, t1.y, t1.z}, {bt1.x, bt1.y, bt1.z}},

            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {t2.x, t2.y, t2.z}, {bt2.x, bt2.y, bt2.z}},
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {t2.x, t2.y, t2.z}, {bt2.x, bt2.y, bt2.z}},
            {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {t2.x, t2.y, t2.z}, {bt2.x, bt2.y, bt2.z}},
        }};

        //std::array<Vertex<Pos3D, Normal, TexCoord>, 6> plane_data {{
        //    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        //    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        //    {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

        //    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        //    {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        //    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        //}};

        buffer_upload_data(plane_vbo, plane_data, GL_STATIC_DRAW);
        vao_configure<Pos3D, Normal, TexCoord, Attr<float, 3>, Attr<float, 3>>(plane_vao.id, plane_vbo.id, 0);
    }

    shader.set_int("u_wall", 0);
    shader.set_int("u_wall_normal", 1);

    shader2.set_int("u_wall", 0);
    shader2.set_int("u_wall_normal", 1);

    pl = {
        {0.5f, 1.0f, 2.0f},
        {0.01f, 0.01f, 0.01f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };
}

static bool tmp {true};
void Normal_Mapping::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }

    if (Input_Manager::instance()->key_pressed(SDL_SCANCODE_N)) {
        normal_mapping = !normal_mapping;
    }

    if (Input_Manager::instance()->key_pressed(SDL_SCANCODE_M)) {
        tmp = !tmp;
    }
}

void Normal_Mapping::render()
{
    bind_frame_buffer_default();
    clear_buffer_all(0, colors::GREY, 1.0f, 0);
    
    bind_vertex_array(plane_vao);
    bind_texture_and_sampler(wall.id, wall_sampler.id, 0);
    if (tmp) bind_texture_and_sampler(wall_normal.id, wall_sampler.id, 1);
    else     bind_texture_and_sampler(wall_normal2.id, wall_sampler.id, 1);

    auto model {glm::translate(glm::mat4{1.0f}, arr_to_vec3(pos))*
                      glm::rotate(glm::mat4{1.0f}, glm::radians(rotation_angles[0]), glm::vec3{1.0f, 0.0f, 0.0f})*
                      glm::rotate(glm::mat4{1.0f}, glm::radians(rotation_angles[1]), glm::vec3{0.0f, 1.0f, 0.0f})*
                      glm::rotate(glm::mat4{1.0f}, glm::radians(rotation_angles[2]), glm::vec3{0.0f, 0.0f, 1.0f})*
                      glm::scale(glm::mat4{1.0f}, arr_to_vec3(scale))};
    if (tbn) {
        shader2.set_mat4("u_vp", projection*camera.get_view());
        shader2.set_mat4("u_model", model);
        shader2.set_vec3("u_camera_pos", camera.get_pos());

        // used in vertex shader to transform into tangent space
        shader2.set_vec3("u_pl_pos", arr_to_vec3(pl.pos));

        {
            shader2.set_vec3("u_pl.pos", arr_to_vec3(pl.pos));
            shader2.set_vec3("u_pl.ambient",    arr_to_vec3(pl.ambient));
            shader2.set_vec3("u_pl.diffuse",    arr_to_vec3(pl.diffuse));
            shader2.set_vec3("u_pl.specular",   arr_to_vec3(pl.specular));
            shader2.set_float("u_pl.constant",  pl.constant);
            shader2.set_float("u_pl.linear",    pl.linear);
            shader2.set_float("u_pl.quadratic", pl.quadratic);
        }

        shader2.use_shader();
    }
    else {
        shader.set_mat4("u_vp", projection*camera.get_view());
        shader.set_mat4("u_model", model);
        shader.set_vec3("u_camera_pos", camera.get_pos());

        shader.set_int("u_normal_mapping", normal_mapping);

        {
            shader.set_vec3("u_pl.pos",        arr_to_vec3(pl.pos));
            shader.set_vec3("u_pl.ambient",    arr_to_vec3(pl.ambient));
            shader.set_vec3("u_pl.diffuse",    arr_to_vec3(pl.diffuse));
            shader.set_vec3("u_pl.specular",   arr_to_vec3(pl.specular));
            shader.set_float("u_pl.constant",  pl.constant);
            shader.set_float("u_pl.linear",    pl.linear);
            shader.set_float("u_pl.quadratic", pl.quadratic);
        }

        shader.use_shader();
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    model_shader.use_shader();
    model = glm::translate(glm::mat4{1.0f}, arr_to_vec3(pl.pos))*
            glm::scale(glm::mat4{1.0f}, glm::vec3{0.5f, 0.5f, 0.5f});
    model_shader.set_mat4("u_vp", projection*camera.get_view());
    model_shader.set_mat4("u_model", model);
    model_shader.set_vec3("u_model_color", arr_to_vec3(pl.diffuse));

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Normal_Mapping::imgui()
{
    ImGui::DragFloat3("pos",     pos.data(), 0.01f);
    ImGui::DragFloat3("scale",   scale.data(), 0.01f);
    ImGui::DragFloat3("rot_XYZ", rotation_angles.data(), 0.1f);

    ImGui::DragFloat3("pl pos",           pl.pos.data(), 0.01f);
    ImGui::ColorPicker3("pl ambient",     pl.ambient.data());
    ImGui::ColorPicker3("pl diffuse",     pl.diffuse.data());
    ImGui::ColorPicker3("pl specular",    pl.specular.data());
    ImGui::SliderFloat( "pl constant",   &pl.constant, 0.0f, 1.0f);
    ImGui::SliderFloat( "pl linear",     &pl.linear, 0.0f, 1.0f);
    ImGui::SliderFloat( "pl quadtratic", &pl.quadratic, 0.0f, 1.0f);
    ImGui::Checkbox("shader 1/2", &tbn);
    if (tbn) ImGui::Text("shader2");
    else     ImGui::Text("shader");
}

void Normal_Mapping::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Fun_With_Textures::Fun_With_Textures()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     shader{"./assets/shaders/texture_fun_vertex.glsl", "./assets/shaders/texture_fun_fragment.glsl"},
     texture{create_texture2d_from_image("./assets/textures/pikapika.png")},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)}
{
    /*
       1         4
       |         |
       |         |
       2_________3 
    */
    std::array<Vertex<Pos2D, TexCoord>, 4> data {{
        {{-0.5f,  0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
    }};
    buffer_upload_data(vbo, data, GL_STATIC_DRAW);

    std::array<u32, 6> indices {0,1,2, 0,2,3};
    buffer_upload_data(ibo, indices, GL_STATIC_DRAW);

    vao_configure<Pos2D, TexCoord>(vao.id, vbo.id, 0);
    vao_connect_ibo(vao, ibo);
}

void Fun_With_Textures::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
}

void Fun_With_Textures::render()
{
    bind_frame_buffer_default();
    clear_buffer_all(0, colors::GREY, 1.0f, 0);
    
    bind_vertex_array(vao);
    bind_texture_and_sampler(texture.id, sampler.id, 0);

    auto model {glm::mat4{1.0f}};

    shader.set_mat4("u_vp", projection*camera.get_view());
    shader.set_mat4("u_model", model);

    static float kek {0.0f};
    shader.set_float("u_dt", kek);
    kek += Timer::instance()->dt();

    shader.use_shader();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Fun_With_Textures::imgui()
{}

void Fun_With_Textures::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Color_Correction_And_Stuff::Color_Correction_And_Stuff()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     lighting_shader{"./assets/shaders/color_correction/lighting_vert.glsl", "./assets/shaders/color_correction/lighting_frag.glsl"},
     screen_shader{"./assets/shaders/color_correction/screen_vert.glsl", "./assets/shaders/color_correction/screen_frag.glsl"},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     hdr_color_texture{create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA16F)},
     hdr_depth_texture{create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F)},
     picking{{}, create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGB32UI),
            create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F), 
            {"./assets/shaders/picking_vertex.glsl", "./assets/shaders/picking_fragment.glsl"},
            create_sampler(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT)}
{
    glNamedFramebufferTexture(picking.fbo.id, GL_COLOR_ATTACHMENT0, picking.color_texture.id, 0);
    glNamedFramebufferTexture(picking.fbo.id, GL_DEPTH_ATTACHMENT,  picking.depth_texture.id, 0);
    {
        const auto status {glCheckNamedFramebufferStatus(picking.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", picking.fbo.id, "incomplete\nstatus", status);
        }
    }

    all_models.reserve(16);
    all_models.emplace_back("./assets/models/tree/tree.obj");
    all_models.emplace_back("./assets/models/backpack/backpack.obj");
    all_models.emplace_back("./assets/models/suzanne/suzanne.obj");
    all_models.emplace_back("./assets/models/icosphere/icosphere.obj");

    all_textures.reserve(16);
    all_textures.emplace_back(create_texture2d_from_image_srgb("./assets/textures/floor.png"));
    all_textures.emplace_back(create_texture2d_from_image_srgb("./assets/models/tree/tree_texture.png", false));
    all_textures.emplace_back(create_texture2d_from_image_srgb("./assets/models/backpack/diffuse.jpg", true));
    all_textures.emplace_back(create_texture2d_colored(peria::colors::TEAL));

    std::array<u32, 6> indices {0,1,2, 0,2,3};

    // plane
    {
        std::vector<Vertex<Pos3D, Normal, TexCoord>> plane_data {{
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 5.0f}},
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {5.0f, 0.0f}},
            {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {5.0f, 5.0f}},
        }};
        
        std::vector<Mesh> vm(1);
        vm.emplace_back(std::move(plane_data), std::vector<u32>{0,1,2, 0,2,3});
        all_models.emplace_back(std::move(vm));
    }

    // initial entities
    {
        entities.emplace_back(Entity{{{},                  {50.0f, 50.0f, 1.0f}, {-90.0f, 0.0f, 0.0f}}, {}, 3, 0});
        entities.emplace_back(Entity{{{ 0.0f, 0.1f, 0.0f}, {1.0f, 1.0f, 1.0f},   {}                  }, {}, 0, 1});
        entities.emplace_back(Entity{{{ 9.0f, 0.1f, 0.0f}, {1.0f, 1.0f, 1.0f},   {}                  }, {}, 0, 1});
        entities.emplace_back(Entity{{{-9.0f, 0.1f, 0.0f}, {1.0f, 1.0f, 1.0f},   {}                  }, {}, 0, 1});
        entities.emplace_back(Entity{{{ 3.0f, 0.3f, 0.0f}, {0.3f, 0.3f, 0.3f},   {}                  }, {}, 1, 2});
    }

    // screen quad
    {
        std::array<Vertex<Pos2D, TexCoord>, 4> screen_quad_data {{
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
        }};
        buffer_upload_data(screen_quad_vbo, screen_quad_data, GL_STATIC_DRAW);

        buffer_upload_data(screen_quad_ibo, indices, GL_STATIC_DRAW);

        vao_configure<Pos2D, TexCoord>(screen_quad_vao.id, screen_quad_vbo.id, 0);
        vao_connect_ibo(screen_quad_vao, screen_quad_ibo);
    }

    // Light setup
    {
        dir_light = {
            {-0.5f, -0.5f, 0.5f},
            {0.05f, 0.05f, 0.05f},
            {1.0f, 1.0f, 1.0f},
            {0.8f, 0.8f, 0.8f},
            {} // pos not used for this demo, we don't have shadows
        };

        pl = {
            {0.0f, 2.0f, -3.0f},
            {0.01f, 0.01f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}
        };

        Ubo_Lights lights {
            to_ubo_directional_light(dir_light),
            {},
            {to_ubo_point_light(pl)},
            {0,
            1}
        };
        
        // configuration
        {
            std::size_t offset {};
            buffer_allocate_data(lights_ubo, sizeof(Ubo_Lights), GL_DYNAMIC_DRAW);

            buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
            offset += sizeof(Ubo_Directional_Light);

            buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
            offset += sizeof(Ubo_Spot_Light)*32;

            buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
            offset += sizeof(Ubo_Point_Light)*32;

            buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());

            glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_ubo.id);
        }
    }

    // HDR fbo setup
    {
        glNamedFramebufferTexture(hdr_fbo.id, GL_DEPTH_ATTACHMENT, hdr_depth_texture.id, 0);
        glNamedFramebufferTexture(hdr_fbo.id, GL_COLOR_ATTACHMENT0, hdr_color_texture.id, 0);

        const auto status {glCheckNamedFramebufferStatus(hdr_fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr_fbo.id, "incomplete\nstatus", status);
        }
    }

    lighting_shader.set_int("u_diffuse_texture", 0);
    screen_shader.set_int("u_hdr_color_texture", 0);
}

void Color_Correction_And_Stuff::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
    if (!is_relative_mouse() && Input_Manager::instance()->mouse_pressed(Mouse_Button::LEFT)) {
        picking.clicked = true;
    }

    Ubo_Lights lights {
        to_ubo_directional_light(dir_light),
        {},
        {to_ubo_point_light(pl)},
        {0,
        1}
    };
    
    {
        std::size_t offset {};

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
        offset += sizeof(Ubo_Directional_Light);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
        offset += sizeof(Ubo_Spot_Light)*32;

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
        offset += sizeof(Ubo_Point_Light)*32;

        buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());
    }
}

void Color_Correction_And_Stuff::render()
{
    // picking phase
    {
        bind_frame_buffer(picking.fbo);
        clear_buffer_all(picking.fbo.id, colors::BLACK, 1.0f, 0);

        picking.shader.use_shader();
        picking.shader.set_mat4("u_vp", projection*camera.get_view());

        for (std::size_t i{}; i<entities.size(); ++i) {
            // drawing models
            if (entities[i].model < 0 || entities[i].model >= static_cast<i32>(all_models.size()) ||
                entities[i].texture_id < 0 || entities[i].texture_id >= static_cast<i32>(all_textures.size())) continue;

                picking.shader.set_mat4("u_model", get_model_mat(entities[i].transform));
                picking.shader.set_uint("u_object_id", i+1);
                const auto& meshes {all_models[entities[i].model].get_meshes()};
                u32 draw_call_id {};
                for (const auto& mesh:meshes) {
                    bind_vertex_array(mesh.vao_id());
                    picking.shader.set_uint("u_draw_id", draw_call_id);
                    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
                }
        }
    }

    if (!is_relative_mouse() && picking.clicked) {
        // get unique color value that we assigned to each drawn object under mouse pos.
        auto [mx, my] {Input_Manager::instance()->get_mouse()};
        my = get_screen_dimensions().y - my;
        std::array<u32, 3> pixels {};
        glGetTextureSubImage(picking.color_texture.id, 0, mx, my, 0, 1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, sizeof(u32)*3, pixels.data());
        if (pixels[0] != 0) {
            auto id {static_cast<i32>(pixels[0] - 1)};
            if (id == picking.selected) {
                picking.selected = -1; // deselect
            }
            else {
                picking.selected = id;
            }
        }
        picking.clicked = false;
    }

    bind_frame_buffer(hdr_fbo);
    clear_buffer_color(hdr_fbo.id, peria::colors::GREY, 0);
    clear_buffer_depth(hdr_fbo.id, 1.0f);

    lighting_shader.set_mat4("u_vp", projection*camera.get_view());
    lighting_shader.set_vec3("u_camera_pos", camera.get_pos());
    lighting_shader.use_shader();

    for (std::size_t i{}; i<entities.size(); ++i) {
        // drawing models
        if (entities[i].model < 0 || entities[i].model >= static_cast<i32>(all_models.size()) ||
            entities[i].texture_id < 0 || entities[i].texture_id >= static_cast<i32>(all_textures.size())) continue;
        bind_texture_and_sampler(all_textures[entities[i].texture_id].id, sampler.id);
        lighting_shader.set_mat4("u_model", get_model_mat(entities[i].transform));
        const auto& meshes {all_models[entities[i].model].get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }

    //{
    //    bind_texture_and_sampler(plane_texture.id, sampler.id, 0);
    //    bind_vertex_array(plane_vao);
    //    const auto model {glm::rotate(glm::mat4{1.0f}, glm::radians(-90.0f), glm::vec3{1.0f, 0.0f, 0.0f})*
    //                      glm::scale(glm::mat4{1.0f}, glm::vec3{50.0f, 50.0f, 1.0f})};
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    //}

    //{
    //    bind_texture_and_sampler(tree_texture.id, sampler.id, 0);
    //    for (const auto i:{0.0f, 9.0f, -9.0f}) {
    //        const auto& meshes {tree.get_meshes()};
    //        const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{i, 0.1f, 0.0f})*
    //                          glm::scale(glm::mat4{1.0f}, glm::vec3{1.0f, 1.0f, 1.0f})};
    //        for (const auto& mesh:meshes) {
    //            bind_vertex_array(mesh.vao_id());
    //            lighting_shader.set_mat4("u_model", model);
    //            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
    //        }
    //    }
    //}

    //{
    //    bind_texture_and_sampler(solid_color_texture.id, sampler.id, 0);
    //    const auto& meshes {monkey.get_meshes()};
    //    const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{3.0f, 0.3f, 0.0f})*
    //                      glm::scale(glm::mat4{1.0f}, glm::vec3{0.3f, 0.3f, 0.3f})};
    //    for (const auto& mesh:meshes) {
    //        bind_vertex_array(mesh.vao_id());
    //        lighting_shader.set_mat4("u_model", model);
    //        glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
    //    }
    //}

    bind_frame_buffer_default();
    clear_buffer_all(0, colors::WHITE, 1.0f, 0);

    screen_shader.set_int("u_gamma", gamma);
    screen_shader.set_int("u_hdr", hdr);
    screen_shader.set_float("u_exposure", exposure);

    bind_texture_and_sampler(hdr_color_texture.id, sampler.id, 0);
    bind_vertex_array(screen_quad_vao);
    screen_shader.use_shader();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Color_Correction_And_Stuff::imgui()
{
    constexpr i32 INF {1000};
    ImGui::SliderFloat3("DirLight dir",          dir_light.direction.data(), -1.0f, 1.0f);
    //ImGui::ColorPicker3("DirLight ambient",      dir_light.ambient.data());
    //ImGui::ColorPicker3("DirLight diffuse",      dir_light.diffuse.data());
    //ImGui::ColorPicker3("DirLight specular",     dir_light.specular.data());

    ImGui::SliderFloat3("DirLight ambient",  dir_light.ambient.data(), 0.0f, INF);
    ImGui::SliderFloat3("DirLight diffuse",  dir_light.diffuse.data(), 0.0f, INF);
    ImGui::SliderFloat3("DirLight specular", dir_light.specular.data(), 0.0f, INF);

    ImGui::Checkbox("gamma correction", &gamma);
    ImGui::Checkbox("hdr", &hdr);
    ImGui::SliderFloat("exposure", &exposure, 0.0f, 50.0f);
    //ImGui::ColorPicker3("PL ambient",  pl.ambient.data());
    //ImGui::ColorPicker3("PL diffuse",  pl.diffuse.data());
    //ImGui::ColorPicker3("PL specular", pl.specular.data());
    
    ImGui::SliderFloat3("PL pos",      pl.pos.data(), -10.0f, 10.0f);
    ImGui::SliderFloat3("PL ambient",  pl.ambient.data(), 0.0f, INF);
    ImGui::SliderFloat3("PL diffuse",  pl.diffuse.data(), 0.0f, INF);
    ImGui::SliderFloat3("PL specular", pl.specular.data(), 0.0f, INF);

    if (ImGui::Button("add entity")) {
        entities.push_back({{}, {}, 0});
        picking.selected = static_cast<i32>(entities.size() - 1);
    }
    if (picking.selected != -1) {
        auto& et {entities[picking.selected].transform};
        ImGui::Text("Selected Entity");
        ImGui::SliderFloat3("pos",      et.pos.data(), -10.0f, 10.0f);
        ImGui::SliderFloat3("rotation", et.angle.data(), -360.0f, 360.0f);
        ImGui::SliderFloat3("scale",    et.scale.data(), -10.0f, 10);
        ImGui::ColorPicker3("color",    entities[picking.selected].color.data());
        ImGui::InputInt("model index",  &entities[picking.selected].model);
        ImGui::InputInt("texture index", &entities[picking.selected].texture_id);
    }

    //for (std::size_t i{}; i<spls.size(); ++i) {
    //    const auto name {"SL["+std::to_string(i)+"]"};
    //    auto& spl{spls[i]};
    //    ImGui::SliderFloat3((name+" dir").c_str(),          spl.direction.data(), -1.0f, 1.0f);
    //    ImGui::SliderFloat3((name+" pos").c_str(),          spl.pos.data(), -50.0f, 50.0f);
    //    ImGui::ColorPicker3((name+" ambient").c_str(),      spl.ambient.data());
    //    ImGui::ColorPicker3((name+" diffuse").c_str(),      spl.diffuse.data());
    //    ImGui::ColorPicker3((name+" specular").c_str(),     spl.specular.data());
    //    ImGui::SliderFloat( (name+" inner_angle").c_str(), &spl.inner_angle, 0.0f, 90.0f);
    //    ImGui::SliderFloat( (name+" outer_angle").c_str(), &spl.outer_angle, 0.0f, 90.0f);
    //}

    //for (std::size_t i{}; i<pls.size(); ++i) {
    //    const auto name {"PL["+std::to_string(i)+"]"};
    //    auto& pl{pls[i]};
    //    ImGui::SliderFloat3((name+" pos").c_str(),          pl.pos.data(), -50.0f, 50.0f);
    //    ImGui::ColorPicker3((name+" ambient").c_str(),      pl.ambient.data());
    //    ImGui::ColorPicker3((name+" diffuse").c_str(),      pl.diffuse.data());
    //    ImGui::ColorPicker3((name+" specular").c_str(),     pl.specular.data());
    //    ImGui::SliderFloat( (name+" constant").c_str(),    &pl.constant, 0.0f, 1.0f);
    //    ImGui::SliderFloat( (name+" linear").c_str(),      &pl.linear, 0.0f, 1.0f);
    //    ImGui::SliderFloat( (name+" quadtratic").c_str(),  &pl.quadratic, 0.0f, 1.0f);
    //}
}

void Color_Correction_And_Stuff::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);

    // picking texture must be recreated
    // TODO: I believe this can actually be improved.
    // if I keep track of resized vs resizing.
    // only update when resizing is done.
    {
        peria::log("recreating picker texture after screen resize");
        picking.depth_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_DEPTH_COMPONENT32F);
        picking.color_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGB32UI);
        glNamedFramebufferTexture(picking.fbo.id, GL_COLOR_ATTACHMENT0, picking.color_texture.id, 0);
        glNamedFramebufferTexture(picking.fbo.id, GL_DEPTH_ATTACHMENT,  picking.depth_texture.id, 0);
        const auto status {glCheckNamedFramebufferStatus(picking.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", picking.fbo.id, "incomplete\nstatus", status);
        }
    }

    {
        hdr_depth_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_DEPTH_COMPONENT32F);
        hdr_color_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA16F);

        glNamedFramebufferTexture(hdr_fbo.id, GL_DEPTH_ATTACHMENT, hdr_depth_texture.id, 0);
        glNamedFramebufferTexture(hdr_fbo.id, GL_COLOR_ATTACHMENT0, hdr_color_texture.id, 0);

        const auto status {glCheckNamedFramebufferStatus(hdr_fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr_fbo.id, "incomplete\nstatus", status);
        }
    }
}

Gizmos::Gizmos()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     picking{{}, create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGB32UI),
            create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F), 
            {"./assets/shaders/picking_vertex.glsl", "./assets/shaders/picking_fragment.glsl"},
            create_sampler(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     light_shader{"./assets/shaders/basic_model_vertex.glsl", "./assets/shaders/basic_model_fragment.glsl"}
{
    glNamedFramebufferTexture(picking.fbo.id, GL_COLOR_ATTACHMENT0, picking.color_texture.id, 0);
    glNamedFramebufferTexture(picking.fbo.id, GL_DEPTH_ATTACHMENT,  picking.depth_texture.id, 0);
    {
        const auto status {glCheckNamedFramebufferStatus(picking.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", picking.fbo.id, "incomplete\nstatus", status);
        }
    }

    all_models.reserve(16);
    all_models.emplace_back("./assets/models/monkey/suzanne.obj");
    all_models.emplace_back("./assets/models/uv_sphere/uv_sphere.obj");
    all_models.emplace_back("./assets/models/ico_sphere/ico_sphere.obj");

    entities.reserve(2048);
}

static bool gizmo_b {false};
void Gizmos::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
    if (!is_relative_mouse() && Input_Manager::instance()->mouse_pressed(Mouse_Button::LEFT)) {
        gizmo_b = true;
    }
}

void Gizmos::render()
{
    // picking phase
    {
        bind_frame_buffer(picking.fbo);
        clear_buffer_all(picking.fbo.id, colors::BLACK, 1.0f, 0);

        picking.shader.use_shader();

        for (std::size_t i{}; i<entities.size(); ++i) {
            if (entities[i].model < 0 || entities[i].model >= static_cast<i32>(all_models.size())) continue;
            picking.shader.set_mat4("u_vp", projection*camera.get_view());
            picking.shader.set_mat4("u_model", get_model_mat(entities[i].transform));
            picking.shader.set_uint("u_object_id", i+1);
            const auto& meshes {all_models[entities[i].model].get_meshes()};
            u32 draw_call_id {};
            for (const auto& mesh:meshes) {
                bind_vertex_array(mesh.vao_id());
                picking.shader.set_uint("u_draw_id", draw_call_id);
                glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
            }
        }
    }
    
    bind_frame_buffer_default();
    clear_buffer_all(0, colors::GREY, 1.0f, 0);

    if (!is_relative_mouse() && gizmo_b) {
        peria::log("Pressed");
        // get unique color value that we assigned to each drawn object under mouse pos.
        auto [mx, my] {Input_Manager::instance()->get_mouse()};
        my = get_screen_dimensions().y - my;
        std::array<u32, 3> pixels {};
        glGetTextureSubImage(picking.color_texture.id, 0, mx, my, 0, 1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, sizeof(u32)*3, pixels.data());
        if (pixels[0] != 0) {
            auto id {static_cast<i32>(pixels[0] - 1)};
            if (id == picking.selected) {
                picking.selected = -1; // deselect
            }
            else {
                picking.selected = id;
            }
        }
        gizmo_b = false;
    }

    light_shader.use_shader();
    light_shader.set_mat4("u_vp", projection*camera.get_view());
    for (std::size_t i{}; i<entities.size(); ++i) {
        if (entities[i].model < 0 || entities[i].model >= static_cast<i32>(all_models.size())) continue;
        light_shader.set_mat4("u_model", get_model_mat(entities[i].transform));
        light_shader.set_vec3("u_model_color", arr_to_vec3(entities[i].color));
        const auto& meshes {all_models[entities[i].model].get_meshes()};
        for (const auto& mesh:meshes) {
            bind_vertex_array(mesh.vao_id());
            glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

void Gizmos::imgui()
{
    if (ImGui::Button("add entity")) {
        entities.push_back({{}, {}, 0});
        picking.selected = static_cast<i32>(entities.size() - 1);
    }
    if (picking.selected != -1) {
        auto& et {entities[picking.selected].transform};
        ImGui::Text("Selected Entity");
        ImGui::SliderFloat3("pos",      et.pos.data(), -10.0f, 10.0f);
        ImGui::SliderFloat3("rotation", et.angle.data(), -360.0f, 360.0f);
        ImGui::SliderFloat3("scale",    et.scale.data(), -10.0f, 10);
        ImGui::ColorPicker3("color",    entities[picking.selected].color.data());
        ImGui::InputInt("model index", &entities[picking.selected].model);
    }
}

void Gizmos::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Multi_Sampled::Multi_Sampled()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     shader{"./assets/shaders/multisampled/default_vert.glsl", "./assets/shaders/multisampled/default_frag.glsl"},
     quad_shader{"./assets/shaders/multisampled/a.glsl", "./assets/shaders/multisampled/b.glsl"},
     sampler{create_sampler(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     info{{}, {create_texture2d(w, h, GL_RGBA8)}, {}},
     info_ms{{}, {create_texture2d_multisample(w, h, GL_RGBA8, 8)}, {}, {create_texture2d(w, h, GL_RGBA8)}, {}}
{
    {
        std::array<Vertex<Pos3D, TexCoord>, 4> quad_data {{
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}},
            {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}},
            {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}
        }};
        std::array<u32, 6> indices {0,1,2, 0,2,3};
        buffer_upload_data(quad_vbo, quad_data, GL_STATIC_DRAW);
        buffer_upload_data(quad_ibo, indices, GL_STATIC_DRAW);
        vao_configure<Pos3D, TexCoord>(quad_vao.id, quad_vbo.id, 0);
        vao_connect_ibo(quad_vao, quad_ibo);
    }

    {
        std::array<Vertex<Pos3D>, 4> triangle_data {{
            {{-0.5f, -0.5f, 0.0f}},
            {{ 0.0f,  0.5f, 0.0f}},
            {{ 0.5f, -0.5f, 0.0f}},
        }};
        buffer_upload_data(tri_vbo, triangle_data, GL_STATIC_DRAW);
        vao_configure<Pos3D>(tri_vao.id, tri_vbo.id, 0);
    }

    glNamedFramebufferTexture(info.fbo.id, GL_COLOR_ATTACHMENT0, info.texture.id, 0);
    if (auto status {glCheckNamedFramebufferStatus(info.fbo.id, GL_FRAMEBUFFER)};
        status != GL_FRAMEBUFFER_COMPLETE) {
        peria::log("Framebuffer incomplete");
    }

    glNamedFramebufferTexture(info_ms.fbo.id, GL_COLOR_ATTACHMENT0, info_ms.texture.id, 0);
    if (auto status {glCheckNamedFramebufferStatus(info_ms.fbo.id, GL_FRAMEBUFFER)};
        status != GL_FRAMEBUFFER_COMPLETE) {
        peria::log("MS Framebuffer incomplete");
    }

    glNamedFramebufferTexture(info_ms.fbo_middle.id, GL_COLOR_ATTACHMENT0, info_ms.texture_middle.id, 0);
    if (auto status {glCheckNamedFramebufferStatus(info_ms.fbo.id, GL_FRAMEBUFFER)};
        status != GL_FRAMEBUFFER_COMPLETE) {
        peria::log("MS Framebuffer incomplete");
    }

    info.proj = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
    info_ms.proj = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
}

void Multi_Sampled::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
}

void Multi_Sampled::render()
{
    {
        // regular triangle
        glViewport(0, 0, w, h);
        bind_frame_buffer(info.fbo.id);
        clear_buffer_color(info.fbo.id, colors::WHITE, 0);
        bind_texture_and_sampler(info.texture.id, sampler.id, 0);
        shader.use_shader();

        const auto model {glm::translate(glm::mat4{1.0f}, glm::vec3{w*0.5f, h*0.5f, 0.0f})*
                          //glm::scale(glm::mat4{1.0f}, glm::vec3{100.0f, 100.0f, 100.0f})};
                          glm::scale(glm::mat4{1.0f}, glm::vec3{10.0f, 10.0f, 10.0f})};

        shader.set_mat4("u_mvp", info.proj*model);
        shader.set_vec3("u_color", glm::vec3{1.0f, 0.0f, 0.3f});
        bind_vertex_array(tri_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // multisampled triangle
        bind_frame_buffer(info_ms.fbo);
        clear_buffer_color(info_ms.fbo.id, colors::WHITE, 0);
        bind_texture_and_sampler(info_ms.texture.id, sampler.id, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, info_ms.fbo.id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, info_ms.fbo_middle.id);
        glBlitNamedFramebuffer(info_ms.fbo.id, info_ms.fbo_middle.id, 0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    // main pass
    {
        const auto screen_dims { peria::get_screen_dimensions() };
        glViewport(0, 0, screen_dims.x, screen_dims.y);
        bind_frame_buffer_default();
        clear_buffer_all(0, colors::GREY, 1.0f, 0);

        quad_shader.use_shader();

        bind_vertex_array(quad_vao);

        const auto model1 {glm::scale(glm::mat4{1.0f}, glm::vec3{10.0f, 10.0f, 1.0f})};
        quad_shader.set_mat4("u_mvp", projection*camera.get_view()*model1);
        bind_texture_and_sampler(info.texture.id, sampler.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        const auto model2 {glm::translate(glm::mat4{1.0f}, glm::vec3{10.2f, 0.0f, 0.0f})*
                           glm::scale(glm::mat4{1.0f}, glm::vec3{10.0f, 10.0f, 1.0f})};
        quad_shader.set_mat4("u_mvp", projection*camera.get_view()*model2);
        bind_texture_and_sampler(info_ms.texture_middle.id, sampler.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Multi_Sampled::imgui()
{
    ImGui::Text("TEST TEST TEST");
}

void Multi_Sampled::recalculate_projection()
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

Aspect_Ratio::Aspect_Ratio()
    :camera{},
     shader{"./assets/shaders/multisampled/a.glsl", "./assets/shaders/multisampled/b.glsl"},
     texture_chiti{create_texture2d_from_image("./assets/textures/chitunia.png", tex_width, tex_height)},
     texture_pika{create_texture2d_from_image("./assets/textures/pikapika.png", tex_width2, tex_height2)},
     fbo_color_texture{create_texture2d(fbo_width, fbo_height, GL_RGBA8)},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)}
{
    {
        std::array<Vertex<Pos3D, TexCoord>, 4> quad_data {{
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}},
            {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}},
            {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}
        }};
        std::array<u32, 6> indices {0,1,2, 0,2,3};
        buffer_upload_data(quad_vbo, quad_data, GL_STATIC_DRAW);
        buffer_upload_data(quad_ibo, indices, GL_STATIC_DRAW);
        vao_configure<Pos3D, TexCoord>(quad_vao.id, quad_vbo.id, 0);
        vao_connect_ibo(quad_vao, quad_ibo);
    }

    glNamedFramebufferTexture(fbo.id, GL_COLOR_ATTACHMENT0, fbo_color_texture.id, 0);
    if (const auto status {glCheckNamedFramebufferStatus(fbo.id, GL_FRAMEBUFFER)};
        status != GL_FRAMEBUFFER_COMPLETE) {
        peria::log("INCOMPLETE FRAMEBUFFER");
    }

    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y);
    fbo_projection = glm::ortho(0.0f, static_cast<float>(fbo_width), 0.0f, static_cast<float>(fbo_height));
    calculate_dimensions();
}

void Aspect_Ratio::update()
{
}

void Aspect_Ratio::render()
{
    {
        glViewport(0, 0, fbo_width, fbo_height);
        bind_frame_buffer(fbo);
        clear_buffer_color(fbo.id, colors::ANTIQUEWHITE, 0);

        bind_vertex_array(quad_vao);

        shader.use_shader();

        const Transform trans1 {{400.0f, 400.0f, 0.0f}, {static_cast<float>(tex_width), static_cast<float>(tex_height), 1.0f}};
        const Transform trans2 {{900.0f, 400.0f, 0.0f}, {static_cast<float>(tex_width2), static_cast<float>(tex_height2), 1.0f}};

        shader.set_mat4("u_mvp", fbo_projection*get_model_mat(trans1));
        bind_texture_and_sampler(texture_chiti.id, sampler.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        shader.set_mat4("u_mvp", fbo_projection*get_model_mat(trans2));
        bind_texture_and_sampler(texture_pika.id, sampler.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // final pass
    {
        const auto sdims {get_screen_dimensions()};
        glViewport(0, 0, sdims.x, sdims.y);
        bind_frame_buffer_default();
        clear_buffer_all(0, colors::BLACK, 1.0f, 0);

        bind_vertex_array(quad_vao);

        shader.use_shader();

        const Transform screen_model {{sdims.x*0.5f, sdims.y*0.5f, 0.0f}, {static_cast<float>(final_width), static_cast<float>(final_height), 1.0f}};
        shader.set_mat4("u_mvp", projection*get_model_mat(screen_model));
        bind_texture_and_sampler(fbo_color_texture.id, sampler.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

}

void Aspect_Ratio::imgui()
{
}

void Aspect_Ratio::recalculate_projection()
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y);
    calculate_dimensions();
}

void Aspect_Ratio::calculate_dimensions()
{
    const auto screen_dims { peria::get_screen_dimensions() };

    // if (sw / sh) == (w / h) return
    // same aspect ratio so stretch to screen dimensions
    if (screen_dims.x*fbo_height == screen_dims.y*fbo_width) {
        final_width = screen_dims.x;
        final_height = screen_dims.y;
        return;
    }

    const float aspect_ratio {static_cast<float>(fbo_width)/static_cast<float>(fbo_height)};
    
    float new_width {screen_dims.x};
    float new_height {new_width/(aspect_ratio)};
    
    if (new_height > screen_dims.y) {
        new_height = screen_dims.y;
        new_width = static_cast<float>(new_height)*(aspect_ratio);
    }

    final_width = static_cast<i32>(new_width);
    final_height = static_cast<i32>(new_height);
}

Bloom::Bloom()
    :camera{{0.0f, 3.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     light_shader{"./assets/shaders/color_correction/lighting_vert.glsl", "./assets/shaders/color_correction/lighting_frag.glsl"},
     screen_shader{"./assets/shaders/color_correction/screen_vert.glsl", "./assets/shaders/color_correction/screen_frag.glsl"},
     blur_shader{"./assets/shaders/color_correction/screen_vert.glsl", "./assets/shaders/color_correction/blur_frag.glsl"},
     point_light_bloom_shader{"./assets/shaders/color_correction/lighting_vert.glsl", "./assets/shaders/color_correction/point_light_bloom_frag.glsl"},
     sampler_repeat{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     hdr{{}, 
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F),
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F),
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F)
     },
     bloom{
         {},
         {
             create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F),
             create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F)
         },
         {create_sampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)},
     },
     solid_color1{create_texture2d_colored(colors::WHITE)},
     solid_color2{create_texture2d_colored(colors::Color{0.5f, 0.7f, 0.7f, 1.0f})},
     solid_color3{create_texture2d_colored(colors::Color{1.0f, 0.0f, 0.0f, 1.0f})},
     floor_texture{create_texture2d_from_image_srgb("./assets/textures/floor.png")},
     brick_texture{create_texture2d_from_image_srgb("./assets/textures/brickwall.jpg")},
     crate_texture{create_texture2d_from_image_srgb("./assets/textures/wooden_container.png")}
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);
    light_shader.set_int("u_diffuse_texture", 0);
    screen_shader.set_int("u_hdr_color_texture", 0);
    screen_shader.set_int("u_bloom_texture", 1);
    blur_shader.set_int("u_texture", 0);

    // hdr
    {
        glNamedFramebufferTexture(hdr.fbo.id, GL_DEPTH_ATTACHMENT, hdr.depth_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT0, hdr.color_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT1, hdr.brightness_texture.id, 0);
        std::array<u32, 2> color_attachments {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glNamedFramebufferDrawBuffers(hdr.fbo.id, 2, color_attachments.data());

        const auto status {glCheckNamedFramebufferStatus(hdr.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr.fbo.id, "incomplete\nstatus", status);
            std::exit(0);
        }
    }

    // bloom
    {
        for (int i{}; i<2; ++i) {
            glNamedFramebufferTexture(bloom.fbos[i].id, GL_COLOR_ATTACHMENT0, bloom.textures[i].id, 0);
            const auto status {glCheckNamedFramebufferStatus(bloom.fbos[i].id, GL_FRAMEBUFFER)};
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                peria::log("FrameBuffer with id", bloom.fbos[i].id, "incomplete\nstatus", status);
                std::exit(0);
            }
        }
    }

    // cube
    {
        buffer_upload_data(cube.vbo, cube_data_with_norms, GL_STATIC_DRAW);
        vao_configure<Pos3D, Normal, TexCoord>(cube.vao.id, cube.vbo.id, 0);
    }

    // screen
    {
        std::array<Vertex<Pos2D, TexCoord>, 4> screen_quad_data {{
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
        }};
        buffer_upload_data(screen_quad.vbo, screen_quad_data, GL_STATIC_DRAW);
        std::array<u32, 6> indices {0,1,2, 0,2,3};
        buffer_upload_data(screen_quad.ibo, indices, GL_STATIC_DRAW);

        vao_configure<Pos2D, TexCoord>(screen_quad.vao.id, screen_quad.vbo.id, 0);
        vao_connect_ibo(screen_quad.vao, screen_quad.ibo);
    }

    // lights
    {
        dir_light = {
            //.direction {-0.5f, -0.5f, -0.5f},
            //.ambient   {0.05f, 0.05f, 0.05f},
            //.diffuse   {10.7f, 10.8f, 10.9f},
            //.specular  {0.8f, 0.8f, 0.8f},
            .direction {},
            .ambient   {},
            .diffuse   {},
            .specular  {},
            .pos       {} // pos not used for this demo, we don't have shadows
        };

        point_lights = {
            Point_Light {
                .pos      {0.0f, 20.0f, 0.0f},
                .ambient  {0.05f, 0.05f, 0.05f},
                .diffuse  {1000.0f, 1000.0f, 1000.0f},
                .specular {100.0f, 100.0f, 100.0f}
            },
            Point_Light {
                .pos      {-40.0f, 25.0f, -30.0f},
                .ambient  {0.05f, 0.05f, 0.05f},
                .diffuse  {2.5f, 2.7f, 4.7f},
                .specular {0.5f, 0.5f, 0.5f}
            },
            Point_Light {
                .pos      {10.0f, 15.0f, -10.0f},
                .ambient  {0.09f, 0.1f, 0.03f},
                .diffuse  {4.0f, 0.0f, 0.0f},
                .specular {0.9f, 0.1f, 0.1f}
            },
        };

        Ubo_Lights lights {
            to_ubo_directional_light(dir_light),
            {},
            {
                to_ubo_point_light(point_lights[0]),
                to_ubo_point_light(point_lights[1]),
                to_ubo_point_light(point_lights[2])
            },
            {0, 3}
        };

        // configuration
        {
            std::size_t offset {};
            buffer_allocate_data(lights_ubo, sizeof(Ubo_Lights), GL_DYNAMIC_DRAW);

            buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
            offset += sizeof(Ubo_Directional_Light);

            buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
            offset += sizeof(Ubo_Spot_Light)*32;

            buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
            offset += sizeof(Ubo_Point_Light)*32;

            buffer_upload_subdata(lights_ubo, offset, sizeof(i32), &lights.counts[0]);
            offset += sizeof(i32);

            buffer_upload_subdata(lights_ubo, offset, sizeof(i32), &lights.counts[1]);

            glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_ubo.id);
        }
    }
}

void Bloom::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }

    Ubo_Lights lights {
        to_ubo_directional_light(dir_light),
        {},
        {
            to_ubo_point_light(point_lights[0]),
            to_ubo_point_light(point_lights[1]),
            to_ubo_point_light(point_lights[2])
        },
        {0, 3}
    };
    
    {
        std::size_t offset {};

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
        offset += sizeof(Ubo_Directional_Light);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
        offset += sizeof(Ubo_Spot_Light)*MAX_SPLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
        offset += sizeof(Ubo_Point_Light)*MAX_PLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());
    }
}

void Bloom::render()
{
    const auto screen_dims {get_screen_dimensions()};
    glViewport(0, 0, screen_dims.x, screen_dims.y); // both fbos have screen_dims

    // lighting pass
    {
        bind_frame_buffer(hdr.fbo);
        clear_buffer_color(hdr.fbo.id, colors::GREY, 0);
        // need to clear brightness color channel to black, otherwise background will also get blurred
        clear_buffer_color(hdr.fbo.id, colors::BLACK, 1); // brightness values go in channel 1
        clear_buffer_depth(hdr.fbo.id, 1.0f);

        light_shader.use_shader();
        light_shader.set_int("u_atn_quadratic", hdr.atn_quad);
        light_shader.set_int("u_do_bloom", bloom.do_bloom);
        light_shader.set_float("u_point_light_intensity", hdr.intensity);
        light_shader.set_mat4("u_vp", projection*camera.get_view());
        light_shader.set_vec3("u_camera_pos", camera.get_pos());

        bind_vertex_array(cube.vao);

        {
            constexpr float d {100.0f};
            constexpr float p {50.0f};
            // cube as floor
            Transform trans {{}, {d, 0.3f, d}, {}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            bind_texture_and_sampler(floor_texture.id, sampler_repeat.id, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // cube as walls
            trans = {{-p, p, 0.0f}, {d, 0.3f, d}, {90.0f, 0.0f, 90.0f}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            bind_texture_and_sampler(brick_texture.id, sampler_repeat.id, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            trans = {{0.0f, p, -p}, {d, 0.3f, d}, {90.0f, 0.0f, 0.0f}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // crates
        {
            Transform trans {{-30.0f, 4.0f, -40.0f}, {7.0f, 7.0f, 7.0f}, {}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            bind_texture_and_sampler(crate_texture.id, sampler_repeat.id, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            trans = {{-20.0f, 11.0f, 0.0f}, {7.0f, 20.0f, 7.0f}, {}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            trans = {{30.0f, 4.0f, -30.0f}, {20.0f, 7.0f, 7.0f}, {}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            trans = {{0.0f, 5.0f, 10.0f}, {8.0f, 8.0f, 8.0f}, {}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //std::array<Texture2D*, 3> colors {&solid_color1, &solid_color2, &solid_color3};
        point_light_bloom_shader.use_shader();
        point_light_bloom_shader.set_mat4("u_vp", projection*camera.get_view());
        for (std::size_t i{}; i<point_lights.size(); ++i) {
            const auto pl {point_lights[i]};
            const Transform trans {pl.pos, {3.0f, 3.0f, 3.0f}, {}};
            point_light_bloom_shader.set_mat4("u_model", get_model_mat(trans));
            point_light_bloom_shader.set_vec3("u_light_color", get_vec3(pl.diffuse));
            //bind_texture_and_sampler(colors[i]->id, sampler_repeat.id, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // bloom blur
    if (bloom.do_bloom) {
        bind_vertex_array(screen_quad.vao);
        blur_shader.use_shader();
        bloom.horizontal = true;
        bool first_iter {true};
        for (int i{}; i<bloom.number_of_blur_passes; ++i) {
            bind_frame_buffer(bloom.fbos[bloom.horizontal]);
            blur_shader.set_int("u_horizontal", bloom.horizontal);
            if (first_iter) {
                bind_texture_and_sampler(hdr.brightness_texture.id, bloom.sampler.id, 0);
                first_iter = false;
            }
            else {
                bind_texture_and_sampler(bloom.textures[!bloom.horizontal].id, bloom.sampler.id, 0);
            }
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            bloom.horizontal = !bloom.horizontal;
        }
    }

    // screen
    {
        bind_frame_buffer_default();
        clear_buffer_all(0, colors::WHITE, 1.0f, 0);

        screen_shader.use_shader();
        bind_vertex_array(screen_quad.vao);

        screen_shader.set_int("u_gamma", hdr.gamma_correction);
        screen_shader.set_int("u_hdr", hdr.do_hdr);
        screen_shader.set_int("u_bloom", bloom.do_bloom);
        screen_shader.set_float("u_exposure", hdr.exposure);

        bind_texture_and_sampler(hdr.color_texture.id, sampler_repeat.id, 0);
        bind_texture_and_sampler(bloom.textures[!bloom.horizontal].id, sampler_repeat.id, 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Bloom::imgui()
{
    if (ImGui::SliderFloat3("DirLight dir", dir_light.direction.data(), -1.0f, 1.0f)) {}
    for (int i{}; i<static_cast<int>(point_lights.size()); ++i) {
        std::string name {"PL"+std::to_string(i)+" diffuse"};
        if (ImGui::SliderFloat3(name.c_str(), point_lights[i].diffuse.data(), 0.0f, diffuse_max)) {}
    }

    if (ImGui::Checkbox("GammaCorrection", &hdr.gamma_correction)) {}
    if (ImGui::Checkbox("HDR", &hdr.do_hdr)) {}
    if (ImGui::Checkbox("Bloom", &bloom.do_bloom)) {}
    if (ImGui::Checkbox("attn_quadratic", &hdr.atn_quad)) {}

    if (ImGui::SliderFloat("Exposure", &hdr.exposure, 0.0f, 5.0f)) {}
    if (ImGui::SliderFloat("Intensity", &hdr.intensity, 0.0f, 50.0f)) {}
    if (ImGui::DragInt("NumberOfBlurPasses", &bloom.number_of_blur_passes, 1, 0, 100)) {}

    if (hdr.diffuse_textures_loaded_as_srgb) ImGui::Text("Textures loaded as sRGB");
    else ImGui::Text("Textures loaded as RGB");

    if (ImGui::Button("Load as sRGB")) {
        hdr.diffuse_textures_loaded_as_srgb = true;
        floor_texture = create_texture2d_from_image_srgb("./assets/textures/floor.png");
        brick_texture = create_texture2d_from_image_srgb("./assets/textures/brickwall.jpg");
        crate_texture = create_texture2d_from_image_srgb("./assets/textures/wooden_container.png");
    }
    if (ImGui::Button("Load as RGB")) {
        hdr.diffuse_textures_loaded_as_srgb = false;
        floor_texture = create_texture2d_from_image("./assets/textures/floor.png");
        brick_texture = create_texture2d_from_image("./assets/textures/brickwall.jpg");
        crate_texture = create_texture2d_from_image("./assets/textures/wooden_container.png");
    }
}

void Bloom::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);

    {
        hdr.color_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA32F);
        hdr.brightness_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA32F);
        hdr.depth_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_DEPTH_COMPONENT32F);

        glNamedFramebufferTexture(hdr.fbo.id, GL_DEPTH_ATTACHMENT, hdr.depth_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT0, hdr.color_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT1, hdr.brightness_texture.id, 0);

        auto status {glCheckNamedFramebufferStatus(hdr.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr.fbo.id, "incomplete\nstatus", status);
        }

        for (int i{}; i<2; ++i) {
            bloom.textures[i] = peria::create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F);

            glNamedFramebufferTexture(bloom.fbos[i].id, GL_COLOR_ATTACHMENT0, bloom.textures[i].id, 0);
            status = glCheckNamedFramebufferStatus(bloom.fbos[i].id, GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                peria::log("FrameBuffer with id", bloom.fbos[i].id, "incomplete\nstatus", status);
            }
        }
    }
}

Deferred_Rendering::Deferred_Rendering()
    :camera{{0.0f, 3.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     light_shader{"./assets/shaders/deferred/lighting_vert.glsl", "./assets/shaders/deferred/lighting_frag.glsl"},
     screen_shader{"./assets/shaders/deferred/screen_vert.glsl", "./assets/shaders/deferred/screen_frag.glsl"},
     lshader{"./assets/shaders/deferred/lighting_vert.glsl", "./assets/shaders/deferred/light_frag.glsl"},
     gbuffer_shader{"./assets/shaders/deferred/gbuffer_vert.glsl", "./assets/shaders/deferred/gbuffer_frag.glsl"},
     deferred_light_shader{"./assets/shaders/deferred/screen_vert.glsl", "./assets/shaders/deferred/deferred_lighting_frag.glsl"},
     sampler_repeat{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     backpack{
         Model{"./assets/models/backpack/backpack.obj"}, 
         create_texture2d_from_image_srgb("./assets/models/backpack/diffuse.jpg", true)},
     container_diffuse{create_texture2d_from_image_srgb("./assets/textures/wooden_container.png")},
     floor_diffuse{create_texture2d_from_image_srgb("./assets/textures/floor.png")},
     hdr{{},
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F),
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F)},
     gbuffer{{},
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F),
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F),
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F),
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F)
     }
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);
    light_shader.set_int("u_diffuse_texture", 0);
    gbuffer_shader.set_int("u_diffuse_texture", 0);
    screen_shader.set_int("u_hdr_color_texture", 0);
    deferred_light_shader.set_int("u_gpos_texture", 0);
    deferred_light_shader.set_int("u_gdiffuse_texture", 1);
    deferred_light_shader.set_int("u_gnormal_texture", 2);

    // cube
    {
        buffer_upload_data(cube.vbo, cube_data_with_norms, GL_STATIC_DRAW);
        vao_configure<Pos3D, Normal, TexCoord>(cube.vao.id, cube.vbo.id, 0);
    }

    // screen
    {
        std::array<Vertex<Pos2D, TexCoord>, 4> screen_quad_data {{
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
        }};
        std::array<u32, 6> indices {0,1,2, 0,2,3};

        buffer_upload_data(screen_quad.vbo, screen_quad_data, GL_STATIC_DRAW);
        buffer_upload_data(screen_quad.ibo, indices, GL_STATIC_DRAW);

        vao_configure<Pos2D, TexCoord>(screen_quad.vao.id, screen_quad.vbo.id, 0);
        vao_connect_ibo(screen_quad.vao, screen_quad.ibo);
    }

    // hdr stuff
    {
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT0, hdr.color_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_DEPTH_ATTACHMENT, hdr.depth_texture.id, 0);
        auto status {glCheckNamedFramebufferStatus(hdr.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr.fbo.id, "incomplete\nstatus", status);
        }
    }

    // deferred stuff
    {
        glNamedFramebufferTexture(gbuffer.fbo.id, GL_DEPTH_ATTACHMENT, gbuffer.depth_texture.id, 0);
        glNamedFramebufferTexture(gbuffer.fbo.id, GL_COLOR_ATTACHMENT0, gbuffer.pos_texture.id, 0);
        glNamedFramebufferTexture(gbuffer.fbo.id, GL_COLOR_ATTACHMENT1, gbuffer.diffuse_color_texture.id, 0);
        glNamedFramebufferTexture(gbuffer.fbo.id, GL_COLOR_ATTACHMENT2, gbuffer.normal_texture.id, 0);

        std::array<u32, 3> attachments {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glNamedFramebufferDrawBuffers(gbuffer.fbo.id, 3, attachments.data());

        auto status {glCheckNamedFramebufferStatus(gbuffer.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", gbuffer.fbo.id, "incomplete\nstatus", status);
        }
    }

    // lights
    {
        {
            float start_x {-25.0f};
            float start_z {-25.0f};
            float w {10.0f};
            for (int i{}; i<16; ++i) {
                for (int j{}; j<16; ++j) {
                    const float r {get_float(1000.0f, 5000.0f)};
                    const float g {get_float(1000.0f, 5000.0f)};
                    const float b {get_float(1000.0f, 5000.0f)};
                    point_lights.emplace_back(
                        Point_Light{
                            .pos{start_x + i*w, get_float(10.0f, 20.0f), start_z + j*w},
                            .ambient{0.001f, 0.001f, 0.001f},
                            .diffuse{r, g, b},
                            .specular{500.0f, 400.0f, 500.0f}
                        }
                    );
                }
            }
        }
        Ubo_Lights lights {
            {},
            {},
            {},
            {0, static_cast<float>(point_lights.size())}
        };
        for (std::size_t i{}; i<point_lights.size(); ++i) {
            lights.point_lights[i] = to_ubo_point_light(point_lights[i]);
        }
        std::size_t offset {};

        buffer_allocate_data(lights_ubo, sizeof(Ubo_Lights), GL_DYNAMIC_DRAW);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
        offset += sizeof(Ubo_Directional_Light);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
        offset += sizeof(Ubo_Spot_Light)*MAX_SPLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
        offset += sizeof(Ubo_Point_Light)*MAX_PLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_ubo.id);
    }

    backpack_transforms.reserve(256);
    cube_transforms.reserve(256);
    constexpr int N {20};
    {
        float start_x {-25.0f};
        float start_z {-25.0f};
        float w {3.5f};
        for (int i{}; i<N; ++i) {
            for (int j{}; j<N; ++j) {
                backpack_transforms.emplace_back(Transform{{start_x + w*i, 3.0f, start_z + w*j}});
            }
        }
    }
    {
        float start_x {-25.0f};
        float start_z {-25.0f};
        float w {3.5f};
        for (int i{}; i<N; ++i) {
            for (int j{}; j<N; ++j) {
                cube_transforms.emplace_back(Transform{{start_x + w*i, 10.0f, start_z + w*j}});
            }
        }
    }
}

void Deferred_Rendering::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }

    {
        Ubo_Lights lights {
            {},
            {},
            {},
            {0, static_cast<float>(point_lights.size())}
        };
        for (std::size_t i{}; i<point_lights.size(); ++i) {
            lights.point_lights[i] = to_ubo_point_light(point_lights[i]);
        }
        std::size_t offset {};

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
        offset += sizeof(Ubo_Directional_Light);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
        offset += sizeof(Ubo_Spot_Light)*MAX_SPLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
        offset += sizeof(Ubo_Point_Light)*MAX_PLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());
    }

}

void Deferred_Rendering::render()
{
    const auto screen_dims {get_screen_dimensions()};
    glViewport(0, 0, screen_dims.x, screen_dims.y);

    if (deferred) {
        bind_frame_buffer(gbuffer.fbo);
        clear_buffer_color(gbuffer.fbo.id, colors::BLACK, 0); // pos
        clear_buffer_color(gbuffer.fbo.id, colors::BLACK, 1); // color
        clear_buffer_color(gbuffer.fbo.id, colors::BLACK, 2); // normals
        clear_buffer_depth(gbuffer.fbo.id, 1.0f);

        // geometry pass
        {
            gbuffer_shader.use_shader();
            gbuffer_shader.set_mat4("u_vp", projection*camera.get_view());
            {
                bind_texture_and_sampler(backpack.backpack_diffuse.id, sampler_repeat.id, 0);
                for (const auto& t:backpack_transforms) {
                    gbuffer_shader.set_mat4("u_model", get_model_mat(t));
                    const auto& meshes {backpack.backpack.get_meshes()};
                    for (const auto& m:meshes) {
                        bind_vertex_array(m.vao_id());
                        glDrawElements(GL_TRIANGLES, m.get_index_count(), GL_UNSIGNED_INT, nullptr);
                    }
                }
            }


            {
                bind_vertex_array(cube.vao);
                bind_texture_and_sampler(container_diffuse.id, sampler_repeat.id, 0);
                for (const auto& t:cube_transforms) {
                    gbuffer_shader.set_mat4("u_model", get_model_mat(t));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                Transform trans = {{0.0f, -4.0f, 0.0f}, {200.0f, 0.2f, 200.0f}, {}};
                gbuffer_shader.set_mat4("u_model", get_model_mat(trans));
                bind_texture_and_sampler(floor_diffuse.id, sampler_repeat.id, 0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // light pass
        {
            bind_frame_buffer(hdr.fbo.id);
            clear_buffer_color(hdr.fbo.id, colors::BLACK, 0);
            clear_buffer_depth(hdr.fbo.id, 1.0f);

            deferred_light_shader.use_shader();
            bind_vertex_array(screen_quad.vao);

            deferred_light_shader.set_vec3("u_camera_pos", camera.get_pos());
            deferred_light_shader.set_float("u_point_light_intensity", hdr.intensity);
            bind_texture_and_sampler(gbuffer.pos_texture.id, sampler_repeat.id, 0);
            bind_texture_and_sampler(gbuffer.diffuse_color_texture.id, sampler_repeat.id, 1);
            bind_texture_and_sampler(gbuffer.normal_texture.id, sampler_repeat.id, 2);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            const auto dims {get_screen_dimensions()};
            // this can be used wit MSAA as well.
            glBlitNamedFramebuffer(gbuffer.fbo.id, hdr.fbo.id, 0, 0, dims.x, dims.y, 0, 0, dims.x, dims.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

            // this also works. But formats need to be same or considered as same, and no MSAA
            //glCopyImageSubData(gbuffer.depth_texture.id, GL_TEXTURE_2D, 0, 0, 0, 0, hdr.depth_texture.id, GL_TEXTURE_2D, 0, 0, 0, 0, dims.x, dims.y, 1);

            bind_vertex_array(cube.vao);
            lshader.use_shader();
            lshader.set_mat4("u_vp", projection*camera.get_view());
            lshader.set_vec3("u_light_color", {1.0f, 1.0f, 1.0f});
            for (const auto& pl:point_lights) {
                Transform trans = {pl.pos, {1.0f, 1.0f, 1.0f}, {}};
                lshader.set_mat4("u_model", get_model_mat(trans));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            glEnable(GL_DEPTH_TEST);
        }

    }
    else { // forward rendering here for comparison

        // light pass
        {
            bind_frame_buffer(hdr.fbo);
            clear_buffer_color(hdr.fbo.id, colors::BLACK, 0);
            clear_buffer_depth(hdr.fbo.id, 1.0f);

            light_shader.use_shader();
            light_shader.set_float("u_point_light_intensity", hdr.intensity);
            light_shader.set_mat4("u_vp", projection*camera.get_view());
            light_shader.set_vec3("u_camera_pos", camera.get_pos());

            {
                bind_texture_and_sampler(backpack.backpack_diffuse.id, sampler_repeat.id, 0);
                const auto& meshes {backpack.backpack.get_meshes()};
                for (const auto& t:backpack_transforms) {
                    light_shader.set_mat4("u_model", get_model_mat(t));
                    for (const auto& m:meshes) {
                        bind_vertex_array(m.vao_id());
                        glDrawElements(GL_TRIANGLES, m.get_index_count(), GL_UNSIGNED_INT, nullptr);
                    }
                }

            }

            {
                bind_vertex_array(cube.vao);
                bind_texture_and_sampler(container_diffuse.id, sampler_repeat.id, 0);
                for (const auto& t:cube_transforms) {
                    light_shader.set_mat4("u_model", get_model_mat(t));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                Transform trans = {{0.0f, -4.0f, 0.0f}, {200.0f, 0.2f, 200.0f}, {}};
                light_shader.set_mat4("u_model", get_model_mat(trans));
                bind_texture_and_sampler(floor_diffuse.id, sampler_repeat.id, 0);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                lshader.use_shader();
                lshader.set_mat4("u_vp", projection*camera.get_view());
                lshader.set_vec3("u_light_color", {1.0f, 1.0f, 1.0f});
                for (const auto& pl:point_lights) {
                    trans = {pl.pos, {1.0f, 1.0f, 1.0f}, {}};
                    lshader.set_mat4("u_model", get_model_mat(trans));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }
    }

    {
        bind_frame_buffer_default();
        clear_buffer_all(0, colors::WHITE, 1.0f, 0);

        screen_shader.use_shader();
        bind_vertex_array(screen_quad.vao);

        screen_shader.set_float("u_exposure", hdr.exposure);

        bind_texture_and_sampler(hdr.color_texture.id, sampler_repeat.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Deferred_Rendering::imgui()
{
    ImGui::Text("dt %f, fps %f", Timer::instance()->dt(), 1.0f / std::max(0.0000001f, Timer::instance()->dt()));
    if (deferred) {
        ImGui::Text("Deffered Rendering");
    }
    else {
        ImGui::Text("Forward Rendering");
    }
    for (std::size_t i{}; i<0; ++i) {
        const auto name {"PL "+std::to_string(i)};
        if (ImGui::SliderFloat3(name.c_str(), point_lights[i].diffuse.data(), 0.0f, 5000.0f)) {}
        if (ImGui::SliderFloat3((name+"pos").c_str(), point_lights[i].pos.data(), -50.0f, 50.0f)) {}
    }
    if (ImGui::SliderFloat("intensity", &hdr.intensity, 0.0f, 100.0f)) {}
    if (ImGui::SliderFloat("exposure", &hdr.exposure, 0.0f, 10.0f)) {}
    if (ImGui::Checkbox("Deferred", &deferred)) {}
}

void Deferred_Rendering::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);

    // hdr
    {
        hdr.color_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA32F);
        hdr.depth_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_DEPTH_COMPONENT32F);

        glNamedFramebufferTexture(hdr.fbo.id, GL_DEPTH_ATTACHMENT, hdr.depth_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT0, hdr.color_texture.id, 0);

        auto status {glCheckNamedFramebufferStatus(hdr.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr.fbo.id, "incomplete\nstatus", status);
        }
    }

    // gbuffer
    {
        gbuffer.depth_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_DEPTH_COMPONENT32F);
        gbuffer.pos_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA32F);
        gbuffer.diffuse_color_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA32F);
        gbuffer.normal_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA32F);

        glNamedFramebufferTexture(gbuffer.fbo.id, GL_DEPTH_ATTACHMENT, gbuffer.depth_texture.id, 0);
        glNamedFramebufferTexture(gbuffer.fbo.id, GL_COLOR_ATTACHMENT0, gbuffer.pos_texture.id, 0);
        glNamedFramebufferTexture(gbuffer.fbo.id, GL_COLOR_ATTACHMENT1, gbuffer.diffuse_color_texture.id, 0);
        glNamedFramebufferTexture(gbuffer.fbo.id, GL_COLOR_ATTACHMENT2, gbuffer.normal_texture.id, 0);

        auto status {glCheckNamedFramebufferStatus(gbuffer.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", gbuffer.fbo.id, "incomplete\nstatus", status);
        }
    }
}

Platonic_Solids::Platonic_Solids()
    :camera{{0.0f, 3.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     light_shader{"./assets/shaders/color_correction/lighting_vert.glsl", "./assets/shaders/color_correction/lighting_frag.glsl"},
     normal_shader{"./assets/shaders/normal_visualization_vert.glsl", "./assets/shaders/deferred/light_frag.glsl", "./assets/shaders/geom.glsl"},
     screen_shader{"./assets/shaders/deferred/screen_vert.glsl", "./assets/shaders/deferred/screen_frag.glsl"},
     sampler_repeat{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     tetra{"./assets/models/platonic_solids/icosahedron.obj"},
     hdr{{},
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_RGBA32F),
         create_texture2d(get_screen_dimensions().x, get_screen_dimensions().y, GL_DEPTH_COMPONENT32F)},
     solid_color1{create_texture2d_colored(colors::DARKGREY)},
     solid_color2{create_texture2d_colored(colors::LIMEGREEN)}
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);
    light_shader.set_int("u_diffuse_texture", 0);
    light_shader.set_int("u_do_bloom", false);
    screen_shader.set_int("u_hdr_color_texture", 0);
    light_shader.set_int("u_atn_quadratic", true);


    std::array<u32, 6> indices {0,1,2, 0,2,3};

    // plane
    {
        std::array<Vertex<Pos3D, Normal, TexCoord>, 6> plane_data {{
            {{-1.0f, 0.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            {{-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, {{ 1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ 1.0f, 0.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        }};
        buffer_upload_data(plane_vbo, plane_data, GL_STATIC_DRAW);

        vao_configure<Pos3D, Normal, TexCoord>(plane_vao.id, plane_vbo.id, 0);
        vao_connect_ibo(plane_vao, screen_quad.ibo);
    }

    // tetra
    {
        const auto v0 {Pos3D{1.0f, 1.0f, 1.0f}};
        const auto v1 {Pos3D{1.0f, -1.0f, -1.0f}};
        const auto v2 {Pos3D{-1.0f, 1.0f, -1.0f}};
        const auto v3 {Pos3D{-1.0f, -1.0f, 1.0f}};

        const auto n0 {glm::cross(
                glm::vec3{v1.data[0]-v0.data[0], v1.data[1]-v0.data[1], v1.data[2]-v0.data[2]},
                glm::vec3{v2.data[0]-v0.data[0], v2.data[1]-v0.data[1], v2.data[2]-v0.data[2]})
        };
        const auto n1 {glm::cross(
                glm::vec3{v2.data[0]-v0.data[0], v2.data[1]-v0.data[1], v2.data[2]-v0.data[2]},
                glm::vec3{v3.data[0]-v0.data[0], v3.data[1]-v0.data[1], v3.data[2]-v0.data[2]})
        };
        const auto n2 {glm::cross(
                glm::vec3{v3.data[0]-v0.data[0], v3.data[1]-v0.data[1], v3.data[2]-v0.data[2]},
                glm::vec3{v1.data[0]-v0.data[0], v1.data[1]-v0.data[1], v1.data[2]-v0.data[2]})
        };
        const auto n3 {glm::cross(
                glm::vec3{v3.data[0]-v1.data[0], v3.data[1]-v1.data[1], v3.data[2]-v1.data[2]},
                glm::vec3{v2.data[0]-v1.data[0], v2.data[1]-v1.data[1], v2.data[2]-v1.data[2]})
        };

        std::array<Vertex<Pos3D, Normal>, 12> tetra_data {{
            {v0, {n0.x, n0.y, n0.z}},
            {v1, {n0.x, n0.y, n0.z}},
            {v2, {n0.x, n0.y, n0.z}},

            {v0, {n1.x, n1.y, n1.z}},
            {v2, {n1.x, n1.y, n1.z}},
            {v3, {n1.x, n1.y, n1.z}},

            {v0, {n2.x, n2.y, n2.z}},
            {v3, {n2.x, n2.y, n2.z}},
            {v1, {n2.x, n2.y, n2.z}},

            {v1, {n3.x, n3.y, n3.z}},
            {v3, {n3.x, n3.y, n3.z}},
            {v2, {n3.x, n3.y, n3.z}}
        }};

        //std::array<Vertex<Pos3D, Normal>, 4> tetra_data {{
        //    {v0, {0.0f, 1.0f, 0.0f}},
        //    {v1, {0.0f, 1.0f, 0.0f}},
        //    {v2, {0.0f, 1.0f, 0.0f}},
        //    {v3, {0.0f, 1.0f, 0.0f}}
        //}};
        std::array<u32, 12> tetra_indices {
            0, 1, 2,
            0, 2, 3,
            0, 3, 1,
            3, 2, 1
        };

        buffer_upload_data(tetrahedron.vbo, tetra_data, GL_STATIC_DRAW);
        buffer_upload_data(tetrahedron.ibo, tetra_indices, GL_STATIC_DRAW);

        vao_configure<Pos3D, Normal>(tetrahedron.vao.id, tetrahedron.vbo.id, 0);
        vao_connect_ibo(tetrahedron.vao, tetrahedron.ibo);
    }

    // screen
    {
        std::array<Vertex<Pos2D, TexCoord>, 4> screen_quad_data {{
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
        }};

        buffer_upload_data(screen_quad.vbo, screen_quad_data, GL_STATIC_DRAW);
        buffer_upload_data(screen_quad.ibo, indices, GL_STATIC_DRAW);

        vao_configure<Pos2D, TexCoord>(screen_quad.vao.id, screen_quad.vbo.id, 0);
        vao_connect_ibo(screen_quad.vao, screen_quad.ibo);
    }

    // hdr stuff
    {
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT0, hdr.color_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_DEPTH_ATTACHMENT, hdr.depth_texture.id, 0);
        auto status {glCheckNamedFramebufferStatus(hdr.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr.fbo.id, "incomplete\nstatus", status);
        }
    }

    // lights
    {
        point_light = Point_Light{
            .pos{0.0f, 5.0f, 0.0f},
            .ambient{0.01f, 0.01f, 0.01f},
            .diffuse{1000.0f, 1000.0f, 500.0f},
            .specular{500.0f, 500.0f, 500.0f}
        };

        Ubo_Lights lights {
            {},
            {},
            {to_ubo_point_light(point_light)},
            {0, 1}
        };
        std::size_t offset {};

        buffer_allocate_data(lights_ubo, sizeof(Ubo_Lights), GL_DYNAMIC_DRAW);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
        offset += sizeof(Ubo_Directional_Light);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
        offset += sizeof(Ubo_Spot_Light)*MAX_SPLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
        offset += sizeof(Ubo_Point_Light)*MAX_PLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_ubo.id);
    }
}

void Platonic_Solids::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }

    {
        Ubo_Lights lights {
            {},
            {},
            {to_ubo_point_light(point_light)},
            {0, 1}
        };
        std::size_t offset {};

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Directional_Light), &lights.directional_light);
        offset += sizeof(Ubo_Directional_Light);

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Spot_Light)*lights.counts[0], lights.spot_lights.data());
        offset += sizeof(Ubo_Spot_Light)*MAX_SPLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(Ubo_Point_Light)*lights.counts[1], lights.point_lights.data());
        offset += sizeof(Ubo_Point_Light)*MAX_PLS;

        buffer_upload_subdata(lights_ubo, offset, sizeof(float)*lights.counts.size(), lights.counts.data());
    }

}

void Platonic_Solids::render()
{
    const auto screen_dims {get_screen_dimensions()};
    glViewport(0, 0, screen_dims.x, screen_dims.y);

    {
        bind_frame_buffer(hdr.fbo);
        clear_buffer_color(hdr.fbo.id, colors::BLACK, 0);
        clear_buffer_depth(hdr.fbo.id, 1.0f);

        bind_vertex_array(plane_vao);

        light_shader.use_shader();
        light_shader.set_float("u_point_light_intensity", hdr.intensity);
        light_shader.set_mat4("u_vp", projection*camera.get_view());
        light_shader.set_vec3("u_camera_pos", camera.get_pos());

        Transform trans {{}, {50.0f, 1.0f, 50.0f}};
        bind_texture_and_sampler(solid_color1.id, sampler_repeat.id);
        light_shader.set_mat4("u_model", get_model_mat(trans));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        bind_texture_and_sampler(solid_color2.id, sampler_repeat.id);
        for (const auto& m:tetra.get_meshes()) {
            bind_vertex_array(m.vao_id());
            trans = {{-2.0f, 2.0f, 1.0f}, {2.0f, 2.0f, 2.0f}};
            light_shader.set_mat4("u_model", get_model_mat(trans));
            glDrawElements(GL_TRIANGLES, m.get_index_count(), GL_UNSIGNED_INT, nullptr);
        }

        // Attempt at my own platonic solids. Not that straightforward. Just import from blender :p
        //bind_vertex_array(tetrahedron.vao);
        //trans = {{-2.0f, 2.0f, 1.0f}, {2.0f, 2.0f, 2.0f}};
        //bind_texture_and_sampler(solid_color2.id, sampler_repeat.id);
        //light_shader.set_mat4("u_model", get_model_mat(trans));

        ////glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
        //glDrawArrays(GL_TRIANGLES, 0, 12);

        //trans = {{point_light.pos}, {0.2f, 0.2f, 0.2f}};
        //light_shader.set_mat4("u_model", get_model_mat(trans));
        //glDrawArrays(GL_TRIANGLES, 0, 12);

        //normal_shader.use_shader();
        //normal_shader.set_mat4("u_vp", projection*camera.get_view());
        //normal_shader.set_vec3("u_camera_pos", camera.get_pos());

        //trans = {{-2.0f, 2.0f, 1.0f}, {2.0f, 2.0f, 2.0f}};
        //normal_shader.set_mat4("u_model", get_model_mat(trans));
        //normal_shader.set_mat4("u_projection", projection);
        //normal_shader.set_mat4("u_view", camera.get_view());
        //normal_shader.set_vec3("u_light_color", {1.0f, 1.0f, 0.0f});
        //glDrawArrays(GL_TRIANGLES, 0, 12);

    }

    {
        bind_frame_buffer_default();
        clear_buffer_all(0, colors::WHITE, 1.0f, 0);

        screen_shader.use_shader();
        bind_vertex_array(screen_quad.vao);

        screen_shader.set_float("u_exposure", hdr.exposure);

        bind_texture_and_sampler(hdr.color_texture.id, sampler_repeat.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Platonic_Solids::imgui()
{
    ImGui::Text("dt %f, fps %f", Timer::instance()->dt(), 1.0f / std::max(0.0000001f, Timer::instance()->dt()));
    if (ImGui::SliderFloat3("PL-Diffuse", point_light.diffuse.data(), 0.0f, 3000.0f)) {}
    if (ImGui::SliderFloat3("PL-Specular", point_light.specular.data(), 0.0f, 3000.0f)) {}
    if (ImGui::SliderFloat3("PL-Pos", point_light.pos.data(), -50.0f, 50.0f)) {}
    if (ImGui::SliderFloat("intensity", &hdr.intensity, 0.0f, 100.0f)) {}
    if (ImGui::SliderFloat("exposure", &hdr.exposure, 0.0f, 10.0f)) {}
}

void Platonic_Solids::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);

    // hdr
    {
        hdr.color_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_RGBA32F);
        hdr.depth_texture = peria::create_texture2d(screen_dims.x, screen_dims.y, GL_DEPTH_COMPONENT32F);

        glNamedFramebufferTexture(hdr.fbo.id, GL_DEPTH_ATTACHMENT, hdr.depth_texture.id, 0);
        glNamedFramebufferTexture(hdr.fbo.id, GL_COLOR_ATTACHMENT0, hdr.color_texture.id, 0);

        auto status {glCheckNamedFramebufferStatus(hdr.fbo.id, GL_FRAMEBUFFER)};
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            peria::log("FrameBuffer with id", hdr.fbo.id, "incomplete\nstatus", status);
        }
    }
}

Pan_Zoom::Pan_Zoom()
    :camera{{0.0f, 3.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     shader{"./assets/shaders/panzoom_vert.glsl", "./assets/shaders/panzoom_frag.glsl"},
     sampler_repeat{create_sampler(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     solid_white{create_texture2d_colored(colors::WHITE)},
     chiti{create_texture2d_from_image("./assets/textures/chitunia.png")}
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y);
    shader.set_int("u_texture", 0);

    // quad
    {
        // this time counter-clockwise
        // 2---------1
        // |         |
        // |         |
        // 3_________0
        std::array<Vertex<Pos2D, TexCoord>, 4> quad_data {{
            {{ 0.5f, -0.5f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        }};
        std::array<u32, 6> quad_indices {0,1,2, 0,2,3};

        buffer_upload_data(quad.vbo, quad_data, GL_STATIC_DRAW);
        buffer_upload_data(quad.ibo, quad_indices, GL_STATIC_DRAW);

        vao_configure<Pos2D, TexCoord>(quad.vao.id, quad.vbo.id, 0);
        vao_connect_ibo(quad.vao, quad.ibo);
    }

}

void Pan_Zoom::update()
{
    cam2d.update(projection);
}

void Pan_Zoom::render()
{
    const auto screen_dims {get_screen_dimensions()};
    glViewport(0, 0, screen_dims.x, screen_dims.y);
    bind_frame_buffer_default();
    clear_buffer_all(0, colors::LIGHTGRAY, 1.0f, 0);

    bind_vertex_array(quad.vao);
    shader.use_shader();
    shader.set_mat4("u_vp", projection*cam2d.view);

    // solid color rect
    {
        glm::vec2 world_pos {2000.0f, 800.0f};
        //const auto view_pos {world_to_view(world_pos)};
        //Transform trans {{view_pos.x, view_pos.y, 0.0f}, {50.0f*zoom_scale, 50.0f*zoom_scale, 1.0f}};
        Transform trans {{world_pos.x, world_pos.y, 0.0f}, {50.0f, 50.0f, 1.0f}};
        shader.set_mat4("u_model", get_model_mat(trans));
        shader.set_vec3("u_color", {0.4f, 0.6f, 0.1f});
        bind_texture_and_sampler(solid_white.id, sampler_repeat.id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // textured rect
    glm::vec2 world_pos {screen_dims.x*0.75f, screen_dims.y*0.75f};
    //const auto view_pos {world_to_view(world_pos)};
    //Transform trans = {{view_pos.x, view_pos.y, 0.0f}, {150.0f*zoom_scale, 150.0f*zoom_scale, 1.0f}};
    Transform trans = {{world_pos.x, world_pos.y, 0.0f}, {150.0f, 150.0f, 1.0f}};
    shader.set_mat4("u_model", get_model_mat(trans));
    shader.set_vec3("u_color", {1.0f, 1.0f, 1.0f});
    bind_texture_and_sampler(chiti.id, sampler_repeat.id);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Pan_Zoom::imgui()
{}

void Pan_Zoom::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y);
}

Batching_Vs_Instancing::Batching_Vs_Instancing()
    :camera{{0.0f, 3.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     batch_shader{"./assets/shaders/batch_instance/batch_vert.glsl", "./assets/shaders/batch_instance/batch_frag.glsl"},
     instance_shader{"./assets/shaders/batch_instance/instance_vert.glsl", "./assets/shaders/batch_instance/instance_frag.glsl"}
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y);

    quad_models.reserve(1000000);
    for (auto& c:colors) {
        c.x = get_float(0.0f, 1.0f);
        c.y = get_float(0.0f, 1.0f);
        c.z = get_float(0.0f, 1.0f);
    }

    // 2---------1
    // |         |
    // |         |
    // 3_________0

    // BATCHING ONLY
    {
        quad_batcher.indices.resize(quad_batcher.max_quads_per_batch*6);
        for (int i{}; i<quad_batcher.max_quads_per_batch; ++i) {
            quad_batcher.indices[6*i]   = 4*i;
            quad_batcher.indices[6*i+1] = 4*i+1;
            quad_batcher.indices[6*i+2] = 4*i+2;

            quad_batcher.indices[6*i+3] = 4*i;
            quad_batcher.indices[6*i+4] = 4*i+2;
            quad_batcher.indices[6*i+5] = 4*i+3;
        }

        buffer_allocate_data(quad_batcher.vbo, quad_batcher.max_quads_per_batch*sizeof(Vertex<Pos2D, Color3>)*4, GL_DYNAMIC_DRAW);
        buffer_upload_data(quad_batcher.ibo, quad_batcher.indices, GL_DYNAMIC_DRAW);
        
        vao_configure<Pos2D, Color3>(quad_batcher.vao.id, quad_batcher.vbo.id, 0);
        vao_connect_ibo(quad_batcher.vao, quad_batcher.ibo);
    }

    // INSTANCING
    {
        std::array<Vertex<Pos2D>, 4> quad_data {{
            {{ 0.5f, -0.5f}},
            {{ 0.5f,  0.5f}},
            {{-0.5f,  0.5f}},
            {{-0.5f, -0.5f}}
        }};
        std::array<u32, 6> indices {0,1,2, 0,2,3};

        buffer_upload_data(quad_instancer.quad_vbo, quad_data, GL_STATIC_DRAW);
        buffer_upload_data(quad_instancer.ibo, indices, GL_STATIC_DRAW);

        vao_configure<Pos2D>(quad_instancer.vao.id, quad_instancer.quad_vbo.id, 0);
        vao_connect_ibo(quad_instancer.vao, quad_instancer.ibo);

        buffer_allocate_data(quad_instancer.color_vbo, quad_instancer.max_instances*sizeof(glm::vec3), GL_DYNAMIC_DRAW);
        vao_configure<Color3>(quad_instancer.vao.id, quad_instancer.color_vbo.id, 1, 1);
        glVertexArrayBindingDivisor(quad_instancer.vao.id, 1, 1);

        buffer_allocate_data(quad_instancer.instance_vbo, quad_instancer.max_instances*sizeof(glm::mat4), GL_DYNAMIC_DRAW);
        vao_configure<Pos4D, Pos4D, Pos4D, Pos4D>(quad_instancer.vao.id, quad_instancer.instance_vbo.id, 2, 2);
        glVertexArrayBindingDivisor(quad_instancer.vao.id, 2, 1);
    }

}

void Batching_Vs_Instancing::resize()
{
    if (use_batching) {
        quad_batcher.vbo = Buffer_Object{};
        quad_batcher.ibo = Buffer_Object{};
        quad_batcher.indices.resize(quad_batcher.max_quads_per_batch*6);
        for (int i{}; i<quad_batcher.max_quads_per_batch; ++i) {
            quad_batcher.indices[6*i]   = 4*i;
            quad_batcher.indices[6*i+1] = 4*i+1;
            quad_batcher.indices[6*i+2] = 4*i+2;

            quad_batcher.indices[6*i+3] = 4*i;
            quad_batcher.indices[6*i+4] = 4*i+2;
            quad_batcher.indices[6*i+5] = 4*i+3;
        }

        buffer_allocate_data(quad_batcher.vbo, quad_batcher.max_quads_per_batch*sizeof(Vertex<Pos2D, Color3>)*4, GL_DYNAMIC_DRAW);
        buffer_upload_data(quad_batcher.ibo, quad_batcher.indices, GL_DYNAMIC_DRAW);
        
        vao_configure<Pos2D, Color3>(quad_batcher.vao.id, quad_batcher.vbo.id, 0);
        vao_connect_ibo(quad_batcher.vao, quad_batcher.ibo);
    }
    else {
        quad_instancer.instance_vbo = Buffer_Object{};
        quad_instancer.color_vbo = Buffer_Object{};

        buffer_allocate_data(quad_instancer.color_vbo, quad_instancer.max_instances*sizeof(glm::vec3), GL_DYNAMIC_DRAW);
        vao_configure<Color3>(quad_instancer.vao.id, quad_instancer.color_vbo.id, 1, 1);
        glVertexArrayBindingDivisor(quad_instancer.vao.id, 1, 1);

        buffer_allocate_data(quad_instancer.instance_vbo, quad_instancer.max_instances*sizeof(glm::mat4), GL_DYNAMIC_DRAW);
        vao_configure<Pos4D, Pos4D, Pos4D, Pos4D>(quad_instancer.vao.id, quad_instancer.instance_vbo.id, 2, 2);
        glVertexArrayBindingDivisor(quad_instancer.vao.id, 2, 1);
    }
}

void Batching_Vs_Instancing::update()
{
    cam2d.update(projection);
}

void Batching_Vs_Instancing::render()
{
    const auto screen_dims {get_screen_dimensions()};
    glViewport(0, 0, screen_dims.x, screen_dims.y);
    bind_frame_buffer_default();
    clear_buffer_all(0, colors::LIGHTGRAY, 1.0f, 0);

    if (use_batching) {
        batch_shader.use_shader();
        batch_shader.set_mat4("u_mvp", projection*cam2d.view);
        bind_vertex_array(quad_batcher.vao);
        int count {static_cast<int>((quad_batcher.data.size()/4))};
        int iterations {count / quad_batcher.max_quads_per_batch};
        int leftover   {count - (quad_batcher.max_quads_per_batch*iterations)};
        quad_batcher.iterations = iterations;
        for (int i{}; i<iterations; ++i) {
            buffer_upload_subdata(quad_batcher.vbo, 0, 
                                  4*quad_batcher.max_quads_per_batch*quad_batcher::vertex_t::stride, 
                                  quad_batcher.data.data()+i*quad_batcher.max_quads_per_batch*4);
            glDrawElements(GL_TRIANGLES, quad_batcher.max_quads_per_batch*6, GL_UNSIGNED_INT, nullptr);
        }
        if (leftover > 0) {
            ++quad_batcher.iterations;
            buffer_upload_subdata(quad_batcher.vbo, 0, 
                                  4*static_cast<u32>(leftover)*quad_batcher::vertex_t::stride, 
                                  quad_batcher.data.data()+iterations*quad_batcher.max_quads_per_batch*4);
            glDrawElements(GL_TRIANGLES, leftover*6, GL_UNSIGNED_INT, nullptr);
        }
    }
    else {
        instance_shader.use_shader();
        instance_shader.set_mat4("u_vp", projection*cam2d.view);
        bind_vertex_array(quad_instancer.vao);

        int count {static_cast<int>((quad_models.size()))};
        int iterations {count / quad_instancer.max_instances};
        int leftover   {count - (quad_instancer.max_instances*iterations)};
        quad_instancer.iterations = iterations;

        for (int i{}; i<iterations; ++i) {
            buffer_upload_subdata(quad_instancer.instance_vbo, 0, 
                                  sizeof(glm::mat4)*quad_instancer.max_instances, 
                                  quad_models.data()+i*quad_instancer.max_instances);
            buffer_upload_subdata(quad_instancer.color_vbo, 0, 
                                  sizeof(glm::vec3)*quad_instancer.max_instances, 
                                  quad_colors.data()+i*quad_instancer.max_instances);
            glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, quad_instancer.max_instances);
        }
        if (leftover > 0) {
            ++quad_instancer.iterations;
            buffer_upload_subdata(quad_instancer.instance_vbo, 0, 
                                  static_cast<u32>(leftover)*sizeof(glm::mat4), 
                                  quad_models.data()+iterations*quad_instancer.max_instances);
            buffer_upload_subdata(quad_instancer.color_vbo, 0, 
                                  sizeof(glm::vec3)*static_cast<u32>(leftover), 
                                  quad_colors.data()+iterations*quad_instancer.max_instances);
            glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, quad_instancer.max_instances);
        }
    }
}

void Batching_Vs_Instancing::imgui()
{
    ImGui::Text("FPS %f", 1.0f/Timer::instance()->dt());
    ImGui::Text("ZoomLevel %f", cam2d.zoom_scale);
    if (ImGui::Checkbox("use_batching", &use_batching)) {}
    if (use_batching) {
        ImGui::Text("MAX BatchSize %d", quad_batcher.max_quads_per_batch);
        ImGui::Text("DrawCalls %d", quad_batcher.iterations);
        if (ImGui::Button("IncreaseMaxBatchSize")) {
            quad_batcher.max_quads_per_batch = std::min(16384, quad_batcher.max_quads_per_batch*2);
            resize();
        }
        if (ImGui::Button("DecreaseMaxBatchSize")) {
            quad_batcher.max_quads_per_batch = std::max(1, quad_batcher.max_quads_per_batch/2);
            resize();
        }
    }
    else {
        ImGui::Text("Max per Instance%d", quad_instancer.max_instances);
        ImGui::Text("Instanced draw calls %d", quad_instancer.iterations);
        if (ImGui::Button("IncreaseMaxPerInstance")) {
            quad_instancer.max_instances = std::min(16384, quad_instancer.max_instances*2);
            resize();
        }
        if (ImGui::Button("DecreaseMaxPerInstance")) {
            quad_instancer.max_instances = std::max(1, quad_instancer.max_instances/2);
            resize();
        }

    }
    if (ImGui::InputInt("quad_count", &quad_count)) {
        quad_models.clear();
        quad_colors.clear();
        quad_batcher.data.resize(quad_count*4);
        int N {static_cast<int>(sqrt(quad_count))};
        int k {};
        for (int i{}; i<N; ++i) {
            for (int j{}; j<N; ++j, k+=4) {
                const auto r {colors[(i+j)%colors.size()].x};
                const auto g {colors[(i+j)%colors.size()].y};
                const auto b {colors[(i+j)%colors.size()].z};
                const auto w {150.0f};
                const auto h {100.0f};
                const glm::vec2 center {j*w, i*h};
                // instance related data
                quad_models.emplace_back(get_model_mat(Transform{{center.x, center.y, 0.0f}, {w, h, 1.0f}}));
                quad_colors.emplace_back(glm::vec3{r, g, b});

                // batcher only related data
                {
                    const glm::vec2 lower_right {center+glm::vec2{w/2,  -h/2}};
                    const glm::vec2 upper_right {center+glm::vec2{w/2,   h/2}};
                    const glm::vec2 upper_left  {center+glm::vec2{-w/2,  h/2}};
                    const glm::vec2 lower_left  {center+glm::vec2{-w/2, -h/2}};
                    quad_batcher.data[k+0]= Vertex<Pos2D, Color3>{{lower_right.x, lower_right.y}, {r, g, b}}; // 0 
                    quad_batcher.data[k+1]= Vertex<Pos2D, Color3>{{upper_right.x, upper_right.y}, {r, g, b}}; // 1
                    quad_batcher.data[k+2]= Vertex<Pos2D, Color3>{{upper_left .x, upper_left .y}, {r, g, b}}; // 2
                    quad_batcher.data[k+3]= Vertex<Pos2D, Color3>{{lower_left .x, lower_left .y}, {r, g, b}}; // 3
                }
            }
        }
    }
}

void Batching_Vs_Instancing::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::ortho(0.0f, screen_dims.x, 0.0f, screen_dims.y);
}

Compute_Shader_Intro::Compute_Shader_Intro()
    :camera{{0.0f, 3.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     screen_shader{"./assets/shaders/simple_screen_quad_vert.glsl", "./assets/shaders/simple_screen_quad_frag.glsl"},
     compute_shader{"./assets/shaders/compute/hello.comp"},
     sampler{create_sampler(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     image{create_texture2d(100, 100, GL_RGBA32F)}
{
    const auto screen_dims { peria::get_screen_dimensions() };
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);

    compute_shader.set_int("u_output", 0);
    compute_shader.set_int("u_texture", 0);

    std::array<u32, 6> indices {0,1,2, 0,2,3};

    // screen
    {
        std::array<Vertex<Pos2D, TexCoord>, 4> screen_quad_data {{
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
        }};

        buffer_upload_data(screen_quad_vbo, screen_quad_data, GL_STATIC_DRAW);
        buffer_upload_data(quad_ibo, indices, GL_STATIC_DRAW);

        vao_configure<Pos2D, TexCoord>(screen_quad_vao.id, screen_quad_vbo.id, 0);
        vao_connect_ibo(screen_quad_vao, quad_ibo);
    }

}

void Compute_Shader_Intro::update()
{
    if (is_relative_mouse()) {
        camera.update();
    }
}

void Compute_Shader_Intro::render()
{
    const auto screen_dims {get_screen_dimensions()};
    glViewport(0, 0, screen_dims.x, screen_dims.y);

    compute_shader.use_shader();

    glBindImageTexture(0, image.id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glDispatchCompute(100, 100, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    {
        bind_frame_buffer_default();
        clear_buffer_all(0, colors::GREY, 1.0f, 0);

        screen_shader.use_shader();
        bind_vertex_array(screen_quad_vao);
        bind_texture_and_sampler(image.id, sampler.id, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Compute_Shader_Intro::imgui()
{
    ImGui::Text("dt %f, fps %f", Timer::instance()->dt(), 1.0f / std::max(0.0000001f, Timer::instance()->dt()));
}

void Compute_Shader_Intro::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 300.f);
}

}
