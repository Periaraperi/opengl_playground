#pragma once

#include <glm/vec4.hpp>

namespace peria::engine {

struct SDL_Window;

class Graphics {
public:
    Graphics();
    ~Graphics();

    void set_clear_color(const glm::vec4& color);
    void clear_buffer();
    void swap_buffers(SDL_Window* window);

private:

public:
    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics& operator=(Graphics&&) = delete;
};

}
