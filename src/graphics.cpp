#include "graphics.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp> 
#include <glm/gtx/string_cast.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <array>
#include <random>
#include <string>

#include "simple_logger.hpp"
#include "materials.hpp"

namespace peria::graphics {

namespace buffer_data {
    /* for 2D I used this convention. for 3D I use the same. I.E. clockwise order - back-facing triangles for OpenGL convention
        1_______2
        |       |
        |   c   |
        |_______|
        0       3
    */

    // I assume that cube has center on (0,0)
    // and we are looking at negative Z-axis at Side 1
    std::vector<Vertex3d> cube_model {
        // SIDE 1 (near)
        {{-0.5f, -0.5f, 0.5f}, colors::SILVER},
        {{-0.5f,  0.5f, 0.5f}, colors::SILVER},
        {{ 0.5f,  0.5f, 0.5f}, colors::SILVER},

        {{-0.5f, -0.5f, 0.5f}, colors::SILVER},
        {{ 0.5f,  0.5f, 0.5f}, colors::SILVER},
        {{ 0.5f, -0.5f, 0.5f}, colors::SILVER},
        
        // SIDE 2 (far)
        {{-0.5f, -0.5f, -0.5f}, colors::OLIVEDRAB},
        {{-0.5f,  0.5f, -0.5f}, colors::OLIVEDRAB},
        {{ 0.5f,  0.5f, -0.5f}, colors::OLIVEDRAB},

        {{-0.5f, -0.5f, -0.5f}, colors::OLIVEDRAB},
        {{ 0.5f,  0.5f, -0.5f}, colors::OLIVEDRAB},
        {{ 0.5f, -0.5f, -0.5f}, colors::OLIVEDRAB},

        // SIDE 3 (LEFT)
        {{-0.5f, -0.5f, -0.5f}, colors::RED},
        {{-0.5f,  0.5f, -0.5f}, colors::RED},
        {{-0.5f,  0.5f,  0.5f}, colors::RED},

        {{-0.5f, -0.5f, -0.5f}, colors::RED},
        {{-0.5f,  0.5f,  0.5f}, colors::RED},
        {{-0.5f, -0.5f,  0.5f}, colors::RED},

        // SIDE 4 (RIGHT)
        {{ 0.5f, -0.5f, -0.5f}, colors::YELLOW},
        {{ 0.5f,  0.5f, -0.5f}, colors::YELLOW},
        {{ 0.5f,  0.5f,  0.5f}, colors::YELLOW},

        {{ 0.5f, -0.5f, -0.5f}, colors::YELLOW},
        {{ 0.5f,  0.5f,  0.5f}, colors::YELLOW},
        {{ 0.5f, -0.5f,  0.5f}, colors::YELLOW},
        
        // SIDE 5 (BOTTOM)
        {{-0.5f, -0.5f,  0.5f}, colors::CORNFLOWERBLUE},
        {{-0.5f, -0.5f, -0.5f}, colors::CORNFLOWERBLUE},
        {{ 0.5f, -0.5f, -0.5f}, colors::CORNFLOWERBLUE},

        {{-0.5f, -0.5f,  0.5f}, colors::CORNFLOWERBLUE},
        {{ 0.5f, -0.5f, -0.5f}, colors::CORNFLOWERBLUE},
        {{ 0.5f, -0.5f,  0.5f}, colors::CORNFLOWERBLUE},

        // SIDE 6 (TOP)
        {{-0.5f,  0.5f,  0.5f}, colors::CYAN},
        {{-0.5f,  0.5f, -0.5f}, colors::CYAN},
        {{ 0.5f,  0.5f, -0.5f}, colors::CYAN},

        {{-0.5f,  0.5f,  0.5f}, colors::CYAN},
        {{ 0.5f,  0.5f, -0.5f}, colors::CYAN},
        {{ 0.5f,  0.5f,  0.5f}, colors::CYAN}
    };

    // tex coords are all the same
    // I use clockwise/back-face triangles
    std::vector<Vertex3d_Textured> cube_model_textured {
        // near
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        
        // far
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},

        // left
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},

        // right
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        
        // bottom
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},

        // top
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}
    };


    // for lighting, for now reuse this for light source and other objects as well
    std::vector<Vertex3d_Lighting> lighting_cube_model {
        // near
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}},
        
        // far
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},

        // left
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                                 
        // right                 
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                                 
        // bottom                
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
                                 
        // top                   
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}
    };

}

