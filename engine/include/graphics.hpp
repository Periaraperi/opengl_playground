#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

namespace Peria_Engine {
class Window;
class Mesh;
class Shader;
class Texture;
struct Window_Settings;
struct Quad_Vertex;

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

    void bind_dynamic_mesh();
    void unbind_dynamic_mesh();

    void bind_quad_shader();
    void unbind_quad_shader();

    void draw_quad(float x, float y, float scale, const glm::vec4& color); // colored quad

    void batch_render_quads(); // main gl Draw calls happen here

private:
    Window* _main_window;
    glm::vec4 _clear_color;
    std::unique_ptr<Mesh> _dynamic_mesh;
    std::unique_ptr<Shader> _quad_shader;
    std::unique_ptr<Texture> _white_texture;
    glm::mat4 _proj;

    struct Quad {
        float x,y,s;
        glm::vec4 color;
        float tex_id;
    };
    std::vector<Quad> _quads_to_render;

    // maximum number of quads one batch can handle
    static constexpr int DEFAULT_MAX_QUADS = 500;

    // helper member functions
    std::array<Quad_Vertex,4> get_quad_vertices(const Quad& q);
    glm::vec2 sdl_to_gl(const glm::vec2& v);
};

}
