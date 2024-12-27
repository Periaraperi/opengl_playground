#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "texture.hpp"
#include "shader.hpp"

namespace peria::graphics {

class Asset_Manager {
public:
    Asset_Manager(const Asset_Manager&) = delete;
    Asset_Manager& operator=(const Asset_Manager&) = delete;

    Asset_Manager(Asset_Manager&&) = delete;
    Asset_Manager& operator=(Asset_Manager&&) = delete;

    static void initialize(const char* executable_path) noexcept;
    static void shutdown() noexcept;

    [[nodiscard]]
    static Asset_Manager* instance() noexcept;

    // path = relative path from assets folder.
    void load_texture(const char* path);

    // path = relative path from assets folder.
    // type_name = u_texture_diffuse or u_texture_specular.
    void load_texture(const char* path, const std::string& type_name);

    // vertex_path and fragment_path are relative paths from assets folder.
    void load_shader(const char* vertex_path, const char* fragment_path);

    // path = relative path from assets folder.
    // will load the texture first if it does not exist
    // else it will cache the value.
    // pointer is readonly that should not be destroyed by caller.
    [[nodiscard]]
    Texture* fetch_texture(const char* path);

    // vertex_path and fragment_path are relative paths from assets folder.
    // will load the shader first if it does not exist
    // else it will cache the value.
    // pointer is readonly that should not be destroyed by caller.
    // key is a combination of vertex and fragment paths.
    [[nodiscard]]
    Shader* fetch_shader(const char* vertex_path, const char* fragment_path);

private:
    static inline Asset_Manager* instance_ptr {nullptr};

    explicit Asset_Manager(const char* execuatable_path);
    ~Asset_Manager();

    std::string path_to_exe;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};

}
