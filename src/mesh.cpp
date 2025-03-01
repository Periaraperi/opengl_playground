#include "mesh.hpp"
#include "graphics.hpp"

namespace peria {

Mesh::Mesh(std::vector<Vertex<Pos3D, Normal, TexCoord>>&& vertex_data,
           std::vector<u32>&& indices)
    :index_count{indices.size()}
{
    buffer_upload_data<Vertex<Pos3D, Normal, TexCoord>>(vbo, vertex_data, GL_STATIC_DRAW);
    buffer_upload_data<u32>(ibo, indices, GL_STATIC_DRAW);
    vao_configure<Pos3D, Normal, TexCoord>(vao.id, vbo.id, 0);
    vao_connect_ibo(vao, ibo);
}

std::size_t Mesh::get_index_count() const
{ return index_count; }

u32 Mesh::vao_id() const
{ return vao.id; }

}
