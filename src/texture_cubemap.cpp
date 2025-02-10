#include "texture_cubemap.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <utility>

#include "simple_logger.hpp"

namespace peria::graphics {

Texture_Cubemap::Texture_Cubemap(i32 width_, i32 height_)
    :width{width_}, height{height_}, channel_count{1}
{
    peria::log("Texture_Cubemap default ctor with DEPTH_COMPONENT32F");
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
    glTextureStorage2D(id, 1, GL_DEPTH_COMPONENT32F, width, height);
}

Texture_Cubemap::Texture_Cubemap(const std::vector<const char*>& cube_map_image_paths)
{
    peria::log("Texture_Cubemap ctor");
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);

    // no need to flip the image, since OpenGL cubemaps use different coordinate system.
    stbi_set_flip_vertically_on_load(0);

    std::vector<std::vector<u8>> face_data(6, std::vector<u8>());

    i8 index {0};
    for (const auto& path:cube_map_image_paths) {
        u8* data {stbi_load(path, &width, &height, &channel_count, 0)};
        if (data == nullptr) {
            peria::log("failed to load res: ", path);
            return;
        }

        face_data[index].resize(width*height*channel_count);
        std::copy(data, data+face_data[index].size(), face_data[index].begin());
        stbi_image_free(data);

        ++index;
    }

    auto internal_format = (channel_count == 4) ? GL_RGBA8 : GL_RGB8;
    auto format          = (channel_count == 4) ? GL_RGBA  : GL_RGB;

    glTextureStorage2D(id, 1, internal_format, width, height);

    for (std::size_t i{}; i<face_data.size(); ++i) {
        glTextureSubImage3D(id, 0, 0, 0, i, width, height, 1, format, GL_UNSIGNED_BYTE, face_data[i].data());
    }

}

Texture_Cubemap& Texture_Cubemap::operator=(Texture_Cubemap&& rhs) noexcept
{
    if (&rhs == this) return *this;
    peria::log("Move assigning Texture_Cubemap"); 

    this->id = std::exchange(rhs.id, 0);
    this->width = std::exchange(rhs.width, 0);
    this->height = std::exchange(rhs.height, 0);
    this->channel_count = std::exchange(rhs.channel_count, 0);
    return *this;
}

Texture_Cubemap::~Texture_Cubemap()
{
    peria::log("Texture_Cubemap dtor");
    glDeleteTextures(1, &id);
}

void Texture_Cubemap::bind(u32 unit) const noexcept
{ glBindTextureUnit(unit, id); }

u32 Texture_Cubemap::texture_id() const noexcept
{ return id; }

}
