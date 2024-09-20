#pragma once

#include <glm/vec4.hpp>

#include <SDL2/SDL.h>
#include <memory>
#include <iostream>

namespace peria::engine {
namespace sdl {
struct initializer {
    explicit initializer(bool use_opengl=false)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            throw std::runtime_error{"SDL_Init failed"};

        if (use_opengl) {
            std::cerr << "Initializing SDL with opengl\n";
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0) 
                throw std::runtime_error{"SDL_GL_SetAttribute failed: " + std::string{SDL_GetError()}};
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) != 0)
                throw std::runtime_error{"SDL_GL_SetAttribute failed: " + std::string{SDL_GetError()}};
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
                throw std::runtime_error{"SDL_GL_SetAttribute failed: " + std::string{SDL_GetError()}};
        }
        else
            std::cerr << "Initializing SDL\n";
    }
    ~initializer()
    {
        SDL_Quit();
        std::cerr << "Quiting SDL\n";
    }
};

// custom deleters
struct window_deleter {
    void operator()(SDL_Window* window) const 
    { std::cerr << "Destroying SDL_Window\n"; SDL_DestroyWindow(window); }
};
struct gl_context_deleter {
    void operator()(SDL_GLContext context) const 
    { std::cerr << "Destroying SDL_GLContext\n"; SDL_GL_DeleteContext(context); }
};
}

struct Window_Settings;
class Input_Manager;
class Graphics;

class Application {
public:
    explicit Application(const Window_Settings& settings);
    virtual ~Application();
    
    void run();
    virtual void update();
    virtual void render();

private:
    sdl::initializer _sdl_initializer{};
    std::unique_ptr<SDL_Window, sdl::window_deleter> _window;
    std::unique_ptr<void, sdl::gl_context_deleter> _context;
    bool _running;
protected:
    std::unique_ptr<Input_Manager> _input_manager;
public:
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;
};
}
