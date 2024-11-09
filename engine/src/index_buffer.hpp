#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>

#include "peria_types.hpp"

namespace graphics {

class Index_Buffer {
public:
    explicit Index_Buffer(const std::vector<u32>& index_data)
    {
        std::cerr << "IBO ctor()\n";
        glCreateBuffers(1, &id);
        glNamedBufferData(id, sizeof(u32)*index_data.size(), index_data.data(), GL_STATIC_DRAW); 
    }

    ~Index_Buffer()
    { std::cerr << "IBO dtor()\n"; glDeleteBuffers(1, &id); }

    Index_Buffer(const Index_Buffer&) = delete;
    Index_Buffer& operator=(const Index_Buffer) = delete;

    Index_Buffer(Index_Buffer&&) noexcept = default;
    Index_Buffer& operator=(Index_Buffer&&) noexcept = default;

    [[nodiscard]]
    u32 buffer_id() const noexcept
    { return id; }

private:
    u32 id;
};

}

