#pragma once

#include <vector>
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Quad_Vertex {
    glm::vec2 pos;
    glm::vec4 color;
    glm::vec2 tex_coord;
};

class Mesh {
public:
    Mesh(const std::vector<Quad_Vertex>& vertex_data);
    Mesh(const std::vector<Quad_Vertex>& vertex_data, const std::vector<uint32_t>& index_data);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    void bind() const;
    void unbind() const;
    uint32_t get_index_count() const;
    uint32_t get_vertex_count() const;
    
private:
    uint32_t _vao;
    uint32_t _vbo;
    uint32_t _ibo;
    uint32_t _vertex_count;
    uint32_t _index_count;

    void init_vao();
    void init_vbo(const std::vector<Quad_Vertex>& vertex_data);
    void init_ibo(const std::vector<uint32_t>& index_data);
    void set_layout();

};
