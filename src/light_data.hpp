#pragma once

#include <array>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>

#include "ubo_data.hpp"

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
    std::array<float, 3> direction;

    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;

    // in degrees
    float inner_angle {};
    float outer_angle {};
};

[[nodiscard]]
inline Ubo_Spot_Light to_ubo_spot_light(const Spot_Light& spot_light) noexcept
{
    return {
        spot_light.pos, 
        {/*padding*/},
        spot_light.direction, 
        {/*padding*/},
        spot_light.ambient, 
        {/*padding*/},
        spot_light.diffuse, 
        {/*padding*/},
        spot_light.specular, 
        std::cos(glm::radians(spot_light.inner_angle)),
        std::cos(glm::radians(spot_light.outer_angle)),
        {/*padding*/}
    };
}

}
