#pragma once

#include "peria_types.hpp"

namespace peria {
    
struct Frame_Buffer {
    Frame_Buffer();
    ~Frame_Buffer();

    Frame_Buffer(const Frame_Buffer&) = delete;
    Frame_Buffer& operator=(const Frame_Buffer&) = delete;

    Frame_Buffer(Frame_Buffer&& rhs) noexcept;
    Frame_Buffer& operator=(Frame_Buffer&& rhs) noexcept;

    u32 id;
};

}
