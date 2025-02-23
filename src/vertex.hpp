#pragma once 

#include <array>
#include <algorithm>
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
struct Vertex {
    static constexpr std::size_t stride {(Ts::bytes + ...)};
    std::array<std::common_type_t<typename Ts::type...>, (Ts::elem_count + ...)> arr;
    constexpr Vertex(const Ts&... attrs) 
    {
        std::size_t offset {};
        ((std::copy(attrs.data.begin(), attrs.data.end(), arr.begin()+offset), offset += Ts::elem_count), ...);
    }
};

template <typename T>
struct is_vertex : std::false_type {};

template <typename... Ts>
struct is_vertex<Vertex<Ts...>> : std::true_type {};

template <typename T>
constexpr inline bool is_vertex_v = is_vertex<T>::value;

using Pos2D    = Attr<float, 2>;
using Pos3D    = Attr<float, 3>;
using Pos4D    = Attr<float, 4>;
using Normal   = Attr<float, 3>;
using TexCoord = Attr<float, 2>;
using Color4   = Attr<float, 4>;

}

