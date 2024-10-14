#pragma once

struct SDL_Window;
typedef void* SDL_GLContext;

namespace peria::engine::sdl {
struct initializer {
    initializer();
    ~initializer();

    initializer(const initializer&) = delete;
    initializer& operator=(const initializer&) = delete;

    initializer(initializer&&) = default;
    initializer& operator=(initializer&&) = default;
};

// custom deleters for SDL entities
struct window_deleter {
    void operator()(SDL_Window* window) const; 
};
struct gl_context_deleter {
    void operator()(SDL_GLContext context) const;
};

}
