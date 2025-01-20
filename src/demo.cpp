#include "demo.hpp"
#include <memory>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <numeric>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "vertex_array.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "sampler.hpp"
#include "asset_manager.hpp"

#include "graphics.hpp"
#include "input_manager.hpp"

namespace {
    std::vector<peria::graphics::vertex::Vertex3d> cube_model {
        // near
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}},
        
        // far
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},

        // left
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                                 
        // right                 
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                                 
        // bottom                
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
                                 
        // top                   
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}
    };

    std::vector<glm::vec3> sky_box_cube_model {
        // near
        {-1.0f, -1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        
        // far
        { 1.0f, -1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},

        // left
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
                              
        // right               
        { 1.0f, -1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f, -1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 1.0f,  1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
                              
        // bottom              
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f, -1.0f},
                              
        // top                 
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f,  1.0f}
    };

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
    glm::mat4 get_model_mat(const Transform& t) noexcept
    {
        return {
            glm::translate(glm::mat4{1.0f}, glm::vec3{t.x, t.y, t.z})*
            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_x), glm::vec3{1.0f, 0.0f, 0.0f})*
            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_y), glm::vec3{0.0f, 1.0f, 0.0f})*
            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_z), glm::vec3{0.0f, 0.0f, 1.0f})*
            glm::scale(glm::mat4{1.0f}, glm::vec3{t.sx, t.sy, t.sz})
        };
    }

    [[nodiscard]]
    glm::vec3 get_vec3(const std::array<float, 3>& color) noexcept
    { return {color[0], color[1], color[2]}; }
}

namespace peria::graphics::demos {

Demo3d::Demo3d()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     default_vao{std::make_unique<Vertex_Array>()},
     light_source_vao{std::make_unique<Vertex_Array>()},
     quad_vao{std::make_unique<Vertex_Array>()},
     quad_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/quad_vertex.glsl", "./assets/shaders/quad_fragment.glsl")},
     default_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")},
     cross_hair_texture{Asset_Manager::instance()->fetch_texture("./assets/textures/cross-hair.png")}
{
    { // configure 2d quad related data. (crosshair info for now)
        quad_shader->use_shader();
        std::array<i32, 8> slots; std::iota(slots.begin(), slots.end(), 0);
        quad_shader->set_array("u_textures", 8, slots.data());

        // for now only used for crosshair
        std::vector<vertex::Vertex2d> quad_data {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}, colors::WHITE, 1},
            {{-0.5f,  0.5f}, {0.0f, 1.0f}, colors::WHITE, 1},
            {{ 0.5f,  0.5f}, {1.0f, 1.0f}, colors::WHITE, 1},
            {{ 0.5f, -0.5f}, {1.0f, 0.0f}, colors::WHITE, 1}
        };
        std::vector<u32> indices {0,1,2, 0,2,3};

        quad_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(quad_data);
        quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

        quad_vao->setup_attribute(Attribute<float>{2, false});
        quad_vao->setup_attribute(Attribute<float>{2, false});
        quad_vao->setup_attribute(Attribute<float>{4, false});
        quad_vao->setup_attribute(Attribute<float>{1, false});

        quad_vao->connect_vertex_buffer(quad_vbo->buffer_id(), sizeof(vertex::Vertex2d));
        quad_vao->connect_index_buffer(quad_ibo->buffer_id());
    }

    { // configure 3d stuff
        cube_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex3d>>(cube_model);

        // pos only
        light_source_vao->setup_attribute(Attribute<float>{3, false});
        light_source_vao->connect_vertex_buffer(cube_vbo->buffer_id(), sizeof(vertex::Vertex3d));

        // pos
        default_vao->setup_attribute(Attribute<float>{3, false});
        // normal
        default_vao->setup_attribute(Attribute<float>{3, false});
        // tex coords
        default_vao->setup_attribute(Attribute<float>{2, false});
        default_vao->connect_vertex_buffer(cube_vbo->buffer_id(), sizeof(vertex::Vertex3d));
    }

    { // samplers
        default_sampler = std::make_unique<Sampler>(0);
    }

    { // other gl or graphics related stuff here
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        peria::graphics::set_vsync(false);
    }

    if (0) {
        std::array arr {
            GL_MAX_VERTEX_UNIFORM_COMPONENTS,
            GL_MAX_GEOMETRY_UNIFORM_COMPONENTS,
            GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
            GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS,
            GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS,
            GL_MAX_VERTEX_ATTRIBS
        };

        std::array names {
            "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
            "GL_MAX_GEOMETRY_UNIFORM_COMPONENTS",
            "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
            "GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS",
            "GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS",
            "GL_MAX_VERTEX_ATTRIBS"
        };
        for (std::size_t i{}; i<arr.size(); ++i) {
            i32 ans;
            glGetIntegerv(arr[i], &ans);
            peria::log(names[i], ans);
        }
        std::terminate();
    }

}

Demo_Combined_Lights::Demo_Combined_Lights()
    :Demo3d{},
     diffuse_texture{Asset_Manager::instance()->fetch_texture("./assets/textures/wooden_container.png")},
     specular_texture{Asset_Manager::instance()->fetch_texture("./assets/textures/specular_wooden_container.png")},
     dragon{std::make_unique<Model>("./assets/models/dragon/dragon.obj")}
{
    auto am {Asset_Manager::instance()};
    { // shaders
        light_source_shader = am->fetch_shader("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl");
        combined_lights_shader = am->fetch_shader("./assets/shaders/cube_vertex.glsl", "./assets/shaders/combined_lights_fragment.glsl");
        combined_lights_shader->use_shader();
        combined_lights_shader->set_int("u_material.diffuse_texture", 0);
        combined_lights_shader->set_int("u_material.specular_texture", 1);
    }
    
    spot_light.angle = 12.5f;
    spot_light.angle = 14.0f;

    bg_color = {0.147f, 0.139f, 0.139f};
}

