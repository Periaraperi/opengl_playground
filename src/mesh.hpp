#pragma once

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

#include "named_buffer_object.hpp"
#include "peria_types.hpp"
#include "sampler.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "camera.hpp"

namespace peria::graphics {

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<vertex::Vertex3d>&& vertex_data_,
         std::vector<u32>&& indices_,
         //std::vector<std::unique_ptr<Texture>>&& textures_,
         std::vector<u32>&& texture_indices_);
    void draw(const Shader* const shader, const glm::mat4& projection, const Camera& camera,
              const std::vector<Texture*>& textures);
    //Mesh(const Mesh&) = delete;
    //Mesh& operator=(const Mesh&) = delete;

    //Mesh(Mesh&&) noexcept = default;
    //Mesh& operator=(Mesh&&) noexcept = default;

    //~Mesh() noexcept = default;
private:
    std::vector<vertex::Vertex3d> vertex_data;
    std::vector<u32> indices;

    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> ibo;
    //std::vector<std::unique_ptr<Texture>> textures;
    std::vector<u32> texture_indices;
    std::unique_ptr<Sampler> default_sampler;
};

}
