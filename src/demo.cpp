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
#include "input_manager.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace {

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
     chiti{create_texture2d_from_image("./assets/textures/chitunia.png")},
     monkey_color{create_texture2d_colored(colors::CYAN)},
     uv_sphere_color{create_texture2d_colored(colors::OLIVE)},
     ico_sphere_color{create_texture2d_colored(colors::TOMATO)},
     shadow_shader{"./assets/shaders/shadow/shadow_vertex.glsl", "./assets/shaders/shadow/shadow_fragment.glsl"},
     //omni_shadow_shader{"./assets/shaders/shadow/omni_shadow_vertex.glsl", "./assets/shaders/shadow/omni_shadow_fragment.glsl",  "./assets/shaders/shadow/omni_shadow_geometry.glsl"},
     light_shader{"./assets/shaders/lighting/light_vertex.glsl","./assets/shaders/lighting/light_fragment.glsl"},
     colored_obj_shader{"./assets/shaders/colored_object_vertex.glsl","./assets/shaders/colored_object_fragment.glsl"},
     line_shader{"./assets/shaders/line_vertex.glsl","./assets/shaders/line_fragment.glsl"},
     shadow_sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER)},
     sampler{create_sampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)},
     monkey{"./assets/models/monkey/suzanne.obj"},
     uv_sphere{"./assets/models/uv_sphere/uv_sphere.obj"},
     ico_sphere{"./assets/models/ico_sphere/ico_sphere.obj"}
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

        shadow_data.light_projection = glm::perspective(glm::radians(light_data.spot_lights[0].inner_angle*2.0f), static_cast<float>(shadow_data.shadowmap_w)/shadow_data.shadowmap_h, 0.1f, 20.0f);
        shadow_data.light_view = glm::lookAt(arr_to_vec3(light_data.spot_lights[0].pos), arr_to_vec3(light_data.spot_lights[0].direction), {0.0f, 1.0f, 0.0f});
    }

    light_shader.set_int("u_texture", 0);
    light_shader.set_int("u_shadowmap", 1);
}

void Shadows::update()
{
    //shadow_data.light_view = glm::lookAt(arr_to_vec3(light_data.directional_light.pos), {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    //const auto& [x, y, z] {light_data.directional_light.pos};
    //light_data.directional_light.direction = {-x, -y, -z};

    //camera.set_pos(arr_to_vec3(campos));

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

void Shadows::render()
{
    // shadow pass
    if (toggle_shadows) {
        bind_frame_buffer(shadow_fbo);
        set_viewport(0, 0, shadow_data.shadowmap_w, shadow_data.shadowmap_h);
        clear_buffer_depth(shadow_fbo.id, 1.0f);

        shadow_shader.set_mat4("u_vp", shadow_data.light_projection*shadow_data.light_view);
        draw_scene(shadow_shader);
    }

    // light pass
    {
        bind_frame_buffer_default();
        const auto screen_dims {get_screen_dimensions()};
        set_viewport(0, 0, screen_dims.x, screen_dims.y);
        clear_buffer_all(0, colors::GREY, 1.0f, 0);

        light_shader.set_mat4("u_vp", projection*camera.get_view());
        light_shader.set_mat4("u_light_vp", shadow_data.light_projection*shadow_data.light_view);
        light_shader.set_vec3("u_camera_pos", camera.get_pos());

        light_shader.set_float("u_min_shadow_bias",        min_bias);
        light_shader.set_float("u_max_shadow_bias",        max_bias);
        light_shader.set_int("u_toggle_shadows",           toggle_shadows);
        light_shader.set_int("u_toggle_directional_light", light_data.toggle_directional_light);
        light_shader.set_int("u_toggle_spot_lights",       light_data.toggle_spot_lights);

        bind_texture_and_sampler(chiti.id, sampler.id);
        bind_texture_and_sampler(shadowmap.id, shadow_sampler.id, 1);

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
     model_shader{"./assets/shaders/basic_model_vertex.glsl","./assets/shaders/basic_model_fragment.glsl"}
{
    recalculate_projection();
}

void Modelebi::update()
{
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

    if (Input_Manager::instance()->mouse_down(Mouse_Button::LEFT) &&
        !is_relative_mouse() &&
        sphere_vs_line(arr_to_vec3(sphere.pos), sphere.radius, camera.get_pos(), mouse_world)) {
        const auto sensitivity {0.005f};

        const auto rel_motion {get_relative_motion()};

        float x{}, y{}, z{};

        sphere.pos[0] += sensitivity*rel_motion.x;
        sphere.pos[1] += sensitivity*rel_motion.y;
        sphere.pos[2] += sensitivity*z;
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

}