void Demo_Combined_Lights::render()
{
    peria::graphics::colors::Color<float> clr {bg_color[0], bg_color[1], bg_color[2], 1.0f};
    peria::graphics::clear_named_buffer(0, clr, 1.0f, 0);

    light_source_vao->bind();
    light_source_shader->use_shader();

    for (const auto& pl:point_lights) {
        const auto& [lx, ly, lz] {pl.pos};
        const auto light_source_model {get_model_mat({
                0.30f, 0.30f, 0.30f, 
                0.0f, 0.0f, 0.0f,
                lx, ly, lz})};

        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*light_source_model);
        light_source_shader->set_vec3("u_light_source_color", get_vec3(pl.diffuse));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    if (debug_mode) {
        // draw center arrows of the world. Can reuse light_source shader
        const auto k {500.0f};
        const auto r {0.02f};
        const auto x_axis_model {get_model_mat({
                k, r, r,
                0.0f, 0.0f, 0.0f,
                k*0.5f + r, r*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*x_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {1.0f, 0.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto y_axis_model {get_model_mat({
                r, k, r,
                0.0f, 0.0f, 0.0f,
                r*0.5f, k*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*y_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 1.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto z_axis_model {get_model_mat({
                r, r, k,
                0.0f, 0.0f, 0.0f,
                r*0.5f, r*0.5f, -k*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*z_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 0.0f, 1.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    combined_lights_shader->use_shader();
    combined_lights_shader->set_vec3("u_view_pos", camera.get_pos());

    { // directional light
        combined_lights_shader->set_vec3("u_directional_light.direction", get_vec3(directional_light.direction));
        combined_lights_shader->set_vec3("u_directional_light.ambient", get_vec3(directional_light.ambient));
        combined_lights_shader->set_vec3("u_directional_light.diffuse", get_vec3(directional_light.diffuse));
        combined_lights_shader->set_vec3("u_directional_light.specular", get_vec3(directional_light.specular));
    }
    { // point lights
        for (std::size_t i{}; i<point_lights.size(); ++i) {
            const auto name      {std::string{"u_point_lights["}+std::to_string(i)+std::string{"]"}};
            const auto pos       {name + ".pos"};
            const auto ambient   {name + ".ambient"};
            const auto diffuse   {name + ".diffuse"};
            const auto specular  {name + ".specular"};
            const auto linear    {name + ".linear"};
            const auto quadratic {name + ".quadratic"};
            combined_lights_shader->set_vec3(pos.c_str(), get_vec3(point_lights[i].pos));
            combined_lights_shader->set_vec3(ambient.c_str(), get_vec3(point_lights[i].ambient));
            combined_lights_shader->set_vec3(diffuse.c_str(), get_vec3(point_lights[i].diffuse));
            combined_lights_shader->set_vec3(specular.c_str(), get_vec3(point_lights[i].specular));
            combined_lights_shader->set_float(linear.c_str(), point_lights[i].attenuation.linear);
            combined_lights_shader->set_float(quadratic.c_str(), point_lights[i].attenuation.quadratic);
        }
        combined_lights_shader->set_int("u_point_lights_count", static_cast<int>(point_lights.size()));
    }
    { // spot light
        combined_lights_shader->set_vec3("u_spot_light.pos", camera.get_pos());
        combined_lights_shader->set_vec3("u_spot_light.direction", camera.get_view_direction());
        combined_lights_shader->set_vec3("u_spot_light.ambient", get_vec3(spot_light.ambient));
        combined_lights_shader->set_vec3("u_spot_light.diffuse", get_vec3(spot_light.diffuse));
        combined_lights_shader->set_vec3("u_spot_light.specular", get_vec3(spot_light.specular));
        combined_lights_shader->set_float("u_spot_light.linear", spot_light.attenuation.linear);
        combined_lights_shader->set_float("u_spot_light.quadratic", spot_light.attenuation.quadratic);
        combined_lights_shader->set_float("u_spot_light.inner_cosine_angle", std::cos(glm::radians(spot_light.angle)));
        combined_lights_shader->set_float("u_spot_light.outer_cosine_angle", std::cos(glm::radians(spot_light.outer_angle)));
    }

    combined_lights_shader->set_mat4("u_vp", projection*camera.get_view());

    dragon->draw(combined_lights_shader);

    default_vao->bind();

    material.shininess = 32;
    combined_lights_shader->set_float("u_material.shininess", material.shininess);
    bind_texture_and_sampler(diffuse_texture, default_sampler.get(), 0);
    bind_texture_and_sampler(specular_texture, default_sampler.get(), 1);

    // draw cubes
    {
        for (const auto& pos:cube_positions) {
            const auto model {get_model_mat(
                {1.0f, 1.0f, 1.0f,
                 0.0f, 0.0f, 0.0f,
                 pos.x, pos.y, pos.z})};
            
            combined_lights_shader->set_mat4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // draw crosshair here
    {
        quad_vao->bind();
        quad_shader->use_shader();
        
        bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);
        const auto d {peria::graphics::get_screen_dimensions()};
        const auto model {get_model_mat(
            {32.0f, 32.0f, 1.0f,
             0.0f, 0.0f, 0.0f,
             d.x*0.5f, d.y*0.5f, 0.0f})};

        quad_shader->set_mat4("u_mvp", ortho_projection*model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Demo_Combined_Lights::update()
{
    auto im {Input_Manager::instance()};
    const auto object_pos {camera.get_pos() + camera_front_magnitude*camera.get_view_direction()};

    if (!im->key_down(SDL_SCANCODE_LSHIFT) && im->mouse_pressed(Mouse_Button::LEFT)) {
        cube_positions.emplace_back(object_pos);
    }
    if (!im->key_down(SDL_SCANCODE_LSHIFT) && im->mouse_pressed(Mouse_Button::RIGHT)) {
        if (!cube_positions.empty()) cube_positions.pop_back();
    }

    if (im->key_down(SDL_SCANCODE_LSHIFT) && im->mouse_pressed(Mouse_Button::LEFT)) {
        point_lights.emplace_back(Point_Light{{object_pos.x, object_pos.y, object_pos.z}, {}, {}, {}});
    }
    if (im->key_down(SDL_SCANCODE_LSHIFT) && im->mouse_pressed(Mouse_Button::RIGHT)) {
        if (!point_lights.empty()) {
            point_lights.pop_back();
            imgui_info.point_lights.pop_back();
        }
    }

    if (im->key_pressed(SDL_SCANCODE_F2)) {
        debug_mode = !debug_mode;
    }
}

void Demo_Combined_Lights::imgui()
{
    auto attn_helper {[](Attenuation& attn) {
        if (ImGui::Button("ATT_DISTANCE_7"))    attn = ATT_DISTANCE_7;
        if (ImGui::Button("ATT_DISTANCE_13"))   attn = ATT_DISTANCE_13;
        if (ImGui::Button("ATT_DISTANCE_20"))   attn = ATT_DISTANCE_20;
        if (ImGui::Button("ATT_DISTANCE_32"))   attn = ATT_DISTANCE_32;
        if (ImGui::Button("ATT_DISTANCE_50"))   attn = ATT_DISTANCE_50;
        if (ImGui::Button("ATT_DISTANCE_65"))   attn = ATT_DISTANCE_65;
        if (ImGui::Button("ATT_DISTANCE_100"))  attn = ATT_DISTANCE_100;
        if (ImGui::Button("ATT_DISTANCE_160"))  attn = ATT_DISTANCE_160;
        if (ImGui::Button("ATT_DISTANCE_200"))  attn = ATT_DISTANCE_200;
        if (ImGui::Button("ATT_DISTANCE_325"))  attn = ATT_DISTANCE_325;
        if (ImGui::Button("ATT_DISTANCE_600"))  attn = ATT_DISTANCE_600;
        if (ImGui::Button("ATT_DISTANCE_3250")) attn = ATT_DISTANCE_3250;
    }};

    auto& [dl, sl, pl, pls] = imgui_info;

    ImGui::Begin("Info");

    const auto cam_pos {camera.get_pos()};
    const auto cam_front {camera.get_view_direction()};
    ImGui::Text("Camera pos = {%f, %f, %f}", cam_pos.x, cam_pos.y, cam_pos.z);
    ImGui::Text("Camera front = {%f, %f, %f}", cam_front.x, cam_front.y, cam_front.z);

    ImGui::ColorEdit3("BgColor", bg_color.data());

    if (ImGui::Button("Directional Light")) {
        dl = !dl;
    }
    if (dl) {
        ImGui::SliderFloat3("Dirlight direction", directional_light.direction.data(), -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::ColorEdit3("Dirlight ambient",  directional_light.ambient.data());
        ImGui::ColorEdit3("Dirlight diffuse",  directional_light.diffuse.data());
        ImGui::ColorEdit3("Dirlight specular", directional_light.specular.data());
    }

    if (ImGui::Button("Spot Light")) {
        sl = !sl;
    }
    if (sl) {
        ImGui::ColorEdit3("SpotLight ambient",  spot_light.ambient.data());
        ImGui::ColorEdit3("SpotLight diffuse",  spot_light.diffuse.data());
        ImGui::ColorEdit3("SpotLight specular", spot_light.specular.data());

        ImGui::SliderFloat("SpotLight specular", &spot_light.angle, 0.0f, 50.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SliderFloat("SpotLight specular", &spot_light.outer_angle, 0.0f, 50.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

        ImGui::Text("Current ATT: %f, %f", spot_light.attenuation.linear, spot_light.attenuation.quadratic);
        attn_helper(spot_light.attenuation);
    }

    if (ImGui::Button("Point Lights")) {
        pl = !pl;
    }
    if (pl) {
        while (pls.size() < point_lights.size()) {
            pls.emplace_back(false);
        }
        
        for (std::size_t i{}; i<point_lights.size(); ++i) {
            const auto prefix_name {std::string{"PointLight"}+std::to_string(i+1)};
            if (ImGui::Button(prefix_name.c_str())) {
                pls[i] = !pls[i];
            }
            if (pls[i]) {
                ImGui::ColorEdit3((prefix_name+" ambient" ).c_str(), point_lights[i].ambient.data());
                ImGui::ColorEdit3((prefix_name+" diffuse" ).c_str(), point_lights[i].diffuse.data());
                ImGui::ColorEdit3((prefix_name+" specular").c_str(), point_lights[i].specular.data());

                ImGui::Text("Current ATT: %f, %f", point_lights[i].attenuation.linear, point_lights[i].attenuation.quadratic);
                attn_helper(point_lights[i].attenuation);
            }
        }
    }

    ImGui::End();
}

Demo_Model::Demo_Model()
    :Demo3d{},
     shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/cube_vertex.glsl", "./assets/shaders/combined_lights_fragment.glsl")}
     //shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")}
{ 
    model = std::make_unique<peria::graphics::Model>("./assets/models/backpack/backpack.obj"); 
    //model = std::make_unique<peria::graphics::Model>("./assets/models/dragon/dragon.obj");
    //model = std::make_unique<peria::graphics::Model>("./assets/models/kek/pirveli_yleoba_xD.obj");
}

void Demo_Model::render()
{
    clear_named_buffer(0, peria::graphics::colors::Color{0.0f, 0.1f, 0.1f, 1.0f}, 1.0f, 0);
    model->draw(shader);
}

void Demo_Model::update()
{}

void Demo_Model::imgui()
{}

Demo_Depth_Testing::Demo_Depth_Testing()
    :Demo3d{},
     shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/depth_fragment.glsl")},
     texture1{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")},
     texture2{Asset_Manager::instance()->fetch_texture("./assets/textures/wooden_container.png")}
{ 
}

void Demo_Depth_Testing::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::Color{0.4f, 0.3f, 0.3f, 1.0f}, 1.0f, 0);

    glDepthFunc(GL_LESS);
    default_vao->bind();

    shader->use_shader();
    shader->set_float("u_near", 0.1f);
    shader->set_float("u_far", 100.0f);

    auto model_cube1 {get_model_mat(
        {1.0f, 1.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 1.0f})};

    auto model_cube2 {get_model_mat(
        {1.5f, 1.5f, 1.5f,
         0.0f, 0.0f, 0.0f,
         -2.0f, 0.0f, 1.0f})};

    auto model_plane {get_model_mat(
        {20.0f, 0.5f, 20.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 0.0f})};

    bind_texture_and_sampler(texture2, default_sampler.get(), 0);
    shader->set_mat4("u_vp", projection*camera.get_view());
    shader->set_mat4("u_model", model_plane);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    bind_texture_and_sampler(texture1, default_sampler.get(), 0);
    shader->set_mat4("u_model", model_cube1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader->set_mat4("u_model", model_cube2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Demo_Depth_Testing::update()
{ }

void Demo_Depth_Testing::imgui()
{}

Demo_Stencil_Testing::Demo_Stencil_Testing()
    :Demo3d{},
     shader1{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")},
     shader2{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/stencil_fragment.glsl")},
     texture1{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")},
     texture2{Asset_Manager::instance()->fetch_texture("./assets/textures/wooden_container.png")}
{ 
}

void Demo_Stencil_Testing::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::Color{0.4f, 0.3f, 0.3f, 1.0f}, 1.0f, 0);
    default_vao->bind();

    shader1->use_shader();

    auto model_cube1 {get_model_mat(
        {1.0f, 1.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 1.0f})};

    auto model_cube2 {get_model_mat(
        {1.5f, 1.5f, 1.5f,
         0.0f, 0.0f, 0.0f,
         -2.0f, 0.0f, 1.0f})};

    auto model_plane {get_model_mat(
        {20.0f, 0.5f, 20.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 0.0f})};

    bind_texture_and_sampler(texture2, default_sampler.get(), 0);
    shader1->set_mat4("u_vp", projection*camera.get_view());
    shader1->set_mat4("u_model", model_plane);
    glStencilMask(0x00);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    bind_texture_and_sampler(texture1, default_sampler.get(), 0);
    shader1->set_mat4("u_model", model_cube1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader1->set_mat4("u_model", model_cube2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisable(GL_DEPTH_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to stencil buffer not to override previous data

    shader2->use_shader();
    shader2->set_mat4("u_vp", projection*camera.get_view());
    shader2->set_vec3("u_outline_color", glm::vec3(0.3f, 0.5f, 1.0f));

    constexpr auto k {1.1f};

    model_cube1 = get_model_mat(
        {1.0f*k, 1.0f*k, 1.0f*k,
         0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 1.0f});

    model_cube2 = get_model_mat(
        {1.5f*k, 1.5f*k, 1.5f*k,
         0.0f, 0.0f, 0.0f,
         -2.0f, 0.0f, 1.0f});

    shader2->set_mat4("u_model", model_cube1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader2->set_mat4("u_model", model_cube2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
}

void Demo_Stencil_Testing::update()
{}

void Demo_Stencil_Testing::imgui()
{}

Another_Demo::Another_Demo()
    :Demo3d{},
     quad_vao{std::make_unique<Vertex_Array>()},
     white_texture{std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(colors::WHITE))},
     quad_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/quad_vertex.glsl", "./assets/shaders/quad_fragment.glsl")},
     shader1{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")},
     texture1{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")}
{
    quad_shader->use_shader();
    std::array<i32, 8> slots; std::iota(slots.begin(), slots.end(), 0);
    quad_shader->set_array("u_textures", 8, slots.data());

    std::vector<vertex::Vertex2d> quad_data {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}, colors::PINK, 1},
        {{-0.5f,  0.5f}, {0.0f, 1.0f}, colors::PINK, 1},
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}, colors::PINK, 1},
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}, colors::PINK, 1}
    };

    std::vector<u32> indices {0,1,2, 0,2,3};

    quad_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(quad_data);
    quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

    quad_vao->setup_attribute(Attribute<float>{2, false});
    quad_vao->setup_attribute(Attribute<float>{2, false});
    quad_vao->setup_attribute(Attribute<float>{4, false});
    quad_vao->setup_attribute(Attribute<float>{1, false});

    quad_vao->connect_vertex_buffer(quad_vbo->buffer_id(), sizeof(vertex::Vertex2d));
    quad_vao->connect_index_buffer(quad_ibo->buffer_id());

    bind_texture_and_sampler(white_texture.get(), default_sampler.get(), 0);
    bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

    cubes.emplace_back(glm::vec3{0.0f, 3.0f, -4.0f});
}

void Another_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::Color{0.2f, 0.3f, 0.3f, 1.0f}, 1.0f, 0 );

    default_vao->bind();
    shader1->use_shader();
    auto model_plane {get_model_mat(
        {20.0f, 0.5f, 20.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 0.0f})};

    bind_texture_and_sampler(texture1, default_sampler.get(), 0);
    shader1->set_mat4("u_vp", projection*camera.get_view());
    shader1->set_mat4("u_model", model_plane);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    {// draw cubes
        for (const auto& pos:cubes) {
            auto model {get_model_mat(
                {1.0f, 1.0f, 1.0f,
                 0.0f, 0.0f, 0.0f,
                 pos.x, pos.y, pos.z})};
            
            shader1->set_mat4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    quad_vao->bind();
    quad_shader->use_shader();
    
    const auto d {peria::graphics::get_screen_dimensions()};
    auto model {get_model_mat(
        {32.0f, 32.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         d.x*0.5f, d.y*0.5f, 0.0f})};

    quad_shader->set_mat4("u_mvp", ortho_projection*model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Another_Demo::update()
{
    auto im {Input_Manager::instance()};

    if (im->mouse_pressed(Mouse_Button::LEFT)) {
        const auto cam_pos {camera.get_pos()};
        const auto cam_front {camera.get_view_direction()};
        cubes.emplace_back(cam_pos + camera_front_magnitude*cam_front);
    }
}

void Another_Demo::imgui()
{
    ImGui::Begin("Another_Demo");
    ImGui::SliderFloat("Camera Front Magnitude", &camera_front_magnitude, 0.0f, 50.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

    const auto cam_pos {camera.get_pos()};
    auto cam_front {camera.get_view_direction()};
    ImGui::Text("Camera pos = {%f, %f, %f}", cam_pos.x, cam_pos.y, cam_pos.z);
    ImGui::Text("Camera front = {%f, %f, %f}", cam_front.x, cam_front.y, cam_front.z);

    ImGui::Text("Cubes positions:\n");
    i32 i{1};
    for (const auto& pos:cubes) {
        ImGui::Text("Cube {%d} pos = {%f, %f, %f}", i++, pos.x, pos.y, pos.z);
    }

    ImGui::End();
}

// ============================================= 2D ================================================
Demo2d::Demo2d()
    :vao{std::make_unique<Vertex_Array>()},
     vbo{std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(QUAD_COUNT * 4 * sizeof(vertex::Vertex2d))},
     white_texture{std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(colors::WHITE))},
     texture_atlas{std::make_unique<Texture>("./assets/textures/mushrooms_sheet.png")},
     default_sampler{std::make_unique<Sampler>(0)}
{
    std::vector<u32> indices; indices.reserve(QUAD_COUNT * 6);
    for (u32 i{}; i<QUAD_COUNT; ++i) {
        indices.emplace_back(4*i);
        indices.emplace_back(4*i + 1);
        indices.emplace_back(4*i + 2);

        indices.emplace_back(4*i);
        indices.emplace_back(4*i + 2);
        indices.emplace_back(4*i + 3);
    }
    ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

    // pos
    vao->setup_attribute(Attribute<float>{2, false});
    // texture coordinates
    vao->setup_attribute(Attribute<float>{2, false});
    // color
    vao->setup_attribute(Attribute<float>{4, false});
    // texture unit
    vao->setup_attribute(Attribute<float>{1, false});

    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(vertex::Vertex2d));
    vao->connect_index_buffer(ibo->buffer_id());

    quad_draw_data.reserve(QUAD_COUNT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Demo_Quads::draw_colored_quad(const Quad& quad, const colors::Color<float>& color) noexcept
{
    const auto& [x, y, w, h] {quad};
    quad_draw_data.emplace_back(vertex::Vertex2d{{x,   y  }, {0.0f, 0.0f}, color, 0});
    quad_draw_data.emplace_back(vertex::Vertex2d{{x,   y+h}, {0.0f, 1.0f}, color, 0});
    quad_draw_data.emplace_back(vertex::Vertex2d{{x+w, y+h}, {1.0f, 1.0f}, color, 0});
    quad_draw_data.emplace_back(vertex::Vertex2d{{x+w, y  }, {1.0f, 0.0f}, color, 0});
}

void Demo_Quads::render_quads() noexcept
{
    if (quad_draw_data.empty()) return;

    auto current_quad_count {quad_draw_data.size() / 4};
    std::size_t offset {};

    vao->bind();
    shader->use_shader();
    shader->set_mat4("u_mvp", projection);
    
    bind_texture_and_sampler(white_texture.get(), default_sampler.get(), 0);
    bind_texture_and_sampler(texture_atlas.get(), default_sampler.get(), 1);
    
    while (current_quad_count > 0) {
        i32 c {}; // count of rects for each batch
        if (current_quad_count >= QUAD_COUNT) {
            c = QUAD_COUNT;
        }
        else { // smaller remaining batch
            c = current_quad_count;
        }

        vbo->set_sub_data(0, c*4*sizeof(vertex::Vertex2d), quad_draw_data.data() + offset);
        glDrawElements(GL_TRIANGLES, c*6, GL_UNSIGNED_INT, nullptr);
        offset += 4*c;
        current_quad_count -= c;
    }

    quad_draw_data.clear();
}

void Demo_Quads::draw_textured_quad(const Quad& quad, const Quad& texture_region) noexcept
{
    const auto& [x, y, w, h] {quad};
    const auto& [ax, ay, aw, ah] {texture_region};

    const auto texture_dimensions {texture_atlas->dimensions()};
    const auto coords {get_texture_coordinates(ax, ay, aw, ah, texture_dimensions.x, texture_dimensions.y)};

    quad_draw_data.emplace_back(vertex::Vertex2d{{x,   y  }, coords[0], colors::WHITE, 1});
    quad_draw_data.emplace_back(vertex::Vertex2d{{x,   y+h}, coords[1], colors::WHITE, 1});
    quad_draw_data.emplace_back(vertex::Vertex2d{{x+w, y+h}, coords[2], colors::WHITE, 1});
    quad_draw_data.emplace_back(vertex::Vertex2d{{x+w, y  }, coords[3], colors::WHITE, 1});
}

Demo_Quads::Demo_Quads()
    :Demo2d{},
     shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/quad_vertex.glsl", "./assets/shaders/quad_fragment.glsl")}
{
    std::array<i32, 8> slots; std::iota(slots.begin(), slots.end(), 0);
    shader->set_array("u_textures", 8, slots.data());
}

void Demo_Quads::render()
{
    peria::graphics::clear_named_buffer(0, colors::GREENYELLOW, 1.0f, 0);
    auto start_x {500.0f};
    auto start_y {220.0f};
    auto width  {16.0f};
    auto height {16.0f};

    draw_colored_quad({100.0f, 200.0f, 300.0f, 200.0f}, peria::graphics::colors::KHAKI);
    for (i32 i{}; i<2; ++i) {
        for (i32 j{}; j<3; ++j) {
            draw_textured_quad({start_x + j*width*6, start_y + i*height*6, width*6, height*6}, {j*width, i*height, width, height});
        }
    }

    render_quads();
}

void Demo_Quads::update()
{}

void Demo_Quads::imgui()
{}

Texture2d_Demo::Texture2d_Demo()
    :Demo2d{},
     quad_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/quad_vertex.glsl", "./assets/shaders/quad_fragment.glsl")}
{
    make_data(1.0f);

    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/grass.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/pikapika.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/shaco1.jpg"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/xD.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/wooden_container.png"));

    samplers.emplace_back(std::make_unique<Sampler>());
}

void Texture2d_Demo::make_data(float tex_coord_scale)
{
    peria::log(tex_coord_scale);
    std::vector<vertex::Vertex2d> data {
        {{-0.5f, -0.5f}, glm::vec2{tex_coord_scale*0.0f, tex_coord_scale*0.0f}, colors::WHITE, 1},
        {{-0.5f,  0.5f}, glm::vec2{tex_coord_scale*0.0f, tex_coord_scale*1.0f}, colors::WHITE, 1},
        {{ 0.5f,  0.5f}, glm::vec2{tex_coord_scale*1.0f, tex_coord_scale*1.0f}, colors::WHITE, 1},
        {{ 0.5f, -0.5f}, glm::vec2{tex_coord_scale*1.0f, tex_coord_scale*0.0f}, colors::WHITE, 1}
    };

    peria::log(data[2].texture_coordinates.x, data[2].texture_coordinates.y);
    vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(data);
    std::vector<u32> indices {0,1,2, 0,2,3};
    ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);
    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(vertex::Vertex2d));
    vao->connect_index_buffer(ibo->buffer_id());
}

void Texture2d_Demo::update()
{ 
    make_data(imgui_info.tex_coords_scale); // doing this every frame is bad. But since we only test 1 quad it is ok.
}

void Texture2d_Demo::render()
{
    peria::graphics::clear_named_buffer(0, colors::GREY, 1.0f, 0);
    vao->bind();
    quad_shader->use_shader();

    const glm::vec2 wh {600.0f, 400.0f};
    const auto dims {graphics::get_screen_dimensions()};
    const glm::vec2 pos {dims.x*0.5f, dims.y*0.5f};
    const auto quad_model {get_model_mat({
            wh.x, wh.y, 1.0f,
            0.0f, 0.0f, 0.0f,
            pos.x, pos.y, 0.0f})};
    quad_shader->set_mat4("u_mvp", projection*quad_model);
    
    bind_texture_and_sampler(textures[tex_index], samplers[sampler_index].get(), 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Texture2d_Demo::imgui()
{
    if (ImGui::Button("next texture")) {
        tex_index = (tex_index+1)%textures.size();
    }
    
    ImGui::SliderFloat("TexCoordScale", &imgui_info.tex_coords_scale, -5.0f, 5.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
}

// ====================================================================

Blending_Demo::Blending_Demo()
    :Demo2d{},
     quad_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/quad_vertex.glsl", "./assets/shaders/quad_fragment.glsl")}
{
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/grass.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/pikapika.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/shaco1.jpg"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/xD.png"));
    textures.emplace_back(Asset_Manager::instance()->fetch_texture("./assets/textures/wooden_container.png"));

    sampler = std::make_unique<Sampler>();

    imgui_info.quads[0].pos = {600.0f, 70.0f};
    imgui_info.quads[0].dims = {1000.0f, 100.0f};

    imgui_info.quads[1].pos = {600.0f, 1000.0f};
    imgui_info.quads[1].dims = {1000.0f, 100.0f};

    imgui_info.quads[2].pos = {170.0f, 300.0f};
    imgui_info.quads[2].dims = {100.0f, 1600.0f};

    imgui_info.quads[3].pos = {1000.0f, 300.0f};
    imgui_info.quads[3].dims = {100.0f, 1600.0f};

    imgui_info.src_value = GL_SRC_ALPHA;
    imgui_info.dst_value = GL_ONE_MINUS_SRC_ALPHA;
}

void Blending_Demo::make_data(float tex_coord_scale, const std::array<float, 4>& color)
{
    peria::log(tex_coord_scale);
    peria::log(color[3]);
    colors::Color<float> c {color[0], color[1], color[2], color[3]};

    std::vector<vertex::Vertex2d> data {
        {{-0.5f, -0.5f}, glm::vec2{tex_coord_scale*0.0f, tex_coord_scale*0.0f}, c, 0},
        {{-0.5f,  0.5f}, glm::vec2{tex_coord_scale*0.0f, tex_coord_scale*1.0f}, c, 0},
        {{ 0.5f,  0.5f}, glm::vec2{tex_coord_scale*1.0f, tex_coord_scale*1.0f}, c, 0},
        {{ 0.5f, -0.5f}, glm::vec2{tex_coord_scale*1.0f, tex_coord_scale*0.0f}, c, 0}
    };

    vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(data);
    std::vector<u32> indices {0,1,2, 0,2,3};
    ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);
    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(vertex::Vertex2d));
    vao->connect_index_buffer(ibo->buffer_id());
}

void Blending_Demo::update()
{ 
    glEnable(GL_BLEND);
    glBlendFunc(imgui_info.src_value, imgui_info.dst_value);
    const auto& [r,g,b,a] = imgui_info.constant_color;
    glBlendColor(r,g,b,a);
}

void Blending_Demo::render()
{
    peria::graphics::clear_named_buffer(0, colors::GREY, 1.0f, 0);
    vao->bind();
    quad_shader->use_shader();
    bind_texture_and_sampler(white_texture.get(), sampler.get(), 0);

    for (const auto& q:imgui_info.quads) {
        make_data(imgui_info.tex_coords_scale, q.color);
        const auto model {get_model_mat({
                q.dims[0], q.dims[1], 1.0f,
                0.0f, 0.0f, 0.0f,
                q.pos[0], q.pos[1], 0.0f})};
        quad_shader->set_mat4("u_mvp", projection*model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Blending_Demo::imgui()
{
    //if (ImGui::Button("next texture1")) {
    //    tex_index_1 = (tex_index_1+1)%textures.size();
    //}
    //if (ImGui::Button("next texture2")) {
    //    tex_index_2 = (tex_index_2+1)%textures.size();
    //}

    const auto dims {graphics::get_screen_dimensions()};
    {
        i32 i {1};
        for (auto& q:imgui_info.quads) {
            auto name {"pos_"+std::to_string(i)};
            ImGui::SliderFloat2(name.c_str(), q.pos.data(), 0.0f, std::max(dims.x, dims.y), "%.3f", ImGuiSliderFlags_AlwaysClamp);
            name = "dims_"+std::to_string(i);
            ImGui::SliderFloat2(name.c_str(), q.dims.data(), 0.0f, std::max(dims.x, dims.y), "%.3f", ImGuiSliderFlags_AlwaysClamp);
            name = "color_"+std::to_string(i);
            ImGui::ColorPicker4(name.c_str(), q.color.data());
            ++i;
        }
    }
    
    ImGui::SliderFloat("TexCoordScale", &imgui_info.tex_coords_scale, -5.0f, 5.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::ColorPicker4("ConstantColor", imgui_info.constant_color.data());

    if (ImGui::Button("SRC")) {
        imgui_info.src = !imgui_info.src;
    }
    if (imgui_info.src) {
        if (ImGui::Button("GL_ZERO")) {
            imgui_info.src_value = GL_ZERO;
        }
        if (ImGui::Button("GL_ONE")) {
            imgui_info.src_value = GL_ONE;
        }
        if (ImGui::Button("GL_SRC_COLOR")) {
            imgui_info.src_value = GL_SRC_COLOR;
        }
        if (ImGui::Button("GL_ONE_MINUS_SRC_COLOR")) {
            imgui_info.src_value = GL_ONE_MINUS_SRC_COLOR;
        }
        if (ImGui::Button("GL_DST_COLOR")) {
            imgui_info.src_value = GL_DST_COLOR;
        }
        if (ImGui::Button("GL_ONE_MINUS_DST_COLOR")) {
            imgui_info.src_value = GL_ONE_MINUS_DST_COLOR;
        }
        if (ImGui::Button("GL_SRC_ALPHA")) {
            imgui_info.src_value = GL_SRC_ALPHA;
        }
        if (ImGui::Button("GL_ONE_MINUS_SRC_ALPHA")) {
            imgui_info.src_value = GL_ONE_MINUS_SRC_ALPHA;
        }
        if (ImGui::Button("GL_DST_ALPHA")) {
            imgui_info.src_value = GL_DST_ALPHA;
        }
        if (ImGui::Button("GL_ONE_MINUS_DST_ALPHA")) {
            imgui_info.src_value = GL_ONE_MINUS_DST_ALPHA;
        }
        if (ImGui::Button("GL_CONSTANT_COLOR")) {
            imgui_info.src_value = GL_CONSTANT_COLOR;
        }
        if (ImGui::Button("GL_ONE_MINUS_CONSTANT_COLOR")) {
            imgui_info.src_value = GL_ONE_MINUS_CONSTANT_COLOR;
        }
        if (ImGui::Button("GL_CONSTANT_ALPHA")) {
            imgui_info.src_value = GL_CONSTANT_ALPHA;
        }
        if (ImGui::Button("GL_ONE_MINUS_CONSTANT_ALPHA")) {
            imgui_info.src_value = GL_ONE_MINUS_CONSTANT_ALPHA;
        }
    }

    if (ImGui::Button("DST")) {
        imgui_info.dst = !imgui_info.dst;
    }
    if (imgui_info.dst) {
        if (ImGui::Button("GL_ZERO")) {
            imgui_info.dst_value = GL_ZERO;
        }
        if (ImGui::Button("GL_ONE")) {
            imgui_info.dst_value = GL_ONE;
        }
        if (ImGui::Button("GL_SRC_COLOR")) {
            imgui_info.dst_value = GL_SRC_COLOR;
        }
        if (ImGui::Button("GL_ONE_MINUS_SRC_COLOR")) {
            imgui_info.dst_value = GL_ONE_MINUS_SRC_COLOR;
        }
        if (ImGui::Button("GL_DST_COLOR")) {
            imgui_info.dst_value = GL_DST_COLOR;
        }
        if (ImGui::Button("GL_ONE_MINUS_DST_COLOR")) {
            imgui_info.dst_value = GL_ONE_MINUS_DST_COLOR;
        }
        if (ImGui::Button("GL_SRC_ALPHA")) {
            imgui_info.dst_value = GL_SRC_ALPHA;
        }
        if (ImGui::Button("GL_ONE_MINUS_SRC_ALPHA")) {
            imgui_info.dst_value = GL_ONE_MINUS_SRC_ALPHA;
        }
        if (ImGui::Button("GL_DST_ALPHA")) {
            imgui_info.dst_value = GL_DST_ALPHA;
        }
        if (ImGui::Button("GL_ONE_MINUS_DST_ALPHA")) {
            imgui_info.dst_value = GL_ONE_MINUS_DST_ALPHA;
        }
        if (ImGui::Button("GL_CONSTANT_COLOR")) {
            imgui_info.dst_value = GL_CONSTANT_COLOR;
        }
        if (ImGui::Button("GL_ONE_MINUS_CONSTANT_COLOR")) {
            imgui_info.dst_value = GL_ONE_MINUS_CONSTANT_COLOR;
        }
        if (ImGui::Button("GL_CONSTANT_ALPHA")) {
            imgui_info.dst_value = GL_CONSTANT_ALPHA;
        }
        if (ImGui::Button("GL_ONE_MINUS_CONSTANT_ALPHA")) {
            imgui_info.dst_value = GL_ONE_MINUS_CONSTANT_ALPHA;
        }
    }
}

Blending_Windows_Demo::Blending_Windows_Demo()
    :Demo3d{}
{
    window = Asset_Manager::instance()->fetch_texture("./assets/textures/transparent_window.png");
    chiti = Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png");
    floor = std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(colors::TEAL));
    sampler = std::make_unique<Sampler>();
    light_source_shader = Asset_Manager::instance()->fetch_shader("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl");

    {
        positions.emplace_back(glm::vec3{1.0f, 0.252f, 1.0f});
        positions.emplace_back(glm::vec3{2.0f, 0.252f, -1.0f});
        positions.emplace_back(glm::vec3{1.70f, 0.252f, -1.9f});
    }

    {
        windows.emplace_back(Window_Transforms{glm::vec3{1.0f, 0.252f, 1.002f}, glm::vec2{0.5f, 0.5f}});
        windows.emplace_back(Window_Transforms{glm::vec3{0.0f, 0.552f, 2.0f}, glm::vec2{1.0f, 1.0f}});
        windows.emplace_back(Window_Transforms{glm::vec3{0.3f, 0.352f, 0.0f}, glm::vec2{0.5f, 0.5f}});
        windows.emplace_back(Window_Transforms{glm::vec3{2.0f, 0.252f, -1.0f+0.002f}, glm::vec2{0.5f, 0.5f}});
        windows.emplace_back(Window_Transforms{glm::vec3{1.6f, 0.252f, -3.0f}, glm::vec2{0.5f, 0.5f}});
    }

    window_vao = std::make_unique<Vertex_Array>();
    std::vector<vertex::Vertex3d> window_data {
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    };

    window_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex3d>>(window_data);

    window_vao->setup_attribute(Attribute<float>{3, false});
    window_vao->setup_attribute(Attribute<float>{3, false});
    window_vao->setup_attribute(Attribute<float>{2, false});
    window_vao->connect_vertex_buffer(window_vbo->buffer_id(), sizeof(vertex::Vertex3d));

    glEnable(GL_BLEND);
}

void Blending_Windows_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GREY, 1, 0.0f);

    default_vao->bind();

    default_shader->use_shader();
    { // draw floor
        const auto model {get_model_mat(
            {500.0f, 0.5f, 500.0f,
             0.0f, 0.0f, 0.0f,
             0.0f, -0.252f, 0.0f})};
        default_shader->set_mat4("u_vp", projection*camera.get_view());
        default_shader->set_mat4("u_model", model);
        bind_texture_and_sampler(floor.get(), sampler.get(), 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    {
        for (const auto& pos:positions) {
             const auto model {get_model_mat(
                {0.5f, 0.5f, 0.5f,
                 0.0f, 0.0f, 0.0f,
                 pos.x, pos.y, pos.z})};
            default_shader->set_mat4("u_vp", projection*camera.get_view());
            default_shader->set_mat4("u_model", model);
            bind_texture_and_sampler(chiti, sampler.get(), 0);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    if (debug_mode) {
        light_source_shader->use_shader();
        default_vao->bind();

        // draw center arrows of the world. Can reuse light_source shader
        const auto k {500.0f};
        const auto r {0.02f};
        const auto x_axis_model {get_model_mat({
                k, r, r,
                0.0f, 0.0f, 0.0f,
                k*0.5f + r, r*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*x_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {1.0f, 0.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto y_axis_model {get_model_mat({
                r, k, r,
                0.0f, 0.0f, 0.0f,
                r*0.5f, k*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*y_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 1.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto z_axis_model {get_model_mat({
                r, r, k,
                0.0f, 0.0f, 0.0f,
                r*0.5f, r*0.5f, -k*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*z_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 0.0f, 1.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    {
        glEnable(GL_BLEND);
        {
            default_shader->use_shader();
            window_vao->bind();
            for (const auto& w:windows) {
                 const auto model {get_model_mat(
                    {w.scale.x, w.scale.y, 0.5f,
                     0.0f, 0.0f, 0.0f,
                     w.pos.x, w.pos.y, w.pos.z})};
                default_shader->set_mat4("u_vp", projection*camera.get_view());
                default_shader->set_mat4("u_model", model);
                bind_texture_and_sampler(window, sampler.get(), 0);
                
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        // draw crosshair here
        glDisable(GL_DEPTH_TEST);
        quad_vao->bind();
        quad_shader->use_shader();
        bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

        const auto d {peria::graphics::get_screen_dimensions()};
        const auto crosshair_model {get_model_mat(
            {32.0f, 32.0f, 1.0f,
             0.0f, 0.0f, 0.0f,
             d.x*0.5f, d.y*0.5f, 0.0f})};

        quad_shader->set_mat4("u_mvp", ortho_projection*crosshair_model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
}

void Blending_Windows_Demo::update()
{
    auto comp {[this](const Window_Transforms& a, const Window_Transforms& b) {
            const auto campos {camera.get_pos()};
            const auto dis1 {glm::length(a.pos-campos)};
            const auto dis2 {glm::length(b.pos-campos)};

            return dis1 > dis2;
    }};
    std::sort(windows.begin(), windows.end(), comp);

    const auto im {Input_Manager::instance()};
    if (im->key_pressed(SDL_SCANCODE_F2)) {
        debug_mode = !debug_mode;
    }
}

void Blending_Windows_Demo::imgui()
{}

Face_Culling_Demo::Face_Culling_Demo()
    :Demo3d{}
{
    chiti = Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png");
    sampler = std::make_unique<Sampler>();
    light_source_shader = Asset_Manager::instance()->fetch_shader("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl");

    glEnable(GL_BLEND);
}

void Face_Culling_Demo::render()
{

    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GREY, 1, 0.0f);
    default_vao->bind();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
    default_shader->use_shader();
    {
        const auto model {get_model_mat(
            {3.5f, 3.5f, 3.5f,
             0.0f, 0.0f, 0.0f,
             4.0f, 5.0f, -5.0f})};
        default_shader->set_mat4("u_vp", projection*camera.get_view());
        default_shader->set_mat4("u_model", model);
        bind_texture_and_sampler(chiti, sampler.get(), 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    if (debug_mode) {
        light_source_shader->use_shader();
        default_vao->bind();

        // draw center arrows of the world. Can reuse light_source shader
        const auto k {500.0f};
        const auto r {0.02f};
        const auto x_axis_model {get_model_mat({
                k, r, r,
                0.0f, 0.0f, 0.0f,
                k*0.5f + r, r*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*x_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {1.0f, 0.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto y_axis_model {get_model_mat({
                r, k, r,
                0.0f, 0.0f, 0.0f,
                r*0.5f, k*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*y_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 1.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto z_axis_model {get_model_mat({
                r, r, k,
                0.0f, 0.0f, 0.0f,
                r*0.5f, r*0.5f, -k*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*z_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 0.0f, 1.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    {
        // draw crosshair here
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        quad_vao->bind();
        quad_shader->use_shader();
        bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

        const auto d {peria::graphics::get_screen_dimensions()};
        const auto crosshair_model {get_model_mat(
            {32.0f, 32.0f, 1.0f,
             0.0f, 0.0f, 0.0f,
             d.x*0.5f, d.y*0.5f, 0.0f})};

        quad_shader->set_mat4("u_mvp", ortho_projection*crosshair_model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
}

void Face_Culling_Demo::update()
{
    const auto im {Input_Manager::instance()};
    if (im->key_pressed(SDL_SCANCODE_F2)) {
        debug_mode = !debug_mode;
    }
}

void Face_Culling_Demo::imgui()
{}

Frame_Buffer_Demo::Frame_Buffer_Demo()
    :Demo3d{},
     chiti{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")},
     light_source_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl")},
     screen_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/screen_vertex.glsl", "./assets/shaders/postprocessing_fragment.glsl")},
     screen_vao{std::make_unique<Vertex_Array>()}
{
    {
        std::vector<vertex::Vertex2d> screen_rect_data {
            // we don't care about color and tex_slot. I am just reusing Vertex2D struct
            {{-1.0f, -1.0f}, {0.0f, 0.0f}, colors::WHITE, 0},
            {{-1.0f,  1.0f}, {0.0f, 1.0f}, colors::WHITE, 0},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}, colors::WHITE, 0},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}, colors::WHITE, 0}
            //{{-0.5f, -0.5f}, {0.0f, 0.0f}, colors::WHITE, 0},
            //{{-0.5f,  0.5f}, {0.0f, 1.0f}, colors::WHITE, 0},
            //{{ 0.5f,  0.5f}, {1.0f, 1.0f}, colors::WHITE, 0},
            //{{ 0.5f, -0.5f}, {1.0f, 0.0f}, colors::WHITE, 0}
        };

        screen_quad_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(screen_rect_data);
        //pos
        screen_vao->setup_attribute(Attribute<float>{2, false});
        // tex coords
        screen_vao->setup_attribute(Attribute<float>{2, false});
        screen_vao->connect_vertex_buffer(screen_quad_vbo->buffer_id(), sizeof(vertex::Vertex2d));
        screen_vao->connect_index_buffer(quad_ibo->buffer_id());
    }

    sampler = std::make_unique<Sampler>();
    const auto screen_dims {graphics::get_screen_dimensions()};
    fbo = std::make_unique<Frame_Buffer>(screen_dims.x, screen_dims.y);
    old_screen_dimensions = screen_dims;

    imgui_info.offset_x = 1.0f/screen_dims.x;
    imgui_info.offset_y = 1.0f/screen_dims.y;
}

void Frame_Buffer_Demo::render()
{
    // bind separate frame buffer and draw to it.
    fbo->bind();
    const auto fbo_dims {fbo->dimensions()};
    peria::graphics::set_viewport(0, 0, fbo_dims.x, fbo_dims.y);
    peria::graphics::clear_named_buffer(fbo->fbo_id(), peria::graphics::colors::PINK, 1.0f, 0);

    default_vao->bind();
    default_shader->use_shader();
    {
        const auto model {get_model_mat(
            {3.5f, 3.5f, 3.5f,
             0.0f, 0.0f, 0.0f,
             4.0f, 5.0f, -5.0f})};
        default_shader->set_mat4("u_vp", projection*camera.get_view());
        default_shader->set_mat4("u_model", model);
        bind_texture_and_sampler(chiti, sampler.get(), 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    if (debug_mode) {
        light_source_shader->use_shader();
        default_vao->bind();

        // draw center arrows of the world. Can reuse light_source shader
        const auto k {500.0f};
        const auto r {0.02f};
        const auto x_axis_model {get_model_mat({
                k, r, r,
                0.0f, 0.0f, 0.0f,
                k*0.5f + r, r*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*x_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {1.0f, 0.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto y_axis_model {get_model_mat({
                r, k, r,
                0.0f, 0.0f, 0.0f,
                r*0.5f, k*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*y_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 1.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto z_axis_model {get_model_mat({
                r, r, k,
                0.0f, 0.0f, 0.0f,
                r*0.5f, r*0.5f, -k*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*z_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 0.0f, 1.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    {
        // draw crosshair here
        glDisable(GL_DEPTH_TEST);
        quad_vao->bind();
        quad_shader->use_shader();
        bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

        //const auto d {peria::graphics::get_screen_dimensions()};
        const auto crosshair_model {get_model_mat(
            {32.0f, 32.0f, 1.0f,
             0.0f, 0.0f, 0.0f,
             fbo_dims.x*0.5f, fbo_dims.y*0.5f, 0.0f})};

        quad_shader->set_mat4("u_mvp", ortho_projection*crosshair_model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glEnable(GL_DEPTH_TEST);
    }

    // bind default framebuffer
    {
        glDisable(GL_DEPTH_TEST);
        const auto screen_dims {peria::graphics::get_screen_dimensions()};
        peria::graphics::set_viewport(0, 0, screen_dims.x, screen_dims.y);
        peria::graphics::clear_named_buffer(0, peria::graphics::colors::WHITE, 1.0f, 0);
        peria::graphics::bind_default_frame_buffer();
        
        screen_vao->bind();
        peria::graphics::bind_texture_and_sampler(fbo->texture(), sampler.get(), 0);
        screen_shader->use_shader();
        screen_shader->set_float("u_offset_x", imgui_info.offset_x);
        screen_shader->set_float("u_offset_y", imgui_info.offset_y);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glEnable(GL_DEPTH_TEST);
    }
}

void Frame_Buffer_Demo::update()
{
    const auto im {Input_Manager::instance()};
    if (im->key_pressed(SDL_SCANCODE_F2)) {
        debug_mode = !debug_mode;
    }
    
    const auto screen_dims {peria::graphics::get_screen_dimensions()};
    if (screen_dims != old_screen_dimensions) {
        fbo = std::make_unique<Frame_Buffer>(screen_dims.x, screen_dims.y);
    }
    old_screen_dimensions = screen_dims;
}

void Frame_Buffer_Demo::imgui()
{
    if (ImGui::Button("Reset Offsets")) {
        const auto screen_dims {peria::graphics::get_screen_dimensions()};
        imgui_info.offset_x = 1.0f / screen_dims.x;
        imgui_info.offset_y = 1.0f / screen_dims.y;
    }
    ImGui::SliderFloat("offset_x", &imgui_info.offset_x, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("offset_y", &imgui_info.offset_y, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
}

Frame_Buffer_Rear_View_Demo::Frame_Buffer_Rear_View_Demo()
    :Demo3d{},
     chiti{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")},
     xD{Asset_Manager::instance()->fetch_texture("./assets/textures/xD.png")},
     light_source_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl")},
     screen_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/screen_vertex.glsl", "./assets/shaders/screen_fragment.glsl")},
     screen_vao{std::make_unique<Vertex_Array>()}
{
    {
        std::vector<vertex::Vertex2d> screen_rect_data {
            // we don't care about color and tex_slot. I am just reusing Vertex2D struct
            {{-1.0f, -1.0f}, {0.0f, 0.0f}, colors::WHITE, 0},
            {{-1.0f,  1.0f}, {0.0f, 1.0f}, colors::WHITE, 0},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}, colors::WHITE, 0},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}, colors::WHITE, 0}
        };

        screen_quad_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(screen_rect_data);
        //pos
        screen_vao->setup_attribute(Attribute<float>{2, false});
        // tex coords
        screen_vao->setup_attribute(Attribute<float>{2, false});
        screen_vao->connect_vertex_buffer(screen_quad_vbo->buffer_id(), sizeof(vertex::Vertex2d));
        screen_vao->connect_index_buffer(quad_ibo->buffer_id());

        std::vector<vertex::Vertex2d> rear_view_rect_data {
            {{0.75f, 0.75f}, {0.0f, 0.0f}, colors::WHITE, 0},
            {{0.75f, 1.0f},  {0.0f, 1.0f}, colors::WHITE, 0},
            {{1.0f,  1.0f},  {1.0f, 1.0f}, colors::WHITE, 0},
            {{1.0f,  0.75f}, {1.0f, 0.0f}, colors::WHITE, 0},
        };

        rear_view_vao = std::make_unique<Vertex_Array>();
        rear_view_quad_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(rear_view_rect_data);
        //pos
        rear_view_vao->setup_attribute(Attribute<float>{2, false});
        // tex coords
        rear_view_vao->setup_attribute(Attribute<float>{2, false});
        rear_view_vao->connect_vertex_buffer(rear_view_quad_vbo->buffer_id(), sizeof(vertex::Vertex2d));
        rear_view_vao->connect_index_buffer(quad_ibo->buffer_id());
    }

    sampler = std::make_unique<Sampler>();
    const auto screen_dims {graphics::get_screen_dimensions()};
    main_scene_fbo = std::make_unique<Frame_Buffer>(screen_dims.x, screen_dims.y);

    rear_view_fbo = std::make_unique<Frame_Buffer>(0.25f*screen_dims.x, 0.25f*screen_dims.y);
}

void Frame_Buffer_Rear_View_Demo::render()
{
    // bind separate frame buffer and draw to it.
    main_scene_fbo->bind();
    const auto fbo_dims {main_scene_fbo->dimensions()};
    peria::graphics::set_viewport(0, 0, fbo_dims.x, fbo_dims.y);
    peria::graphics::clear_named_buffer(main_scene_fbo->fbo_id(), peria::graphics::colors::PINK, 1.0f, 0);

    { // draw ones for main scene
        default_vao->bind();
        default_shader->use_shader();
        {
            auto model {get_model_mat(
                {3.5f, 3.5f, 3.5f,
                 0.0f, 0.0f, 0.0f,
                 4.0f, 5.0f, -5.0f})};
            default_shader->set_mat4("u_vp", projection*camera.get_view());
            default_shader->set_mat4("u_model", model);
            bind_texture_and_sampler(chiti, sampler.get(), 0);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);

            bind_texture_and_sampler(xD, sampler.get(), 0);
            model = get_model_mat(
                {3.5f, 3.5f, 3.5f,
                 0.0f, 0.0f, 0.0f,
                 4.0f, 5.0f, 5.0f});
            default_shader->set_mat4("u_model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        if (debug_mode) {
            light_source_shader->use_shader();
            default_vao->bind();

            // draw center arrows of the world. Can reuse light_source shader
            const auto k {500.0f};
            const auto r {0.02f};
            const auto x_axis_model {get_model_mat({
                    k, r, r,
                    0.0f, 0.0f, 0.0f,
                    k*0.5f + r, r*0.5f, r*0.5f})};
            light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*x_axis_model);
            light_source_shader->set_vec3("u_light_source_color", {1.0f, 0.0f, 0.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);

            const auto y_axis_model {get_model_mat({
                    r, k, r,
                    0.0f, 0.0f, 0.0f,
                    r*0.5f, k*0.5f, r*0.5f})};
            light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*y_axis_model);
            light_source_shader->set_vec3("u_light_source_color", {0.0f, 1.0f, 0.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);

            const auto z_axis_model {get_model_mat({
                    r, r, k,
                    0.0f, 0.0f, 0.0f,
                    r*0.5f, r*0.5f, -k*0.5f})};
            light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*z_axis_model);
            light_source_shader->set_vec3("u_light_source_color", {0.0f, 0.0f, 1.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {
            // draw crosshair here
            glDisable(GL_DEPTH_TEST);
            quad_vao->bind();
            quad_shader->use_shader();
            bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

            //const auto d {peria::graphics::get_screen_dimensions()};
            const auto crosshair_model {get_model_mat(
                {32.0f, 32.0f, 1.0f,
                 0.0f, 0.0f, 0.0f,
                 fbo_dims.x*0.5f, fbo_dims.y*0.5f, 0.0f})};

            quad_shader->set_mat4("u_mvp", ortho_projection*crosshair_model);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glEnable(GL_DEPTH_TEST);
        }
    }

    rear_view_fbo->bind();
    const auto rear_fbo_dims {main_scene_fbo->dimensions()};
    peria::graphics::set_viewport(0, 0, rear_fbo_dims.x, rear_fbo_dims.y);
    peria::graphics::clear_named_buffer(rear_view_fbo->fbo_id(), peria::graphics::colors::PLUM, 1.0f, 0);
    { // draw to rear view
        glm::mat4 mm {glm::rotate(glm::mat4{1.0f}, glm::radians(180.0f), glm::vec3{0.0f, 1.0f, 0.0f})*camera.get_view()};

        default_vao->bind();
        default_shader->use_shader();
        {
            auto model {get_model_mat(
                {3.5f, 3.5f, 3.5f,
                 0.0f, 0.0f, 0.0f,
                 4.0f, 5.0f, -5.0f})};
            default_shader->set_mat4("u_vp", projection*mm);
            default_shader->set_mat4("u_model", model);
            bind_texture_and_sampler(chiti, sampler.get(), 0);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);

            bind_texture_and_sampler(xD, sampler.get(), 0);
            model = get_model_mat(
                {3.5f, 3.5f, 3.5f,
                 0.0f, 0.0f, 0.0f,
                 4.0f, 5.0f, 5.0f});
            default_shader->set_mat4("u_model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        if (debug_mode) {
            light_source_shader->use_shader();
            default_vao->bind();

            // draw center arrows of the world. Can reuse light_source shader
            const auto k {500.0f};
            const auto r {0.02f};
            const auto x_axis_model {get_model_mat({
                    k, r, r,
                    0.0f, 0.0f, 0.0f,
                    k*0.5f + r, r*0.5f, r*0.5f})};
            light_source_shader->set_mat4("u_mvp", projection*mm*x_axis_model);
            light_source_shader->set_vec3("u_light_source_color", {1.0f, 0.0f, 0.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);

            const auto y_axis_model {get_model_mat({
                    r, k, r,
                    0.0f, 0.0f, 0.0f,
                    r*0.5f, k*0.5f, r*0.5f})};
            light_source_shader->set_mat4("u_mvp", projection*mm*y_axis_model);
            light_source_shader->set_vec3("u_light_source_color", {0.0f, 1.0f, 0.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);

            const auto z_axis_model {get_model_mat({
                    r, r, k,
                    0.0f, 0.0f, 0.0f,
                    r*0.5f, r*0.5f, -k*0.5f})};
            light_source_shader->set_mat4("u_mvp", projection*mm*z_axis_model);
            light_source_shader->set_vec3("u_light_source_color", {0.0f, 0.0f, 1.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {
            // draw crosshair here
            glDisable(GL_DEPTH_TEST);
            quad_vao->bind();
            quad_shader->use_shader();
            bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

            //const auto d {peria::graphics::get_screen_dimensions()};
            const auto crosshair_model {get_model_mat(
                {32.0f, 32.0f, 1.0f,
                 0.0f, 0.0f, 0.0f,
                 fbo_dims.x*0.5f, fbo_dims.y*0.5f, 0.0f})};

            quad_shader->set_mat4("u_mvp", ortho_projection*crosshair_model);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glEnable(GL_DEPTH_TEST);
        }
    }

    // bind default framebuffer
    {
        glDisable(GL_DEPTH_TEST);
        const auto screen_dims {peria::graphics::get_screen_dimensions()};
        peria::graphics::set_viewport(0, 0, screen_dims.x, screen_dims.y);
        peria::graphics::clear_named_buffer(0, peria::graphics::colors::WHITE, 1.0f, 0);
        peria::graphics::bind_default_frame_buffer();
        
        screen_vao->bind();
        peria::graphics::bind_texture_and_sampler(main_scene_fbo->texture(), sampler.get(), 0);
        screen_shader->use_shader();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        rear_view_vao->bind();
        peria::graphics::bind_texture_and_sampler(rear_view_fbo->texture(), sampler.get(), 0);
        screen_shader->use_shader();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glEnable(GL_DEPTH_TEST);
    }
}

void Frame_Buffer_Rear_View_Demo::update()
{
    const auto im {Input_Manager::instance()};
    if (im->key_pressed(SDL_SCANCODE_F2)) {
        debug_mode = !debug_mode;
    }

    const auto screen_dims {peria::graphics::get_screen_dimensions()};
    if (screen_dims != old_screen_dimensions) {
        main_scene_fbo = std::make_unique<Frame_Buffer>(screen_dims.x, screen_dims.y);
        rear_view_fbo = std::make_unique<Frame_Buffer>(0.75f*screen_dims.x, 0.75f*screen_dims.y);
    }
    old_screen_dimensions = screen_dims;
}

void Frame_Buffer_Rear_View_Demo::imgui()
{}

Sky_Box_Demo::Sky_Box_Demo()
    :Demo3d{},
     chiti{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")},
     light_source_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl")},
     sky_box_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/sky_box_vertex.glsl", "./assets/shaders/sky_box_fragment.glsl")},
     reflection_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/reflection_vertex.glsl", "./assets/shaders/reflection_fragment.glsl")}
{
    sampler = std::make_unique<Sampler>();
    sampler_skybox= std::make_unique<Sampler>(2);
    std::vector<const char*> face_paths {
        "./assets/textures/skyboxes/ocean/right.jpg",
        "./assets/textures/skyboxes/ocean/left.jpg",
        "./assets/textures/skyboxes/ocean/top.jpg",
        "./assets/textures/skyboxes/ocean/bottom.jpg",
        "./assets/textures/skyboxes/ocean/front.jpg",
        "./assets/textures/skyboxes/ocean/back.jpg",
    };
    sky_box_texture = std::make_unique<Texture_Cubemap>(face_paths);
    sky_box_vao = std::make_unique<Vertex_Array>();
    sky_box_vbo = std::make_unique<Named_Buffer_Object<glm::vec3>>(sky_box_cube_model);

    // pos only. pos = direction vector for sampling texture coords of cubemap
    sky_box_vao->setup_attribute(Attribute<float>(3, false));
    sky_box_vao->connect_vertex_buffer(sky_box_vbo->buffer_id(), sizeof(glm::vec3));
}

void Sky_Box_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GREY, 1.0f, 0);

    default_vao->bind();
    auto model {get_model_mat(
        {1.5f, 1.5f, 1.5f,
         0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f})};
    if (use_reflection_shader) {
        reflection_shader->use_shader();
        reflection_shader->set_mat4("u_vp", projection*camera.get_view());
        reflection_shader->set_mat4("u_model", model);
        reflection_shader->set_vec3("u_camera_pos", camera.get_pos());
        bind_texture_and_sampler(sky_box_texture.get(), sampler_skybox.get(), 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    else {
        default_shader->use_shader();
        default_shader->set_mat4("u_vp", projection*camera.get_view());
        default_shader->set_mat4("u_model", model);
        bind_texture_and_sampler(chiti, sampler.get(), 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    if (debug_mode) {
        light_source_shader->use_shader();
        default_vao->bind();

        // draw center arrows of the world. Can reuse light_source shader
        const auto k {500.0f};
        const auto r {0.02f};
        const auto x_axis_model {get_model_mat({
                k, r, r,
                0.0f, 0.0f, 0.0f,
                k*0.5f + r, r*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*x_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {1.0f, 0.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto y_axis_model {get_model_mat({
                r, k, r,
                0.0f, 0.0f, 0.0f,
                r*0.5f, k*0.5f, r*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*y_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 1.0f, 0.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto z_axis_model {get_model_mat({
                r, r, k,
                0.0f, 0.0f, 0.0f,
                r*0.5f, r*0.5f, -k*0.5f})};
        light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*z_axis_model);
        light_source_shader->set_vec3("u_light_source_color", {0.0f, 0.0f, 1.0f});
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // draw sky box
    {
        glDepthFunc(GL_LEQUAL); // needed for optimzed skybox drawing with z trick

        bind_texture_and_sampler(sky_box_texture.get(), sampler_skybox.get());
        sky_box_vao->bind();
        sky_box_shader->use_shader();
        const auto view_without_translation {glm::mat4(glm::mat3(camera.get_view()))};
        sky_box_shader->set_mat4("u_vp", projection*view_without_translation);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS);
    }

    {
        // draw crosshair here
        glDisable(GL_DEPTH_TEST);
        quad_vao->bind();
        quad_shader->use_shader();
        bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

        const auto d {peria::graphics::get_screen_dimensions()};
        const auto crosshair_model {get_model_mat(
            {32.0f, 32.0f, 1.0f,
             0.0f, 0.0f, 0.0f,
             d.x*0.5f, d.y*0.5f, 0.0f})};

        quad_shader->set_mat4("u_mvp", ortho_projection*crosshair_model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glEnable(GL_DEPTH_TEST);
    }
}

void Sky_Box_Demo::update()
{
    const auto im {Input_Manager::instance()};
    if (im->key_pressed(SDL_SCANCODE_F2)) {
        debug_mode = !debug_mode;
    }

    if (im->key_pressed(SDL_SCANCODE_Q)) {
        use_reflection_shader = !use_reflection_shader;
    }
}

void Sky_Box_Demo::imgui()
{}

Ubo_Demo::Ubo_Demo()
    :Demo3d{},
     shader1{Asset_Manager::instance()->fetch_shader("./assets/shaders/ubo_test_vertex.glsl", "./assets/shaders/ubo_test_fragment.glsl")},
     shader2{Asset_Manager::instance()->fetch_shader("./assets/shaders/ubo_test_vertex.glsl", "./assets/shaders/ubo_test2_fragment.glsl")}
{
    vao = std::make_unique<Vertex_Array>();
    auto cube_data = sky_box_cube_model;
    for (auto& i:cube_data) {
        i *= 0.5f;
    }
    vbo = std::make_unique<Named_Buffer_Object<glm::vec3>>(cube_data);
    vao->setup_attribute(Attribute<float>(3, false));
    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(glm::vec3));
    ubo = std::make_unique<Named_Buffer_Object<float>>(40*sizeof(float));
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo->buffer_id(), 0, 40*sizeof(float));
}

void Ubo_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GRAY, 1.0f, 0);

    default_vao->bind();
    auto model1 {get_model_mat(
        {1.0f, 1.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f})};
    
    ubo->set_sub_data(0, sizeof(glm::mat4), glm::value_ptr(projection));
    ubo->set_sub_data(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.get_view()));
    std::array clrs {
        glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
        glm::vec4{0.0f, 0.5f, 1.0f, 1.0f},
    };
    ubo->set_sub_data(2*sizeof(glm::mat4), 2*sizeof(glm::vec4), clrs.data());

    shader1->use_shader();
    shader1->set_mat4("u_model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    auto model2 {get_model_mat(
        {1.0f, 1.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         3.0f, 0.0f, 0.0f})};
    shader2->use_shader();
    shader2->set_mat4("u_model", model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    if(0) {
        // draw crosshair here
        glDisable(GL_DEPTH_TEST);
        quad_vao->bind();
        quad_shader->use_shader();
        bind_texture_and_sampler(cross_hair_texture, default_sampler.get(), 1);

        const auto d {peria::graphics::get_screen_dimensions()};
        const auto crosshair_model {get_model_mat(
            {32.0f, 32.0f, 1.0f,
             0.0f, 0.0f, 0.0f,
             d.x*0.5f, d.y*0.5f, 0.0f})};

        quad_shader->set_mat4("u_mvp", ortho_projection*crosshair_model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glEnable(GL_DEPTH_TEST);
    }
}

void Ubo_Demo::update()
{
    const auto im {Input_Manager::instance()};
    if (im->key_pressed(SDL_SCANCODE_F2)) {
        debug_mode = !debug_mode;
    }
}

void Ubo_Demo::imgui()
{}

Geometry_Shader_Demo::Geometry_Shader_Demo()
    :Demo3d{},
     shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/geometry_vertex.glsl", "./assets/shaders/geometry_fragment.glsl", "./assets/shaders/geometry_geometry.glsl")}
{
    vao = std::make_unique<Vertex_Array>();
    std::vector<Vert2d> data {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}
    };
    vbo = std::make_unique<Named_Buffer_Object<Vert2d>>(data);
    vao->setup_attribute(Attribute<float>{2, false});
    vao->setup_attribute(Attribute<float>{3, false});
    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(Vert2d));
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void Geometry_Shader_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GRAY, 1.0f, 0);
    vao->bind();
    shader->use_shader();

    glDrawArrays(GL_POINTS, 0, 4);
}

void Geometry_Shader_Demo::update()
{
}

void Geometry_Shader_Demo::imgui()
{}

Geometry_Shader_Explode_Demo::Geometry_Shader_Explode_Demo()
    :Demo3d{},
     shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/explode_vertex.glsl", "./assets/shaders/explode_fragment.glsl", "./assets/shaders/explode_geometry.glsl")},
     chiti{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")}
{
    vao = std::make_unique<Vertex_Array>();
    vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex3d>>(cube_model);
    vao->setup_attribute(Attribute<float>{3, false});
    vao->setup_attribute(Attribute<float>{3, false});
    vao->setup_attribute(Attribute<float>{2, false});
    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(vertex::Vertex3d));

    sampler = std::make_unique<Sampler>();
}

void Geometry_Shader_Explode_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GRAY, 1.0f, 0);
    vao->bind();
    shader->use_shader();

    bind_texture_and_sampler(chiti, sampler.get());
    const auto model {get_model_mat(
        {1.0f, 1.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f})};
    shader->set_mat4("u_vp", projection*camera.get_view());
    shader->set_mat4("u_model", model);
    shader->set_float("u_fake_time", tt);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Geometry_Shader_Explode_Demo::update()
{
    tt += 0.00166f;
}

void Geometry_Shader_Explode_Demo::imgui()
{}

Geometry_Shader_Normals_Demo::Geometry_Shader_Normals_Demo()
    :Demo3d{},
     shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")},
     shader_normal{Asset_Manager::instance()->fetch_shader("./assets/shaders/geometry_normals_vertex.glsl", "./assets/shaders/geometry_normals_fragment.glsl", "./assets/shaders/geometry_normals_geometry.glsl")},
     dragon{std::make_unique<Model>("./assets/models/dragon/dragon.obj")},
     backpack{std::make_unique<Model>("./assets/models/backpack/backpack.obj")}
{
    sampler = std::make_unique<Sampler>();
}

void Geometry_Shader_Normals_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GRAY, 1.0f, 0);

    shader->use_shader();
    const auto model {get_model_mat(
        {1.0f, 1.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f})};
    shader->set_mat4("u_vp", projection*camera.get_view());
    shader->set_mat4("u_model", model);

    if (b) dragon->draw(shader);
    else backpack->draw(shader);

    shader_normal->use_shader();
    shader_normal->set_mat4("u_view", camera.get_view());
    shader_normal->set_mat4("u_projection", projection);
    shader_normal->set_mat4("u_model", model);
    if (b) dragon->draw(shader_normal);
    else backpack->draw(shader_normal);
}

void Geometry_Shader_Normals_Demo::update()
{
    if (Input_Manager::instance()->key_pressed(SDL_SCANCODE_Q)) {
        b = !b;
    }
}

void Geometry_Shader_Normals_Demo::imgui()
{}

Instancing_Demo::Instancing_Demo()
    :Demo3d{},
     instancing_quads_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/instancing_quads_vertex.glsl", "./assets/shaders/instancing_quads_fragment.glsl")},
     model_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")},
     instance_planet_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/instancing_planet_vertex.glsl", "./assets/shaders/instancing_planet_fragment.glsl")}
{
    {
        vao = std::make_unique<Vertex_Array>();
        std::vector<Vert2d> data {
            {{-0.95f, -0.95f}, {1.0f, 0.0f, 0.0f}},
            {{-0.95f, -0.85f}, {0.0f, 1.0f, 0.0f}},
            {{-0.85f, -0.85f}, {0.0f, 0.0f, 1.0f}},
            {{-0.85f, -0.95f}, {0.0f, 0.0f, 0.0f}},
        };

        for (i32 i{0}; i<10; ++i) {
            for (i32 j{1}; j<=10; ++j) {
                offsets.push_back({j*0.15f, i*0.15f});
            }
        }

        vbo = std::make_unique<Named_Buffer_Object<Vert2d>>(data);
        vao->setup_attribute(Attribute<float>{2, false});
        vao->setup_attribute(Attribute<float>{3, false});
        vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(Vert2d));
        vao->connect_index_buffer(quad_ibo->buffer_id());
    }
    sampler = std::make_unique<Sampler>();

    {
        planet = std::make_unique<Model>("./assets/models/planet/planet.obj");
        asteroid = std::make_unique<Model>("./assets/models/rock/rock.obj");
    }

    { // asteroid transforms
        const auto get_random_model {
            [](glm::vec3 pp) {
                const auto angle_x {get_int(0, 1)*get_float(0.0f, 360.0f)};
                const auto angle_y {get_int(0, 1)*get_float(0.0f, 360.0f)};
                const auto angle_z {get_int(0, 1)*get_float(0.0f, 360.0f)};

                glm::vec3 random_dir {};
                const auto a {glm::radians(get_float(0.0f, 360.0f))};
                random_dir.x = std::sin(a);
                random_dir.y = std::sin(a)*get_float(0.2f, 1.0f);
                random_dir.z = std::cos(a);

                auto p {pp + 50.0f*normalize(random_dir)};

                const auto scale {get_float(1.0f, 3.0f)};
                return Transform{
                    scale, scale, scale,
                    angle_x, angle_y, angle_z,
                    p.x, p.y, p.z
                };

            }
        };
        
        const i32 N {50000};
        asteroid_transforms.reserve(N);
        planet_pos = {0.0f, 0.0f, -20.0f};
        for (i32 i{}; i<N; ++i) {
            asteroid_transforms.emplace_back(get_random_model(planet_pos));
        }

        // instancing info
        std::vector<glm::mat4> models(N);
        for (i32 i{}; i<N; ++i) {
            models[i] = get_model_mat(asteroid_transforms[i]);
        }

        model_vbo = std::make_unique<Named_Buffer_Object<glm::mat4>>(models);
        std::cerr << sizeof(glm::mat4)*models.size();
        const auto meshes {asteroid->get_meshes()};
        for (std::size_t j{}; j<meshes.size(); ++j) {
            const auto m {meshes[j]};
            const auto mesh_vao {m->get_vao_ptr()};

            mesh_vao->setup_attribute(Attribute<float>{4, false}, 1);
            mesh_vao->setup_attribute(Attribute<float>{4, false}, 1);
            mesh_vao->setup_attribute(Attribute<float>{4, false}, 1);
            mesh_vao->setup_attribute(Attribute<float>{4, false}, 1);
            mesh_vao->connect_vertex_buffer(model_vbo->buffer_id(), sizeof(glm::mat4), 1);

            glVertexArrayBindingDivisor(mesh_vao->get_id(), 1, 1);
        }
    }
}

void Instancing_Demo::render()
{
    peria::graphics::clear_named_buffer(0, peria::graphics::colors::GRAY, 1.0f, 0);
    
    if (b) { // quads example
        vao->bind();
        instancing_quads_shader->use_shader();

        for (std::size_t i{}; i<offsets.size(); ++i) {
            const auto name {"u_offsets["+std::to_string(i)+"]"};
            instancing_quads_shader->set_vec2(name.c_str(), offsets[i]);
        }
        
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, offsets.size());
    }
    else { // planet example
        { // draw planet
            model_shader->use_shader();
            model_shader->set_mat4("u_vp", projection*camera.get_view());
            const auto model {get_model_mat(
                {3.0f, 3.0f, 3.0f,
                 0.0f, 0.0f, 0.0f,
                 planet_pos.x, planet_pos.y, planet_pos.z})};
            model_shader->set_mat4("u_model", model);

            const auto meshes {planet->get_meshes()}; // quick and dirty way to test this shit
            for (std::size_t i{}; i<meshes.size(); ++i) {
                const auto& m {meshes[i]};
                m->bind_mesh_vao();
                const auto index_count {m->get_index_count()};
                const auto& texture_paths {m->get_texture_paths()}; // guaranteed only 1 texture

                Texture* texture {Asset_Manager::instance()->fetch_texture(texture_paths[0].c_str())};
                bind_texture_and_sampler(texture, sampler.get());

                glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
            }
        }

        if (!draw_instanced) { // draw asteroids
            model_shader->set_mat4("u_vp", projection*camera.get_view());
            const auto meshes {asteroid->get_meshes()};

            for (std::size_t i{}; i<asteroid_transforms.size(); ++i) {
                const auto model {get_model_mat(asteroid_transforms[i])};
                model_shader->set_mat4("u_model", model);

                for (std::size_t j{}; j<meshes.size(); ++j) {
                    const auto& m {meshes[j]};
                    m->bind_mesh_vao();
                    const auto index_count {m->get_index_count()};
                    const auto& texture_paths {m->get_texture_paths()}; // guaranteed only 1 texture

                    Texture* texture {Asset_Manager::instance()->fetch_texture(texture_paths[0].c_str())};
                    bind_texture_and_sampler(texture, sampler.get());

                    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
                }
            }
        }
        else { // draw asteroids instanced
            instance_planet_shader->use_shader();
            instance_planet_shader->set_mat4("u_vp", projection*camera.get_view());
            const auto meshes {asteroid->get_meshes()};

            for (std::size_t j{}; j<meshes.size(); ++j) {
                const auto& m {meshes[j]};
                m->bind_mesh_vao();
                const auto index_count {m->get_index_count()};
                const auto& texture_paths {m->get_texture_paths()}; // guaranteed only 1 texture

                Texture* texture {Asset_Manager::instance()->fetch_texture(texture_paths[0].c_str())};
                bind_texture_and_sampler(texture, sampler.get());

                glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr, asteroid_transforms.size());
            }
        }
    }

}

void Instancing_Demo::update()
{
    if (Input_Manager::instance()->key_pressed(SDL_SCANCODE_Q)) {
        b = !b;
    }

    if (Input_Manager::instance()->key_pressed(SDL_SCANCODE_E)) {
        draw_instanced = !draw_instanced;
    }
}

void Instancing_Demo::imgui()
{}

MSAA_Demo::MSAA_Demo()
    :Demo3d{},
     cube_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")},
     screen_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/screen_vertex.glsl", "./assets/shaders/screen_fragment.glsl")}
{
    vao = std::make_unique<Vertex_Array>();
    vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex3d>>(cube_model);
    vao->setup_attribute(Attribute<float>{3, false});
    vao->setup_attribute(Attribute<float>{3, false});
    vao->setup_attribute(Attribute<float>{2, false});
    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(vertex::Vertex3d));

    // screen vao/vbo
    {
        screen_vao = std::make_unique<Vertex_Array>();
        std::vector<vertex::Vertex2d> data {
            {{-1.0f, -1.0f}, {0.0f, 0.0f}, colors::WHITE, 0},
            {{-1.0f,  1.0f}, {0.0f, 1.0f}, colors::WHITE, 0},
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}, colors::WHITE, 0},
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}, colors::WHITE, 0}
        };
        screen_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex2d>>(data);
        screen_vao->setup_attribute(Attribute<float>{2, false});
        screen_vao->setup_attribute(Attribute<float>{2, false});
        screen_vao->connect_vertex_buffer(screen_vbo->buffer_id(), sizeof(vertex::Vertex2d));
        screen_vao->connect_index_buffer(quad_ibo->buffer_id());
    }

    const auto screen_dims {peria::graphics::get_screen_dimensions()};
    ms_fbo = std::make_unique<Frame_Buffer>(screen_dims.x, screen_dims.y, 4);
    inter_fbo = std::make_unique<Frame_Buffer>(screen_dims.x, screen_dims.y);

    white_texture = std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(colors::WHITE));
    sampler = std::make_unique<Sampler>();
}

void MSAA_Demo::render()
{
    ms_fbo->bind();
    const auto ms_fbo_dims {ms_fbo->dimensions()};
    peria::graphics::set_viewport(0, 0, ms_fbo_dims.x, ms_fbo_dims.y);
    peria::graphics::clear_named_buffer(ms_fbo->fbo_id(), colors::GREY, 1.0f, 0);

    vao->bind();
    cube_shader->use_shader();
    cube_shader->set_mat4("u_vp", projection*camera.get_view());

    const auto model {get_model_mat(
        {2.0f, 2.0f, 2.0f,
         45.0f, 50.0f, 10.0f,
         0.0f, 0.0f, 0.0f})};
    cube_shader->set_mat4("u_model", model);
    bind_texture_and_sampler(white_texture.get(), sampler.get());

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // with this we draw straight to the screen, 
    // but lose access to texture as texture2d in shader for postprocessing
    //glBlitNamedFramebuffer(ms_fbo->fbo_id(), 0, 
    //                       0, 0, ms_fbo_dims.x, ms_fbo_dims.y,
    //                       0, 0, ms_fbo_dims.x, ms_fbo_dims.y,
    //                       GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // dims are same
    glBlitNamedFramebuffer(ms_fbo->fbo_id(), inter_fbo->fbo_id(), 
                           0, 0, ms_fbo_dims.x, ms_fbo_dims.y,
                           0, 0, ms_fbo_dims.x, ms_fbo_dims.y,
                           GL_COLOR_BUFFER_BIT, GL_NEAREST);

    const auto screen_dims {peria::graphics::get_screen_dimensions()};
    peria::graphics::bind_default_frame_buffer();
    peria::graphics::clear_named_buffer(0, colors::GREY, 1.0f, 0);
    peria::graphics::set_viewport(0, 0, screen_dims.x, screen_dims.y);
    bind_texture_and_sampler(inter_fbo->texture(), sampler.get());
    screen_vao->bind();
    screen_shader->use_shader();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void MSAA_Demo::update()
{}

void MSAA_Demo::imgui()
{}

}
