#include "app.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "simple_logger.hpp"
#include "input_manager.hpp"
#include "graphics.hpp"
//#include "asset_manager.hpp"

namespace sdl {
Initializer::Initializer() noexcept
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        peria::log("SDL Init failed");
        return;
    } peria::log("SDL initialized successfully");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    initialized = true;
}

Initializer::~Initializer()
{ peria::log("Shutting down SDL"); SDL_Quit(); }

void Window_Deleter::operator()(SDL_Window* window) const noexcept
{ peria::log("Destroying SDL_Window"); SDL_DestroyWindow(window); }

void GL_Context_Deleter::operator()(SDL_GLContext context) const noexcept
{ peria::log("Destroying SDL_GLContext"); SDL_GL_DeleteContext(context); } 

}

namespace peria {

App::App(App_Settings&& settings_)
    :settings{std::move(settings_)}
{
    peria::log("App ctor()");
    if (!sdl_initializer.initialized) {
        return;
    }

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
        return;
    } peria::log("SDL_Window created successfully");

    context = std::unique_ptr<void, sdl::GL_Context_Deleter>(
        SDL_GL_CreateContext(window.get())
    );
    if (context == nullptr) {
        peria::log("SDL Error:", SDL_GetError()); 
        return;
    } peria::log("SDL_GLContext created successfully");

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        peria::log("GL loader failed");
        return;
    }

    {
        auto base_path {SDL_GetBasePath()};
        if (base_path == nullptr) {
            peria::log("SDL Error:", SDL_GetError()); 
            return;
        }
        executable_path = base_path;
        SDL_free(base_path);
        peria::log("Executable path:", executable_path);
    }

    Input_Manager::initialize();

    //Asset_Manager::initialize(executable_path.c_str());

    // gl/graphics related initial settings
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        peria::set_vsync(false);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    peria::set_screen_dimensions(settings.window_width, settings.window_height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
    ImGui_ImplOpenGL3_Init("#version 460");

    demoebi.emplace_back(std::make_unique<demos::Textured_Cube>());

    app_initialized = true;
}

App::~App()
{
    peria::log("App dtor()");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    Input_Manager::shutdown();
    
    //Asset_Manager::shutdown();
}

bool App::is_initialized() const noexcept
{ return app_initialized; }

void App::run()
{
    bool running {true};
    auto input_manager {Input_Manager::instance()};

    auto rel_mouse {true};
    i32 demo_id {};

    while (running) {
        input_manager->update_mouse();

        // Poll for events, and react to window resize and mouse movement events here
        for (SDL_Event ev; SDL_PollEvent(&ev);) {
            ImGui_ImplSDL2_ProcessEvent(&ev);
            if (ev.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (ev.type == SDL_WINDOWEVENT) {
                if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                    settings.window_width = ev.window.data1;
                    settings.window_height = ev.window.data2;
                    peria::set_viewport(0, 0, settings.window_width, settings.window_height);
                    peria::set_screen_dimensions(settings.window_width, settings.window_height);
                    for (const auto& d:demoebi) {
                        d->recalculate_projection();
                    }
                }
            }
            else if (ev.type == SDL_MOUSEMOTION) {
                demoebi[demo_id]->get_camera().update_camera_front(ev.motion.xrel, -ev.motion.yrel);
            }
        }

        if (input_manager->key_pressed(SDL_SCANCODE_F1)) {
            rel_mouse = !rel_mouse;
            if (rel_mouse) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
            else {
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
        }

        if (input_manager->key_pressed(SDL_SCANCODE_F2)) {
            demo_id = (demo_id + 1) % demoebi.size();
        }

        demoebi[demo_id]->update();
        
        input_manager->update_prev_state();

        // ================================= Rendering =================================
        peria::start_imgui_frame();

        demoebi[demo_id]->render();
        demoebi[demo_id]->imgui();

        peria::imgui_render();

        SDL_GL_SwapWindow(window.get());

        SDL_Delay(1);
    }

}

}
