#pragma once

#include "frame_buffer.hpp"
#include "light_data.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "sampler.hpp"
#include "model.hpp"
#include "shadowmapper.hpp"
#include "entity.hpp"
#include "cam2d.hpp"

namespace peria::demos {

struct Demo {
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void imgui() = 0;
    virtual void recalculate_projection() = 0;

    [[nodiscard]]
    virtual Camera& get_camera() = 0;

    glm::mat4 projection;

    virtual ~Demo() {}
};

struct Textured_Cube : public Demo {
    Textured_Cube();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Vertex_Array vao;
    Buffer_Object vbo;
    Texture2D tex;
    Sampler sampler;
    Shader shader;
};

struct Kvadebi : public Demo {
    Kvadebi();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Vertex_Array vao1;
    Vertex_Array vao2;
    Buffer_Object vbo1;
    Buffer_Object vbo2;
    Buffer_Object ibo;
    Texture2D solid_color;
    Texture2D tex1;
    Sampler sampler;
    Shader shader;
    
    struct Trans {
        std::array<float, 3> pos;
        std::array<float, 3> scale;
        float angle {0.0f};
    };

    Trans t1;
    Trans t2;
    float speed {0.0f};
};

struct Shadows : public Demo {
    Shadows();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    struct Shadow_data {
        i32 shadowmap_w {2048};
        i32 shadowmap_h {2048};
        glm::mat4 light_projection;
        glm::mat4 light_view;
    };
    bool toggle_shadows {true};

    Shadow_data shadow_data;

    float min_bias  {0.0f};
    float max_bias  {0.0f};

    struct Light_Data {
        Directional_Light directional_light;
        std::array<Spot_Light, 8> spot_lights;
        i32 active_spot_lights {};

        bool toggle_directional_light {false};
        bool toggle_spot_lights       {false};
    } light_data;

    Vertex_Array cube_vao;
    Vertex_Array line_vao;
    Buffer_Object cube_vbo;
    Buffer_Object line_vbo;
    Frame_Buffer shadow_fbo;
    Frame_Buffer picking_fbo;

    Texture2D shadowmap;
    Texture2D picking_depth;
    Texture2D picking_color;
    Texture2D chiti;
    Texture2D monkey_color;
    Texture2D uv_sphere_color;
    Texture2D ico_sphere_color;

    Shader shadow_shader;
    Shader omni_shadow_shader;
    Shader light_shader;
    Shader colored_obj_shader;
    Shader line_shader;
    Shader picking_shader;
    Sampler shadow_sampler;
    Sampler sampler;

    Model monkey;
    Model uv_sphere;
    Model ico_sphere;

    std::array<float, 3> campos;
    std::array<float, 3> camviewdir;
    glm::vec4 mouse_ray;

    Shadowmapper shadowmapper;

private:
    void draw_scene(const Shader& shader);
    void draw_scene_for_picking();
};

struct Transformations : public Demo {
    Transformations();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;
    Camera camera2;
    bool use_main_camera {true};

    Vertex_Array cube_vao;
    Buffer_Object cube_vbo;

    Texture2D chiti;

    Shader colored_obj_shader;
    Sampler sampler;

    glm::vec3 rotating_cube_position;
    float angle {0.0f};
    bool do_ortho{false};
    std::array<float, 3> scale {1.0f, 1.0f, 1.0f};
};

struct Modelebi : public Demo {
    Modelebi();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Model monkey;
    Model uv_sphere;
    Model tree;

    Shader shader;
    Shader model_shader;
    Sampler sampler;
    Texture2D texture;
};

struct Lines : public Demo {
    Lines();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Vertex_Array vao;
    Buffer_Object vbo;
    Shader line_shader;
    Shader model_shader;
    Model uv_sphere;

    struct Sphere {
        std::array<float, 3> pos {};
        float radius {};
    } sphere;

    struct Line {
        std::array<float, 3> p {};
        std::array<float, 3> dir {};
        float t {};
        std::array<float, 3> color {};
    } line;
    std::array<Vertex<Pos3D, Color4>, 2> line_data;
    std::vector<Line> lines;
};

struct Mouse_Moving_Basic : Demo {
    Mouse_Moving_Basic();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Shader model_shader;
    Model uv_sphere;

    glm::vec3 mouse_world {};

    struct Sphere {
        std::array<float, 3> pos {};
        float radius {};
    } sphere;
};

struct Mouse_Picking : Demo {
    Mouse_Picking();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Shader model_shader;
    Shader picking_shader;
    Model uv_sphere;

    Frame_Buffer picking_fbo;
    Texture2D    picking_texture;
    Texture2D    picking_depth_texture;
    Sampler      sampler;

    struct Sphere {
        std::array<float, 3> pos {};
        float radius {};
    } sphere;
};

struct Many_Shadows : Demo {
    Many_Shadows();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;
    
    Vertex_Array cube_vao;
    Vertex_Array line_vao;
    Buffer_Object cube_vbo;
    Buffer_Object lights_ubo;
    Buffer_Object shadows_ubo;
    Buffer_Object line_vbo;
    std::array<Model, 3> models;
    Model tree;
    Texture2D tree_texture;

