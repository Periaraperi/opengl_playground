#pragma once 

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "attenuation.hpp"
#include "peria_color.hpp"

namespace peria::graphics::vertex {

struct Transform {
    float sx, sy, sz; // scale values
    float rot_x, rot_y, rot_z; // angle rotation around _axis
    float x, y, z; // translation values
};

struct Vertex2d {
    glm::vec2 pos;
    glm::vec2 texture_coordinates;
    peria::graphics::colors::Color<float> color;
    float texture_unit;
};

// if we want to draw solid colored cube,
// we can create 1 by 1 colored textures
struct Vertex3d {
    glm::vec3 pos {};
    glm::vec3 normal {};
    glm::vec2 tex_coordinates {};
};

struct Vertex3d_Colored {
    glm::vec3 pos;
    glm::vec3 normal;
    peria::graphics::colors::Color<float> color;
};

struct Point_Light {
    std::array<float, 3> pos {};
    std::array<float, 3> ambient {};
    std::array<float, 3> diffuse {};
    std::array<float, 3> specular {};

    Attenuation attenuation {ATT_DISTANCE_160};
};

struct Directional_Light {
    std::array<float, 3> direction {};
    std::array<float, 3> ambient {};
    std::array<float, 3> diffuse {};
    std::array<float, 3> specular {};
};

struct Spot_Light {
    std::array<float, 3> pos {};
    std::array<float, 3> direction {};
    std::array<float, 3> ambient {};
    std::array<float, 3> diffuse {};
    std::array<float, 3> specular {};

    Attenuation attenuation {ATT_DISTANCE_65};
    float angle {12.5f};
    float outer_angle {16.0f};
};
}
