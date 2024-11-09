#include "texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <algorithm>

#include "simple_logger.hpp"

namespace peria::graphics {

Texture::Texture(const char* res_path)
{
    peria::log("Texture ctor");

    stbi_set_flip_vertically_on_load(1);
    u8* data {stbi_load(res_path, &width, &height, &channel_count, 0)};
    if (data == nullptr) {
        peria::log("failed to load res; ", res_path);
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
    
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateTextureMipmap(id);
}

Texture::Texture(i32 width_, i32 height_, const Color<u8>& color) 
    :width{width_}, height{height_}, channel_count{4},
     texture_data{std::vector<u8>(width*height*channel_count)}
{
    for (std::size_t i{}; i<texture_data.size(); i+=channel_count) {
        texture_data[i+0] = color.r;
        texture_data[i+1] = color.g;
        texture_data[i+2] = color.b;
        texture_data[i+3] = color.a;
    }

    //for (i32 i{}; i<height; ++i) {
    //    for (i32 j{}; j<width; ++j) {
    //        texture_data[i*width + j*channel_count+0] = color.r;
    //        texture_data[i*width + j*channel_count+1] = color.g;
    //        texture_data[i*width + j*channel_count+2] = color.b;
    //        texture_data[i*width + j*channel_count+3] = color.a;
    //    }
    //}

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    
    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
    auto format          = (channel_count == 4) ? GL_RGBA  : GL_RGB;

    glTextureStorage2D(id, 1, internal_format, width, height);
    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data.data());
    
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateTextureMipmap(id);
}

Texture::~Texture()
{
    peria::log("Texture dtor");
    glDeleteTextures(1, &id);
}

void Texture::bind(u8 slot) noexcept
{ glBindTextureUnit(slot, id); }

u32 Texture::texture_id() const noexcept
{ return id; }

}
