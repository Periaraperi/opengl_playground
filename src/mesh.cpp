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

    white_texture = std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(colors::WHITE));
}

void Mesh::draw(const Shader* const shader)
{
    vao->bind();
    shader->use_shader();

    shader->set_mat4("u_model", glm::mat4{1.0f});

    if (texture_paths.empty()) {
        default_sampler->bind(0);
        white_texture->bind(0);
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
}

}