namespace {

void bind_texture_and_sampler(const std::unique_ptr<Texture>& texture, 
                              const std::unique_ptr<Sampler>& sampler, u32 unit = 0) noexcept
{
    texture->bind(unit);
    sampler->bind(unit);
}

void bind_texture_and_sampler(const Texture* const texture, 
                              const Sampler* const sampler, u32 unit = 0) noexcept
{
    texture->bind(unit);
    sampler->bind(unit);
}

[[nodiscard]]
std::array<glm::vec2, 4> get_texture_coordinates(float x, float y, float w, float h, float atlas_width, float atlas_height) noexcept
{
    return {{
        {x/atlas_width,     y/atlas_height    },
        {x/atlas_width,     (y+h)/atlas_height},
        {(x+w)/atlas_width, (y+h)/atlas_height},
        {(x+w)/atlas_width, y/atlas_height    }
    }};
}

struct Transform {
    float sx, sy, sz; // scale values
    float rot_x, rot_y, rot_z; // angle rotation around _axis
    float x, y, z; // translation values
};

Transform trans_1 {1, 1, 1, 0, 0, 0, 0, 0, -3.0f};
Transform view_trans {1, 1, 1, 0, 0, 0, 0, 0, -3.0f};

[[nodiscard]]
Matrix4 get_model_mat(const Transform& t) noexcept
{
    return {
        peria::graphics::translate(t.x, t.y, t.z)*
        peria::graphics::rotate(glm::radians(t.rot_x), glm::radians(t.rot_y), glm::radians(t.rot_z))*
        peria::graphics::scale(t.sx, t.sy, t.sz)
    };
}

[[nodiscard]]
glm::mat4 get_model_mat2(const Transform& t) noexcept
{
    return {
        glm::translate(glm::mat4{1.0f}, glm::vec3{t.x, t.y, t.z})*
        glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_x), glm::vec3{1.0f, 0.0f, 0.0f})*
        glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_y), glm::vec3{0.0f, 1.0f, 0.0f})*
        glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_z), glm::vec3{0.0f, 0.0f, 1.0f})*
        glm::scale(glm::mat4{1.0f}, glm::vec3{t.sx, t.sy, t.sz})
    };
}

[[nodiscard]]
glm::vec3 get_vec3(const std::array<float, 3>& color) noexcept
{ return {color[0], color[1], color[2]}; }

// random
std::random_device rd = std::random_device();
std::mt19937 generator(rd());

[[nodiscard]]
int get_int(int l, int r)
{
    std::uniform_int_distribution<> dist(l, r);
    return dist(rd);
}

std::array positions {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

std::array rotations {
    glm::vec3(0.0f, 0.0f, 0.0f), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
    glm::vec3((float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f), (float)get_int(0.0f, 360.0f)), 
};

struct Light_Source {
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;
    std::array<float, 3> pos {0.592f, 1.250f, -2.611f};
};

struct Lighting_Demo_Vars {
    peria::graphics::materials::Material material;
    Light_Source light_source;
    //Transform object_transform {7.2f, 5.6f, 1.4f, 60.0f, -18.0f, -30.0f, 0.0f, 0.0f, 0.0f};
    Transform object_transform {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    std::array<i32, 8> specular_coefficient {2, 4, 8, 16, 32, 64, 128, 256};
    i32 specualr_coeff_idx {3};

    bool world_space {true};
} lighting_vars;

std::array<float, 3> bg_color;


[[nodiscard]]
std::unique_ptr<Texture> create_solid_texture(const std::array<float, 3>& color)
{
    const colors::Color<float> c {
        color[0],
        color[1],
        color[2],
        1.0f
    };
    return std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(c));
}

}

