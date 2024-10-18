#pragma once

#include "peria_types.hpp"

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <type_traits>

namespace peria::engine {

enum class vertex_buffer_type {
    STATIC = 0,
    DYNAMIC
};

template<typename T>
concept vbo_type = std::is_same_v<T, vertex_buffer_type>;

template<typename Vertex, typename Type = vertex_buffer_type> requires vbo_type<Type>
class vertex_buffer {
public:
    vertex_buffer() = default;

    explicit vertex_buffer(const std::vector<Vertex>& data)
        :vertex_data{data}
    { std::cerr << "vertex_buffer()\n"; init(); }

    explicit vertex_buffer(std::vector<Vertex>&& data)
        :vertex_data{std::move(data)}
    { std::cerr << "vertex_buffer move ctor()\n"; init(); }

    ~vertex_buffer()
    { std::cerr << "deleting vertex buffer\n"; glDeleteBuffers(1, &id); }

    vertex_buffer(const vertex_buffer&) = delete;
    vertex_buffer& operator=(const vertex_buffer&) = delete;
    vertex_buffer(vertex_buffer&&) noexcept = default;
    vertex_buffer& operator=(vertex_buffer&&) noexcept = default;
private:
    void init()
    {
        glCreateBuffers(1, &id);
        if (type == vertex_buffer_type::STATIC) {
            std::cerr << "static vbo\n";
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);
        }
        else {
            std::cerr << "dynamic vbo\n";
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertex_data.size(), vertex_data.data(), GL_DYNAMIC_DRAW);
        }
    }

private:
    u32 id;

    Type type;
    std::vector<Vertex> vertex_data;
};
}
