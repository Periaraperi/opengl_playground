#pragma once

#include "frame_buffer.hpp"
#include "light_data.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "sampler.hpp"

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
        i32 shadowmap_w {1024};
        i32 shadowmap_h {1024};
        float min_bias  {0.01f};
        float max_bias  {0.1f};
        glm::mat4 light_projection;
        glm::mat4 light_view;
    };
    Shadow_data shadow_data;

    Vertex_Array cube_vao;
    Buffer_Object cube_vbo;
    Frame_Buffer shadow_fbo;

    Texture2D shadowmap;
    Texture2D chiti;


    Shader shadow_shader;
    Shader omni_shadow_shader;
    Shader light_shader;
    Sampler shadow_sampler;
    Sampler sampler;

    Directional_Light directional_light;

};


}