Graphics::Graphics()
    //:camera{{9.99455f, 1.25345f, -6.73005f}, {0.0f, 0.0f, 0.0f,}, {0.0f, 1.0f, 0.0f}}
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f,}, {0.0f, 1.0f, 0.0f}}
{ 
    peria::log("Graphics init");
    SDL_GL_SetSwapInterval(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    { // setup batch buffers for quads
        batch_quad_vao = std::make_unique<Vertex_Array>();
        batch_quad_vbo = std::make_unique<Named_Buffer_Object<Vertex>>(batch_quad_vbo_size);

        auto quad_count {batch_quad_vbo_size / 4 / sizeof(Vertex)};
        std::vector<u32> indices; indices.reserve(quad_count * 6);
        for (u32 i{}; i<quad_count; ++i) {
            indices.emplace_back(4*i);
            indices.emplace_back(4*i + 1);
            indices.emplace_back(4*i + 2);

            indices.emplace_back(4*i);
            indices.emplace_back(4*i + 2);
            indices.emplace_back(4*i + 3);
        }
        batch_quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

        // pos
        batch_quad_vao->setup_attribute(Attribute<float>{2, false});
        // texture coordinates
        batch_quad_vao->setup_attribute(Attribute<float>{2, false});
        // color
        batch_quad_vao->setup_attribute(Attribute<float>{4, false});
        // texture unit
        batch_quad_vao->setup_attribute(Attribute<float>{1, false});
        batch_quad_vao->connect_vertex_buffer(batch_quad_vbo->buffer_id(), sizeof(Vertex)); batch_quad_vao->connect_index_buffer(batch_quad_ibo->buffer_id());

        draw_quad_command_data.reserve(quad_count);
    }

    quad_shader = std::make_unique<Shader>("./assets/quad_vertex.glsl", "./assets/quad_fragment.glsl");
    {
        std::array<i32, 3> arr{0, 1, 2};
        quad_shader->set_array("u_textures", arr.size(), arr.data());
    }

    { // setup cube vao/vbo
        cube_vao = std::make_unique<Vertex_Array>();
        cube_vbo = std::make_unique<Named_Buffer_Object<Vertex3d>>(buffer_data::cube_model);

        // pos vec3
        cube_vao->setup_attribute(Attribute<float>{3, false});
        // color 
        cube_vao->setup_attribute(Attribute<float>{4, false});
        cube_vao->connect_vertex_buffer(cube_vbo->buffer_id(), sizeof(Vertex3d));
    }

    { // setup textured cube vao/vbo
        cube_vao_textured = std::make_unique<Vertex_Array>();
        cube_vbo_textured = std::make_unique<Named_Buffer_Object<Vertex3d_Textured>>(buffer_data::cube_model_textured);

        // pos vec3
        cube_vao_textured->setup_attribute(Attribute<float>{3, false});
        // color 
        cube_vao_textured->setup_attribute(Attribute<float>{2, false});
        cube_vao_textured->connect_vertex_buffer(cube_vbo_textured->buffer_id(), sizeof(Vertex3d_Textured));
    }

    { // setup lighting related vao/vbos
        // light source
        light_source_vao = std::make_unique<Vertex_Array>();
        light_source_vbo = std::make_unique<Named_Buffer_Object<Vertex3d_Lighting>>(buffer_data::lighting_cube_model);

        // pos only
        light_source_vao->setup_attribute(Attribute<float>{3, false});
        light_source_vao->connect_vertex_buffer(light_source_vbo->buffer_id(), sizeof(Vertex3d_Lighting));

        // other objects in lighting scenes. (cubes basically)
        lighting_vao = std::make_unique<Vertex_Array>();
        lighting_vbo = std::make_unique<Named_Buffer_Object<Vertex3d_Lighting>>(buffer_data::lighting_cube_model);

        // pos
        lighting_vao->setup_attribute(Attribute<float>{3, false});
        // normal
        lighting_vao->setup_attribute(Attribute<float>{3, false});
        // tex coords
        lighting_vao->setup_attribute(Attribute<float>{2, false});
        lighting_vao->connect_vertex_buffer(lighting_vbo->buffer_id(), sizeof(Vertex3d_Lighting));
    }

    cube_shader = std::make_unique<Shader>("./assets/cube_vertex.glsl", "./assets/cube_fragment.glsl");
    cube_shader_textured = std::make_unique<Shader>("./assets/cube_vertex_textured.glsl", "./assets/cube_fragment_textured.glsl");
    cube_shader_textured->set_int("u_texture", 0);

    light_source_shader = std::make_unique<Shader>("./assets/light_source_vertex.glsl", "./assets/light_source_fragment.glsl");
    lighting_shader = std::make_unique<Shader>("./assets/lighting_vertex.glsl", "./assets/lighting_fragment.glsl");
    lighting_shader->set_int("u_material.diffuse_texture", 0);
    lighting_shader->set_int("u_material.specular_texture", 1);
    lighting_shader_view = std::make_unique<Shader>("./assets/lighting_vertex_view.glsl", "./assets/lighting_fragment_view.glsl");

    white_texture = std::make_unique<Texture>(1, 1, colors::Color<float>::to_u8_color(colors::WHITE));
    texture_atlas = std::make_unique<Texture>("./assets/mushrooms_sheet.png");
    chiti = std::make_unique<Texture>("./assets/chitunia.png");
    wooden_container = std::make_unique<Texture>("./assets/wooden_container.png");
    specular_wooden_container = std::make_unique<Texture>("./assets/specular_wooden_container.png");
    solid_color_material_texture_diffuse = create_solid_texture(lighting_vars.material.diffuse);
    solid_color_material_texture_specular = create_solid_texture(lighting_vars.material.specular);
    //chiti = std::make_unique<Texture>("./assets/LashaRaGwirs.png");

    sampler1 = std::make_unique<Sampler>(0);
    sampler2 = std::make_unique<Sampler>(1);

    bind_texture_and_sampler(white_texture, sampler2, 0);
    bind_texture_and_sampler(texture_atlas, sampler1, 1);

    {
        lighting_vars.material.diffuse[0] = colors::BLUEVIOLET.r;
        lighting_vars.material.diffuse[1] = colors::BLUEVIOLET.g;
        lighting_vars.material.diffuse[2] = colors::BLUEVIOLET.b;

        lighting_vars.light_source.ambient[0] = 0.5f;
        lighting_vars.light_source.ambient[1] = 0.5f;
        lighting_vars.light_source.ambient[2] = 0.5f;

        lighting_vars.light_source.diffuse[0] = colors::WHITE.r;
        lighting_vars.light_source.diffuse[1] = colors::WHITE.g;
        lighting_vars.light_source.diffuse[2] = colors::WHITE.b;

        lighting_vars.light_source.specular[0] = colors::WHITE.r;
        lighting_vars.light_source.specular[1] = colors::WHITE.g;
        lighting_vars.light_source.specular[2] = colors::WHITE.b;

        lighting_vars.light_source.pos[0] = 2.0f;
        lighting_vars.light_source.pos[1] = 1.0f;
        lighting_vars.light_source.pos[2] = -4.0f;
    }
}

void Graphics::set_viewport(i32 x, i32 y, i32 w, i32 h) noexcept
{ glViewport(x, y, w, h); }

void Graphics::clear_color() noexcept
{ 
    const auto& [r, g, b, a] = background_color;
    glClearColor(r, g, b, a); 
}

void Graphics::set_clear_color(const colors::Color<float>& color) noexcept
{ 
    background_color = color; 

    // for imgui
    bg_color = {color.r, color.g, color.b}; 
}

void Graphics::clear_buffer() noexcept
{ glClear(clear_buffer_bit_flags); }

void Graphics::set_vsync(bool vsync) noexcept
{ (vsync) ? SDL_GL_SetSwapInterval(1) : SDL_GL_SetSwapInterval(0); }

void Graphics::set_clear_buffer_bits(bool clear_color, bool clear_depth, bool clear_stencil) noexcept
{
    u32 flags{};
    if (clear_color)   flags |= GL_COLOR_BUFFER_BIT;
    if (clear_depth)   flags |= GL_DEPTH_BUFFER_BIT;
    if (clear_stencil) flags |= GL_STENCIL_BUFFER_BIT;
    clear_buffer_bit_flags = flags;
}

void Graphics::set_batch_quad_count(i32 quad_count) noexcept
{
    batch_quad_vbo_size = quad_count * 4 * sizeof(Vertex);

    // reacreate vbo and ibo
    batch_quad_vbo = std::make_unique<Named_Buffer_Object<Vertex>>(batch_quad_vbo_size);

    std::vector<u32> indices; indices.reserve(quad_count * 6);
    for (u32 i{}; i<static_cast<u32>(quad_count); ++i) {
        indices.emplace_back(4*i);
        indices.emplace_back(4*i + 1);
        indices.emplace_back(4*i + 2);

        indices.emplace_back(4*i);
        indices.emplace_back(4*i + 2);
        indices.emplace_back(4*i + 3);
    }
    batch_quad_ibo = std::make_unique<Named_Buffer_Object<u32>>(indices);

    batch_quad_vao->connect_vertex_buffer(batch_quad_vbo->buffer_id(), sizeof(Vertex));
    batch_quad_vao->connect_index_buffer(batch_quad_ibo->buffer_id());

    draw_quad_command_data.clear();
}

void Graphics::draw_colored_quad(const Quad& quad, const colors::Color<float>& color) noexcept
{
    const auto& [x, y, w, h] {quad};
    draw_quad_command_data.emplace_back(Vertex{{x,   y  }, {0.0f, 0.0f}, color, 0});
    draw_quad_command_data.emplace_back(Vertex{{x,   y+h}, {0.0f, 1.0f}, color, 0});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y+h}, {1.0f, 1.0f}, color, 0});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y  }, {1.0f, 0.0f}, color, 0});
}

