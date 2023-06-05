#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "logger.hpp"
#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <iostream>

namespace Peria_Engine {
Graphics::Graphics(const Window_Settings& props)
    :_main_window(nullptr), _clear_color({0.0f,0.0f,0.0f,1.0f}), 
    _dynamic_mesh(nullptr), _quad_shader(nullptr), _white_texture(nullptr),
    _proj(glm::mat4(1.0f))
{
    PERIA_LOG("Graphics Init");
    // init SDL
    SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
    
    SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4));
    SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,2));
    SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE));

    _main_window = new Window(props);
    PERIA_ASSERT(_main_window!=nullptr);
    PERIA_ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)); 
    _main_window->window_resize(props.width,props.height); // to init viewport
                                                           
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

    _dynamic_mesh = std::make_unique<Mesh>(DEFAULT_MAX_QUADS);

    // load textures
    _white_texture = std::make_unique<Texture>();

    // default quad shader init
    _quad_shader = std::make_unique<Shader>("assets/default.vert","assets/default.frag");
    _quad_shader->bind();
    std::array<int,3> texture_slots;
    for (int i=0; i<3; ++i) // max of 3 textures at the same time
        texture_slots[i] = i;
    _quad_shader->set_array("u_textures",texture_slots.size(),&texture_slots[0]);
    _white_texture->bind(0);

    glm::vec2 window_dim = _main_window->get_dimensions();
    _proj = glm::ortho(0.0f,window_dim.x,0.0f,window_dim.y,-1.0f,1.0f);
}

Graphics::~Graphics()
{
    PERIA_LOG("Graphics ShutDown");
    delete _main_window;
    SDL_Quit();
}

void Graphics::set_clear_color(const glm::vec4& color)
{_clear_color = color;}

void Graphics::clear_buffer()
{
    auto& c = _clear_color;
    GL_CALL(glClearColor(c.r,c.g,c.b,c.a));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Graphics::swap_buffers()
{
    SDL_GL_SwapWindow(_main_window->get_sdl_window());
}

void Graphics::bind_dynamic_mesh()
{
    _dynamic_mesh->bind();
}

void Graphics::unbind_dynamic_mesh()
{
    _dynamic_mesh->unbind();
}

void Graphics::bind_quad_shader()
{
    _quad_shader->bind();
}

void Graphics::unbind_quad_shader()
{
    _quad_shader->unbind();
}

void Graphics::draw_quad(float x, float y, float scale, const glm::vec4& color)
{
    _quads_to_render.push_back({x,y,scale,color,0});
}

void Graphics::batch_render_quads()
{
    _quad_shader->set_mat4("u_mvp",_proj);
    // generate vertex data from quads_to_render
    std::vector<Quad_Vertex> data;
    int quad_counter = 0;
    int N = (int)_quads_to_render.size();
    for (int i=0; i<N; ++i) {
        if (quad_counter<DEFAULT_MAX_QUADS) {
            std::array<Quad_Vertex,4> current_quad = get_quad_vertices(_quads_to_render[i]);
            for (int j=0; j<4; ++j) {
                data.push_back(current_quad[j]);
            }
            ++quad_counter;
        }
        if (quad_counter==DEFAULT_MAX_QUADS) { // render what we have, i.e flush
            _dynamic_mesh->update_buffer(data,0);
            
            GL_CALL(glDrawElements(GL_TRIANGLES,quad_counter*6,GL_UNSIGNED_INT,0));
            data.clear();
            quad_counter = 0;
        }
    }
    if (!data.empty()) { // render what we have, i.e flush
        _dynamic_mesh->update_buffer(data,0);
        
        GL_CALL(glDrawElements(GL_TRIANGLES,quad_counter*6,GL_UNSIGNED_INT,0));
        data.clear();
    }

    _quads_to_render.clear();
}

// helpers here
std::array<Quad_Vertex,4> Graphics::get_quad_vertices(const Quad& q)
{
    std::array<Quad_Vertex,4> v = {
        {{sdl_to_gl({q.x-q.s,q.y-q.s}),q.color,{0.0f,0.0f},q.tex_id},
         {sdl_to_gl({q.x-q.s,q.y+q.s}),q.color,{0.0f,1.0f},q.tex_id},
         {sdl_to_gl({q.x+q.s,q.y+q.s}),q.color,{1.0f,1.0f},q.tex_id},
         {sdl_to_gl({q.x+q.s,q.y-q.s}),q.color,{1.0f,0.0f},q.tex_id}}
    };
    return v; 
}

glm::vec2 Graphics::sdl_to_gl(const glm::vec2& v)
{
    auto h = _main_window->get_dimensions().y;
    return {v.x,h-v.y};
}

}
