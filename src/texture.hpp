#pragma once

#include "peria_color.hpp"
#include "peria_types.hpp"
#include <array>

namespace peria {

struct Texture2D {
    Texture2D() noexcept;

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& rhs) noexcept;
    Texture2D& operator=(Texture2D&& rhs) noexcept;

    ~Texture2D();

    u32 id;
};

struct Cubemap {
    Cubemap() noexcept;

    Cubemap(const Cubemap&) = delete;
    Cubemap& operator=(const Cubemap&) = delete;

    Cubemap(Cubemap&& rhs) noexcept;
    Cubemap& operator=(Cubemap&& rhs) noexcept;

    ~Cubemap();

    u32 id;
};

// allocates texture storage and loads png or jpg image
void load_texture2d_from_image(const Texture2D& texture, const char* path) noexcept;

// allocates 1x1 texture storage and loads solid color data
void create_colored_texture2d(const Texture2D& texture, const colors::Color<float>& color) noexcept;

// allocates texture storage for cubemap and loads images
void load_cubemap_from_file(const Cubemap& cubemap, const std::array<const char*, 6>& file_paths) noexcept;

// allocates texture storage for cubemap.
void create_blank_cubemap(const Cubemap& cubemap) noexcept;

}


//enum class Texture_Type {
//    NONE = 0,
//    DIFFUSE,
//    SPECULAR
//};
//
//class Texture {
//public:
//    Texture() = default;
//
//    // loads image resource
//    Texture(const char* res_path, bool gamma=false);
//    Texture(const char* res_path, Texture_Type type_name_, bool gamma=false);
//
//    // creates solid color texture
//    Texture(i32 width_, i32 height_, const colors::Color<u8>& color);
//
//    // Create empty texture for framebuffer color attachment
//    Texture(i32 width_, i32 height_);
//
//    // Create multisampled texture for framebuffer color attachment
//    Texture(i32 width_, i32 height_, i32 samples);
//
//    Texture(const Texture&) = delete;
//    Texture& operator=(const Texture&) = delete;
//
//    Texture(Texture&&) noexcept;
//    Texture& operator=(Texture&& rhs) noexcept;
//
//    ~Texture();
//
//    [[nodiscard]]
//    u32 texture_id() const noexcept;
//
//    [[nodiscard]]
//    glm::vec2 dimensions() const noexcept;
//
//    [[nodiscard]]
//    Texture_Type get_type_name() const;
//
//    [[nodiscard]]
//    std::string get_path() const;
//
//    void bind(u32 unit = 0) const noexcept;
//
//private:
//    u32 id{};
//    i32 width{};
//    i32 height{};
//    i32 channel_count{};
//    std::vector<u8> texture_data;
//    std::string path;
//    Texture_Type type_name;
//};
//
