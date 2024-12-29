#include "mesh.hpp"
#include <glm/glm.hpp>

#include "asset_manager.hpp"

namespace {
    [[nodiscard]]
    glm::vec3 get_vec3(const std::array<float, 3>& color) noexcept
    { return {color[0], color[1], color[2]}; }
}

namespace peria::graphics {

Mesh::Mesh(std::vector<vertex::Vertex3d>&& vertex_data_,
           std::vector<u32>&& indices_,
           std::vector<std::string>&& texture_paths_)
    :vertex_data(std::move(vertex_data_)),
     indices(std::move(indices_)),
     default_sampler{std::make_unique<Sampler>(0)},
     texture_paths(std::move(texture_paths_))
{
    vao = std::make_unique<Vertex_Array>();
    vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex3d>>(vertex_data);
    ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

    // pos
    vao->setup_attribute(Attribute<float>{3, false});
    // normals
    vao->setup_attribute(Attribute<float>{3, false});
    // tex coords
    vao->setup_attribute(Attribute<float>{2, false});

    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(vertex::Vertex3d));
    vao->connect_index_buffer(ibo->buffer_id());

    directional_light.direction = {-0.2f, -1.0f, -0.3f};
    directional_light.ambient  = {0.3f, 0.24f, 0.14f};
    directional_light.diffuse  = {0.7f, 0.42f, 0.26f};
    directional_light.specular = {0.5f, 0.5f,  0.5f };

    point_lights.emplace_back(Point_Light{{ 0.7f,  0.2f,  2.0f},  {1.0f*0.1f, 0.6f*0.1f, 0.0f*0.1f}, {1.0f, 0.6f, 0.0f}, {1.0f, 0.6f, 0.0f}, ATT_DISTANCE_50});
    point_lights.emplace_back(Point_Light{{ 2.3f, -3.3f, -4.0f},  {1.0f*0.1f, 0.0f*0.1f, 0.0f*0.1f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, ATT_DISTANCE_100});

    spot_light.angle = 12.5f;
    spot_light.angle = 14.0f;
    spot_light.ambient = {0.0f, 0.0f, 0.0f};
    spot_light.diffuse = {0.8f, 0.8f, 0.0f};
    spot_light.specular = {0.8f, 0.8f, 0.0f};
}

void Mesh::draw(const Shader* const shader, const glm::mat4& projection, const Camera& camera)
{
    vao->bind();
    shader->use_shader();

    shader->set_mat4("u_vp", projection*camera.get_view());
    shader->set_mat4("u_model", glm::mat4{1.0f});

    shader->set_vec3("u_view_pos", camera.get_pos());

    { // directional light
        shader->set_vec3("u_directional_light.direction", get_vec3(directional_light.direction));
        shader->set_vec3("u_directional_light.ambient", get_vec3(directional_light.ambient));
        shader->set_vec3("u_directional_light.diffuse", get_vec3(directional_light.diffuse));
        shader->set_vec3("u_directional_light.specular", get_vec3(directional_light.specular));
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
            shader->set_vec3(pos.c_str(), get_vec3(point_lights[0].pos));
            shader->set_vec3(ambient.c_str(), get_vec3(point_lights[0].ambient));
            shader->set_vec3(diffuse.c_str(), get_vec3(point_lights[0].diffuse));
            shader->set_vec3(specular.c_str(), get_vec3(point_lights[0].specular));
            shader->set_float(linear.c_str(), point_lights[0].attenuation.linear);
            shader->set_float(quadratic.c_str(), point_lights[0].attenuation.quadratic);
        }
    }
    { // spot light
        shader->set_vec3("u_spot_light.pos", camera.get_pos());
        shader->set_vec3("u_spot_light.direction", camera.get_view_direction());
        shader->set_vec3("u_spot_light.ambient", get_vec3(spot_light.ambient));
        shader->set_vec3("u_spot_light.diffuse", get_vec3(spot_light.diffuse));
        shader->set_vec3("u_spot_light.specular", get_vec3(spot_light.specular));
        shader->set_float("u_spot_light.linear", spot_light.attenuation.linear);
        shader->set_float("u_spot_light.quadratic", spot_light.attenuation.quadratic);
        shader->set_float("u_spot_light.inner_cosine_angle", std::cos(glm::radians(spot_light.angle)));
        shader->set_float("u_spot_light.outer_cosine_angle", std::cos(glm::radians(spot_light.outer_angle)));
    }
    
    for(std::size_t i{}; i < texture_paths.size(); ++i) {
        auto texture {Asset_Manager::instance()->fetch_texture(texture_paths[i].c_str())};
        std::string number;
        std::string name {texture->get_type_name()};
        if(name == "u_texture_diffuse")
            shader->set_int("u_material.diffuse_texture", i);
        else if(name == "u_texture_specular")
            shader->set_int("u_material.specular_texture", i);

        default_sampler->bind(i);
        texture->bind(i);
    }
    shader->set_float("u_material.shininess", 64);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    //u32 diffuse_number  {1};
    //u32 specular_number {1};
    //for(std::size_t i{}; i < texture_paths.size(); ++i) {
    //    auto texture {Asset_Manager::instance()->fetch_texture(texture_paths[i].c_str())};
    //    std::string number;
    //    std::string name {texture->get_type_name()};
    //    if(name == "u_texture_diffuse")
    //        number = std::to_string(diffuse_number++);
    //    else if(name == "u_texture_specular")
    //        number = std::to_string(specular_number++);

    //    shader->set_int((name + number).c_str(), i);

    //    default_sampler->bind(i);
    //    texture->bind(i);
    //}

    //if (indices.empty()) {
    //    glDrawArrays(GL_TRIANGLES, 0, 36);
    //}
    //else {
    //    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //}
}

}
