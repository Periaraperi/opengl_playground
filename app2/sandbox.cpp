#include "engine.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class Sandbox : public Peria_Engine::Application {
public:
    Sandbox(const Peria_Engine::Window_Settings& props)
        :Application(props)
    {
        PERIA_LOG("Sandbox Constructor!");
        _graphics->set_clear_color({0.0f,0.65f,0.12f,1.0f});
    //    _quad_shader = std::make_unique<Shader>("assets/default.vert","assets/default.frag");
    //    _quad_shader->enable();
    //    std::array<int,3> arr = {0,1,2};
    //    _quad_shader->set_array("u_textures",3,&arr[0]);
    //    _tex = std::make_unique<Texture>("assets/pikapika.png");
    //    _tex->bind(0);

    //    glm::vec4 col = {1.0f,1.0f,1.0f,1.0f};
    //    std::vector<Quad_Vertex> data = {
    //        {{{-0.5f,-0.5f},col,{0.0f,0.0f},0.0f},
    //         {{-0.5f, 0.5f},col,{0.0f,1.0f},0.0f},
    //         {{ 0.5f, 0.5f},col,{1.0f,1.0f},0.0f},
    //         {{ 0.5f,-0.5f},col,{1.0f,0.0f},0.0f}}
    //    };
    //    std::vector<uint32_t> indices = {0,1,2, 0,2,3};
    //    _quad_mesh = std::make_unique<Mesh>(data,indices);
    //    _proj = glm::ortho(0.0f,800.0f,0.0f,600.0f,-1.0f,1.0f);
    }

    ~Sandbox() override
    {PERIA_LOG("Sandbox Destructor!");}

    void update() override
    {
        if (_input_manager->key_pressed(PERIA_KEY_W)) {
            std::cout << "update\n";
        }
    }

    void render() override
    {
    }
    glm::mat4 get_model(float x, float y, float scale)
    {
        return glm::translate(glm::mat4(1.0f),{x,y,0.0f})*
               glm::scale(glm::mat4(1.0f),{scale,scale,1.0f});
    }
private:
    //std::unique_ptr<Shader> _quad_shader;
    //std::unique_ptr<Mesh> _quad_mesh;
    //std::unique_ptr<Texture> _tex;
    //glm::mat4 _proj;
};

int main()
{
    Peria_Engine::Window_Settings props = {
        "sandbox",800,600,false
    };
    Peria_Engine::Application* sandbox = new Sandbox(props);
    sandbox->run();
    delete sandbox;
    return 0;
}
