#pragma once

#include <array>
#include <glm/vec3.hpp>

namespace peria {

[[nodiscard]]
inline glm::vec3 arr_to_vec3(const std::array<float, 3> arr) noexcept
{ return {arr[0], arr[1], arr[2]}; }

struct Directional_Light {
    std::array<float, 3> direction;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;

    // fake position for shadow mapping
    std::array<float, 3> pos;
};

struct Spot_Light {
    std::array<float, 3> pos;
    float pad_1;

    std::array<float, 3> direction;
    float pad_2;

    std::array<float, 3> ambient;
    float pad_3;

    std::array<float, 3> diffuse;
    float pad_4;

    std::array<float, 3> specular;

    float cos_inner_angle {};
    float cos_outer_angle {};
    std::array<float, 3> pad_5;
};

struct Ubo_Spot_Light {
    float px, py, pz;
    float dx, dy, dz;
    float ambient_r,  ambient_g,  ambient_b;
    float diffuse_r,  diffuse_g,  diffuse_b;
    float specular_r, specular_g, specular_b;
    float cos_inner_angle;
    float cos_outer_angle; // 68 bytes = sizeof(float)*17
    float pad[3]; // 64 bytes = 4 chunks. Last 4 bytes in 5th chunk, + 12bytes padding.
};

[[nodiscard]]
inline Spot_Light make_spot_light(const glm::vec3& pos,
                                  const glm::vec3& direction, 
                                  const glm::vec3& ambient,
                                  const glm::vec3& diffuse,
                                  const glm::vec3& specular,
                                  const float cos_inner_angle,
                                  const float cos_outer_angle) noexcept
{
    return {
        {pos.x, pos.y, pos.z}, 
        {/*padding*/},
        {direction.x, direction.y, direction.z}, 
        {/*padding*/},
        {ambient.x, ambient.y, ambient.z}, 
        {/*padding*/},
        {diffuse.x, diffuse.y, diffuse.z}, 
        {/*padding*/},
        {specular.x, specular.y, specular.z}, 
        cos_inner_angle,
        cos_outer_angle,
        {/*padding*/}
    };
}

}
