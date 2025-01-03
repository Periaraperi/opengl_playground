#include "demo.hpp"
#include <memory>

#include <glm/gtc/matrix_transform.hpp>
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

    // random
    std::random_device rd = std::random_device();
    std::mt19937 generator(rd());

    [[nodiscard]]
    int get_int(int l, int r)
    {
        std::uniform_int_distribution<> dist(l, r);
        return dist(rd);
    }

    std::array positions {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    std::array rotations {
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
        glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    };

    struct Transform {
        float sx, sy, sz; // scale values
        float rot_x, rot_y, rot_z; // angle rotation around _axis
        float x, y, z; // translation values
    };

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
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
{
    { // buffers
        light_source_vao = std::make_unique<Vertex_Array>();
        cube_vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex3d>>(cube_model);

        // pos only
        light_source_vao->setup_attribute(Attribute<float>{3, false});
        light_source_vao->connect_vertex_buffer(cube_vbo->buffer_id(), sizeof(vertex::Vertex3d));

        // other objects in lighting scenes.
        vao = std::make_unique<Vertex_Array>();

        // pos
        vao->setup_attribute(Attribute<float>{3, false});
        // normal
        vao->setup_attribute(Attribute<float>{3, false});
        // tex coords
        vao->setup_attribute(Attribute<float>{2, false});
        vao->connect_vertex_buffer(cube_vbo->buffer_id(), sizeof(vertex::Vertex3d));
    }

    { // textures 
        texture = std::make_unique<Texture>("./assets/textures/wooden_container.png");
        specular_texture = std::make_unique<Texture>("./assets/textures/specular_wooden_container.png");
    }

    { // samplers
        sampler = std::make_unique<Sampler>(0);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    peria::graphics::set_clear_buffer_bits(true, true, true);
    peria::graphics::set_vsync(false);
}

Demo_Combined_Lights::Demo_Combined_Lights()
    :Demo3d{}
{
    { // shaders
        light_source_shader = std::make_unique<Shader>("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl");
        combined_lights_shader = std::make_unique<Shader>("./assets/shaders/cube_vertex.glsl", "./assets/shaders/combined_lights_fragment.glsl");
        combined_lights_shader->set_int("u_material.diffuse_texture", 0);
        combined_lights_shader->set_int("u_material.specular_texture", 1);
    }
    
    //std::array<float, 3> ambient {0.2f, 0.2f, 0.2f};
    //std::array<float, 3> diffuse {1.0f, 1.0f, 1.0f};
    //std::array<float, 3> specular {1.0f, 1.0f, 1.0f};

    directional_light.direction = {-0.2f, -1.0f, -0.3f};
    directional_light.ambient  = {0.3f, 0.24f, 0.14f};
    directional_light.diffuse  = {0.7f, 0.42f, 0.26f};
    directional_light.specular = {0.5f, 0.5f,  0.5f };

    point_lights.emplace_back(Point_Light{{ 0.7f,  0.2f,  2.0f},  {1.0f*0.1f, 0.6f*0.1f, 0.0f*0.1f}, {1.0f, 0.6f, 0.0f}, {1.0f, 0.6f, 0.0f}, ATT_DISTANCE_50});
    point_lights.emplace_back(Point_Light{{ 2.3f, -3.3f, -4.0f},  {1.0f*0.1f, 0.0f*0.1f, 0.0f*0.1f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, ATT_DISTANCE_100});
    point_lights.emplace_back(Point_Light{{-4.0f,  2.0f, -12.0f}, {1.0f*0.1f, 1.0f*0.1f, 0.0f*0.1f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, ATT_DISTANCE_100});
    point_lights.emplace_back(Point_Light{{ 0.0f,  0.0f, -3.0f},  {0.2f*0.1f, 0.2f*0.1f, 1.0f*0.1f}, {0.2f, 0.2f, 1.0f}, {0.2f, 0.2f, 1.0f}, ATT_DISTANCE_100});

    spot_light.angle = 12.5f;
    spot_light.angle = 14.0f;
    spot_light.ambient = {0.0f, 0.0f, 0.0f};
    spot_light.diffuse = {0.8f, 0.8f, 0.0f};
    spot_light.specular = {0.8f, 0.8f, 0.0f};
}

void Demo_Combined_Lights::render()
{
    bind_texture_and_sampler(texture.get(), sampler.get(), 0);
    bind_texture_and_sampler(specular_texture.get(), sampler.get(), 1);

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

    vao->bind();

    combined_lights_shader->use_shader();
    combined_lights_shader->set_vec3("u_view_pos", camera.get_pos());

    material.shininess = 32;
    combined_lights_shader->set_float("u_material.shininess", material.shininess);

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
            combined_lights_shader->set_vec3(pos.c_str(), get_vec3(point_lights[0].pos));
            combined_lights_shader->set_vec3(ambient.c_str(), get_vec3(point_lights[0].ambient));
            combined_lights_shader->set_vec3(diffuse.c_str(), get_vec3(point_lights[0].diffuse));
            combined_lights_shader->set_vec3(specular.c_str(), get_vec3(point_lights[0].specular));
            combined_lights_shader->set_float(linear.c_str(), point_lights[0].attenuation.linear);
            combined_lights_shader->set_float(quadratic.c_str(), point_lights[0].attenuation.quadratic);
        }
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

    // all objects are same, just in different positions and orientation
    for (std::size_t i{}; i<positions.size(); ++i) {
        const auto object_model {get_model_mat({
                1.0f, 1.0f, 1.0f,
                rotations[i].x, rotations[i].y, rotations[i].z,
                positions[i].x, positions[i].y, positions[i].z})};
        combined_lights_shader->set_mat4("u_model", object_model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Demo_Combined_Lights::update()
{
    peria::graphics::set_clear_color(peria::graphics::colors::Color{0.75f, 0.52f, 0.3f, 1.0f});
}

void Demo_Combined_Lights::imgui()
{}


Demo_Model::Demo_Model()
    :Demo3d{},
     shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/cube_vertex.glsl", "./assets/shaders/combined_lights_fragment.glsl")}
     //shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")}
{ 
    //model = std::make_unique<peria::graphics::Model>("./assets/models/backpack/backpack.obj"); 
    //model = std::make_unique<peria::graphics::Model>("./assets/models/dragon/dragon.obj");
    model = std::make_unique<peria::graphics::Model>("./assets/models/kek/pirveli_yleoba_xD.obj");
}

void Demo_Model::render()
{
    model->draw(shader, projection, camera);
}

void Demo_Model::update()
{ peria::graphics::set_clear_color(peria::graphics::colors::Color{0.0f, 0.1f, 0.1f, 1.0f}); }

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
    glDepthFunc(GL_LESS);
    vao->bind();

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

    bind_texture_and_sampler(texture2, sampler.get(), 0);
    shader->set_mat4("u_vp", projection*camera.get_view());
    shader->set_mat4("u_model", model_plane);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    bind_texture_and_sampler(texture1, sampler.get(), 0);
    shader->set_mat4("u_model", model_cube1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader->set_mat4("u_model", model_cube2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Demo_Depth_Testing::update()
{ peria::graphics::set_clear_color(peria::graphics::colors::Color{0.4f, 0.3f, 0.3f, 1.0f}); }

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
    vao->bind();

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

    bind_texture_and_sampler(texture2, sampler.get(), 0);
    shader1->set_mat4("u_vp", projection*camera.get_view());
    shader1->set_mat4("u_model", model_plane);
    glStencilMask(0x00);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    bind_texture_and_sampler(texture1, sampler.get(), 0);
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
{ peria::graphics::set_clear_color(peria::graphics::colors::Color{0.4f, 0.3f, 0.3f, 1.0f}); }

void Demo_Stencil_Testing::imgui()
{ }

Another_Demo::Another_Demo()
    :Demo3d{},
     quad_vao{std::make_unique<Vertex_Array>()},
     white_texture{std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(colors::WHITE))},
     quad_shader{Asset_Manager::instance()->fetch_shader("./assets/shaders/quad_vertex.glsl", "./assets/shaders/quad_fragment.glsl")},
     shader1{Asset_Manager::instance()->fetch_shader("./assets/shaders/model_vertex.glsl", "./assets/shaders/model_fragment.glsl")},
     texture1{Asset_Manager::instance()->fetch_texture("./assets/textures/chitunia.png")},
     texture_crosshair{Asset_Manager::instance()->fetch_texture("./assets/textures/cross-hair.png")}
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

    bind_texture_and_sampler(white_texture.get(), sampler.get(), 0);
    bind_texture_and_sampler(texture_crosshair, sampler.get(), 1);

    cubes.emplace_back(glm::vec3{0.0f, 3.0f, -4.0f});
}

void Another_Demo::render()
{
    vao->bind();
    shader1->use_shader();
    auto model_plane {get_model_mat(
        {20.0f, 0.5f, 20.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 0.0f})};

    bind_texture_and_sampler(texture1, sampler.get(), 0);
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

    peria::graphics::set_clear_color(peria::graphics::colors::Color{0.2f, 0.3f, 0.3f, 1.0f});
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
    set_clear_color(colors::GREENYELLOW);
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

}
