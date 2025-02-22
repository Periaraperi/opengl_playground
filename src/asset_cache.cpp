#include "asset_cache.hpp"
#include "simple_logger.hpp"

#include "texture.hpp"
#include <glad/glad.h>

namespace {

// removes prefix "./" if exists
[[nodiscard]]
std::string remove_prefix(const char* path)
{
    std::string p {path};
    if (p.size() > 2 && p[0] == '.' && p[1] == '/') {
        p = p.substr(2);
    }
    return p;
}

[[nodiscard]]
std::string get_shader_key(const char* vertex_path, const char* fragment_path, const char* geometry_path)
{
    auto vp {remove_prefix(vertex_path)};
    auto fp {remove_prefix(fragment_path)};
    auto gp {remove_prefix(geometry_path)};
    return vp + "-" + fp.substr(fp.find_last_of("/"));
}

}

namespace peria {

Asset_Cache::Asset_Cache(const char* execuatable_path)
    :path_to_exe{execuatable_path}
{ peria::log("Asset_Cache ctor()"); }

Asset_Cache::~Asset_Cache()
{ peria::log("Asset_Cache dtor()"); }

void Asset_Cache::initialize(const char* executable_path) noexcept
{
    if (!instance_ptr) instance_ptr = new Asset_Cache(executable_path);
    else peria::log("Asset_Cache instance already initialized");
}

void Asset_Cache::shutdown() noexcept
{ delete instance_ptr; instance_ptr = nullptr; }

Asset_Cache* Asset_Cache::instance() noexcept
{ return instance_ptr; }

bool Asset_Cache::add_texture_2d(const char* name, Texture2D&& texture)
{
    const auto resource_key {path_to_exe+remove_prefix(name)};
    if (texture_2ds.find(resource_key) == texture_2ds.end()) {
        texture_2ds.insert({resource_key, std::move(texture)});
        return true;
    }
    return false;
}

Texture2D* Asset_Cache::fetch_texture_2d(const char* name)
{
    const auto resource_key {path_to_exe+remove_prefix(name)};

    if (texture_2ds.find(resource_key) == texture_2ds.end()) {
        return nullptr;
    }

    return &texture_2ds[resource_key];
}

void Asset_Cache::load_shader(const char* vertex_path, const char* fragment_path, const char* geometry_path /* = ""*/)
{
    const auto resource_key {path_to_exe+get_shader_key(vertex_path, fragment_path, geometry_path)};

    if (shaders.find(resource_key) == shaders.end()) {
        shaders.insert({resource_key, Shader{vertex_path, fragment_path, geometry_path}});
    }
}

Shader* Asset_Cache::fetch_shader(const char* vertex_path, const char* fragment_path, const char* geometry_path /* = ""*/)
{
    const auto resource_key {path_to_exe+get_shader_key(vertex_path, fragment_path, geometry_path)};

    if (shaders.find(resource_key) == shaders.end()) {
        shaders.insert({resource_key, Shader{vertex_path, fragment_path, geometry_path}});
    }

    return &shaders[resource_key];
}

}
