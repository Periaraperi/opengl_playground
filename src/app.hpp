#pragma once

#include <memory>
#include <string>
#include <vector>

#include "peria_types.hpp"
#include "demo.hpp"

typedef struct SDL_Window SDL_Window;
typedef void *SDL_GLContext;

namespace sdl {

struct Initializer {
    Initializer() noexcept;

    Initializer(const Initializer&) = delete;
    Initializer& operator=(const Initializer&) = delete;

    Initializer(Initializer&&) noexcept = default;
    Initializer& operator=(Initializer&&) noexcept = default;

    ~Initializer();

    bool initialized{};
};

struct Window_Deleter {
    void operator()(SDL_Window* window) const noexcept;
};

struct GL_Context_Deleter {
    void operator()(SDL_GLContext context) const noexcept;
}; 

}

namespace peria {

struct App_Settings {
    std::string title {"app"};
    i32 window_width  {800};
    i32 window_height {600};
    bool resizable    {false};
};

class App {
public:
    explicit App(App_Settings&& settings_);

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    App(App&&) noexcept = default;
    App& operator=(App&&) noexcept = default;

    ~App();

    [[nodiscard]] bool is_initialized() const noexcept;

    void run();
private:
    bool app_initialized {};
    App_Settings settings {};
    std::string executable_path;

    sdl::Initializer sdl_initializer {};
    std::unique_ptr<SDL_Window, sdl::Window_Deleter> window;
    std::unique_ptr<void, sdl::GL_Context_Deleter> context;
    
    std::vector<std::unique_ptr<demos::Demo>> demoebi;
};

}
