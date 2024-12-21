#pragma once

#include <memory>
#include <array>
#include <vector>

#include "attenuation.hpp"
#include "graphics.hpp"
#include "materials.hpp"
#include "camera.hpp"
#include "vertex.hpp"

#include "vertex_array.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "sampler.hpp"

namespace peria::graphics::demos {

struct Point_Light {
    std::array<float, 3> pos;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;

    Attenuation attenuation {ATT_DISTANCE_65};
};

struct Directional_Light {
    std::array<float, 3> direction;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;
};

struct Spot_Light {
    std::array<float, 3> pos;
    std::array<float, 3> direction;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;

    Attenuation attenuation {ATT_DISTANCE_65};
    float angle {12.5f};
    float outer_angle {16.0f};
};

struct Demo2d {
    Demo2d();
    virtual void render() = 0;
    virtual void update() = 0;

    void draw_colored_quad(const Quad& quad, const colors::Color<float>& color) noexcept;
    void draw_textured_quad(const Quad& quad, const Quad& texture_region) noexcept;
    void render_quads() noexcept;

    glm::mat4 projection;

    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> ibo;
    
    std::unique_ptr<Shader> shader;

    std::unique_ptr<Texture> white_texture; // for colored rects
    std::unique_ptr<Texture> texture_atlas;
    std::unique_ptr<Sampler> default_sampler;

    static constexpr u32 QUAD_COUNT {4096}; 
    std::vector<vertex::Vertex2d> quad_draw_data;
};

struct Demo_Quads : public Demo2d {
    Demo_Quads();
    void render() override;
    void update() override;
};

struct Demo3d {
    Demo3d();
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void imgui() = 0;

    Camera camera;
    glm::mat4 projection;

    std::unique_ptr<Vertex_Array> light_source_vao;
    std::unique_ptr<Vertex_Array> vao;

    std::unique_ptr<Texture> texture;
    std::unique_ptr<Texture> specular_texture;

    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> cube_vbo;

    std::unique_ptr<Sampler> sampler;
};

struct Demo_Combined_Lights : public Demo3d {
    Demo_Combined_Lights();
    void render() override;
    void update() override;
    void imgui() override;

    std::unique_ptr<Shader> light_source_shader;
    std::unique_ptr<Shader> combined_lights_shader;

    std::vector<Point_Light> point_lights;
    Directional_Light directional_light;
    Spot_Light spot_light;
    materials::Material material;
};

}
