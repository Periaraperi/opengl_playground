#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <memory>
#include <array>
#include <iostream>

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
    { peria::log("Destroying SDL_GLContext"); SDL_GL_DeleteContext(context); } }; }
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
        settings.resizable = true;

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
            peria::log("SDL Error:", SDL_GetError()); throw std::runtime_error{"Failed to create SDL_GLContext"}; } peria::log("SDL_GLContext created successfully"); if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            peria::log("GL loader failed");
            throw std::runtime_error{"Could not initialize GLAD"};
        }

        auto graphics = std::make_unique<peria::graphics::Graphics>();

        if (graphics == nullptr) {
            peria::log("Graphics init failed");
            throw std::runtime_error{"Could not initialize Graphics"};
        }
        graphics->set_clear_buffer_bits(true, true);

        graphics->peria_perspective(
                    45.0f, 
                    static_cast<float>(settings.window_width) / static_cast<float>(settings.window_height), 
                    0.1f, 100.0f);

        graphics->peria_ortho(0.0f, settings.window_width, 0.0f, settings.window_height);

        std::array<peria::graphics::Color<float>, 4> colors {
            peria::graphics::TEAL,
            peria::graphics::SALMON,
            peria::graphics::PLUM,
            peria::graphics::BURLYWOOD,
        };

        // IMGUI setup
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.Fonts->AddFontDefault();
        auto main_font {io.Fonts->AddFontFromFileTTF("./assets/iosevka-regular.ttf", 18)};

        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
        ImGui_ImplOpenGL3_Init("#version 460");

        glm::vec3 dir {0.0f, 0.0f, -1.0f};
        glm::vec3 up {0.0f, 1.0f, 0.0f};

        auto pitch {0.0f};
        auto yaw {-90.0f};

        float x{}, y{}, z{};
        SDL_SetRelativeMouseMode(SDL_TRUE);
        bool should_update_camera {true};

        // main loop here
        bool running {true};
        while (running) {
            x = 0.0f, y = 0.0f, z = 0.0f;
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
                        graphics->set_viewport(0, 0, settings.window_width, settings.window_height);
                        graphics->peria_ortho(0.0f, settings.window_width, 0.0f, settings.window_height);
                        graphics->peria_perspective(
                                    45.0f, 
                                    static_cast<float>(settings.window_width) / static_cast<float>(settings.window_height), 
                                    0.1f, 100.0f);
                    }
                }
                else if (ev.type == SDL_KEYDOWN) {
                    if (ev.key.keysym.scancode == SDL_SCANCODE_O) {
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                        should_update_camera = true;
                    }
                    if (ev.key.keysym.scancode == SDL_SCANCODE_P) {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        should_update_camera = false;
                    }

                    if (should_update_camera) {
                        float speed {0.05f};
                        if (ev.key.keysym.scancode == SDL_SCANCODE_W) {
                            graphics->get_camera().update_pos(dir*speed);
                        }
                        if (ev.key.keysym.scancode == SDL_SCANCODE_S) {
                            graphics->get_camera().update_pos(-dir*speed);
                        }
                        if (ev.key.keysym.scancode == SDL_SCANCODE_D) {
                            graphics->get_camera().update_pos(glm::normalize(glm::cross(dir, up))*speed);
                        }
                        if (ev.key.keysym.scancode == SDL_SCANCODE_A) {
                            graphics->get_camera().update_pos(-glm::normalize(glm::cross(dir, up))*speed);
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

            if (should_update_camera) {
                graphics->get_camera().update(dir);
            }

            graphics->start_imgui_frame(main_font);
            //graphics->imgui_transforms();
            //graphics->imgui_matrix_info();
            graphics->imgui_lighting();
            
            graphics->clear_color(peria::graphics::SEAGREEN);
            graphics->clear_buffer();

            auto start_x {500.0f};
            auto start_y {220.0f};
            auto width  {16.0f};
            auto height {16.0f};

            if (0) {
                graphics->draw_colored_quad({100.0f, 200.0f, 300.0f, 200.0f}, peria::graphics::KHAKI);
                for (i32 i{}; i<2; ++i) {
                    for (i32 j{}; j<3; ++j) {
                        graphics->draw_textured_quad({start_x + j*width*6, start_y + i*height*6, width*6, height*6}, {j*width, i*height, width, height});
                    }
                }
            }

            if (0) {
                start_x = 0.0f;
                start_y = 0.0f;
                width   = 16.0f;
                height  = 16.0f;
                for (i32 i{}; i<200; ++i) {
                    for (i32 j{}; j<300; ++j) {
                        graphics->draw_colored_quad({start_x + j*width, start_y + i*height, width, height}, colors[(i+j)%colors.size()]);
                    }
                }
            }

            //graphics->render2d();
            //graphics->render3d();
            graphics->render3d_lighting();
            graphics->imgui_render();
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
