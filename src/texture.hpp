#pragma once

#include "peria_color.hpp"
#include "peria_types.hpp"
#include "simple_logger.hpp"
#include <vector>
#include <glm/vec2.hpp>

namespace peria::graphics {

class Texture {
public:
    Texture() = default;

    // loads image resource
    explicit Texture(const char* res_path);

    // creates solid color texture
    Texture(i32 width_, i32 height_, const colors::Color<u8>& color);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&& rhs) noexcept;

    ~Texture();

    [[nodiscard]]
    u32 texture_id() const noexcept;

    [[nodiscard]]
    glm::vec2 dimensions() const noexcept;

    void bind(u32 unit = 0) const noexcept;

private:
    u32 id{};
    i32 width{};
    i32 height{};
    i32 channel_count{};
    std::vector<u8> texture_data;
};

}
