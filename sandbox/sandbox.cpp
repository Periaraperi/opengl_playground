#include "engine.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <array>

struct Quad {
    float x,y,s;
    glm::vec4 color;
};

class Sandbox : public Peria_Engine::Application {
public:
    Sandbox(const Peria_Engine::Window_Settings& props)
        :Application(props)
    {
        PERIA_LOG("Sandbox Constructor!");
        _graphics->set_clear_color({0.0f,0.65f,0.12f,1.0f});
    }

    ~Sandbox() override
    {PERIA_LOG("Sandbox Destructor!");}

    void update() override
    {
        auto mouse = _input_manager->get_mouse_pos();
        glm::vec4 cl = {0.65f,0.0f,0.4f,1.0f};
        if (_input_manager->mouse_pressed(Mouse_Button::LEFT)) {
            _rects.push_back({mouse.x,mouse.y,50.0f,cl});
        }
    }

    void render() override
    {
        for (auto& r:_rects) {
            _graphics->draw_quad(r.x,r.y,r.s,r.color);
        }
    }
private:
    std::vector<Quad> _rects;
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
