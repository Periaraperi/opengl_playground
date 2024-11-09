#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "./peria_engine/peria_types.hpp"

namespace peria::graphics {
class Shader {
public:
    Shader() = default;
    Shader(std::string vertex_path, std::string fragment_path);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) noexcept = default;
    Shader& operator=(Shader&&) noexcept = default;

    void use_shader() const noexcept;

    ~Shader();

    void set_int(const char* u_name, i32 val) noexcept;
    void set_float(const char* u_name, float val) noexcept;
    void set_vec2(const char* u_name, const glm::vec2& v) noexcept;
    void set_vec3(const char* u_name, const glm::vec3& v) noexcept;
    void set_vec4(const char* u_name, const glm::vec4& v) noexcept;
    void set_mat4(const char* u_name, const glm::mat4& m) noexcept;
    void set_array(const char* u_name, i32 count, i32* arr) noexcept;

private: 
    u32 id;

    std::string vertex_source;
    std::string fragment_source;
};
}
