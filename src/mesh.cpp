#include "mesh.hpp"

namespace peria::graphics {

Mesh::Mesh(std::vector<vertex::Vertex3d>&& vertex_data_,
           std::vector<u32>&& indices_,
           std::vector<std::string>&& texture_paths_)
    :index_count{indices_.size()},
     texture_paths(std::move(texture_paths_))
{
    vao = std::make_unique<Vertex_Array>();
    vbo = std::make_unique<Named_Buffer_Object<vertex::Vertex3d>>(std::move(vertex_data_));
    ibo = std::make_unique<Named_Buffer_Object<u32>>(std::move(indices_));

    // pos
    vao->setup_attribute(Attribute<float>{3, false});
    // normals
    vao->setup_attribute(Attribute<float>{3, false});
    // tex coords
    vao->setup_attribute(Attribute<float>{2, false});

    vao->connect_vertex_buffer(vbo->buffer_id(), sizeof(vertex::Vertex3d));
    vao->connect_index_buffer(ibo->buffer_id());
}

const std::vector<std::string>& 
Mesh::get_texture_paths() const
{ return texture_paths; }

std::size_t Mesh::get_index_count() const
{ return index_count; }

Vertex_Array* Mesh::get_vao_ptr() const
{ return vao.get(); }

}
