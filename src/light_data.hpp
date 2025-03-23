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

struct Point_Light {
    std::array<float, 3> pos;

    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;

    float constant  {1.0f};
    float linear    {0.07f};
    float quadratic {0.017f};
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

[[nodiscard]]
inline Ubo_Directional_Light to_ubo_directional_light(const Directional_Light& dir_light) noexcept
{
    return {
        dir_light.direction, 
        {/*padding*/},
        dir_light.ambient, 
        {/*padding*/},
        dir_light.diffuse, 
        {/*padding*/},
        dir_light.specular, 
        {/*padding*/},
        dir_light.pos, 
        {/*padding*/}
    };
}

[[nodiscard]]
inline Ubo_Point_Light to_ubo_point_light(const Point_Light& point_light) noexcept
{
    return {
        point_light.pos, 
        {/*padding*/},
        point_light.ambient, 
        {/*padding*/},
        point_light.diffuse, 
        {/*padding*/},
        point_light.specular, 
        point_light.constant, 
        point_light.linear, 
        point_light.quadratic, 
        {/*padding*/}
    };
}

}
