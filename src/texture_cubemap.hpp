#pragma once

#include "peria_types.hpp"
#include <vector>
#include <glm/vec2.hpp>

namespace peria {

class Texture_Cubemap {
public:
    Texture_Cubemap() = default;

    Texture_Cubemap(i32 width_, i32 height_);
    Texture_Cubemap(const std::vector<const char*>& cube_map_image_paths);

    Texture_Cubemap(const Texture_Cubemap&) = delete;
    Texture_Cubemap& operator=(const Texture_Cubemap&) = delete;

    Texture_Cubemap(Texture_Cubemap&&) noexcept;
    Texture_Cubemap& operator=(Texture_Cubemap&& rhs) noexcept;

    ~Texture_Cubemap();

    [[nodiscard]]
    u32 texture_id() const noexcept;

    void bind(u32 unit = 0) const noexcept;
private:
    u32 id{};
    // dimensions of each face is same, width by height
    i32 width{};
    i32 height{};
    i32 channel_count{};
};

}
