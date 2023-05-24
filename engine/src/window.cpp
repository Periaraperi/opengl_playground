#include "window.hpp"

#include "logger.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace Peria_Engine {
Window::Window(const Window_Settings& props)
    :_title(props.title), _width(props.width), _height(props.height),
     _window(nullptr), _context(nullptr)
{
    PERIA_LOG("Init Window");
    PERIA_ASSERT(_width>0 && _height>0);
    
    uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
    if (props.resizable) flags |= SDL_WINDOW_RESIZABLE;
    _window = (SDL_Window*)SDL_CALL(SDL_CreateWindow(_title.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               _width,
                               _height,
                               flags));
    _context = SDL_CALL(SDL_GL_CreateContext(_window));
}

Window::~Window()
{
    PERIA_LOG("Shut Down Window!");
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
}

void Window::window_resize(int w, int h)
{
    PERIA_ASSERT(w>0 && h>0);
    _width = w;
    _height = h;
    GL_CALL(glViewport(0,0,_width,_height));
}

glm::vec2 Window::get_dimensions()
{return {_width,_height};}

SDL_Window* Window::get_sdl_window()
{return _window;}
}
