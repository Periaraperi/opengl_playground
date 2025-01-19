#pragma once

#include "peria_types.hpp"
#include <glad/glad.h>
#include <iostream>
#include <type_traits>
#include <vector>

template<typename T>
constexpr auto get_gl_type()
{
    if constexpr (std::is_same_v<T, float>) {
        return GL_FLOAT;
    }
    if constexpr (std::is_same_v<T, double>) {
        return GL_DOUBLE;
    }
    if constexpr (std::is_same_v<T, i32>) {
        return GL_INT;
    }
    if constexpr (std::is_same_v<T, u32>) {
        return GL_UNSIGNED_INT;
    }
    if constexpr (std::is_same_v<T, i16>) {
        return GL_SHORT;
    }
    if constexpr (std::is_same_v<T, u16>) {
        return GL_UNSIGNED_SHORT;
    }
    if constexpr (std::is_same_v<T, i8>) {
        return GL_BYTE;
    }
    if constexpr (std::is_same_v<T, u8>) {
        return GL_UNSIGNED_BYTE;
    }
}

namespace peria::graphics {

template<typename T>
struct Attribute {
    i32 element_count {};
    bool is_normalized{false};
};

class Vertex_Array {
public:
    Vertex_Array() noexcept;
    ~Vertex_Array();

    Vertex_Array(const Vertex_Array&) = delete;
    Vertex_Array& operator=(const Vertex_Array) = delete;

    Vertex_Array(Vertex_Array&&) noexcept = default;
    Vertex_Array& operator=(Vertex_Array&&) noexcept = default;

    template<typename T>
    void setup_attribute(const Attribute<T>& attribute, u32 binding_index = 0) noexcept
    {
        glEnableVertexArrayAttrib(id, attribute_count);
        glVertexArrayAttribBinding(id, attribute_count, binding_index);
        glVertexArrayAttribFormat(
                id,
                attribute_count,
                attribute.element_count,
                get_gl_type<T>(),
                (attribute.is_normalized) ? GL_TRUE : GL_FALSE,
                offsets[binding_index]);
        offsets[binding_index] += sizeof(T)*attribute.element_count;
        ++attribute_count;
    }

    void connect_vertex_buffer(u32 vbo, std::size_t stride, u32 binding_index = 0) const noexcept;
    void connect_index_buffer(u32 ibo) const noexcept;

    void bind() const noexcept;
    [[nodiscard]] u32 get_id() const noexcept;

private:
    u32 id;
    i32 attribute_count{}; // this is basically attribute layout index/slot.
    std::vector<i32> offsets; // relative offsets for each binding point
};
}
