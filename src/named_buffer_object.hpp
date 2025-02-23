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

}
