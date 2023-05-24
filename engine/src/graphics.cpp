#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "logger.hpp"
#include "window.hpp"

namespace Peria_Engine {
Graphics::Graphics(const Window_Settings& props)
    :_main_window(nullptr), _clear_color({0.0f,0.0f,0.0f,1.0f})
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

//void Graphics::Draw_Quad(float x, float y, float scale)
//{
//
//}

}
