#include "demo.hpp"

#include <glad/glad.h>
#include <array>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <stb_image.h>

#include "simple_logger.hpp"
#include "graphics.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <memory>
//#include <algorithm>
//
//#include <random>
//#include <numeric>
//
//#include <imgui.h>
//#include <imgui_impl_sdl2.h>
//#include <imgui_impl_opengl3.h>
//
//#include "graphics.hpp"

namespace {
    struct Vertex_Pos_Tex {
        glm::vec3 pos;
        glm::vec2 tex_coord;
    };

    std::array<Vertex_Pos_Tex, 36> cube_data {{
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
                                 
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                                 
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                                 
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}
    }};

//    std::vector<peria::graphics::vertex::Vertex3d> cube_model {
//        // near
//        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},
//        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}},
//        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},
//        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},
//        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},
//        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}},
//        
//        // far
//        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
//        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
//        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
//        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
//        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
//        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
//
//        // left
//        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
//        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//                                 
//        // right                 
//        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
//        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//                                 
//        // bottom                
//        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
//        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
//        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
//        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
//        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
//        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
//                                 
//        // top                   
//        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
//        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
//        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
//        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}
//    };
//
//    std::vector<glm::vec3> sky_box_cube_model {
//        // near
//        {-1.0f, -1.0f,  1.0f},
//        {-1.0f,  1.0f,  1.0f},
//        { 1.0f,  1.0f,  1.0f},
//        {-1.0f, -1.0f,  1.0f},
//        { 1.0f,  1.0f,  1.0f},
//        { 1.0f, -1.0f,  1.0f},
//        
//        // far
//        { 1.0f, -1.0f, -1.0f},
//        { 1.0f,  1.0f, -1.0f},
//        {-1.0f,  1.0f, -1.0f},
//        { 1.0f, -1.0f, -1.0f},
//        {-1.0f,  1.0f, -1.0f},
//        {-1.0f, -1.0f, -1.0f},
//
//        // left
//        {-1.0f, -1.0f, -1.0f},
//        {-1.0f,  1.0f, -1.0f},
//        {-1.0f,  1.0f,  1.0f},
//        {-1.0f, -1.0f, -1.0f},
//        {-1.0f,  1.0f,  1.0f},
//        {-1.0f, -1.0f,  1.0f},
//                              
//        // right               
//        { 1.0f, -1.0f,  1.0f},
//        { 1.0f,  1.0f,  1.0f},
//        { 1.0f,  1.0f, -1.0f},
//        { 1.0f, -1.0f,  1.0f},
//        { 1.0f,  1.0f, -1.0f},
//        { 1.0f, -1.0f, -1.0f},
//                              
//        // bottom              
//        {-1.0f, -1.0f, -1.0f},
//        {-1.0f, -1.0f,  1.0f},
//        { 1.0f, -1.0f,  1.0f},
//        {-1.0f, -1.0f, -1.0f},
//        { 1.0f, -1.0f,  1.0f},
//        { 1.0f, -1.0f, -1.0f},
//                              
//        // top                 
//        {-1.0f,  1.0f,  1.0f},
//        {-1.0f,  1.0f, -1.0f},
//        { 1.0f,  1.0f, -1.0f},
//        {-1.0f,  1.0f,  1.0f},
//        { 1.0f,  1.0f, -1.0f},
//        { 1.0f,  1.0f,  1.0f}
//    };
//
//    // random
//    std::random_device rd = std::random_device();
//    std::mt19937 generator(rd());
//
//    [[nodiscard]]
//    int get_int(int l, int r)
//    {
//        std::uniform_int_distribution<> dist(l, r);
//        return dist(rd);
//    }
//
//    [[nodiscard]]
//    float get_float(float l, float r)
//    {
//        std::uniform_real_distribution<> dist(l, r);
//        return dist(rd);
//    }
//
//
//    [[nodiscard]]
//    glm::mat4 get_model_mat(const Transform& t) noexcept
//    {
//        return {
//            glm::translate(glm::mat4{1.0f}, glm::vec3{t.x, t.y, t.z})*
//            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_x), glm::vec3{1.0f, 0.0f, 0.0f})*
//            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_y), glm::vec3{0.0f, 1.0f, 0.0f})*
//            glm::rotate(glm::mat4{1.0f}, glm::radians(t.rot_z), glm::vec3{0.0f, 0.0f, 1.0f})*
//            glm::scale(glm::mat4{1.0f}, glm::vec3{t.sx, t.sy, t.sz})
//        };
//    }
//
//    [[nodiscard]]
//    glm::vec3 get_vec3(const std::array<float, 3>& color) noexcept
//    { return {color[0], color[1], color[2]}; }
//}
}

namespace {
    template<typename T>
    void load_texture(const peria::Texture<T>& tex, const char* path)
    {
        if constexpr(std::is_same_v<T, peria::Texture2D>) {
            stbi_set_flip_vertically_on_load(1);

            i32 width, height, channel_count;
            u8* data {stbi_load(path, &width, &height, &channel_count, 0)};

            if (data == nullptr) {
                peria::log("failed to load res: ", path);
                return;
            }

            i32 internal_format {channel_count == 4 ? GL_RGBA8 : GL_RGB8};
            i32 format          {channel_count == 4 ? GL_RGBA : GL_RGB};

            glTextureStorage2D(tex.id, 1, internal_format, width, height);
            glTextureSubImage2D(tex.id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
            glGenerateTextureMipmap(tex.id);

            stbi_image_free(data); 
            data = nullptr;
        }
        else if constexpr(std::is_same_v<T, peria::Texture_Cubemap>) {
            static_assert(false, "FUCK YOU");
        }
    }
}

namespace peria::demos {

Textured_Cube::Textured_Cube()
    :camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
     shader{"./assets/shaders/a_vertex.glsl", "./assets/shaders/a_fragment.glsl"}
{
    // vao/vbo
    {
        glNamedBufferData(vbo.id, sizeof(Vertex_Pos_Tex)*cube_data.size(), cube_data.data(), GL_STATIC_DRAW);

        // pos
        glEnableVertexArrayAttrib(vao.id, 0);
        glVertexArrayAttribBinding(vao.id, 0, 0);
        glVertexArrayAttribFormat(vao.id, 0, 3, GL_FLOAT, GL_FALSE, 0);

        // tex coord
        glEnableVertexArrayAttrib(vao.id, 1);
        glVertexArrayAttribBinding(vao.id, 1, 0);
        glVertexArrayAttribFormat(vao.id, 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos_Tex::pos));

        glVertexArrayVertexBuffer(vao.id, 0, vbo.id, 0, sizeof(Vertex_Pos_Tex));
    }

    { // sampler settings
        glSamplerParameteri(sampler.id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler.id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    { // texture
        load_texture(tex, "./assets/textures/chitunia.png");
    }

    shader.set_int("u_texture", 0);

    recalculate_projection();
}

void Textured_Cube::recalculate_projection()
{
    const auto screen_dims {peria::get_screen_dimensions()};
    projection = glm::perspective(glm::radians(45.0f), screen_dims.x / screen_dims.y, 0.1f, 100.f);
}

void Textured_Cube::update()
{ camera.update(); }

void Textured_Cube::render()
{
    peria::clear_named_buffer(0, colors::SILVER, 1.0f, 0);

    glBindVertexArray(vao.id);
    shader.use_shader();

    shader.set_mat4("u_mvp", projection*camera.get_view());
    glBindTextureUnit(0, tex.id);
    glBindSampler(0, sampler.id);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Textured_Cube::imgui()
{}

}
