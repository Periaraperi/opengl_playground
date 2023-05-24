#pragma once

#include <glm/vec4.hpp>

namespace Peria_Engine {
class Window;
struct Window_Settings;

class Graphics {
public:
    Graphics(const Window_Settings& props);
    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics& operator=(Graphics&&) = delete;
    ~Graphics();

    void set_clear_color(const glm::vec4& color);
    void clear_buffer();
    void swap_buffers();

    void Draw_Quad(float x, float y, float scale); // colored quad
private:
    Window* _main_window;
    glm::vec4 _clear_color;
};

}
