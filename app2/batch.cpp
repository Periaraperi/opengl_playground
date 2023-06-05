#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stb_image.h>

#include <iostream>
#include <array>
#include <vector>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "logger.hpp"
#include "input_manager.hpp"

#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"

// globals
int window_w = 800;
int window_h = 600;
// globals end

// calculates model matrix based on params
glm::mat4 get_model(float x, float y, float scale)
{
    return glm::translate(glm::mat4(1.0f),{x,y,0.0f})*
           glm::scale(glm::mat4(1.0f),{scale,scale,1.0f});
}

glm::vec2 convert_to_gl(const glm::vec2& p)
{return {p.x,window_h-p.y};}

float convert_to_gl(float p)
{return window_h-p;}

struct Quad {
    float x,y,d;
    glm::vec4 c;
    float tex_id; // 0 is white tex
};
std::vector<Quad> quads_to_render;
void draw_quad(float x, float y, float d, const glm::vec4& color) 
{
    quads_to_render.push_back({x,y,d,color,0});
}
void draw_quad(float x, float y, float d, int tex_id)
{
    quads_to_render.push_back({x,y,d,glm::vec4(1.0f),(float)tex_id});
}

std::array<Quad_Vertex,4> get_quad_vertices(const Quad& q) 
{
    std::array<Quad_Vertex,4> vv = {
        {{{q.x-q.d,q.y-q.d},q.c,{0.0f,0.0f},q.tex_id},
         {{q.x-q.d,q.y+q.d},q.c,{0.0f,1.0f},q.tex_id},
         {{q.x+q.d,q.y+q.d},q.c,{1.0f,1.0f},q.tex_id},
         {{q.x+q.d,q.y-q.d},q.c,{1.0f,0.0f},q.tex_id}}
    };
    return vv;
}

int main()
{
    srand(time(0));

    // SDL WINDOW and GL context INITIALIZATON
    SDL_CALL(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window* window = (SDL_Window*)SDL_CALL(SDL_CreateWindow("app1",
                                                                SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                window_w,window_h,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));

    SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4));
    SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,2));
    SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE));

    SDL_GLContext context = SDL_CALL(SDL_GL_CreateContext(window));
    
    PERIA_ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)); 
    GL_CALL(glViewport(0,0,window_w,window_h));

    // enable blending
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

    // SDL WINDOW and GL context INITIALIZATON ============================= end

    auto& input_mg = Input_Manager::get();

    // textures
    Texture white_tex;
    Texture xd_tex("assets/xD.png");
    Texture pikapika("assets/pikapika.png");

    // quad shader
    Shader sh("assets/default.vert","assets/default.frag");
    sh.enable();
    std::array<int,3> texture_slots;
    for (int i=0; i<3; ++i) // max of 3 textures at the same time
        texture_slots[i] = i;
    sh.set_array("u_textures",texture_slots.size(),&texture_slots[0]);
    white_tex.bind();
    xd_tex.bind(1);
    pikapika.bind(2);

    glm::mat4 proj = glm::ortho(0.0f,(float)window_w,0.0f,(float)window_h,-1.0f,1.0f);

    // Mesh for batching
    constexpr int MAX_QUADS = 10;
    Mesh dynamic_mesh(MAX_QUADS);
    int count_draw_calls = 0;


    bool running = 1;
    while (running) {
        input_mg.update_mouse();
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type==SDL_QUIT) {
                running = false;
                break;
            }
        }

        // =========================== LOGIC and UPDATES =============================

        auto mouse = convert_to_gl(input_mg.get_mouse_pos()); // mouse position in gl coord system
        
        if (input_mg.mouse_pressed(Mouse_Button::LEFT)) {
            glm::vec4 color = {0.434f,0.233f,0.2552f,1.0f};
            draw_quad(mouse.x,mouse.y,20.0f,color);
        }

        if (input_mg.mouse_pressed(Mouse_Button::RIGHT)) {
            draw_quad(mouse.x,mouse.y,20.0f,1);
        }
        if (input_mg.mouse_pressed(Mouse_Button::MID)) {
            draw_quad(mouse.x,mouse.y,20.0f,2);
        }

        input_mg.update_prev_input_state();
        
        // ========================================== RENDERING =============================================
        GL_CALL(glClearColor(0.8f,0.9f,0.9f,1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        
        // start quad batching here
        dynamic_mesh.bind();
        sh.enable();
        sh.set_mat4("u_mvp",proj);

        std::vector<Quad_Vertex> data;
        int cnt = 0;
        for (int i=0; i<(int)quads_to_render.size(); ++i) {
            if (cnt<MAX_QUADS) {
                std::array<Quad_Vertex,4> current_quad = get_quad_vertices(quads_to_render[i]);
                for (int j=0; j<4; ++j) {
                    data.push_back(current_quad[j]);
                }
                ++cnt;
            }
            if (cnt==MAX_QUADS) { // render what we have, i.e flush
                dynamic_mesh.update_buffer(data,0);
                
                GL_CALL(glDrawElements(GL_TRIANGLES,cnt*6,GL_UNSIGNED_INT,0));
                ++count_draw_calls;
                data.clear();
                cnt = 0;
            }
        }
        if (!data.empty()) {
            dynamic_mesh.update_buffer(data,0);
            GL_CALL(glDrawElements(GL_TRIANGLES,cnt*6,GL_UNSIGNED_INT,0));
            ++count_draw_calls;
        }
        std::cout << count_draw_calls << '\n';
        count_draw_calls = 0;

        SDL_GL_SwapWindow(window);
    }

    Input_Manager::shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


