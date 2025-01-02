#pragma once

#include <memory>
#include <vector>

#include "graphics.hpp"
#include "materials.hpp"
#include "camera.hpp"
#include "vertex.hpp"

#include "vertex_array.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "sampler.hpp"

#include "model.hpp"

using namespace peria::graphics::vertex;

namespace peria::graphics::demos {

struct Demo2d {
    Demo2d();
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void imgui() = 0;

    glm::mat4 projection;

    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> ibo;
    
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
    void imgui() override;

private:
    void draw_colored_quad(const Quad& quad, const colors::Color<float>& color) noexcept;
    void draw_textured_quad(const Quad& quad, const Quad& texture_region) noexcept;
    void render_quads() noexcept;
    Shader* shader;
};

struct Demo3d {
    Demo3d();
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void imgui() = 0;

    Camera camera;
    glm::mat4 projection;
    glm::mat4 ortho_projection;

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

struct Demo_Model : public Demo3d {
    Demo_Model();
    Shader* shader;
    std::unique_ptr<peria::graphics::Model> model;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Demo_Depth_Testing : Demo3d {
    Demo_Depth_Testing();
    Shader* shader;
    Texture* texture1;
    Texture* texture2;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Demo_Stencil_Testing : Demo3d {
    Demo_Stencil_Testing();
    Shader* shader1;
    Shader* shader2;
    Texture* texture1;
    Texture* texture2;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Another_Demo : Demo3d {
    Another_Demo();

    std::unique_ptr<Vertex_Array> quad_vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> quad_vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> quad_ibo;

    std::unique_ptr<Texture> white_texture;
    Shader* quad_shader;
    Shader* shader1;
    Texture* texture1;
    Texture* texture_crosshair;
    
    void render() override;
    void update() override;
    void imgui() override;
};

}
