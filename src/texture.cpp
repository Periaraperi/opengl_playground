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

}
