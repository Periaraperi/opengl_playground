#include "app.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "simple_logger.hpp"
#include "input_manager.hpp"
#include "graphics.hpp"
#include "asset_cache.hpp"
#include "timer.hpp"

#ifdef PERIA_DEBUG
    #include "gl_errors.hpp"
#endif

namespace sdl {
Initializer::Initializer() noexcept
{
#ifndef PERIA_DEBUG
    for (int i{}; i<SDL_GetNumVideoDrivers(); ++i) {
        if (std::string{SDL_GetVideoDriver(i)} == "wayland") {
            SDL_SetHintWithPriority(SDL_HINT_VIDEO_DRIVER, "wayland", SDL_HINT_OVERRIDE);
            break;
        }
    }
#endif
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        peria::log("SDL Init failed");
        return;
    } peria::log("SDL initialized successfully");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
#ifdef PERIA_DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
    initialized = true;
}

Initializer::~Initializer()
{ peria::log("Shutting down SDL"); SDL_Quit(); }

void Window_Deleter::operator()(SDL_Window* window) const noexcept
{ peria::log("Destroying SDL_Window"); SDL_DestroyWindow(window); }

void GL_Context_Deleter::operator()(SDL_GLContextState* context) const noexcept
{ peria::log("Destroying SDL_GLContext"); SDL_GL_DestroyContext(context); } 

}

namespace peria {

App::App(App_Settings&& settings_)
    :settings{std::move(settings_)}
{
    peria::log("App ctor()");
    if (!sdl_initializer.initialized) {
        return;
    }

    u32 window_flags {SDL_WINDOW_OPENGL};
    if (settings.resizable) window_flags |= SDL_WINDOW_RESIZABLE;

    window = std::unique_ptr<SDL_Window, sdl::Window_Deleter>(
        SDL_CreateWindow(
            settings.title.c_str(), 
            settings.window_width,
            settings.window_height,
            window_flags)
    );
    if (window == nullptr) {
        peria::log("SDL Error:", SDL_GetError());
        return;
    } peria::log("SDL_Window created successfully");

    context = std::unique_ptr<SDL_GLContextState, sdl::GL_Context_Deleter>(
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
        peria::log("Executable path:", executable_path);
    }

#ifdef PERIA_DEBUG
        int gl_flags {};
        glGetIntegerv(GL_CONTEXT_FLAGS, &gl_flags);
        if (gl_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            std::println("Debug context flag is ON!");
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debug_callback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else {
            std::println("Debug context is not supported!");
        }
        int attachment_count {};
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &attachment_count);
        std::println("COLOR ATTACHMENT COUNT = {}", attachment_count);
#endif

    Input_Manager::initialize();

    Asset_Cache::initialize(executable_path.c_str());

    Timer::initialize();

    // gl/graphics related initial settings
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        peria::set_vsync(true);
        SDL_SetWindowRelativeMouseMode(window.get(), true);
    }

    peria::set_screen_dimensions(settings.window_width, settings.window_height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window.get(), context.get());
    ImGui_ImplOpenGL3_Init("#version 460");

    //demoebi.emplace_back(std::make_unique<demos::Textured_Cube>());
    //demoebi.emplace_back(std::make_unique<demos::Kvadebi>());
    //demoebi.emplace_back(std::make_unique<demos::Fun_With_Textures>());
    //demoebi.emplace_back(std::make_unique<demos::Many_Shadows>());
    //demoebi.emplace_back(std::make_unique<demos::Shadows>());
    //demoebi.emplace_back(std::make_unique<demos::Transformations>());
    //demoebi.emplace_back(std::make_unique<demos::Modelebi>());
    //demoebi.emplace_back(std::make_unique<demos::Lines>());
    //demoebi.emplace_back(std::make_unique<demos::Mouse_Moving_Basic>());
    //demoebi.emplace_back(std::make_unique<demos::Mouse_Picking>());
    //demoebi.emplace_back(std::make_unique<demos::Gizmos>());
    //demoebi.emplace_back(std::make_unique<demos::Color_Correction_And_Stuff>());
    //demoebi.emplace_back(std::make_unique<demos::Normal_Mapping>());
    //demoebi.emplace_back(std::make_unique<demos::Multi_Sampled>());
    //demoebi.emplace_back(std::make_unique<demos::Aspect_Ratio>());
    //demoebi.emplace_back(std::make_unique<demos::Bloom>());
    //demoebi.emplace_back(std::make_unique<demos::Deferred_Rendering>());
    demoebi.emplace_back(std::make_unique<demos::Platonic_Solids>());

    app_initialized = true;
}

App::~App()
{
    peria::log("App dtor()");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    Input_Manager::shutdown();
    
    Asset_Cache::shutdown();

    Timer::shutdown();
}

bool App::is_initialized() const noexcept
{ return app_initialized; }

void App::run()
{
    bool running {true};
    auto input_manager {Input_Manager::instance()};

    i32 demo_id {};

    while (running) {
        Timer::instance()->update();

        input_manager->update_mouse();

        // Poll for events, and react to window resize and mouse movement events here
        for (SDL_Event ev; SDL_PollEvent(&ev);) {
            ImGui_ImplSDL3_ProcessEvent(&ev);
            if (ev.type == SDL_EVENT_QUIT) {
                running = false;
                break;
            }
            else if (ev.type == SDL_EVENT_WINDOW_RESIZED) {
                settings.window_width = ev.window.data1;
                settings.window_height = ev.window.data2;
                peria::set_viewport(0, 0, settings.window_width, settings.window_height);
                peria::set_screen_dimensions(settings.window_width, settings.window_height);
                for (const auto& d:demoebi) {
                    d->recalculate_projection();
                }
            }
            else if (ev.type == SDL_EVENT_MOUSE_MOTION) {
                set_relative_motion(ev.motion.xrel, -ev.motion.yrel);
                if (is_relative_mouse()) demoebi[demo_id]->get_camera().update_camera_front(ev.motion.xrel, -ev.motion.yrel);
            }
        }

        // GENERAL SHORTCUT UPDATES
    
        if (input_manager->key_pressed(SDL_SCANCODE_F1)) {
            if (!is_relative_mouse()) {
                set_relative_mouse(window.get(), true);
            }
            else {
                set_relative_mouse(window.get(), false);
            }
        }
        if (input_manager->key_pressed(SDL_SCANCODE_F2)) {
            demo_id = (demo_id + 1) % demoebi.size();
        }

        while (Timer::instance()->do_fixed_step()) {
        }

        // value for state interpolation, will need to fix jagged graphics for moving objects
        [[maybe_unused]] const auto alpha {Timer::instance()->leftover_accum()};

        //if (rel_mouse) demoebi[demo_id]->get_camera().update();
        demoebi[demo_id]->update();

        input_manager->update_prev_state();
        // ================================= Rendering =================================
        peria::start_imgui_frame();

        demoebi[demo_id]->render();
        demoebi[demo_id]->imgui();

        peria::imgui_render();
        SDL_GL_SwapWindow(window.get());

        SDL_Delay(1); // artifical delay of 1ms to not go bonkers
    }

}

}
