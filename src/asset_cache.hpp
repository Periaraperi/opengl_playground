#pragma once

#include <string>
#include <unordered_map>

#include "texture.hpp"
#include "shader.hpp"

namespace peria {

class Asset_Cache {
public:
    Asset_Cache(const Asset_Cache&) = delete;
    Asset_Cache& operator=(const Asset_Cache&) = delete;

    Asset_Cache(Asset_Cache&&) = delete;
    Asset_Cache& operator=(Asset_Cache&&) = delete;

    static void initialize(const char* executable_path) noexcept;
    static void shutdown() noexcept;

    [[nodiscard]]
    static Asset_Cache* instance() noexcept;

    // std::moves texture to cache.
    // returns false if texture with same name already exists.
    bool add_texture_2d(const char* name, Texture2D&& tex);

    void load_shader(const char* vertex_path, const char* fragment_path, const char* geometry_path="");

    // returns pointer to Texture2D struct.
    // returns nullptr if such name was not found in cache.
    [[nodiscard]]
    Texture2D* fetch_texture_2d(const char* name);

    [[nodiscard]]
    Shader* fetch_shader(const char* vertex_path, const char* fragment_path, const char* geometry_path="");

private:
    static inline Asset_Cache* instance_ptr {nullptr};

    explicit Asset_Cache(const char* execuatable_path);
    ~Asset_Cache();

    std::string path_to_exe;

    std::unordered_map<std::string, Texture2D> texture_2ds;
    std::unordered_map<std::string, Cubemap>   cubemaps;
    std::unordered_map<std::string, Shader>    shaders;
};

}