void Graphics::draw_textured_quad(const Quad& quad, const Quad& texture_region) noexcept
{
    const auto& [x, y, w, h] {quad};
    const auto& [ax, ay, aw, ah] {texture_region};

    const auto texture_dimensions {texture_atlas->dimensions()};
    const auto coords {get_texture_coordinates(ax, ay, aw, ah, texture_dimensions.x, texture_dimensions.y)};

    draw_quad_command_data.emplace_back(Vertex{{x,   y  }, coords[0], colors::WHITE, 1});
    draw_quad_command_data.emplace_back(Vertex{{x,   y+h}, coords[1], colors::WHITE, 1});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y+h}, coords[2], colors::WHITE, 1});
    draw_quad_command_data.emplace_back(Vertex{{x+w, y  }, coords[3], colors::WHITE, 1});
}

void Graphics::render2d() noexcept
{
    if (draw_quad_command_data.empty()) return;

    const auto max_quad_count {batch_quad_vbo_size / 4 / sizeof(Vertex)};
    auto current_quad_count {draw_quad_command_data.size() / 4};
    std::size_t offset {};

    batch_quad_vao->bind();
    quad_shader->use_shader();
    quad_shader->set_mat4("u_mvp", screen_ortho_projection);

    // this was for testing my own projection mat
    // quad_shader->set_mat4("u_mvp", peria_ortho_projection);
    
    while (current_quad_count > 0) {
        i32 c {}; // count of rects for each batch
        if (current_quad_count >= max_quad_count) {
            c = max_quad_count;
        }
        else { // smaller remaining batch
            c = current_quad_count;
        }

        batch_quad_vbo->set_sub_data(0, c*4*sizeof(Vertex), draw_quad_command_data.data() + offset);
        glDrawElements(GL_TRIANGLES, c*6, GL_UNSIGNED_INT, nullptr);
        offset += 4*c;
        current_quad_count -= c;
    }

    draw_quad_command_data.clear();
}

