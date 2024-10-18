#include "shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

namespace graphics {

[[nodiscard]]
std::string parse_file(const char* path)
{
    std::ifstream ifs{path};
    if (ifs.is_open()) {
        std::stringstream ss{};
        ss << ifs.rdbuf();
        return ss.str();
    }
    else {
        std::cerr << "Error while opening file at path: " << path << '\n';
    }
    return "";
}

[[nodiscard]]
u32 compile_shader(const char* src, u32 type) noexcept
{
	u32 shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	// check if successfully compiled
	i32 success;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Couldn't compile shader\n" << log << '\n';
	}

	return shader;
}

void create_shader_program(u32& id, u32 vertex_shader, u32 fragment_shader) noexcept
{
    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);

    // check if successfully linked
	i32 success;
	char log[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(id, 512, nullptr, log);
        std::cerr << "Couldn't link shader program\n" << log << '\n';
	}
}

Shader::Shader(const char* vertex_path, const char* fragment_path)
    :vertex_source{parse_file(vertex_path)},
     fragment_source{parse_file(fragment_path)}
{
    std::cerr << "Creating Shader program\n";
    auto vertex_shader = compile_shader(vertex_source.c_str(), GL_VERTEX_SHADER);
    auto fragment_shader = compile_shader(fragment_source.c_str(), GL_FRAGMENT_SHADER);
    
    create_shader_program(id, vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

}

Shader::~Shader()
{
    std::cerr << "Deleting Shader program\n";
    glDeleteProgram(id);
}

void Shader::use_shader() noexcept
{ glUseProgram(id); }

void Shader::set_int(const char* u_name, i32 val) noexcept
{ glUniform1i(glGetUniformLocation(id, u_name), val); }

void Shader::set_float(const char* u_name, float val) noexcept
{ glUniform1f(glGetUniformLocation(id, u_name), val); }

void Shader::set_vec2(const char* u_name, const glm::vec2& v) noexcept
{ glUniform2f(glGetUniformLocation(id, u_name), v.x, v.y); }

void Shader::set_vec3(const char* u_name, const glm::vec3& v) noexcept
{ glUniform3f(glGetUniformLocation(id, u_name), v.x, v.y, v.z); }

void Shader::set_vec4(const char* u_name, const glm::vec4& v) noexcept
{ glUniform4f(glGetUniformLocation(id, u_name), v.x, v.y, v.z, v.w); }

void Shader::set_mat4(const char* u_name, const glm::mat4& m) noexcept
{ glUniformMatrix4fv(glGetUniformLocation(id, u_name), 1, GL_FALSE, glm::value_ptr(m)); }

void Shader::set_array(const char* u_name, i32 count, i32* arr) noexcept 
{ glUniform1iv(glGetUniformLocation(id, u_name), count, arr); }

}
