#include "application.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "logger.hpp"
#include "input_manager.hpp"
#include "graphics.hpp"

namespace Peria_Engine {
Application::Application(const Window_Settings& props)
    :_graphics(nullptr), _input_manager(nullptr), _running(false) 
{
    PERIA_LOG("Application Constructor!");
    // init SDL window and glContext
    _graphics = new Graphics(props);
    PERIA_ASSERT(_graphics!=nullptr);

    // init input manager
    _input_manager = Input_Manager::get();
    PERIA_ASSERT(_input_manager!=nullptr);

    _running = true;
}

Application::~Application()
{
    PERIA_LOG("Application Destructor!");
    Input_Manager::shutdown();
    delete _graphics;
}

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

        _input_manager->update_prev_input_state();

        // ======================= RENDERING ======================
        _graphics->clear_buffer();

        render(); // render commands here, add to render queue
        
        // bind shader and dynamic mesh for quads
        _graphics->bind_dynamic_mesh();
        _graphics->bind_quad_shader();

        _graphics->batch_render_quads(); // will batch render all quads in render queue
        
        _graphics->unbind_quad_shader();
        _graphics->unbind_dynamic_mesh();
        
        _graphics->swap_buffers();
    }
}

void Application::update()
{}

void Application::render()
{}

}
