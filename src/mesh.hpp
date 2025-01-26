#pragma once

#include <memory>
#include <vector>

#include "named_buffer_object.hpp"
#include "peria_types.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"
#include "vertex.hpp"

namespace peria::graphics {

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<vertex::Vertex3d>&& vertex_data_,
         std::vector<u32>&& indices_,
         std::vector<std::string>&& texture_paths_);
    
    [[nodiscard]] const std::vector<std::string>& get_texture_paths() const;
    [[nodiscard]] std::size_t get_index_count() const;
    [[nodiscard]] Vertex_Array* get_vao_ptr() const;
private:
    std::size_t index_count {};

    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> ibo;

    std::vector<std::string> texture_paths;
};

}
