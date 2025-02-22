#pragma once 

#include <array>
#include "peria_types.hpp"

namespace peria {

// TODO: maybe expand this for other types as well. Currently working only for floats.
template <typename T, u32 D>
struct Attr {
    static_assert(std::is_same_v<T, float> && D >= 1 && D <= 4, "Attribute must have type float and contain 1, 2, 3, or 4 elems");
    using type = T;
    static constexpr u32 elem_count {D};
    static constexpr std::size_t bytes {sizeof(T)*D};
    std::array<T, D> data;
};

template <typename... Ts>
struct Vertex : Ts... {
    static constexpr std::size_t stride {(Ts::bytes + ...)};
};

using Pos2D    = Attr<float, 2>;
using Pos3D    = Attr<float, 3>;
using Normal   = Attr<float, 3>;
using TexCoord = Attr<float, 2>;

}

