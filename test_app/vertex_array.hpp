#pragma once

#include "peria_types.hpp"
#include <glad/glad.h>
#include <type_traits>

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

namespace graphics {

template<typename T>
struct Attribute {
    using type = T;

    i32 element_count;
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
    void setup_attribute(const Attribute<T>& attribute) noexcept
    {
        glEnableVertexArrayAttrib(id, attribute_count);
        glVertexArrayAttribBinding(id, attribute_count, 0);
        glVertexArrayAttribFormat(
                id,
                attribute_count,
                attribute.element_count,
                get_gl_type<T>(),
                (attribute.is_normalized) ? GL_TRUE : GL_FALSE,
                offset);
        offset += sizeof(T)*attribute.element_count;
        ++attribute_count;
    }
    void connect_vertex_buffer(u32 vbo, std::size_t stride) noexcept;
    void connect_index_buffer(u32 ibo) noexcept;

    void bind() noexcept;

private:
    u32 id;
    i32 attribute_count{};
    i32 offset{};
};
}
