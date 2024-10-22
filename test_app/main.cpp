#include <glad/glad.h>
#include <SDL2/SDL.h>

#include <glm/vec2.hpp>

#include <cstdlib>

#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include "peria_types.hpp"
#include "peria_color.hpp"
#include "shader.hpp"
#include "graphics.hpp"
#include "vertex_array.hpp"
#include "named_buffer_object.hpp"

namespace sdl {
struct Window_Deleter {
    void operator()(SDL_Window* window) const
    { std::cerr << "Destroying SDL_Window\n"; SDL_DestroyWindow(window); }
};

struct GL_Context_Deleter {
    void operator()(SDL_GLContext context) const
    { std::cerr << "Destroying SDL_GLContext\n"; SDL_GL_DeleteContext(context); }
};

struct Window_Settings {
    std::string title{"app"};
    i32 w{800};
    i32 h{600};
    bool resizable{false};
};
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    { // outer braces for window and glContext and assets to go out of scope first

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL Init failed\n";
            return EXIT_FAILURE;
        }

        sdl::Window_Settings window_settings{};
        u32 window_flags {SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN};
        if (window_settings.resizable) window_flags |= SDL_WINDOW_RESIZABLE;

        auto window = std::unique_ptr<SDL_Window, sdl::Window_Deleter>(
            SDL_CreateWindow(
                window_settings.title.c_str(), 
                SDL_WINDOWPOS_CENTERED, 
                SDL_WINDOWPOS_CENTERED,
                window_settings.w,
                window_settings.h,
                window_flags)
        );
        if (window == nullptr) {
            std::cerr << "SDL Error: " << SDL_GetError() << '\n';
            return EXIT_FAILURE;
        } std::cerr << "SDL_Window created successfully\n";

        auto context = std::unique_ptr<void, sdl::GL_Context_Deleter>(
            SDL_GL_CreateContext(window.get())
        );
        if (context == nullptr) {
            std::cerr << "SDL Error: " << SDL_GetError() << '\n';
            return EXIT_FAILURE;
        } std::cerr << "SDL_GLContext created successfully\n";

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cerr << "GL loader failed\n";
            return EXIT_FAILURE;
        }

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
        
        //u32 vao, vbo, ibo;

        // older state machine with glGens version
        //{
        //    graphics::gen_vao(&vao);
        //    graphics::bind_vao(vao);

        //    graphics::gen_buffer_object(&vbo);
        //    graphics::bind_buffer_object(vbo, GL_ARRAY_BUFFER);
        //    graphics::populate_vbo(quad_data);

        //    graphics::gen_buffer_object(&ibo);
        //    graphics::bind_buffer_object(ibo, GL_ELEMENT_ARRAY_BUFFER);
        //    graphics::populate_ibo(indices);

        //    std::size_t offset{0};

        //    glEnableVertexAttribArray(0);
        //    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (const void*)offset);
        //    offset += sizeof(graphics::Vertex::pos);

        //    glEnableVertexAttribArray(1);
        //    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (const void*)offset);
        //    offset += sizeof(graphics::Vertex::color);

        //    glBindVertexArray(0); // unbind vao
        //}

        //{ // newer DSA version
        //    graphics::create_vao(&vao);

        //    graphics::create_buffer_object(&vbo);
        //    graphics::populate_named_buffer_object(vbo, quad_data);

        //    graphics::create_buffer_object(&ibo);
        //    graphics::populate_named_buffer_object(ibo, indices);

        //    std::size_t offset{0};

        //    glEnableVertexArrayAttrib(vao, 0);
        //    glVertexArrayAttribBinding(vao, 0, 0);
        //    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, offset);
        //    offset += sizeof(graphics::Vertex::pos);

        //    glEnableVertexArrayAttrib(vao, 1);
        //    glVertexArrayAttribBinding(vao, 1, 0);
        //    glVertexArrayAttribFormat(vao, 1, 4, GL_FLOAT, GL_FALSE, offset);
        //    offset += sizeof(graphics::Vertex::color);

        //    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(graphics::Vertex));
        //    glVertexArrayElementBuffer(vao, ibo);
        //}
        //graphics::bind_vao(vao);

        // with encapsulated class
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
                        window_settings.w = ev.window.data1;
                        window_settings.h = ev.window.data2;
                        graphics::set_viewport(0, 0, window_settings.w, window_settings.h);
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

            graphics::swap_buffers(window.get());

            SDL_Delay(1);
        }
    }

    std::cerr << "Shutting down SDL\n";
    SDL_Quit();
    return EXIT_SUCCESS;
}
