#pragma once

#include <memory>

#include "peria_types.hpp"
#include "render_buffer.hpp"
#include "texture.hpp"

namespace peria::graphics {

class Frame_Buffer {
public:
    Frame_Buffer() = default;
    Frame_Buffer(i32 width_, i32 height_);

    // use for multisample fbos
    Frame_Buffer(i32 width_, i32 height_, i32 samples);

    Frame_Buffer(const Frame_Buffer&) = delete;
    Frame_Buffer& operator=(const Frame_Buffer&) = delete;

    Frame_Buffer(Frame_Buffer&&) noexcept;
    Frame_Buffer& operator=(Frame_Buffer&& rhs) noexcept;

    ~Frame_Buffer();

    void bind() const noexcept;

    [[nodiscard]]
    glm::vec2 dimensions() const noexcept;

    // returns texture id of color attachment texture.
    [[nodiscard]]
    u32 texture_id() const noexcept;

    [[nodiscard]]
    u32 fbo_id() const noexcept;

    // returns readonly pointer to color attachment texture object
    [[nodiscard]]
    Texture* texture() const noexcept;

private:
    u32 id;
    std::unique_ptr<Texture> color_attachment_texture;
    std::unique_ptr<Render_Buffer> depth_stencil_buffer;
};

}
