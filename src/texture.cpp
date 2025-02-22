#include "texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <utility>

#include "simple_logger.hpp"

namespace peria {

Texture2D::Texture2D() noexcept
{
    peria::log("Texture2D ctor()");
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
}

Texture2D::Texture2D(Texture2D&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{}

Texture2D& Texture2D::operator=(Texture2D&& rhs) noexcept
{
    if (&rhs == this) return *this;

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

Texture2D::~Texture2D()
{ peria::log("Texture dtor()"); glDeleteTextures(1, &id); }

Cubemap::Cubemap() noexcept
{
    peria::log("Cubemap ctor()");
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
}

Cubemap::Cubemap(Cubemap&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{}

Cubemap& Cubemap::operator=(Cubemap&& rhs) noexcept
{
    if (&rhs == this) return *this;

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

Cubemap::~Cubemap()
{ peria::log("Texture dtor()"); glDeleteTextures(1, &id); }


void load_texture2d_from_image(const Texture2D& texture, const char* path) noexcept
{
    stbi_set_flip_vertically_on_load(1);

    i32 width, height, channel_count;
    u8* data {stbi_load(path, &width, &height, &channel_count, 0)};

    if (data == nullptr) {
        peria::log("failed to load res: ", path);
        return;
    }

    i32 internal_format {channel_count == 4 ? GL_RGBA8 : GL_RGB8};
    i32 format          {channel_count == 4 ? GL_RGBA : GL_RGB};

    glTextureStorage2D(texture.id, 1, internal_format, width, height);
    glTextureSubImage2D(texture.id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(texture.id);

    stbi_image_free(data); 
    data = nullptr;
}

void create_colored_texture2d(const Texture2D& texture, const colors::Color<float>& color) noexcept
{
    const auto c {colors::Color<float>::to_u8_color(color)};
    std::array<u8, 3> data {c.r, c.g, c.b};

    i32 width{1}, height{1};
    glTextureStorage2D(texture.id, 1, GL_RGB8, width, height);
    glTextureSubImage2D(texture.id, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glGenerateTextureMipmap(texture.id);
}

void load_cubemap_from_file(const Cubemap& cubemap, const std::array<const char*, 6>& file_paths) noexcept
{}

void create_blank_cubemap(const Cubemap& cubemap) noexcept
{}





//Texture::Texture(const char* res_path, bool gamma)
//    :type_name{Texture_Type::NONE}
//{
//    peria::log("Texture ctor");
//
//    stbi_set_flip_vertically_on_load(1);
//    u8* data {stbi_load(res_path, &width, &height, &channel_count, 0)};
//    if (data == nullptr) {
//        peria::log("failed to load res: ", res_path);
//        return;
//    }
//
//    texture_data.resize(width*height*channel_count);
//    std::copy(data, data+texture_data.size(), texture_data.begin());
//    
//    stbi_image_free(data); 
//    data = nullptr;
//
//    glCreateTextures(GL_TEXTURE_2D, 1, &id);
//    
//    i32 internal_format;
//    if (gamma) {
//        internal_format = (channel_count == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
//    }
//    else {
//        internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
//    }
//
//    auto format          = (channel_count == 4) ? GL_RGBA  : GL_RGB;
//
//    glTextureStorage2D(id, 1, internal_format, width, height);
//    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data.data());
//    
//    glGenerateTextureMipmap(id);
//}
//
//Texture::Texture(const char* res_path, Texture_Type type_name_, bool gamma)
//    :path{res_path},
//     type_name{type_name_}
//{
//    peria::log("Texture ctor");
//
//    stbi_set_flip_vertically_on_load(1);
//    u8* data {stbi_load(res_path, &width, &height, &channel_count, 0)};
//    if (data == nullptr) {
//        peria::log("failed to load res: ", res_path);
//        return;
//    }
//
//    texture_data.resize(width*height*channel_count);
//    std::copy(data, data+texture_data.size(), texture_data.begin());
//    
//    stbi_image_free(data); 
//    data = nullptr;
//
//    glCreateTextures(GL_TEXTURE_2D, 1, &id);
//    
//    i32 internal_format;
//    if (gamma) {
//        internal_format = (channel_count == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
//    }
//    else {
//        internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
//    }
//
//    auto format         = (channel_count == 4) ? GL_RGBA  : GL_RGB;
//
//    glTextureStorage2D(id, 1, internal_format, width, height);
//    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data.data());
//    
//    glGenerateTextureMipmap(id);
//}
//
//Texture::Texture(i32 width_, i32 height_, const colors::Color<u8>& color) 
//    :width{width_}, height{height_}, channel_count{4},
//     texture_data{std::vector<u8>(width*height*channel_count)},
//     type_name{Texture_Type::NONE}
//{
//    peria::log("Colored Texture ctor");
//    for (std::size_t i{}; i<texture_data.size(); i+=channel_count) {
//        texture_data[i+0] = color.r;
//        texture_data[i+1] = color.g;
//        texture_data[i+2] = color.b;
//        texture_data[i+3] = color.a;
//    }
//
//    glCreateTextures(GL_TEXTURE_2D, 1, &id);
//    
//    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
//    auto format          = (channel_count == 4) ? GL_RGBA  : GL_RGB;
//
//    glTextureStorage2D(id, 1, internal_format, width, height);
//    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data.data());
//
//    glGenerateTextureMipmap(id);
//}
//
//Texture::Texture(i32 width_, i32 height_)
//    :width{width_}, height{height_}, channel_count{3}
//{
//    peria::log("FBO color attachment Texture ctor");
//    glCreateTextures(GL_TEXTURE_2D, 1, &id);
//    
//    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
//    glTextureStorage2D(id, 1, internal_format, width, height);
//}
//
//Texture::Texture(i32 width_, i32 height_, i32 samples)
//    :width{width_}, height{height_}, channel_count{3}
//{
//    peria::log("FBO multisampled color attachment Texture ctor");
//    glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &id);
//    
//    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
//    glTextureStorage2DMultisample(id, samples, internal_format, width, height, GL_TRUE);
//}
//
//Texture::Texture(Texture&& rhs) noexcept
//    :id{std::exchange(rhs.id, 0)},
//     width{std::exchange(rhs.width, 0)},
//     height{std::exchange(rhs.height, 0)},
//     channel_count{std::exchange(rhs.channel_count, 0)},
//     texture_data(std::move(rhs.texture_data))
//{ peria::log("Move constructing Texture"); }
//
//Texture& Texture::operator=(Texture&& rhs) noexcept
//{
//    if (&rhs == this) return *this;
//    peria::log("Move assigning Texture"); 
//
//    this->id = std::exchange(rhs.id, 0);
//    this->width = std::exchange(rhs.width, 0);
//    this->height = std::exchange(rhs.height, 0);
//    this->channel_count = std::exchange(rhs.channel_count, 0);
//    this->texture_data = std::move(rhs.texture_data);
//    return *this;
//}
//
//Texture::~Texture()
//{
//    peria::log("Texture dtor");
//    glDeleteTextures(1, &id);
//}
//
//void Texture::bind(u32 unit) const noexcept
//{ glBindTextureUnit(unit, id); }
//
//u32 Texture::texture_id() const noexcept
//{ return id; }
//
//glm::vec2 Texture::dimensions() const noexcept
//{ return {width, height}; }
//
//Texture_Type Texture::get_type_name() const
//{ return type_name; }
//
//std::string Texture::get_path() const
//{ return path; }

}
