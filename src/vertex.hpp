#pragma once 

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "peria_color.hpp"

namespace peria::graphics::vertex {

// for 2d
struct Vertex {
    glm::vec2 pos;
    glm::vec2 texture_coordinates;
    peria::graphics::colors::Color<float> color;
    float texture_unit;
};

// if we want to draw solid colored cube,
// we can create 1 by 1 colored textures
struct Vertex3d {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coordinates;
};

struct Vertex3d_Colored {
    glm::vec3 pos;
    glm::vec3 normal;
    peria::graphics::colors::Color<float> color;
};

}
