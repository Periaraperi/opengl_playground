#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include "peria_color.hpp"
#include "peria_types.hpp"
#include "simple_logger.hpp"
#include "graphics.hpp"

namespace sdl {

struct Initializer {
    Initializer()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            peria::log("SDL Init failed");
            throw std::runtime_error{"Failed to init SDL"};
        } peria::log("SDL initialized successfully");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }

    Initializer(const Initializer&) = delete;
    Initializer& operator=(const Initializer&) = delete;

    Initializer(Initializer&&) noexcept = default;
    Initializer& operator=(Initializer&&) noexcept = default;

    ~Initializer()
    {
        peria::log("Shutting down SDL");
        SDL_Quit();
    }
};

struct Window_Deleter {
    void operator()(SDL_Window* window) const
    { peria::log("Destroying SDL_Window"); SDL_DestroyWindow(window); }
};

struct GL_Context_Deleter {
    void operator()(SDL_GLContext context) const
    { peria::log("Destroying SDL_GLContext"); SDL_GL_DeleteContext(context); }
};
}

struct App_Settings {
    std::string title {"app"};
    i32 window_width  {800};
    i32 window_height {600};
    bool resizable    {false};
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try {
        sdl::Initializer sdl_init{};
        App_Settings settings{};

        u32 window_flags {SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN};
        if (settings.resizable) window_flags |= SDL_WINDOW_RESIZABLE;

        auto window = std::unique_ptr<SDL_Window, sdl::Window_Deleter>(
            SDL_CreateWindow(
                settings.title.c_str(), 
                SDL_WINDOWPOS_CENTERED, 
                SDL_WINDOWPOS_CENTERED,
                settings.window_width,
                settings.window_height,
                window_flags)
        );
        if (window == nullptr) {
            peria::log("SDL Error:", SDL_GetError());
            throw std::runtime_error{"Failed to create SDL_Window"};
        } peria::log("SDL_Window created successfully");

        auto context = std::unique_ptr<void, sdl::GL_Context_Deleter>(
            SDL_GL_CreateContext(window.get())
        );
        if (context == nullptr) {
            peria::log("SDL Error:", SDL_GetError());
            throw std::runtime_error{"Failed to create SDL_GLContext"};
        } peria::log("SDL_GLContext created successfully");

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            peria::log("GL loader failed");
            throw std::runtime_error{"Could not initialize GLAD"};
        }

        auto graphics = std::make_unique<peria::graphics::Graphics>(glm::ortho(
                0.0f, static_cast<float>(settings.window_width), 
                0.0f, static_cast<float>(settings.window_height)));
        if (graphics == nullptr) {
            peria::log("Graphics init failed");
            throw std::runtime_error{"Could not initialize Graphics"};
        }
        graphics->set_clear_buffer_bits();

        // main loop here

        bool running{true};
        while (running) {
            for (SDL_Event ev; SDL_PollEvent(&ev);) {
                if (ev.type == SDL_QUIT) {
                    running = false;
                    break;
                }
                else if (ev.type == SDL_WINDOWEVENT) {
                    if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                        settings.window_width = ev.window.data1;
                        settings.window_height = ev.window.data2;
                        graphics->set_viewport(0, 0, settings.window_width, settings.window_height);
                    }
                }
            }
            
            graphics->clear_color(peria::graphics::SEAGREEN);
            graphics->clear_buffer();

            SDL_GL_SwapWindow(window.get());

            SDL_Delay(1);
        }
    }
    catch (const std::runtime_error& e) {
        peria::log(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
