#pragma once

//#include <memory>
//#include <vector>
//
//#include "graphics.hpp"
//#include "materials.hpp"
//#include "texture_cubemap.hpp"
//#include "vertex.hpp"
//
//#include "named_buffer_object.hpp"
//#include "shader.hpp"
//#include "texture.hpp"
//#include "frame_buffer.hpp"
//#include "sampler.hpp"
//
//#include "model.hpp"

#include "vertex_array.hpp"
#include "texture.hpp"
#include "sampler.hpp"
#include "named_buffer_object.hpp"
#include "shader.hpp"
#include "camera.hpp"

namespace peria::demos {

struct Demo {
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void imgui() = 0;
    virtual void recalculate_projection() = 0;
    virtual Camera& get_camera() = 0;

    glm::mat4 projection;

    virtual ~Demo() {}
};

//struct Demo_Refactor : public Demo {
//    void update() override
//    {peria::log("UPDATE");}
//
//    void render() override
//    {peria::log("RENDER");}
//
//    void imgui() override
//    {peria::log("IMGUI");}
//};
//
//struct Demo_Refactor2 : public Demo {
//    void update() override
//    {peria::log("UPDATE 2");}
//
//    void render() override
//    {peria::log("RENDER 2");}
//
//    void imgui() override
//    {peria::log("IMGUI 2");}
//};

struct Textured_Cube : public Demo {
    Textured_Cube();
    void update() override;
    void render() override;
    void imgui() override;
    void recalculate_projection() override;

    Camera& get_camera() override {return camera;}
    Camera camera;

    Vertex_Array vao;
    Buffer_Object vbo;
    Texture<Texture2D> tex;
    Sampler sampler;
    Shader shader;
};



}
