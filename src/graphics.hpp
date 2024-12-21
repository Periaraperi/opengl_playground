#pragma once

//#include <glad/glad.h>
//#include <glm/vec2.hpp>
//#include <glm/vec3.hpp>
//#include <memory>

#include "peria_types.hpp"
#include "peria_color.hpp"
//#include "sampler.hpp"
//#include "vertex_array.hpp"
//#include "named_buffer_object.hpp"
//#include "shader.hpp"
//#include "texture.hpp"
//#include "camera.hpp"

namespace peria::renderer {

struct Graphics_Info {
    graphics::colors::Color<float> bg {graphics::colors::BLACK};
    u32 clear_buffer_bit_flags {};
};
void clear_color() noexcept;
void set_clear_color(const peria::graphics::colors::Color<float>& color) noexcept;
void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
void set_clear_buffer_bits(bool clear_color = true, bool clear_depth = false, bool clear_stencil = false) noexcept;
void clear_buffer() noexcept;
void set_vsync(bool vsync) noexcept;

void start_imgui_frame() noexcept;
void imgui_render() noexcept;

}

//namespace peria::graphics {
//
//struct Quad {
//    float x, y;
//    float w, h;
//};
//
//struct Vertex {
//    glm::vec2 pos;
//    glm::vec2 texture_coordinates;
//    colors::Color<float> color;
//    float texture_unit;
//};
//
//struct Vertex3d {
//    glm::vec3 pos;
//    colors::Color<float> color;
//};
//
//struct Vertex3d_Lighting {
//    glm::vec3 pos;
//    glm::vec3 normal;
//    glm::vec2 tex_coordinates;
//};
//
//struct Vertex3d_Textured {
//    glm::vec3 pos;
//    glm::vec2 tex_coords;
//};
//
//class Graphics {
//public:
//    Graphics();
//
//    Graphics(const Graphics&) = delete;
//    Graphics& operator=(const Graphics&) = delete;
//
//    Graphics(Graphics&&) noexcept = default;
//    Graphics& operator=(Graphics&&) noexcept = default;
//
//    ~Graphics();
//
//    void clear_color() noexcept;
//    void set_clear_color(const colors::Color<float>& color) noexcept;
//    void set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept;
//    void set_clear_buffer_bits(bool clear_color = true, bool clear_depth = false, bool clear_stencil = false) noexcept;
//    void clear_buffer() noexcept;
//    void set_vsync(bool vsync) noexcept;
//
//    void set_batch_quad_count(i32 quad_count) noexcept;
//    
//    void draw_colored_quad(const Quad& quad, const colors::Color<float>& color) noexcept;
//    void draw_textured_quad(const Quad& quad, const Quad& texture_region) noexcept;
//
//    void render2d() noexcept;
//    void render3d() noexcept;
//
//    // lighting demos go here
//    void render3d_lighting() noexcept;
//    void render3d_directional_lighting() noexcept;
//    void render3d_point_lighting() noexcept;
//    void render3d_spot_light() noexcept;
//
//    // for temp 3d testing
//    void render_cube() noexcept;
//    void render_cube_textured() noexcept;
//
//    void peria_perspective(float fov_y, float aspect_ratio, float near, float far) noexcept; // for testing 3d change this later
//    void peria_ortho(float left, float right, float bottom, float top) noexcept;
//
//    // imgui
//    void start_imgui_frame(ImFont* font);
//    void imgui_render();
//
//    void imgui_transforms();
//    void imgui_matrix_info();
//    void imgui_lighting();
//
//    void inc_specular_coefficient() noexcept;
//    void dec_specular_coefficient() noexcept;
//
//    [[nodiscard]]
//    Camera& get_camera()
//    { return camera; }
//
//
//private:
//    u32 clear_buffer_bit_flags {};
//
//    glm::mat4 screen_ortho_projection;
//    Matrix4 peria_ortho_projection;
//
//    glm::mat4 perspective_projection;
//    Matrix4 peria_perspective_projection;
//
//    Camera camera;
//
//    colors::Color<float> background_color {};
//
//    // ===============================================================================================
//    //                                        Buffers
//    // ===============================================================================================
//    
//    // batch data for quads
//    // size in bytes to store N quad vertices (by default N = 4096)
//    std::size_t batch_quad_vbo_size {4096 * 4 * sizeof(Vertex)}; 
//    std::unique_ptr<Vertex_Array> batch_quad_vao;
//    std::unique_ptr<Named_Buffer_Object<Vertex>> batch_quad_vbo;
//    std::unique_ptr<Named_Buffer_Object<u32>> batch_quad_ibo;
//    std::vector<Vertex> draw_quad_command_data;
//
//    // regular colored cube data
//    std::unique_ptr<Vertex_Array> cube_vao;
//    std::unique_ptr<Named_Buffer_Object<Vertex3d>> cube_vbo;
//
//    // textured cube data
//    std::unique_ptr<Vertex_Array> cube_vao_textured;
//    std::unique_ptr<Named_Buffer_Object<Vertex3d_Textured>> cube_vbo_textured;
//
//    // lighting related
//    std::unique_ptr<Vertex_Array> light_source_vao;
//    std::unique_ptr<Named_Buffer_Object<Vertex3d_Lighting>> light_source_vbo;
//
//    std::unique_ptr<Vertex_Array> lighting_vao;
//    std::unique_ptr<Named_Buffer_Object<Vertex3d_Lighting>> lighting_vbo;
//    // ===============================================================================================
//
//    // ===============================================================================================
//    //                                        Shaders
//    // ===============================================================================================
//    std::unique_ptr<Shader> quad_shader;
//    std::unique_ptr<Shader> cube_shader;
//    std::unique_ptr<Shader> cube_shader_textured;
//    std::unique_ptr<Shader> light_source_shader;
//    std::unique_ptr<Shader> lighting_shader; // calcualte lighting with phong model in world space
//    std::unique_ptr<Shader> lighting_shader_view; // same as above, but in view space
//    std::unique_ptr<Shader> directional_light_shader;
//    std::unique_ptr<Shader> point_light_shader;
//    std::unique_ptr<Shader> spot_light_shader;
//    // ===============================================================================================
//
//    // ===============================================================================================
//    //                                        Textures
//    // ===============================================================================================
//    std::unique_ptr<Texture> white_texture;
//    std::unique_ptr<Texture> texture_atlas;
//    std::unique_ptr<Texture> chiti;
//    std::unique_ptr<Texture> wooden_container;
//    std::unique_ptr<Texture> specular_wooden_container;
//    std::unique_ptr<Texture> emission_map;
//    std::unique_ptr<Texture> solid_color_material_texture_diffuse;
//    std::unique_ptr<Texture> solid_color_material_texture_specular;
//    // ===============================================================================================
//
//    // ===============================================================================================
//    //                                        Samplers
//    // ===============================================================================================
//    std::unique_ptr<Sampler> sampler1;
//    std::unique_ptr<Sampler> sampler2;
//    // ===============================================================================================
//};
//}
