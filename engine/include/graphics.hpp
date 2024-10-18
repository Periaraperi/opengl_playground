#pragma once

#include "peria_color.hpp"
#include "peria_types.hpp"
#include "vertex_array.hpp"
#include "vertex_types.hpp"

#include <glm/mat4x4.hpp>

typedef struct SDL_Window SDL_Window;

namespace peria::engine {

template <typename T, typename U>
class vertex_buffer;

enum class vertex_buffer_type;

class graphics {
public:
    graphics();
    ~graphics();

    void set_clear_color(const peria_color<float>& color);
    void clear_buffer();
    void swap_buffers(SDL_Window* window);

    void set_viewport(i32 x, i32 y, u32 w, u32 h);

private:
    peria_color<float> clear_color{peria::color::WHITE};

    vertex_array vao;
    vertex_buffer<simple_vertex, vertex_buffer_type> vbo;
public:
    graphics(const graphics&) = delete;
    graphics& operator=(const graphics&) = delete;
    graphics(graphics&&) = default;
    graphics& operator=(graphics&&) = default;
};

}
