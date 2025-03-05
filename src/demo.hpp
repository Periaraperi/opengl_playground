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

    Texture2D shadowmap;
    Texture2D chiti;
    Texture2D monkey_color;
    Texture2D uv_sphere_color;
    Texture2D ico_sphere_color;

    Shader shadow_shader;
    Shader omni_shadow_shader;
    Shader light_shader;
    Shader colored_obj_shader;
    Shader line_shader;
    Sampler shadow_sampler;
    Sampler sampler;

    Model monkey;
    Model uv_sphere;
    Model ico_sphere;

    std::array<float, 3> campos;
    std::array<float, 3> camviewdir;
    glm::vec4 mouse_ray;


private:
    void draw_scene(const Shader& shader);
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

    Shader model_shader;
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
    struct Line {
        std::array<float, 3> p1 {};
        std::array<float, 3> p2 {};
        std::array<float, 3> color {};
    } line;

    struct Line2 {
        std::array<float, 3> p {};
        std::array<float, 3> dir {};
        float scalar {};
        std::array<float, 3> color {};
    } line2;
    std::array<Vertex<Pos3D, Color4>, 2> line_data;
    std::vector<Line> lines;
    std::vector<Line2> ls;
};

}
