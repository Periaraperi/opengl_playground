#pragma once

#include <glad/glad.h>
#include <utility>

#include "peria_types.hpp"
#include "vertex.hpp"

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

// sets vertex attrib layout for specific vao and binding index and connects vbo to it.
template <typename... Ts>
void configure_vao(u32 vao, u32 vbo, u32 binding_index) noexcept
{
    auto setup = [&](u32 attribute_index, u32 elem_count, std::size_t offset){
        glEnableVertexArrayAttrib(vao, attribute_index);
        glVertexArrayAttribBinding(vao, attribute_index, binding_index);
        glVertexArrayAttribFormat(vao, attribute_index, elem_count, GL_FLOAT, GL_FALSE, offset);
    };

    [&]<std::size_t... I>(std::index_sequence<I...>) {
        std::size_t offset {0};
        (setup(I, Ts::elem_count, std::exchange(offset, offset+Ts::bytes)), ...);
    }(std::make_index_sequence<sizeof...(Ts)>{});

    glVertexArrayVertexBuffer(vao, binding_index, vbo, 0, Vertex<Ts...>::stride);
}

void connect_ibo(u32 vao, u32 ibo) noexcept;

}
