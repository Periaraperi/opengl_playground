#pragma once

#include <array>

namespace peria {

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

}