static auto FOV {45.0f};

void Graphics::render_cube() noexcept
{
    cube_vao->bind();
    cube_shader->use_shader();

    const auto view {peria::graphics::translate(trans_1.x, trans_1.y, trans_1.z)};
    peria_perspective(FOV, 800.0f/600.0f, 0.1f, 100.0f);

    cube_shader->set_mat4("u_mvp", peria_perspective_projection*view);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Graphics::render_cube_textured() noexcept
{
    cube_vao_textured->bind();
    cube_shader_textured->use_shader();
    bind_texture_and_sampler(chiti, sampler1, 0);

    const auto model {peria::graphics::rotate(glm::radians(trans_1.rot_x), glm::radians(trans_1.rot_y), glm::radians(trans_1.rot_z))};
    const auto view {peria::graphics::translate(trans_1.x, trans_1.y, trans_1.z)};

    cube_shader_textured->set_mat4("u_mvp", peria_perspective_projection*view*model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Graphics::render3d() noexcept
{
    cube_vao_textured->bind();
    cube_shader_textured->use_shader();
    bind_texture_and_sampler(chiti, sampler1, 0);

    if (0) {
        for (std::size_t i{}; i<positions.size(); ++i) {
            const auto& v {positions[i]};
            const auto& r {rotations[i]};
            const auto model {get_model_mat2({
                    1.0f, 1.0f, 1.0f, 
                    r.x, r.y, r.z,
                    v.x, v.y, v.z})};
            cube_shader_textured->set_mat4("u_mvp", perspective_projection*camera.get_view()*model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // with my matrix class
    if (1) {
        for (std::size_t i{}; i<positions.size(); ++i) {
            const auto& v {positions[i]};
            const auto& r {rotations[i]};
            const auto model {get_model_mat({
                    1.0f, 1.0f, 1.0f, 
                    r.x, r.y, r.z,
                    v.x, v.y, v.z})};
            cube_shader_textured->set_mat4("u_mvp", peria_perspective_projection*camera.get_peria_view()*model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}

void Graphics::render3d_lighting() noexcept
{
    light_source_vao->bind();
    light_source_shader->use_shader();

    const auto& [lx, ly, lz] {lighting_vars.light_source.pos};
    const auto light_source_model {get_model_mat2({
            0.30f, 0.30f, 0.30f, 
            0.0f, 0.0f, 0.0f,
            lx, ly, lz})};

    light_source_shader->set_mat4("u_mvp", perspective_projection*camera.get_view()*light_source_model);
    light_source_shader->set_vec3("u_light_source_color", get_vec3(lighting_vars.light_source.diffuse));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lighting_vao->bind();

    const auto object_model {get_model_mat2({
            lighting_vars.object_transform.sx, lighting_vars.object_transform.sy, lighting_vars.object_transform.sz,
            lighting_vars.object_transform.rot_x, lighting_vars.object_transform.rot_y, lighting_vars.object_transform.rot_z,
            lighting_vars.object_transform.x, lighting_vars.object_transform.y, lighting_vars.object_transform.z})};

    //bind_texture_and_sampler(wooden_container.get(), sampler1.get(), 0);
    //bind_texture_and_sampler(specular_wooden_container.get(), sampler1.get(), 1);
    
    bind_texture_and_sampler(solid_color_material_texture_diffuse.get(), sampler1.get(), 0);
    bind_texture_and_sampler(solid_color_material_texture_specular.get(), sampler1.get(), 1);
    lighting_shader->use_shader();

    if (lighting_vars.world_space) {

        lighting_shader->set_vec3("u_light.pos", {lx, ly, lz});
        lighting_shader->set_vec3("u_view_pos", camera.get_pos());

        //lighting_shader->set_vec3("u_material.ambient", get_vec3(lighting_vars.material.ambient));
        //lighting_shader->set_vec3("u_material.diffuse", get_vec3(lighting_vars.material.diffuse));
        //lighting_shader->set_vec3("u_material.specular", get_vec3(lighting_vars.material.specular));

        lighting_vars.material.shininess = lighting_vars.specular_coefficient[lighting_vars.specualr_coeff_idx];
        lighting_shader->set_float("u_material.shininess", lighting_vars.material.shininess);

        lighting_shader->set_vec3("u_light.ambient", get_vec3(lighting_vars.light_source.ambient));
        lighting_shader->set_vec3("u_light.diffuse", get_vec3(lighting_vars.light_source.diffuse));
        lighting_shader->set_vec3("u_light.specular", get_vec3(lighting_vars.light_source.specular));

        lighting_shader->set_mat4("u_vp", perspective_projection*camera.get_view());
        lighting_shader->set_mat4("u_model", object_model);
    }
    else {
        lighting_shader_view->use_shader();

        // note that since we need light pos in vertex shader we are not supplying pos in Light struct in Fragment Shader
        lighting_shader_view->set_vec3("u_light_source_pos", {lx, ly, lz}); // in vertex shader
        lighting_shader_view->set_vec3("u_view_pos", camera.get_pos());

        lighting_shader_view->set_vec3("u_material.ambient", get_vec3(lighting_vars.material.ambient));
        lighting_shader_view->set_vec3("u_material.diffuse", get_vec3(lighting_vars.material.diffuse));
        lighting_shader_view->set_vec3("u_material.specular", get_vec3(lighting_vars.material.specular));

        lighting_vars.material.shininess = lighting_vars.specular_coefficient[lighting_vars.specualr_coeff_idx];
        lighting_shader_view->set_float("u_material.shininess", lighting_vars.material.shininess);

        lighting_shader_view->set_vec3("u_light.ambient", get_vec3(lighting_vars.light_source.ambient));
        lighting_shader_view->set_vec3("u_light.diffuse", get_vec3(lighting_vars.light_source.diffuse));
        lighting_shader_view->set_vec3("u_light.specular", get_vec3(lighting_vars.light_source.specular));

        lighting_shader_view->set_mat4("u_projection", perspective_projection);
        lighting_shader_view->set_mat4("u_view", camera.get_view());
        lighting_shader_view->set_mat4("u_model", object_model);
    }


    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Graphics::peria_ortho(float left, float right, float bottom, float top) noexcept
{ 
    screen_ortho_projection = glm::ortho(left, right, bottom, top);
    peria_ortho_projection = peria::graphics::get_ortho_projection(left, right, bottom, top);
}

void Graphics::peria_perspective(float fov_y, float aspect_ratio, float near, float far) noexcept
{ 
    perspective_projection = glm::perspective(glm::radians(fov_y), aspect_ratio, near, far); 
    peria_perspective_projection = peria::graphics::get_perspective_projection(glm::radians(fov_y), aspect_ratio, near, far); 
}

void Graphics::start_imgui_frame(ImFont* font)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    //ImGui::PushFont(font); // must be after ImGui::NewFrame(), at least seems like so xD
}

void Graphics::imgui_render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Graphics::imgui_lighting()
{
    ImGui::Begin("Lighting");

    ImGui::ColorEdit3("ClearColor", bg_color.data()); 
    background_color.r = bg_color[0];
    background_color.g = bg_color[1];
    background_color.b = bg_color[2];

    ImGui::SliderFloat3("LightSourcePosition", lighting_vars.light_source.pos.data(), -10.0f, 10.0f);
    ImGui::ColorEdit3("LightSource-Ambient", lighting_vars.light_source.ambient.data()); 
    ImGui::ColorEdit3("LightSource-Diffuse", lighting_vars.light_source.diffuse.data()); 
    ImGui::ColorEdit3("LightSource-Specular", lighting_vars.light_source.specular.data()); 

    ImGui::ColorEdit3("ObjectMaterial-Ambient", lighting_vars.material.ambient.data()); 
    ImGui::ColorEdit3("ObjectMaterial-Diffuse", lighting_vars.material.diffuse.data()); 
    ImGui::ColorEdit3("ObjectMaterial-Specular", lighting_vars.material.specular.data()); 
    ImGui::Text("Shininess %d\n", lighting_vars.specular_coefficient[lighting_vars.specualr_coeff_idx]);

    ImGui::InputFloat("Object Trans X", &lighting_vars.object_transform.x, 0.2f);
    ImGui::InputFloat("Object Trans Y", &lighting_vars.object_transform.y, 0.2f);
    ImGui::InputFloat("Object Trans Z", &lighting_vars.object_transform.z, 0.2f);

    ImGui::InputFloat("Object Scale X", &lighting_vars.object_transform.sx, 0.2f);
    ImGui::InputFloat("Object Scale Y", &lighting_vars.object_transform.sy, 0.2f);
    ImGui::InputFloat("Object Scale Z", &lighting_vars.object_transform.sz, 0.2f);

    ImGui::InputFloat("Rot X", &lighting_vars.object_transform.rot_x, 2.0f);
    ImGui::InputFloat("Rot Y", &lighting_vars.object_transform.rot_y, 2.0f);
    ImGui::InputFloat("Rot Z", &lighting_vars.object_transform.rot_z, 2.0f);

    ImGui::Text("Materials\n");
    if (ImGui::Button("Emerald")) {
        lighting_vars.material = materials::EMERALD;
        solid_color_material_texture_diffuse = create_solid_texture(materials::EMERALD.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::EMERALD.specular);
    }
    if (ImGui::Button("Jade")) {
        lighting_vars.material = materials::JADE;
        solid_color_material_texture_diffuse = create_solid_texture(materials::JADE.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::JADE.specular);
    }
    if (ImGui::Button("Obsidian")) {
        lighting_vars.material = materials::OBSIDIAN;
        solid_color_material_texture_diffuse = create_solid_texture(materials::OBSIDIAN.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::OBSIDIAN.specular);
    }
    if (ImGui::Button("Pearl")) {
        lighting_vars.material = materials::PEARL;
        solid_color_material_texture_diffuse = create_solid_texture(materials::PEARL.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::PEARL.specular);
    }
    if (ImGui::Button("Ruby")) {
        lighting_vars.material = materials::RUBY;
        solid_color_material_texture_diffuse = create_solid_texture(materials::RUBY.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::RUBY.specular);
    }
    if (ImGui::Button("Turquoise")) {
        lighting_vars.material = materials::TURQUOISE;
        solid_color_material_texture_diffuse = create_solid_texture(materials::TURQUOISE.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::TURQUOISE.specular);
    }
    if (ImGui::Button("Brass")) {
        lighting_vars.material = materials::BRASS;
        solid_color_material_texture_diffuse = create_solid_texture(materials::BRASS.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::BRASS.specular);
    }
    if (ImGui::Button("Bronze")) {
        lighting_vars.material = materials::BRONZE;
        solid_color_material_texture_diffuse = create_solid_texture(materials::BRONZE.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::BRONZE.specular);
    }
    if (ImGui::Button("Chrome")) {
        lighting_vars.material = materials::CHROME;
        solid_color_material_texture_diffuse = create_solid_texture(materials::CHROME.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::CHROME.specular);
    }
    if (ImGui::Button("Copper")) {
        lighting_vars.material = materials::COPPER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::COPPER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::COPPER.specular);
    }
    if (ImGui::Button("Gold")) {
        lighting_vars.material = materials::GOLD;
        solid_color_material_texture_diffuse = create_solid_texture(materials::GOLD.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::GOLD.specular);
    }
    if (ImGui::Button("Silver")) {
        lighting_vars.material = materials::SILVER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::SILVER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::SILVER.specular);
    }
    if (ImGui::Button("Black_Plastic")) {
        lighting_vars.material = materials::BLACK_PLASTIC;
        solid_color_material_texture_diffuse = create_solid_texture(materials::BLACK_PLASTIC.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::BLACK_PLASTIC.specular);
    }
    if (ImGui::Button("Cyan_Plastic")) {
        lighting_vars.material = materials::CYAN_PLASTIC;
        solid_color_material_texture_diffuse = create_solid_texture(materials::CYAN_PLASTIC.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::CYAN_PLASTIC.specular);
    }
    if (ImGui::Button("Green_Plastic")) {
        lighting_vars.material = materials::GREEN_PLASTIC;
        solid_color_material_texture_diffuse = create_solid_texture(materials::GREEN_PLASTIC.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::GREEN_PLASTIC.specular);
    }
    if (ImGui::Button("Red_Plastic")) {
        lighting_vars.material = materials::RED_PLASTIC;
        solid_color_material_texture_diffuse = create_solid_texture(materials::RED_PLASTIC.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::RED_PLASTIC.specular);
    }
    if (ImGui::Button("White_Plastic")) {
        lighting_vars.material = materials::WHITE_PLASTIC;
        solid_color_material_texture_diffuse = create_solid_texture(materials::WHITE_PLASTIC.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::WHITE_PLASTIC.specular);
    }
    if (ImGui::Button("Yellow_Plastic")) {
        lighting_vars.material = materials::YELLOW_PLASTIC;
        solid_color_material_texture_diffuse = create_solid_texture(materials::YELLOW_PLASTIC.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::YELLOW_PLASTIC.specular);
    }
    if (ImGui::Button("Black_Rubber")) {
        lighting_vars.material = materials::BLACK_RUBBER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::BLACK_RUBBER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::BLACK_RUBBER.specular);
    }
    if (ImGui::Button("Cyan_Rubber")) {
        lighting_vars.material = materials::CYAN_RUBBER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::CYAN_RUBBER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::CYAN_RUBBER.specular);
    }
    if (ImGui::Button("Green_Rubber")) {
        lighting_vars.material = materials::GREEN_RUBBER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::GREEN_RUBBER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::GREEN_RUBBER.specular);
    }
    if (ImGui::Button("Red_Rubber")) {
        lighting_vars.material = materials::RED_RUBBER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::RED_RUBBER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::RED_RUBBER.specular);
    }
    if (ImGui::Button("White_Rubber")) {
        lighting_vars.material = materials::WHITE_RUBBER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::WHITE_RUBBER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::WHITE_RUBBER.specular);
    }
    if (ImGui::Button("Yellow_Rubber")) {
        lighting_vars.material = materials::YELLOW_RUBBER;
        solid_color_material_texture_diffuse = create_solid_texture(materials::YELLOW_RUBBER.diffuse);
        solid_color_material_texture_specular = create_solid_texture(materials::YELLOW_RUBBER.specular);
    }

    ImGui::Checkbox("Do Lighting in WorldSpace", &lighting_vars.world_space);

    ImGui::End();
}

void Graphics::imgui_transforms()
{
    ImGui::Begin("Transforms");
    ImGui::InputFloat("translate X", &trans_1.x, 0.05f);
    ImGui::InputFloat("translate Y", &trans_1.y, 0.05f);
    ImGui::InputFloat("translate Z", &trans_1.z, 0.05f);

    ImGui::InputFloat("rot-angle X", &trans_1.rot_x, 2.0f);
    ImGui::InputFloat("rot-angle Y", &trans_1.rot_y, 2.0f);
    ImGui::InputFloat("rot-angle Z", &trans_1.rot_z, 2.0f);

    ImGui::InputFloat("View Trans X", &view_trans.x, 0.1f);
    ImGui::InputFloat("View Trans Y", &view_trans.y, 0.1f);
    ImGui::InputFloat("View Trans Z", &view_trans.z, 0.1f);

    ImGui::End();

}

void Graphics::inc_specular_coefficient() noexcept
{ lighting_vars.specualr_coeff_idx = std::min(lighting_vars.specualr_coeff_idx + 1, static_cast<i32>(lighting_vars.specular_coefficient.size()-1)); }

void Graphics::dec_specular_coefficient() noexcept
{ lighting_vars.specualr_coeff_idx = std::max(lighting_vars.specualr_coeff_idx - 1, 0); }

void Graphics::imgui_matrix_info()
{
    ImGui::Begin("Transforms");

    auto cam {camera.get_view()};
    auto s {glm::to_string(cam)};
    for (std::size_t i{}; i<s.size(); ++i) {
        if (s[i] == ')' && i+1 < s.size() && s[i+1] == ',') {
            s[i+1] = '\n';
        }
    }
    s = s.substr(6);
    ImGui::Text("%s", s.c_str());

    auto p_cam {camera.get_peria_view().to_string()};
    ImGui::Text("\n%s", p_cam.c_str());

    ImGui::End();
}

Graphics::~Graphics()
{ peria::log("Graphics shutdown"); }
}
