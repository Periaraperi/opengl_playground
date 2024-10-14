#include "application.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

#include "input_manager.hpp"
#include "graphics.hpp"
#include "window_settings.hpp"

namespace peria::engine {
application::application(window_settings&& settings_)
    :sdl_initializer{}, settings{std::move(settings_)}, running{false}
{
    u32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
    if (settings.resizable) flags |= SDL_WINDOW_RESIZABLE;

    window = std::unique_ptr<SDL_Window, sdl::window_deleter>(SDL_CreateWindow(
                                                               settings.title.c_str(), 
                                                               SDL_WINDOWPOS_CENTERED,
                                                               SDL_WINDOWPOS_CENTERED,
                                                               settings.width, 
                                                               settings.height, 
                                                               flags));
    if (!window) {
        throw std::runtime_error{"SDL Error: " + std::string{SDL_GetError()}};
    }
    std::cerr << "SDL_Window created successfully\n";

    context = std::unique_ptr<void, sdl::gl_context_deleter>(SDL_GL_CreateContext(window.get()));
    if (!context) {
        throw std::runtime_error{"SDL Error: " + std::string{SDL_GetError()}};
    }
    std::cerr << "SDL_GLContext created successfully\n";

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        throw std::runtime_error{"GL loader failed"};
    }

    input = std::make_unique<input_manager>();
    graphics = std::make_unique<peria::engine::graphics>();

    running = true;
}

application::~application()
{ std::cerr << "application shutting down\n"; }

void application::run()
{
    while (running) {
        input->update_mouse();
        for (SDL_Event ev; SDL_PollEvent(&ev);) {
            if (ev.type==SDL_QUIT) {
                running = false;
                break;
            }
        }

        update();
        input->update_prev_state();

        graphics->clear_buffer();
        render();
        graphics->swap_buffers(window.get());
    }
}

void application::update()
{}

void application::render()
{

}

}