    Sampler shadow_sampler;
    Sampler sampler;

    std::array<Texture2D, 4> colors;

    Shader light_shader;
    Shader shadow_shader;
    Shader model_shader;
    Shader model_shader2;
    Shader line_shader;

    Shadowmapper dir_light_shadowmapper;
    std::array<Shadowmapper, 1> spl_shadowmappers;

    Directional_Light dir_light;
    std::array<Spot_Light, 1> spls;
    std::array<Point_Light, 1> pls;
    bool render_global_axis {false};
};

struct Normal_Mapping : Demo {
    Normal_Mapping();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Vertex_Array  plane_vao;
    Buffer_Object plane_vbo;

    Shader shader;
    Shader shader2;
    Shader model_shader;

    Texture2D wall;
    Texture2D wall_normal;
    Texture2D wall_normal2;
    Sampler   wall_sampler;

    Point_Light pl;

    std::array<float, 3> pos   {};
    std::array<float, 3> scale {1.0f, 1.0f, 1.0f};
    std::array<float, 3> rotation_angles {};

    bool normal_mapping {false};
    bool tbn {false};
};

struct Fun_With_Textures : Demo {
    Fun_With_Textures();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Vertex_Array  vao;
    Buffer_Object vbo;
    Buffer_Object ibo;

    Shader shader;

    Texture2D texture;
    Sampler sampler;
};

struct Color_Correction_And_Stuff : Demo {
    Color_Correction_And_Stuff();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    Vertex_Array  screen_quad_vao;
    Buffer_Object screen_quad_vbo;
    Buffer_Object screen_quad_ibo;

    Shader lighting_shader;
    Shader screen_shader;
    Sampler sampler;

    Frame_Buffer hdr_fbo;
    Texture2D hdr_color_texture;
    Texture2D hdr_depth_texture;

    Buffer_Object lights_ubo;
    Directional_Light dir_light;
    Point_Light pl;

    bool gamma{false};
    bool hdr{false};
    float exposure {1.0f};

    struct Picking_Info {
        Frame_Buffer fbo;
        Texture2D    color_texture;
        Texture2D    depth_texture;
        Shader       shader;
        Sampler      sampler;
        int          selected{-1};
        bool         clicked{false};
    } picking;

    std::vector<Model> all_models;
    std::vector<Texture2D> all_textures;
    std::vector<Entity> entities;
};

struct Gizmos : Demo {
    Gizmos();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override {return camera;}
    Camera camera;

    std::vector<Model> all_models;

    struct Picking_Info {
        Frame_Buffer fbo;
        Texture2D color_texture;
        Texture2D depth_texture;
        Shader    shader;
        Sampler   sampler;
        int       selected{-1};
    } picking;

    Shader light_shader;
    std::vector<Entity> entities;
};

struct Multi_Sampled : Demo {
    Multi_Sampled();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override { return camera; }
    Camera camera;

    Shader shader;
    Shader quad_shader;
    //Shader shader_multisampled;
    Sampler sampler;
    i32 w {40};
    i32 h {30};
    struct info {
        Frame_Buffer fbo;
        Texture2D texture;
        glm::mat4 proj;
    } info;

    struct info_multisampled {
        Frame_Buffer fbo;
        Texture2D texture;

        Frame_Buffer fbo_middle;
        Texture2D texture_middle;

        glm::mat4 proj;
    } info_ms;

    Vertex_Array tri_vao;
    Buffer_Object tri_vbo;

    Vertex_Array quad_vao;
    Buffer_Object quad_vbo;
    Buffer_Object quad_ibo;

};

struct Aspect_Ratio : Demo {
    Aspect_Ratio();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    void calculate_dimensions();

    [[nodiscard]]
    Camera& get_camera() override { return camera; }
    Camera camera;

    glm::mat4 projection;
    glm::mat4 fbo_projection;

    Shader shader;
    
    i32 tex_width {};
    i32 tex_height {};
    i32 tex_width2 {};
    i32 tex_height2 {};
    i32 fbo_width {1600};
    i32 fbo_height {900};
    i32 final_width {};
    i32 final_height {};

    Texture2D texture_chiti;
    Texture2D texture_pika;
    Texture2D fbo_color_texture;
    Sampler sampler;

    Vertex_Array quad_vao;
    Buffer_Object quad_vbo;
    Buffer_Object quad_ibo;
    Frame_Buffer fbo;
};

struct Bloom : Demo {
    Bloom();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override { return camera; }
    Camera camera;

    glm::mat4 projection;
    Shader light_shader;
    Shader screen_shader;
    Shader blur_shader;
    Shader point_light_bloom_shader;
    Sampler sampler_repeat;

    struct hdr {
        // floating point framebuffer for scene rendering and lighting calculations
        Frame_Buffer fbo;
        Texture2D color_texture;
        Texture2D brightness_texture; // for bloom
        Texture2D depth_texture;

        bool do_hdr {true};
        bool gamma_correction {true};
        float exposure {1.0f};
        float intensity {1.0f};
        bool atn_quad {false};
        bool diffuse_textures_loaded_as_srgb {true};
    } hdr;

