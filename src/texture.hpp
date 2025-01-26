#pragma once

#include "peria_color.hpp"
#include "peria_types.hpp"
#include <string>
#include <vector>
#include <glm/vec2.hpp>

namespace peria::graphics {

enum class Texture_Type {
    NONE = 0,
    DIFFUSE,
    SPECULAR
};

class Texture {
public:
    Texture() = default;

    // loads image resource
    Texture(const char* res_path, bool gamma=false);
    Texture(const char* res_path, Texture_Type type_name_, bool gamma=false);

    // creates solid color texture
    Texture(i32 width_, i32 height_, const colors::Color<u8>& color);

    // Create empty texture for framebuffer color attachment
    Texture(i32 width_, i32 height_);

    // Create multisampled texture for framebuffer color attachment
    Texture(i32 width_, i32 height_, i32 samples);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&& rhs) noexcept;

    ~Texture();

    [[nodiscard]]
    u32 texture_id() const noexcept;

    [[nodiscard]]
    glm::vec2 dimensions() const noexcept;

    [[nodiscard]]
    Texture_Type get_type_name() const;

    [[nodiscard]]
    std::string get_path() const;

    void bind(u32 unit = 0) const noexcept;

private:
    u32 id{};
    i32 width{};
    i32 height{};
    i32 channel_count{};
    std::vector<u8> texture_data;
    std::string path;
    Texture_Type type_name;
};

}
