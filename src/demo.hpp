#pragma once

#include <memory>
#include <array>
#include <vector>

#include "attenuation.hpp"
#include "materials.hpp"
#include "camera.hpp"
#include "vertex.hpp"

namespace peria::graphics {
class Shader;
class Sampler;
class Texture;
class Vertex_Array;
template<typename T> class Named_Buffer_Object;
}

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

struct Demo {
    Demo();
    virtual void render() = 0;
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

struct Demo_Point_Light : public Demo {
    Demo_Point_Light();
    void render() override;
    void imgui() override;

    std::unique_ptr<Shader> light_source_shader;
    std::unique_ptr<Shader> lighting_shader;

    Point_Light light_source;
    materials::Material material;
};

struct Demo_Combined_Lights : public Demo {
    Demo_Combined_Lights();
    void render() override;
    void imgui() override;

    std::unique_ptr<Shader> light_source_shader;
    std::unique_ptr<Shader> combined_lights_shader;

    std::vector<Point_Light> point_lights;
    Directional_Light directional_light;
    Spot_Light spot_light;
    materials::Material material;
};

}
