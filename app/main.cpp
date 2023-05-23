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

constexpr int MAXN = 500;
std::vector<Quad_Vertex> dynamic_buffer_data(4*MAXN);


// start batching quads
//
// while quad count < MAXN
//
// generate vertex info and indices
//
// update dynamic buffer
//
// flush/render

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
    
    glm::vec4 color = {1.0f,0.3f,0.5f,1.0f};
    std::vector<Quad_Vertex> data = {
        {{{-0.5f,-0.5f},color,{0.0f,0.0f}},
         {{-0.5f, 0.5f},color,{0.0f,1.0f}},
         {{ 0.5f, 0.5f},color,{1.0f,1.0f}},
         {{ 0.5f,-0.5f},color,{1.0f,0.0f}}}
    };
    std::vector<uint32_t> indices = {0,1,2, 0,2,3};
    Mesh quad(data,indices);

    //glm::vec4 color_circle = {0.0f,1.0f,0.2f,1.0f};
    glm::vec4 color_circle = {0.0f,0.0f,0.0f,1.0f};
    float rr = 200.0f;
    std::vector<Circle_Vertex> data_circle {
        {{{-0.5f,-0.5f},{300.0f,300.0f},rr,color_circle},
         {{-0.5f, 0.5f},{300.0f,300.0f},rr,color_circle},
         {{ 0.5f, 0.5f},{300.0f,300.0f},rr,color_circle},
         {{ 0.5f,-0.5f},{300.0f,300.0f},rr,color_circle}}
    };
    Mesh circle_mesh(data_circle,indices);
    glm::mat4 circle_model = get_model(300.0f,300.0f,2.0f*rr);

    // textures
    Texture white_tex;
    Texture pikapika("assets/pikapika.png");
    Texture shaco("assets/shaco1.jpg");
    Texture xd_tex("assets/xD.png");
    Texture ururu("assets/ukvirs.png");
    Texture chitunia("assets/chitunia.png");

    Shader sh("assets/default.vert","assets/default.frag");
    sh.enable();
    sh.set_int("u_texture",0);
    white_tex.bind();

    Shader circle_shader("assets/circle.vert","assets/circle.frag");

    glm::mat4 proj = glm::ortho(0.0f,(float)window_w,0.0f,(float)window_h,-1.0f,1.0f);

    // cursor
    glm::vec2 cursor;
    constexpr float cursor_scale = 15.0f;
    //GL_CALL(glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ));

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
        if (input_mg.key_pressed(SDL_SCANCODE_R))
            std::cout << mouse.x << " " << mouse.y << '\n';
        cursor = mouse;
        glm::mat4 cursor_model = get_model(cursor.x,cursor.y,cursor_scale);

        input_mg.update_prev_input_state();
        
        // ========================================== RENDERING =============================================
        GL_CALL(glClearColor(0.8f,0.9f,0.9f,1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        quad.bind();
        sh.enable();

        sh.set_mat4("u_mvp",proj*cursor_model);
        GL_CALL(glDrawElements(GL_TRIANGLES,quad.get_index_count(),GL_UNSIGNED_INT,0));

        quad.unbind();

        circle_mesh.bind();
        circle_shader.enable();
        
        sh.set_mat4("u_mvp",proj*circle_model);
        GL_CALL(glDrawElements(GL_TRIANGLES,circle_mesh.get_index_count(),GL_UNSIGNED_INT,0));

        circle_mesh.unbind();

        SDL_GL_SwapWindow(window);
    }

    Input_Manager::shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