    struct bloom {
        std::array<Frame_Buffer, 2> fbos;
        std::array<Texture2D, 2> textures;
        Sampler sampler;
        bool horizontal {true};
        bool do_bloom {false};
        int number_of_blur_passes {10};
    } bloom;

    struct Cube {
        Vertex_Array vao;
        Buffer_Object vbo;
    } cube;

    struct Screen_Quad {
        Vertex_Array vao;
        Buffer_Object vbo;
        Buffer_Object ibo;
    } screen_quad;

    Texture2D solid_color1;
    Texture2D solid_color2;
    Texture2D solid_color3;
    Texture2D floor_texture;
    Texture2D brick_texture;
    Texture2D crate_texture;

    Directional_Light dir_light;
    std::array<Point_Light, 3> point_lights;
    Buffer_Object lights_ubo;
    static constexpr float diffuse_max {5000.0f};
};

struct Deferred_Rendering : Demo {
    Deferred_Rendering();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override { return camera; }
    Camera camera;

    Shader light_shader;
    Shader screen_shader; // final screen quad
    Shader lshader; // final screen quad
    Shader gbuffer_shader;
    Shader deferred_light_shader;
    Sampler sampler_repeat;

    struct backpack {
        Model backpack;
        Texture2D backpack_diffuse;
        //Texture2D backpack_specular; add this later. First test with diffuse only
    } backpack;

    struct Cube {
        Vertex_Array vao;
        Buffer_Object vbo;
    } cube;
    Texture2D container_diffuse;
    Texture2D floor_diffuse;

    struct hdr {
        Frame_Buffer fbo;
        Texture2D color_texture;
        Texture2D depth_texture;
        float exposure {1.0f};
        float intensity {1.0f};
    } hdr;

    struct Screen_Quad {
        Vertex_Array vao;
        Buffer_Object vbo;
        Buffer_Object ibo;
    } screen_quad;

    struct g_buffer {
        Frame_Buffer fbo;
        Texture2D depth_texture;
        Texture2D pos_texture;
        Texture2D diffuse_color_texture;
        Texture2D normal_texture;
    } gbuffer;

    Buffer_Object lights_ubo;
    std::vector<Point_Light> point_lights;
    bool deferred {false};

    glm::mat4 projection;
    std::vector<Transform> backpack_transforms;
    std::vector<Transform> cube_transforms;
};

struct Platonic_Solids : Demo {
    Platonic_Solids();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override { return camera; }
    Camera camera;

    Shader light_shader;
    Shader normal_shader;
    Shader screen_shader; // final screen quad
    Sampler sampler_repeat;
    Model tetra;

    struct hdr {
        Frame_Buffer fbo;
        Texture2D color_texture;
        Texture2D depth_texture;
        float exposure {1.0f};
        float intensity {1.0f};
    } hdr;

    struct Screen_Quad {
        Vertex_Array vao;
        Buffer_Object vbo;
        Buffer_Object ibo;
    } screen_quad;

    Buffer_Object lights_ubo;
    Point_Light point_light;

    glm::mat4 projection;

    struct Tetrahedron {
        Vertex_Array vao;
        Buffer_Object vbo;
        Buffer_Object ibo;
    } tetrahedron;


    Vertex_Array plane_vao;
    Buffer_Object plane_vbo;
    Texture2D solid_color1;
    Texture2D solid_color2;
    
};

struct Pan_Zoom : Demo {
    Pan_Zoom();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override { return camera; }
    Camera camera; // NOT USED HERE, I KNOW THIS CODE IS A MESS. :P

    Shader shader;
    Sampler sampler_repeat;
    Texture2D solid_white;
    Texture2D chiti;

    struct quad {
        Vertex_Array vao;
        Buffer_Object vbo;
        Buffer_Object ibo;
    } quad;

    Camera2D cam2d;
};

struct Batching_Vs_Instancing : Demo {
    Batching_Vs_Instancing();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    [[nodiscard]]
    Camera& get_camera() override { return camera; }
    Camera camera; // NOT USED HERE, I KNOW THIS CODE IS A MESS. :P

    Shader batch_shader;
    Shader instance_shader;

    //struct quad {
    //    Vertex_Array vao;
    //    Buffer_Object vbo;
    //    Buffer_Object ibo;
    //} quad;

    std::array<glm::vec3, 128> colors;

    std::vector<glm::mat4> quad_models;
    std::vector<glm::vec3> quad_colors;

    struct quad_batcher {
        Vertex_Array vao;
        Buffer_Object vbo;
        Buffer_Object ibo;

        int max_quads_per_batch {256};
        int iterations {};
        int quad_count {};
        using vertex_t = Vertex<Pos2D, Color3>;
        std::vector<vertex_t> data;
        std::vector<u32> indices;
    } quad_batcher;

    struct quad_instancer {
        Vertex_Array vao;
        Buffer_Object instance_vbo;
        Buffer_Object quad_vbo;
        Buffer_Object color_vbo;
        Buffer_Object ibo;

        int max_instances {256};
        int quad_count {};
    } quad_instancer;

    void resize_batcher();

    bool use_batching {true};

    Camera2D cam2d;
};


}
