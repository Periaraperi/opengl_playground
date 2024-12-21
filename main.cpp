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

    glm::vec3 dir {0.0f, 0.0f, -1.0f};
    glm::vec3 up {0.0f, 1.0f, 0.0f};

    auto pitch {0.0f};
    auto yaw {-90.0f};

    SDL_SetRelativeMouseMode(SDL_TRUE);
    bool should_update_camera {true};

    std::vector<std::unique_ptr<peria::graphics::demos::Demo>> demos;
    demos.emplace_back(std::make_unique<peria::graphics::demos::Demo_Point_Light>());
    demos.emplace_back(std::make_unique<peria::graphics::demos::Demo_Combined_Lights>());
    {
        auto projection = glm::perspective(
                45.0f,
                static_cast<float>(settings.window_width) / static_cast<float>(settings.window_height),
                0.1f, 100.0f);
        demos[0]->projection = projection;
        demos[1]->projection = projection;
    }

    auto current_demo {demos[1].get()};

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
                }
            }
            else if (ev.type == SDL_MOUSEMOTION) {
                if (should_update_camera) {
                    const auto mouse_delta_x = static_cast<float>(ev.motion.xrel);
                    const auto mouse_delta_y = static_cast<float>(-ev.motion.yrel);

                    const auto sensitivity {0.05f};
                    yaw += mouse_delta_x * sensitivity;
                    pitch += mouse_delta_y * sensitivity;

                    if (pitch > 89.0f)
                        pitch = 89.0f;
                    if (pitch < -89.0f)
                        pitch = -89.0f;

                    glm::vec3 front;
                    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
                    front.y = std::sin(glm::radians(pitch));
                    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
                    
                    dir = glm::normalize(front);
                }
            }
        }


        if (input_manager->key_pressed(SDL_SCANCODE_O)) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            should_update_camera = true;
        }
        if (input_manager->key_pressed(SDL_SCANCODE_P)) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            should_update_camera = false;
        }

        if (should_update_camera) {
            float speed {0.05f};
            if (input_manager->key_down(SDL_SCANCODE_LSHIFT)) {
                speed *= 2.0f;
            }
            if (input_manager->key_down(SDL_SCANCODE_W)) {
                current_demo->camera.update_pos(dir*speed);
            }
            if (input_manager->key_down(SDL_SCANCODE_S)) {
                current_demo->camera.update_pos(-dir*speed);
            }
            if (input_manager->key_down(SDL_SCANCODE_D)) {
                current_demo->camera.update_pos(glm::normalize(glm::cross(dir, up))*speed);
            }
            if (input_manager->key_down(SDL_SCANCODE_A)) {
                current_demo->camera.update_pos(-glm::normalize(glm::cross(dir, up))*speed);
            }
        }

        input_manager->update_prev_state();
        if (should_update_camera) {
            current_demo->camera.update(dir);
        }

        current_demo->update();
        
        // ================================= Rendering =================================
        peria::graphics::start_imgui_frame();
        current_demo->imgui();
        
        peria::graphics::clear_color();
        peria::graphics::clear_buffer();

        current_demo->render();

        peria::graphics::imgui_render();
        SDL_GL_SwapWindow(window.get());

        SDL_Delay(1);
    }

    return EXIT_SUCCESS;
}
