#pragma once

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

#include "materials.hpp"
#include "named_buffer_object.hpp"
#include "peria_types.hpp"
#include "sampler.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "vertex.hpp"
#include "camera.hpp"

using namespace peria::graphics::vertex;

namespace peria::graphics {

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<vertex::Vertex3d>&& vertex_data_,
         std::vector<u32>&& indices_,
         std::vector<std::string>&& texture_paths_);
    void draw(const Shader* const shader, const glm::mat4& projection, const Camera& camera);
private:
    std::vector<vertex::Vertex3d> vertex_data;
    std::vector<u32> indices;

    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> ibo;
    std::unique_ptr<Sampler> default_sampler;

    std::vector<std::string> texture_paths;

    std::vector<Point_Light> point_lights;
    Directional_Light directional_light;
    Spot_Light spot_light;
    materials::Material material;
};

}
