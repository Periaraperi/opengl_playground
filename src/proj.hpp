#pragma once

#include <array>
#include "peria_types.hpp"

namespace peria::graphics {

// Fix translation values.
struct Ortho_Projection_Matrix {
    Ortho_Projection_Matrix() = default;
    Ortho_Projection_Matrix(float left, float right, float bottom, float top)
    {
        // O O O O
        // O O O O
        // O O O O
        // O O O O
        auto linear_index = [](i32 r, i32 c) -> std::size_t {
            return r*4 + c;
        };

        mat[linear_index(0, 0)] = 2.0f / (right - left);
        mat[linear_index(0, 1)] = 0.0f;
        mat[linear_index(0, 2)] = 0.0f;
        mat[linear_index(0, 3)] = -1.0f;

        mat[linear_index(1, 0)] = 0.0f;
        mat[linear_index(1, 1)] = 2.0f / (top - bottom);
        mat[linear_index(1, 2)] = 0.0f;
        mat[linear_index(1, 3)] = -1.0f;

        mat[linear_index(2, 0)] = 0.0f;
        mat[linear_index(2, 1)] = 0.0f;
        mat[linear_index(2, 2)] = 1.0f;
        mat[linear_index(2, 3)] = 0.0f;

        mat[linear_index(3, 0)] = 0.0f;
        mat[linear_index(3, 1)] = 0.0f;
        mat[linear_index(3, 2)] = 0.0f;
        mat[linear_index(3, 3)] = 1.0f;
    }

    std::array<float, 16> mat {};
};

}
