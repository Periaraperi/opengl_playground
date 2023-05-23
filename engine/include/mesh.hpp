#pragma once

#include <vector>
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Quad_Vertex {
    glm::vec2 pos;
    glm::vec4 color;
    glm::vec2 tex_coord;
    float tex_id;
};

struct Circle_Vertex {
    glm::vec2 pos;
    glm::vec2 center;
    float radius;
    glm::vec4 color;
};

// called Mesh for now, is not exactly what Mesh shoud be
// Mesh is basically a combination of different opengl buffers, i.e vertex array, vertex buffer and index buffer
// Mesh handles the layout of our buffer data
// Mesh can handle static buffers for static draws
//
// Mesh can also handle dynamic buffer of some max size for batch rendering
class Mesh {
public:
    Mesh(const std::vector<Quad_Vertex>& vertex_data);
    Mesh(const std::vector<Quad_Vertex>& vertex_data, const std::vector<uint32_t>& index_data);
    Mesh(const std::vector<Circle_Vertex>& vertex_data, const std::vector<uint32_t>& index_data);
    Mesh(int max_quads);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    void bind() const;
    void unbind() const;
    uint32_t get_vertex_count() const;
    uint32_t get_index_count() const;

    void update_buffer(const std::vector<Quad_Vertex>& v, int offset);
    void update_index_buffer(uint32_t quad_count);
    
    
private:
    uint32_t _vao;
    uint32_t _vbo;
    uint32_t _ibo;
    uint32_t _vertex_count;
    uint32_t _index_count;

    void init_vao();
    void init_vbo(const std::vector<Quad_Vertex>& vertex_data);
    void init_vbo(const std::vector<Circle_Vertex>& vertex_data);
    void init_ibo(const std::vector<uint32_t>& index_data);
    void set_layout();
    void set_circle_layout();

};
