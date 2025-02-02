#pragma once

#include <memory>
#include <vector>

#include "graphics.hpp"
#include "materials.hpp"
#include "camera.hpp"
#include "texture_cubemap.hpp"
#include "vertex.hpp"

#include "vertex_array.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "frame_buffer.hpp"
#include "sampler.hpp"

#include "model.hpp"

using namespace peria::graphics::vertex;

namespace peria::graphics::demos {

struct Demo2d {
    Demo2d();
    virtual ~Demo2d() = default;
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
    ~Demo_Quads() = default;
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
    virtual ~Demo3d() = default;
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
    ~Demo_Combined_Lights() = default;
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
    ~Demo_Model() = default;
    Shader* shader;
    std::unique_ptr<peria::graphics::Model> model;
    std::unique_ptr<Texture> colored_texture;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Demo_Depth_Testing : Demo3d {
    Demo_Depth_Testing();
    ~Demo_Depth_Testing() = default;
    Shader* shader;
    Texture* texture1;
    Texture* texture2;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Demo_Stencil_Testing : Demo3d {
    Demo_Stencil_Testing();
    ~Demo_Stencil_Testing() = default;
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
    ~Another_Demo() = default;

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

struct Texture2d_Demo : Demo2d {
    Texture2d_Demo();
    ~Texture2d_Demo() = default;

    Shader* quad_shader;
    std::size_t tex_index {};
    std::vector<Texture*> textures;
    std::size_t sampler_index {};
    std::vector<std::unique_ptr<Sampler>> samplers;

    void make_data(float tex_coord_scale);
    
    struct Imgui_Info {
        float tex_coords_scale {1.0f};
    } imgui_info;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Blending_Demo : Demo2d {
    Blending_Demo();
    ~Blending_Demo() = default;

    Shader* quad_shader;
    //std::size_t tex_index_1 {0};
    //std::size_t tex_index_2 {0};
    std::vector<Texture*> textures;
    std::unique_ptr<Sampler> sampler;

    void make_data(float tex_coord_scale, const std::array<float, 4>& color);

    struct Colored_Quad {
        std::array<float, 2> pos {};
        std::array<float, 2> dims {200.0f, 100.0f};
        std::array<float, 4> color {0.0f, 0.0f, 0.0f, 1.0f};
    };

    struct Imgui_Info {
        float tex_coords_scale {1.0f};
        bool src {false};
        bool dst {false};
        i32 src_value {0};
        i32 dst_value {0};
        std::array<float, 4> constant_color {0.0f, 0.0f, 0.0f, 1.0f};

        std::array<Colored_Quad, 4> quads{};
    } imgui_info;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Blending_Windows_Demo : Demo3d {
    Blending_Windows_Demo();
    ~Blending_Windows_Demo() = default;

    struct Window_Transforms {
        glm::vec3 pos;
        glm::vec2 scale;
    };
    Texture* chiti;
    Texture* window;
    std::unique_ptr<Texture> floor;
    Shader* light_source_shader; // using for axis
    std::unique_ptr<Sampler> sampler;
    std::vector<glm::vec3> positions;
    std::vector<Window_Transforms> windows;
    std::unique_ptr<Vertex_Array> window_vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> window_vbo;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Face_Culling_Demo : Demo3d {
    Face_Culling_Demo();
    ~Face_Culling_Demo() = default;

    Texture* chiti;
    Shader* light_source_shader; // using for axis
    std::unique_ptr<Sampler> sampler;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Frame_Buffer_Demo : Demo3d {
    Frame_Buffer_Demo();
    ~Frame_Buffer_Demo() = default;

    Texture* chiti;
    Shader* light_source_shader; // using for axis
    Shader* screen_shader;
    std::unique_ptr<Vertex_Array> screen_vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> screen_quad_vbo;
    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<Frame_Buffer> fbo;

    glm::vec2 old_screen_dimensions;

    struct Imgui_Info {
        float offset_x {};
        float offset_y {};
    } imgui_info;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Frame_Buffer_Rear_View_Demo : Demo3d {
    Frame_Buffer_Rear_View_Demo();
    ~Frame_Buffer_Rear_View_Demo() = default;

    Texture* chiti;
    Texture* xD;
    Shader* light_source_shader; // using for axis
    Shader* screen_shader;
    std::unique_ptr<Vertex_Array> screen_vao;
    std::unique_ptr<Vertex_Array> rear_view_vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> screen_quad_vbo;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> rear_view_quad_vbo;
    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<Frame_Buffer> main_scene_fbo;
    std::unique_ptr<Frame_Buffer> rear_view_fbo;

    glm::vec2 old_screen_dimensions;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Sky_Box_Demo : Demo3d {
    Sky_Box_Demo();
    ~Sky_Box_Demo() = default;

    Texture* chiti;
    Shader* light_source_shader; // using for axis
    Shader* sky_box_shader;
    Shader* reflection_shader;
    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<Sampler> sampler_skybox;
    std::unique_ptr<Texture_Cubemap> sky_box_texture;

    std::unique_ptr<Vertex_Array> sky_box_vao;
    std::unique_ptr<Named_Buffer_Object<glm::vec3>> sky_box_vbo;

    bool use_reflection_shader {false};

    void render() override;
    void update() override;
    void imgui() override;
};

struct Ubo_Demo : Demo3d {
    Ubo_Demo();
    ~Ubo_Demo() = default;

    Shader* shader1;
    Shader* shader2;
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<glm::vec3>> vbo;
    std::unique_ptr<Named_Buffer_Object<float>> ubo;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Geometry_Shader_Demo : Demo3d {
    Geometry_Shader_Demo();
    ~Geometry_Shader_Demo() = default;

    struct Vert2d {
        glm::vec2 pos;
        glm::vec3 color;
    };

    Shader* shader;
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<Vert2d>> vbo;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Geometry_Shader_Explode_Demo : Demo3d {
    Geometry_Shader_Explode_Demo();
    ~Geometry_Shader_Explode_Demo() = default;

    Shader* shader;
    Texture* chiti;
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    std::unique_ptr<Sampler> sampler;
    float tt {0.0f};

    void render() override;
    void update() override;
    void imgui() override;
};

struct Geometry_Shader_Normals_Demo : Demo3d {
    Geometry_Shader_Normals_Demo();
    ~Geometry_Shader_Normals_Demo() = default;

    Shader* shader;
    Shader* shader_normal;
    std::unique_ptr<Model> dragon;
    std::unique_ptr<Model> backpack;
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    std::unique_ptr<Sampler> sampler;
    bool b{false};

    void render() override;
    void update() override;
    void imgui() override;
};

struct Instancing_Demo : Demo3d {
    Instancing_Demo();
    ~Instancing_Demo() = default;

    struct Vert2d {
        glm::vec2 pos;
        glm::vec3 color;
    };

    Shader* instancing_quads_shader;
    Shader* model_shader;
    Shader* instance_planet_shader;

    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<Vert2d>> vbo;
    std::unique_ptr<Named_Buffer_Object<glm::mat4>> model_vbo;

    std::unique_ptr<Model> planet;
    std::unique_ptr<Model> asteroid;
    std::unique_ptr<Sampler> sampler;

    glm::vec3 planet_pos;
    std::vector<Transform> asteroid_transforms;

    std::vector<glm::vec2> offsets;
    bool b{false};
    bool draw_instanced{true};

    void render() override;
    void update() override;
    void imgui() override;
};

struct MSAA_Demo : Demo3d {
    MSAA_Demo();
    ~MSAA_Demo() = default;

    Shader* cube_shader;
    Shader* screen_shader;
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Vertex_Array> screen_vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex2d>> screen_vbo;
    
    std::unique_ptr<Frame_Buffer> ms_fbo;
    std::unique_ptr<Frame_Buffer> inter_fbo;

    std::unique_ptr<Texture> white_texture;
    std::unique_ptr<Sampler> sampler;

    void render() override;
    void update() override;
    void imgui() override;
};

struct Blinn_Phong_Demo : Demo3d {
    Blinn_Phong_Demo();
    ~Blinn_Phong_Demo() = default;

    Shader* shader;
    Shader*light_source_shader;
    Texture* floor_texture;
    Texture* floor_texture2;
    std::unique_ptr<Texture> white_texture;
    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    Directional_Light directional_light;
    std::vector<Point_Light> point_lights;
    bool blinn {false};
    bool do_directional_light {false};
    bool do_point_light {true};
    bool do_spot_light {false};
    bool do_gamma_correction {false};
    float shininess {32.0f};
    bool show_point_lights {false};
    bool do_attenuation {false};
    bool reverse_gamma {false};
    bool do_linear {false};
    bool use_srgb_texture {false};
    std::array<float, 3> bg_color {0.663f, 0.663f, 0.663f};

    void render() override;
    void update() override;
    void imgui() override;
};

struct Shadow_Mapping_Demo : Demo3d {
    Shadow_Mapping_Demo();
    ~Shadow_Mapping_Demo();

    Shader* shader;
    Shader* shader_shadow_map;
    Shader* static_object_shader;
    Texture* floor_texture;
    Texture* chiti;
    std::unique_ptr<Sampler> shadow_sampler;
    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<Vertex_Array> vao;
    std::unique_ptr<Vertex_Array> cube_vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> vbo;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> cube_vbo;
    Directional_Light directional_light;

    glm::mat4 light_proj;
    glm::mat4 light_view;
    float min_bias {0.0f};
    float max_bias {0.0f};

    float left   {};
    float right  {};
    float bottom {};
    float top    {};
    float near   {};
    float far    {};

    std::vector<Transform> cubes;
    std::array<i32, 2> shadowmap_dims {1024, 1024};

    u32 shadowmap_fbo;
    u32 shadowmap_texture;
    void render() override;
    void update() override;
    void imgui() override;

private:
    void recreate_framebuffer() noexcept;
};

struct Spot_Lights_Demo : Demo3d {
    Spot_Lights_Demo();
    ~Spot_Lights_Demo();

    Texture* floor_texture;
    Texture* chiti;
    Shader* shader;
    Shader* static_object_shader;
    Shader* shadow_shader;

    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<Sampler> shadow_sampler;
    std::unique_ptr<Vertex_Array> plane_vao;
    std::unique_ptr<Vertex_Array> cube_vao;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> plane_vbo;
    std::unique_ptr<Named_Buffer_Object<vertex::Vertex3d>> cube_vbo;
    Spot_Light spot_light {};
    bool draw_direction {false};

    std::vector<Transform> cubes;

    u32 shadow_fbo;
    u32 shadowmap;
    i32 shadowmap_width {1024};
    i32 shadowmap_height {1024};
    glm::mat4 light_proj;
    glm::mat4 light_view;
    float light_fov {45.0f};
    float near {1.0f};
    float far {10.0f};
    float min_bias {0.0f};
    float max_bias {0.0f};
    void render() override;
    void update() override;
    void imgui() override;
};


}
