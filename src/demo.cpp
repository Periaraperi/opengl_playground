#include "demo.hpp"

#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "vertex_array.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "sampler.hpp"

#include "graphics.hpp"

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

    void bind_texture_and_sampler(const peria::graphics::Texture* const texture, 
                                  const peria::graphics::Sampler* const sampler, u32 unit = 0) noexcept
    {
        texture->bind(unit);
        sampler->bind(unit);
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

Demo::Demo()
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
    peria::graphics::set_clear_buffer_bits(true, true);
    peria::graphics::set_vsync(false);
}

Demo_Point_Light::Demo_Point_Light()
    :Demo{}
{
    { // shaders
        light_source_shader = std::make_unique<Shader>("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl");
        lighting_shader = std::make_unique<Shader>("./assets/shaders/point_light_vertex.glsl", "./assets/shaders/point_light_fragment.glsl");
        lighting_shader->set_int("u_material.diffuse_texture", 0);
        lighting_shader->set_int("u_material.specular_texture", 1);
    }

    light_source.ambient[0] = 0.5f;
    light_source.ambient[1] = 0.5f;
    light_source.ambient[2] = 0.5f;

    light_source.diffuse[0] = colors::WHITE.r;
    light_source.diffuse[1] = colors::WHITE.g;
    light_source.diffuse[2] = colors::WHITE.b;

    light_source.specular[0] = colors::WHITE.r;
    light_source.specular[1] = colors::WHITE.g;
    light_source.specular[2] = colors::WHITE.b;

    light_source.pos[0] = 2.0f;
    light_source.pos[1] = 1.0f;
    light_source.pos[2] = -4.0f;
}

void Demo_Point_Light::render()
{
    bind_texture_and_sampler(texture.get(), sampler.get(), 0);
    bind_texture_and_sampler(specular_texture.get(), sampler.get(), 1);

    light_source_vao->bind();
    light_source_shader->use_shader();

    const auto& [lx, ly, lz] {light_source.pos};
    const auto light_source_model {get_model_mat({
            0.30f, 0.30f, 0.30f, 
            0.0f, 0.0f, 0.0f,
            lx, ly, lz})};

    light_source_shader->set_mat4("u_mvp", projection*camera.get_view()*light_source_model);
    light_source_shader->set_vec3("u_light_source_color", get_vec3(light_source.diffuse));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    vao->bind();

    lighting_shader->use_shader();
    lighting_shader->set_vec3("u_light.pos", {lx, ly, lz});
    lighting_shader->set_vec3("u_view_pos", camera.get_pos());

    // TODO: change this later, keep hardcoded for now
    material.shininess = 32;
    lighting_shader->set_float("u_material.shininess", material.shininess);

    lighting_shader->set_vec3("u_light.ambient", get_vec3(light_source.ambient));
    lighting_shader->set_vec3("u_light.diffuse", get_vec3(light_source.diffuse));
    lighting_shader->set_vec3("u_light.specular", get_vec3(light_source.specular));

    lighting_shader->set_float("u_light.constant", 1.0f);
    lighting_shader->set_float("u_light.linear", light_source.attenuation.linear);
    lighting_shader->set_float("u_light.quadratic", light_source.attenuation.quadratic);

    lighting_shader->set_mat4("u_vp", projection*camera.get_view());

    // all objects are same, just in different positions and orientation
    for (std::size_t i{}; i<positions.size(); ++i) {
        const auto object_model {get_model_mat({
                1.0f, 1.0f, 1.0f,
                rotations[i].x, rotations[i].y, rotations[i].z,
                positions[i].x, positions[i].y, positions[i].z})};
        lighting_shader->set_mat4("u_model", object_model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Demo_Point_Light::update()
{}

void Demo_Point_Light::imgui()
{
    ImGui::Begin("Lighting");

    ImGui::SliderFloat3("LightSourcePosition", light_source.pos.data(), -10.0f, 10.0f);
    ImGui::ColorEdit3("LightSource-Ambient", light_source.ambient.data()); 
    ImGui::ColorEdit3("LightSource-Diffuse", light_source.diffuse.data()); 
    ImGui::ColorEdit3("LightSource-Specular", light_source.specular.data()); 

    ImGui::ColorEdit3("ObjectMaterial-Ambient", material.ambient.data()); 
    ImGui::ColorEdit3("ObjectMaterial-Diffuse", material.diffuse.data()); 
    ImGui::ColorEdit3("ObjectMaterial-Specular", material.specular.data()); 

    if (ImGui::Button("ATT_DISTANCE_7")) {
        light_source.attenuation = ATT_DISTANCE_7;
    }
    if (ImGui::Button("ATT_DISTANCE_13")) {
        light_source.attenuation = ATT_DISTANCE_13;
    }
    if (ImGui::Button("ATT_DISTANCE_20")) {
        light_source.attenuation = ATT_DISTANCE_20;
    }
    if (ImGui::Button("ATT_DISTANCE_32")) {
        light_source.attenuation = ATT_DISTANCE_32;
    }
    if (ImGui::Button("ATT_DISTANCE_50")) {
        light_source.attenuation = ATT_DISTANCE_50;
    }
    if (ImGui::Button("ATT_DISTANCE_65")) {
        light_source.attenuation = ATT_DISTANCE_65;
    }
    if (ImGui::Button("ATT_DISTANCE_100")) {
        light_source.attenuation = ATT_DISTANCE_100;
    }
    if (ImGui::Button("ATT_DISTANCE_160")) {
        light_source.attenuation = ATT_DISTANCE_160;
    }
    if (ImGui::Button("ATT_DISTANCE_200")) {
        light_source.attenuation = ATT_DISTANCE_200;
    }
    if (ImGui::Button("ATT_DISTANCE_325")) {
        light_source.attenuation = ATT_DISTANCE_325;
    }
    if (ImGui::Button("ATT_DISTANCE_600")) {
        light_source.attenuation = ATT_DISTANCE_600;
    }
    if (ImGui::Button("ATT_DISTANCE_3250")) {
        light_source.attenuation = ATT_DISTANCE_3250;
    }

    ImGui::End();
}

Demo_Combined_Lights::Demo_Combined_Lights()
    :Demo{}
{
    { // shaders
        light_source_shader = std::make_unique<Shader>("./assets/shaders/light_source_vertex.glsl", "./assets/shaders/light_source_fragment.glsl");
        combined_lights_shader = std::make_unique<Shader>("./assets/shaders/generic_cube_vertex.glsl", "./assets/shaders/combined_lights_fragment.glsl");
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
{
}

}
