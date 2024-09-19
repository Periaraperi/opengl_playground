#pragma once

#include <glm/vec4.hpp>

namespace Peria_Engine {
// forward declarations
struct Window_Settings;
class Input_Manager;
class Graphics;

class Application {
public:
    Application(const Window_Settings& props);
    virtual ~Application();
    
    void run();
    virtual void update();
    virtual void render();

protected:
    Graphics* _graphics;
    Input_Manager* _input_manager;
private:
    bool _running;

public:
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;
};
}
