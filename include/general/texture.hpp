#pragma once

#include <string>

class Texture {
public:
    Texture(); // default construction of white texture 1x1, will be bound to slot 0
    Texture(const std::string& tex_path);
    ~Texture();
    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    void bind(uint32_t texture_slot=0) const;
    void unbind() const;
private:
    uint32_t _id;
    int _width;
    int _height;
    int _nchannels;
};
