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
    float camera_front_magnitude {1.0f};
    bool debug_mode {false};

    // ================================ Vertex Arrays ================================
    std::unique_ptr<Vertex_Array> default_vao;
    std::unique_ptr<Vertex_Array> light_source_vao;
    std::unique_ptr<Vertex_Array> quad_vao;
    // ===============================================================================
    
    // ================================ Buffers ======================================
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> cube_vbo;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> quad_vbo;
    std::unique_ptr<Named_Buffer_Object<u32>> quad_ibo;
    // ===============================================================================

    // ============================= Samplers ========================================
    std::unique_ptr<Sampler> default_sampler;
    // ===============================================================================

    // ============================= Shaders =========================================
    Shader* quad_shader; // draws colored or textured quads to screen
    Shader* default_shader; // default shader to draw 3d models or cubes without lighting
    // ===============================================================================

    // ============================= Textures ========================================
    Texture* cross_hair_texture;
    // ===============================================================================
};

struct Demo_Combined_Lights : public Demo3d {
    Demo_Combined_Lights();
    void render() override;
    void update() override;
    void imgui() override;

    Shader* light_source_shader;
    Shader* combined_lights_shader;

    Texture* diffuse_texture;
    Texture* specular_texture;

    std::vector<Point_Light> point_lights;
    Directional_Light directional_light;
    Spot_Light spot_light;
    materials::Material material;
    std::unique_ptr<Model> dragon;

    std::vector<glm::vec3> cube_positions;

    std::array<float, 3> bg_color {1.0f, 1.0f, 1.0f};

    struct Imgui_Info {
        bool directional_light {false};
        bool spot_light {false};
        bool point_light {false};
        std::vector<bool> point_lights;
    } imgui_info;
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

    std::vector<glm::vec3> cubes;
    
    void render() override;
    void update() override;
    void imgui() override;
};

struct Texture2d_Demo : Demo3d {
    Texture2d_Demo();

    
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> vbo;

    std::size_t tex_index {};
    std::vector<Texture*> textures;
    std::size_t sampler_index {};
    std::vector<std::unique_ptr<Sampler>> samplers;

    void make_data(float tex_coord_scale);


    void render() override;
    void update() override;
    void imgui() override;
};

}
