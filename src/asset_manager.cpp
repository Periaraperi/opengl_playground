#include "asset_manager.hpp"
#include "simple_logger.hpp"

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
std::string get_shader_key(const char* vertex_path, const char* fragment_path)
{
    auto vp {remove_prefix(vertex_path)};
    auto fp {remove_prefix(fragment_path)};
    return vp + "-" + fp.substr(fp.find_last_of("/"));
}

}

namespace peria::graphics {

Asset_Manager::Asset_Manager(const char* execuatable_path)
    :path_to_exe{execuatable_path}
{ peria::log("Asset_Manager ctor()"); }

Asset_Manager::~Asset_Manager()
{ peria::log("Asset_Manager dtor()"); }

void Asset_Manager::initialize(const char* executable_path) noexcept
{
    if (!instance_ptr) instance_ptr = new Asset_Manager(executable_path);
    else peria::log("Asset_Manager instance already initialized");
}

void Asset_Manager::shutdown() noexcept
{ delete instance_ptr; instance_ptr = nullptr; }

Asset_Manager* Asset_Manager::instance() noexcept
{ return instance_ptr; }

void Asset_Manager::load_texture(const char* path)
{
    const auto resource_key {path_to_exe+remove_prefix(path)};

    if (textures.find(resource_key) == textures.end()) {
        textures.insert({resource_key, std::make_unique<Texture>(path)});
        peria::log("loading texture with key", resource_key);
    }
    else {
        peria::log("load_texture skips loading");
    }
}

void Asset_Manager::load_texture(const char* path, const std::string& type_name)
{
    const auto resource_key {path_to_exe+remove_prefix(path)};

    if (textures.find(resource_key) == textures.end()) {
        textures.insert({resource_key, std::make_unique<Texture>(path, type_name)});
        peria::log("loading texture with key", resource_key);
    }
    else {
        peria::log("load_texture skips loading");
    }
}

Texture* Asset_Manager::fetch_texture(const char* path) 
{
    const auto resource_key {path_to_exe+remove_prefix(path)};
    if (textures.find(resource_key) == textures.end()) {
        textures.insert({resource_key, std::make_unique<Texture>(path)});
        peria::log("fetch_texture loads texture with key", resource_key);
    }
    else {
        peria::log("fetch_texture caches", resource_key);
    }

    return textures[resource_key].get();
}

void Asset_Manager::load_shader(const char* vertex_path, const char* fragment_path)
{
    const auto resource_key {path_to_exe+get_shader_key(vertex_path, fragment_path)};

    if (shaders.find(resource_key) == shaders.end()) {
        shaders.insert({resource_key, std::make_unique<Shader>(vertex_path, fragment_path)});
        peria::log("loading shader with key", resource_key);
    }
    else {
        peria::log("load_shader skips loading");
    }
}

Shader* Asset_Manager::fetch_shader(const char* vertex_path, const char* fragment_path)
{
    const auto resource_key {path_to_exe+get_shader_key(vertex_path, fragment_path)};

    if (shaders.find(resource_key) == shaders.end()) {
        shaders.insert({resource_key, std::make_unique<Shader>(vertex_path, fragment_path)});
        peria::log("fetch_shader loads shader with key", resource_key);
    }
    else {
        peria::log("fetch_shader caches", resource_key);
    }

    return shaders[resource_key].get();
}


}
