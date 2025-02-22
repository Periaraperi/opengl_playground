#pragma once

#include <glad/glad.h>

#include "peria_types.hpp"

namespace peria {

struct Buffer_Object {
    Buffer_Object();
    ~Buffer_Object();

    Buffer_Object(const Buffer_Object&) = delete;
    Buffer_Object& operator=(const Buffer_Object&) = delete;

    Buffer_Object(Buffer_Object&& rhs) noexcept;
    Buffer_Object& operator=(Buffer_Object&& rhs) noexcept;

    u32 id;
};


/*
template <typename T>
class Named_Buffer_Object {
public:
    explicit Named_Buffer_Object(const std::vector<T>& data) noexcept
    {
        peria::log("Buffer ctor()");
        glCreateBuffers(1, &id);
        glNamedBufferData(id, sizeof(T)*data.size(), data.data(), GL_STATIC_DRAW);
    }

    explicit Named_Buffer_Object(std::vector<T>&& data) noexcept
    {
        peria::log("Buffer ctor()");
        glCreateBuffers(1, &id);
        glNamedBufferData(id, sizeof(T)*data.size(), data.data(), GL_STATIC_DRAW);
    }

    explicit Named_Buffer_Object(std::size_t size) noexcept
    {
        peria::log("Buffer ctor()");
        glCreateBuffers(1, &id);
        glNamedBufferData(id, size, nullptr, GL_DYNAMIC_DRAW); 
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

    void set_sub_data(i32 buffer_offset, std::size_t data_size, const void* data) noexcept
    { glNamedBufferSubData(id, buffer_offset, data_size, data); }

private:
    u32 id;
};
*/
}
