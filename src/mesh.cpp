#include "mesh.hpp"

#include "asset_manager.hpp"

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
}

void Mesh::draw(const Shader* const shader, const glm::mat4& projection, const Camera& camera)
{
    vao->bind();
    shader->use_shader();

    shader->set_mat4("u_mvp", projection*camera.get_view());

    u32 diffuse_number  {1};
    u32 specular_number {1};
    for(std::size_t i{}; i < texture_paths.size(); ++i) {
        auto texture {Asset_Manager::instance()->fetch_texture(texture_paths[i].c_str())};
        std::string number;
        std::string name {texture->get_type_name()};
        if(name == "u_texture_diffuse")
            number = std::to_string(diffuse_number++);
        else if(name == "u_texture_specular")
            number = std::to_string(specular_number++);

        shader->set_int((name + number).c_str(), i);

        default_sampler->bind(i);
        texture->bind(i);
    }

    if (indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}

}
