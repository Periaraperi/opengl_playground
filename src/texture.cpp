#include "texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <algorithm>
#include <utility>

#include "simple_logger.hpp"

namespace peria::graphics {

Texture::Texture(const char* res_path)
{
    peria::log("Texture ctor");

    stbi_set_flip_vertically_on_load(1);
    u8* data {stbi_load(res_path, &width, &height, &channel_count, 0)};
    if (data == nullptr) {
        peria::log("failed to load res: ", res_path);
        return;
    }

    texture_data.resize(width*height*channel_count);
    std::copy(data, data+texture_data.size(), texture_data.begin());
    
    stbi_image_free(data); 
    data = nullptr;

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    
    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
    auto format          = (channel_count == 4) ? GL_RGBA  : GL_RGB;

    glTextureStorage2D(id, 1, internal_format, width, height);
    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data.data());
    
    glGenerateTextureMipmap(id);
}

Texture::Texture(const char* res_path, const std::string& type_name_)
    :type_name{type_name_}, path{res_path}
{
    peria::log("Texture ctor");
    peria::log(type_name);

    stbi_set_flip_vertically_on_load(1);
    u8* data {stbi_load(res_path, &width, &height, &channel_count, 0)};
    if (data == nullptr) {
        peria::log("failed to load res: ", res_path);
        return;
    }

    texture_data.resize(width*height*channel_count);
    std::copy(data, data+texture_data.size(), texture_data.begin());
    
    stbi_image_free(data); 
    data = nullptr;

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    
    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
    auto format          = (channel_count == 4) ? GL_RGBA  : GL_RGB;

    glTextureStorage2D(id, 1, internal_format, width, height);
    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data.data());
    
    glGenerateTextureMipmap(id);
}

Texture::Texture(i32 width_, i32 height_, const colors::Color<u8>& color) 
    :width{width_}, height{height_}, channel_count{4},
     texture_data{std::vector<u8>(width*height*channel_count)}
{
    peria::log("Colored Texture ctor");
    for (std::size_t i{}; i<texture_data.size(); i+=channel_count) {
        texture_data[i+0] = color.r;
        texture_data[i+1] = color.g;
        texture_data[i+2] = color.b;
        texture_data[i+3] = color.a;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    
    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
    auto format          = (channel_count == 4) ? GL_RGBA  : GL_RGB;

    glTextureStorage2D(id, 1, internal_format, width, height);
    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data.data());

    glGenerateTextureMipmap(id);
}

Texture::Texture(i32 width_, i32 height_)
    :width{width_}, height{height_}, channel_count{3}
{
    peria::log("FBO color attachment Texture ctor");
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    
    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
    glTextureStorage2D(id, 1, internal_format, width, height);
}

Texture::Texture(Texture&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)},
     width{std::exchange(rhs.width, 0)},
     height{std::exchange(rhs.height, 0)},
     channel_count{std::exchange(rhs.channel_count, 0)},
     texture_data(std::move(rhs.texture_data))
{ peria::log("Move constructing Texture"); }

Texture& Texture::operator=(Texture&& rhs) noexcept
{
    if (&rhs == this) return *this;
    peria::log("Move assigning Texture"); 

    this->id = std::exchange(rhs.id, 0);
    this->width = std::exchange(rhs.width, 0);
    this->height = std::exchange(rhs.height, 0);
    this->channel_count = std::exchange(rhs.channel_count, 0);
    this->texture_data = std::move(rhs.texture_data);
    return *this;
}

Texture::~Texture()
{
    peria::log("Texture dtor");
    glDeleteTextures(1, &id);
}

void Texture::bind(u32 unit) const noexcept
{ glBindTextureUnit(unit, id); }

u32 Texture::texture_id() const noexcept
{ return id; }

glm::vec2 Texture::dimensions() const noexcept
{ return {width, height}; }

std::string Texture::get_type_name() const
{ return type_name; }

std::string Texture::get_path() const
{ return path; }
}
