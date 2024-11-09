#include "./peria_engine/application.hpp"
#include "./peria_engine/peria_color.hpp"
#include "./peria_engine/peria_types.hpp"

#include <glad/glad.h>
#include "SDL2/SDL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// engine specific implementation details not available as public interface 
#include "graphics_impl.hpp"
#include "simple_logger.hpp"

namespace peria {

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

struct Application::App_Impl {
    App_Impl(App_Settings&& settings_)
        :settings{std::move(settings_)}
    {
        u32 window_flags {SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN};
        if (settings.resizable) window_flags |= SDL_WINDOW_RESIZABLE;

        window = std::unique_ptr<SDL_Window, sdl::Window_Deleter>(
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

        context = std::unique_ptr<void, sdl::GL_Context_Deleter>(
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

        { // graphics setup
            graphics = std::make_unique<graphics::Graphics>(glm::ortho(
                    0.0f, static_cast<float>(settings.window_width), 
                    0.0f, static_cast<float>(settings.window_height)));
            if (graphics == nullptr) {
                peria::log("Graphics init failed");
                throw std::runtime_error{"Could not initialize Graphics"};
            }
            graphics->set_clear_buffer_bits();
        }
    }

    App_Settings settings;
    sdl::Initializer sdl_initializer{};
    std::unique_ptr<SDL_Window, sdl::Window_Deleter> window;
    std::unique_ptr<void, sdl::GL_Context_Deleter> context;
    std::unique_ptr<graphics::Graphics> graphics;
};

Application::Application()
    :app_impl{std::make_unique<App_Impl>(App_Settings{})}
{}

Application::Application(const char* title, i32 window_width, i32 window_height, bool resizable)
    :app_impl{std::make_unique<App_Impl>(App_Settings{std::string{title}, window_width, window_height, resizable})}
{}

void Application::run()
{
    {
        const auto& renderer = app_impl->graphics;

        bool b{true};
        while (b) {
            for (SDL_Event ev; SDL_PollEvent(&ev);) {
                if (ev.type == SDL_QUIT) {
                    b = false;
                    break;
                }
                else if (ev.type == SDL_WINDOWEVENT) {
                    if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                        app_impl->settings.window_width = ev.window.data1;
                        app_impl->settings.window_height = ev.window.data2;
                        renderer->set_viewport(0, 0, app_impl->settings.window_width, app_impl->settings.window_height);
                    }
                }
            }
            update(); // temp for testing, will add fixed update loop later
            
            renderer->clear_color(graphics::SEAGREEN);
            renderer->clear_buffer();

            render(); // render draw calls from user will go here

            SDL_GL_SwapWindow(app_impl->window.get());

            SDL_Delay(1);
        }
    }
}

// ==========================================================================================================================
// ==================================================== users draw calls ====================================================
// ==========================================================================================================================

// these draw calls are horribly bad/inneficient
// we recreate shaders and buffers on each frame on every draw call
// we do this for simple testing for now.
// TODO: implement render-command type like system and batching

void Application::draw_rect(const graphics::Color<float>& color)
{
    app_impl->graphics->render_quad(200.0f, 200.0f, 100.0f, 50.0f, color);
}

// ==========================================================================================================================

Application::Application(Application&&) noexcept = default;
Application& Application::operator=(Application&&) noexcept = default;
Application::~Application() = default;

}
