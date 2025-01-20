#pragma once

#include "peria_types.hpp"

namespace peria::graphics {

class Render_Buffer {
public:
    // used for depth/stencil attachment for fbos
    Render_Buffer(i32 width_, i32 height_);
    // used for depth/stencil attachment for multisampled fbos
    Render_Buffer(i32 width_, i32 height_, i32 samples);

    Render_Buffer(const Render_Buffer&) = delete;
    Render_Buffer& operator=(const Render_Buffer&) = delete;

    Render_Buffer(Render_Buffer&&) noexcept;
    Render_Buffer& operator=(Render_Buffer&& rhs) noexcept;

    ~Render_Buffer();

    [[nodiscard]]
    u32 render_buffer_id() const noexcept;
private:
    u32 id;
};

}
