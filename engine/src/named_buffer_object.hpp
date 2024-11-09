#pragma once

#include <glad/glad.h>
#include <vector>

#include "./peria_engine/peria_types.hpp"
#include "simple_logger.hpp"

namespace peria::graphics {

template <typename T>
class Named_Buffer_Object {
public:
    explicit Named_Buffer_Object(const std::vector<T>& data) noexcept
    {
        peria::log("Buffer ctor()");
        glCreateBuffers(1, &id);
        glNamedBufferData(id, sizeof(T)*data.size(), data.data(), GL_STATIC_DRAW); 
    }

    ~Named_Buffer_Object()
    { peria::log("Buffer dtor()"); glDeleteBuffers(1, &id); }

    Named_Buffer_Object(const Named_Buffer_Object&) = delete;
    Named_Buffer_Object& operator=(const Named_Buffer_Object) = delete;

    Named_Buffer_Object(Named_Buffer_Object&&) noexcept = default;
    Named_Buffer_Object& operator=(Named_Buffer_Object&&) noexcept = default;

    [[nodiscard]]
    u32 buffer_id() const noexcept
    { return id; }

private:
    u32 id;
};

}
