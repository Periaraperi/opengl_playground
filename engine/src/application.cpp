#include "application.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "logger.hpp"
#include "window_settings.hpp"
#include "input_manager.hpp"

namespace peria::engine {
Application::Application(const Window_Settings& settings)
    :_sdl_initializer{settings.opengl}, _running{false}
{
    u32 flags = SDL_WINDOW_SHOWN;
    if (settings.resizable) flags |= SDL_WINDOW_RESIZABLE;
    if (settings.opengl)    flags |= SDL_WINDOW_OPENGL;

    _window = std::unique_ptr<SDL_Window, sdl::window_deleter>(SDL_CreateWindow(
                                                               settings.title.c_str(), 
                                                               SDL_WINDOWPOS_CENTERED,
                                                               SDL_WINDOWPOS_CENTERED,
                                                               settings.width, 
                                                               settings.height, 
                                                               flags));
    if (!_window) {
        throw std::runtime_error{"SDL Error: " + std::string{SDL_GetError()}};
    }
    std::cerr << "SDL_Window created successfully\n";

    if (settings.opengl) {
        _context = std::unique_ptr<void, sdl::gl_context_deleter>(SDL_GL_CreateContext(_window.get()));
        if (!_context) {
            throw std::runtime_error{"SDL Error: " + std::string{SDL_GetError()}};
        }
        std::cerr << "SDL_GLContext created successfully\n";
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        throw std::runtime_error{"GL loader failed"};
    }

    _input_manager = std::make_unique<Input_Manager>();

    _running = true;
}

Application::~Application()
{ PERIA_LOG("Application Destructor!"); }

void Application::run()
{
    while (_running) {
        _input_manager->update_mouse();
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type==SDL_QUIT) {
                _running = false;
                break;
            }
        }
        
        update();

        _input_manager->update_prev_state();

        // ======================= RENDERING ======================
        //_graphics->clear_buffer();

        render(); // render commands here, add to render queue
        
        // bind shader and dynamic mesh for quads
        //_graphics->bind_dynamic_mesh();
        //_graphics->bind_quad_shader();

        //_graphics->batch_render_quads(); // will batch render all quads in render queue
        //
        //_graphics->unbind_quad_shader();
        //_graphics->unbind_dynamic_mesh();
        //
        //_graphics->swap_buffers();
    }
}

void Application::update()
{}

void Application::render()
{}

}
