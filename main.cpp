#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <memory>

#include "peria_types.hpp"
#include "simple_logger.hpp"
#include "graphics.hpp"
#include "input_manager.hpp"
#include "demo.hpp"

namespace sdl {

struct Initializer {
    Initializer()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            peria::log("SDL Init failed");
            return;
        } peria::log("SDL initialized successfully");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        initialized = true;
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

    bool initialized{};
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
    sdl::Initializer sdl_init{};
    if (!sdl_init.initialized) {
        return EXIT_FAILURE;
    }

    App_Settings settings{};
    settings.resizable = true;

    u32 window_flags {SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN};
    if (settings.resizable) window_flags |= SDL_WINDOW_RESIZABLE;

    auto window {std::unique_ptr<SDL_Window, sdl::Window_Deleter>(
        SDL_CreateWindow(
            settings.title.c_str(), 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED,
            settings.window_width,
            settings.window_height,
            window_flags)
    )};
    if (window == nullptr) {
        peria::log("SDL Error:", SDL_GetError());
        return EXIT_FAILURE;
    } peria::log("SDL_Window created successfully");

    auto context {std::unique_ptr<void, sdl::GL_Context_Deleter>(
        SDL_GL_CreateContext(window.get())
    )};

    if (context == nullptr) {
        peria::log("SDL Error:", SDL_GetError()); 
        return EXIT_FAILURE;
    } peria::log("SDL_GLContext created successfully");

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        peria::log("GL loader failed");
        return EXIT_FAILURE;
    }

    auto input_manager {std::make_unique<Input_Manager>()};

    // IMGUI setup
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.Fonts->AddFontDefault();

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
    ImGui_ImplOpenGL3_Init("#version 460");

    SDL_SetRelativeMouseMode(SDL_TRUE);
    bool should_update_camera {true};

    std::vector<std::unique_ptr<peria::graphics::demos::Demo3d>> demos;
    demos.emplace_back(std::make_unique<peria::graphics::demos::Demo_Combined_Lights>());
    {
        auto projection = glm::perspective(
                45.0f,
                static_cast<float>(settings.window_width) / static_cast<float>(settings.window_height),
                0.1f, 100.0f);
        for (auto& d:demos) {
            d->projection = projection;
        }
    }

    auto current_demo_3d {demos[0].get()};

    std::vector<std::unique_ptr<peria::graphics::demos::Demo2d>> demos_2d;
    demos_2d.emplace_back(std::make_unique<peria::graphics::demos::Demo_Quads>());
    {
        auto projection = glm::ortho(0.0f, static_cast<float>(settings.window_width), 0.0f, static_cast<float>(settings.window_height));
        for (auto& d:demos_2d) {
            d->projection = projection;
        }
    }
    auto current_demo_2d {demos_2d[0].get()};

    auto do_2d {false};
    auto do_3d {true};

    // main loop here
    bool running {true};
    while (running) {
        input_manager->update_mouse();

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
                    peria::graphics::set_viewport(0, 0, settings.window_width, settings.window_height);
                    for (auto& d:demos) {
                        d->projection = glm::perspective(
                                45.0f,
                                static_cast<float>(settings.window_width) / static_cast<float>(settings.window_height),
                                0.1f, 100.0f);
                    }
                    for (auto& d:demos_2d) {
                        d->projection = glm::ortho(0.0f, static_cast<float>(settings.window_width), 0.0f, static_cast<float>(settings.window_height));
                    }
                }
            }
            else if (ev.type == SDL_MOUSEMOTION) {
                if (should_update_camera && do_3d) {
                    current_demo_3d->camera.update_camera_front(
                            static_cast<float>(ev.motion.xrel),
                            static_cast<float>(-ev.motion.yrel));
                }
            }
        }

        if (input_manager->key_pressed(SDL_SCANCODE_2)) {
            do_2d = true;
            do_3d = false;
        }
        if (input_manager->key_pressed(SDL_SCANCODE_3)) {
            do_2d = false;
            do_3d = true;
        }

        if (input_manager->key_pressed(SDL_SCANCODE_O) && do_3d) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            should_update_camera = true;
        }
        if (input_manager->key_pressed(SDL_SCANCODE_P) && do_3d) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            should_update_camera = false;
        }

        if (should_update_camera && do_3d) {
            current_demo_3d->camera.update(input_manager.get());
        }

        input_manager->update_prev_state();

        if (do_2d) current_demo_2d->update();
        if (do_3d) current_demo_3d->update();
        
        // ================================= Rendering =================================
        peria::graphics::start_imgui_frame();
        if (do_3d) current_demo_3d->imgui();
        
        peria::graphics::clear_color();
        peria::graphics::clear_buffer();

        if (do_2d) current_demo_2d->render();
        if (do_3d) current_demo_3d->render();

        if (do_2d) current_demo_2d->render_quads();
        peria::graphics::imgui_render();
        SDL_GL_SwapWindow(window.get());

        SDL_Delay(1);
    }

    return EXIT_SUCCESS;
}
