#pragma once

#include "peria_color.hpp"

typedef struct SDL_Window SDL_Window;

namespace peria::engine {

class graphics {
public:
    graphics();
    ~graphics();

    void set_clear_color(const peria_color<float>& color);
    void clear_buffer();
    void swap_buffers(SDL_Window* window);

private:
    peria_color<float> clear_color{peria::color::WHITE};
public:
    graphics(const graphics&) = delete;
    graphics& operator=(const graphics&) = delete;
    graphics(graphics&&) = default;
    graphics& operator=(graphics&&) = default;
};

}
