#include "application.hpp"

#include <glad/glad.h>
#include "SDL2/SDL.h"

#include <exception>
#include <vector>
#include <array>

#include "simple_logger.hpp"
#include "peria_types.hpp"
#include "peria_color.hpp"
#include "shader.hpp"
#include "graphics.hpp"
#include "vertex_array.hpp"
#include "named_buffer_object.hpp"

namespace peria {

namespace sdl {

struct Initializer {
    Initializer()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            peria::log("SDL Init failed");
            throw std::runtime_error{"Failed to init SDL"};
        }
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
    }

    App_Settings settings;
    sdl::Initializer sdl_initializer{};
    std::unique_ptr<SDL_Window, sdl::Window_Deleter> window;
    std::unique_ptr<void, sdl::GL_Context_Deleter> context;
};

Application::Application()
    :app_impl{std::make_unique<App_Impl>(App_Settings{})}
{}

Application::Application(const char* title, i32 window_width, i32 window_height, bool resizable)
    :app_impl{std::make_unique<App_Impl>(App_Settings{std::string{title}, window_width, window_height, resizable})}
{}

void Application::run()
{
    { // outer braces for window and glContext and assets to go out of scope first

        graphics::Shader shader{"./assets/simple.vert", "./assets/simple.frag"};

        std::vector<graphics::Vertex> quad_data {
            {{-0.5f, -0.5f}, graphics::GREEN},
            {{-0.5f,  0.5f}, graphics::GREEN},
            {{ 0.5f,  0.5f}, graphics::GREEN},
            {{ 0.5f, -0.5f}, graphics::GREEN}
        };

        std::vector<graphics::Vertex> quad_data2 {
            {{-0.25f, -0.25f}, graphics::LIME},
            {{-0.25f,  0.25f}, graphics::LIME},
            {{ 0.25f,  0.25f}, graphics::LIME},
            {{ 0.25f, -0.25f}, graphics::LIME}
        };

        std::vector<u32> indices {0,1,2, 0,2,3};

        std::array<graphics::Vertex_Array, 2> vaos;

        graphics::Named_Buffer_Object vbo{quad_data};
        graphics::Named_Buffer_Object ibo{indices};

        vaos[0].setup_attribute(graphics::Attribute<float>{2, false});
        vaos[0].setup_attribute(graphics::Attribute<float>{4, false});

        vaos[0].connect_vertex_buffer(vbo.buffer_id(), sizeof(graphics::Vertex));
        vaos[0].connect_index_buffer(ibo.buffer_id());

        graphics::Named_Buffer_Object vbo2{quad_data2};
        vaos[1].setup_attribute(graphics::Attribute<float>{2, false});
        vaos[1].setup_attribute(graphics::Attribute<float>{4, false});

        vaos[1].connect_vertex_buffer(vbo2.buffer_id(), sizeof(graphics::Vertex));
        vaos[1].connect_index_buffer(ibo.buffer_id());

        bool running{true};
        i32 i{0};
        while (running) {
            for (SDL_Event ev; SDL_PollEvent(&ev);) {
                if (ev.type == SDL_QUIT) {
                    running = false;
                    break;
                }
                else if (ev.type == SDL_WINDOWEVENT) {
                    if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                        app_impl->settings.window_width = ev.window.data1;
                        app_impl->settings.window_height = ev.window.data2;
                        graphics::set_viewport(0, 0, app_impl->settings.window_width, app_impl->settings.window_height);
                    }
                }
                else if (ev.type == SDL_KEYUP)
                {
                    auto k = ev.key.keysym.scancode;
                    if (k == SDL_SCANCODE_SPACE) {
                        i = (i+1) % vaos.size();
                    }
                }
            }
            
            graphics::clear_color(graphics::TEAL);
            graphics::clear_buffer();

            vaos[i].bind();
            shader.use_shader();
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

            graphics::swap_buffers(app_impl->window.get());

            SDL_Delay(1);
        }
    }
}

Application::Application(Application&&) noexcept = default;
Application& Application::operator=(Application&&) noexcept = default;
Application::~Application() = default;

}
