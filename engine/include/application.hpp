#pragma once

#include "initializer.hpp"
#include "window_settings.hpp"
#include <memory>

namespace peria::engine {

class input_manager;
class graphics;

class application {
public:
    explicit application(window_settings&& settings_);
    virtual ~application();
    
    void run();
    virtual void update();
    virtual void render();

private:
    peria::engine::sdl::initializer sdl_initializer{};
    std::unique_ptr<SDL_Window, peria::engine::sdl::window_deleter> window;
    std::unique_ptr<void, peria::engine::sdl::gl_context_deleter> context;

    window_settings settings;

    bool running;
protected:
    std::unique_ptr<input_manager> input;
    std::unique_ptr<peria::engine::graphics> graphics;
public:
    application(const application&) = delete;
    application& operator=(const application&) = delete;
    application(application&&) = default;
    application& operator=(application&&) = default;
};
}
