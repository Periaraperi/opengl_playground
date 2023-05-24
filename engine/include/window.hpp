#pragma once

#include <glm/vec2.hpp>
#include "window_settings.hpp"

// forward declerations
typedef struct SDL_Window SDL_Window;
typedef void *SDL_GLContext;

namespace Peria_Engine {

class Window {
public:
    Window(const Window_Settings& props);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
    ~Window();
    
    SDL_Window* get_sdl_window();
    void window_resize(int w, int h);
    glm::vec2 get_dimensions();
private:
    std::string _title;
    int _width;
    int _height;
    SDL_Window* _window;
    SDL_GLContext _context;
};
}
