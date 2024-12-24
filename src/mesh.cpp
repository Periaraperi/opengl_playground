#include "mesh.hpp"

namespace peria::graphics {

Mesh::Mesh(std::vector<vertex::Vertex3d>&& vertex_data_,
           std::vector<u32>&& indices_,
           //std::vector<std::unique_ptr<Texture>>&& textures_
           std::vector<u32>&& texture_indices_)
    :vertex_data(std::move(vertex_data_)),
     indices(std::move(indices_)),
     //textures(std::move(textures_)),
     texture_indices(std::move(texture_indices_)),
     default_sampler{std::make_unique<Sampler>(0)}
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

void Mesh::draw(const Shader* const shader, const glm::mat4& projection, const Camera& camera,
                const std::vector<Texture*>& textures)
{
    vao->bind();
    shader->use_shader();

    shader->set_mat4("u_mvp", projection*camera.get_view());

    u32 diffuse_number  {1};
    u32 specular_number {1};
    for(std::size_t i{}; i < texture_indices.size(); ++i) {
        auto index {texture_indices[i]};
        std::string number;
        std::string name = textures[index]->get_type_name();
        if(name == "u_texture_diffuse")
            number = std::to_string(diffuse_number++);
        else if(name == "u_texture_specular")
            number = std::to_string(specular_number++);

        shader->set_int((name + number).c_str(), i);

        default_sampler->bind(i);
        textures[index]->bind(i);
    }
    //for(std::size_t i{}; i < textures.size(); ++i) {
    //    std::string number;
    //    std::string name = textures[i]->get_type_name();
    //    if(name == "u_texture_diffuse")
    //        number = std::to_string(diffuse_number++);
    //    else if(name == "u_texture_specular")
    //        number = std::to_string(specular_number++);

    //    shader->set_int((name + number).c_str(), i);

    //    default_sampler->bind(i);
    //    textures[i]->bind(i);
    //}

    if (indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}

}
