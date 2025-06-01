#pragma once

#include <array>


namespace peria {
    struct Transform {
        std::array<float, 3> pos   {};
        std::array<float, 3> scale {1.0f, 1.0f, 1.0f};
        std::array<float, 3> angle {};
    };

    struct Entity {
        Transform transform {};
        std::array<float, 3> color {};
        int model {-1};
        int mesh  {-1};
        int texture_id  {-1};
    };
}
