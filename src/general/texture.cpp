#include "texture.hpp"

#include "logger.hpp"

#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture()
    :_id(0), _width(1), _height(1), _nchannels(4)
{
    GL_CALL(glGenTextures(1,&_id));
    GL_CALL(glBindTexture(GL_TEXTURE_2D,_id));
    
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST));

    // 1 by 1 white texture
    uint32_t white_texture = 0xFFFFFFFF;
    
    GL_CALL(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,_width,_height,
                         0,GL_RGBA,GL_UNSIGNED_BYTE,&white_texture));

    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

    GL_CALL(glBindTexture(GL_TEXTURE_2D,0)); // unbind for now
}

Texture::Texture(const std::string& tex_path)
    :_id(0), _width(0), _height(0), _nchannels(0)
{
    stbi_set_flip_vertically_on_load(true);
    uint8_t* texture_data = stbi_load(tex_path.c_str(),&_width,&_height,&_nchannels,0); 

    PERIA_ASSERT(texture_data!=nullptr);
    GL_CALL(glGenTextures(1,&_id));
    GL_CALL(glBindTexture(GL_TEXTURE_2D,_id));
    
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST));

    if (_nchannels==3) { // for jpg
        GL_CALL(glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,_width,_height,
                             0,GL_RGB,GL_UNSIGNED_BYTE,texture_data));
    } else if (_nchannels==4) { // for png
        GL_CALL(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,_width,_height,
                             0,GL_RGBA,GL_UNSIGNED_BYTE,texture_data));
    }
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

    GL_CALL(glBindTexture(GL_TEXTURE_2D,0)); // unbind for now

    stbi_image_free(texture_data);
}

Texture::~Texture()
{
    GL_CALL(glDeleteTextures(1,&_id));   
}

void Texture::bind(uint32_t texture_slot /* = 0 */) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0+texture_slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D,_id));
}

void Texture::unbind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D,0));
}


