#pragma once

#include <cstdint>
#include <string>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

class Shader {
public:
	Shader(const std::string& vertex_path, const std::string& fragment_path);
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) = delete;
	~Shader();

	void enable() const;

    void set_int(const std::string& u_name, int val);
	void set_float(const std::string& u_name, float val);
	void set_vec2(const std::string& u_name, const glm::vec2& v);
	void set_vec3(const std::string& u_name, const glm::vec3& v);
	void set_vec4(const std::string& u_name, const glm::vec4& v);
    void set_mat4(const std::string& u_name, const glm::mat4& m);
    void set_array(const std::string& u_name, int count, int* arr);

private:
	uint32_t _id;
	std::string parse_shader(const std::string& path);
	uint32_t compile_shader(const std::string& src, uint32_t type);
	void create_shader_program(uint32_t vertex_shader, uint32_t fragment_shader);
};
