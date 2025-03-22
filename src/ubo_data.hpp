#pragma once

#include "peria_types.hpp"
#include <array>

namespace peria {

struct Ubo_Directional_Light {
    std::array<float, 3> direction;
    float pad_1;

    std::array<float, 3> ambient;
    float pad_2;

    std::array<float, 3> diffuse;
    float pad_3;

    std::array<float, 3> specular;
    float pad_4;

    // fake position for shadow mapping
    std::array<float, 3> pos;
    float pad_5;
};

struct Ubo_Spot_Light {
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

struct Ubo_Lights {
    Ubo_Directional_Light directional_light;
    std::array<Ubo_Spot_Light, 32> spot_lights;
    i32 spot_light_count {};
    std::array<float, 3> pad;
};

}
