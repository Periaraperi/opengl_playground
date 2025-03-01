#pragma once

#include <vector>

#include "named_buffer_object.hpp"
#include "peria_types.hpp"
#include "vertex_array.hpp"
#include "vertex.hpp"

namespace peria {

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<Vertex<Pos3D, Normal, TexCoord>>&& vertex_data,
         std::vector<u32>&& indices);
    
    [[nodiscard]] std::size_t get_index_count() const;
    [[nodiscard]] u32 vao_id() const;
private:
    std::size_t index_count {};

    Vertex_Array  vao;
    Buffer_Object vbo;
    Buffer_Object ibo;
};

}
