#pragma once

#include "peria_types.hpp"

namespace peria {

struct Vertex_Array {
    Vertex_Array() noexcept;
    ~Vertex_Array();

    Vertex_Array(const Vertex_Array&) = delete;
    Vertex_Array& operator=(const Vertex_Array&) = delete;

    Vertex_Array(Vertex_Array&& rhs) noexcept;
    Vertex_Array& operator=(Vertex_Array&& rhs) noexcept;

    u32 id;
};

}
