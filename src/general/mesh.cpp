#include "mesh.hpp"

#include "logger.hpp"
#include <glad/glad.h>
#include <stdlib.h>

Mesh::Mesh(const std::vector<Quad_Vertex>& vertex_data)
    :_vertex_count(vertex_data.size()), 
     _index_count(0)
{
    init_vao(); 
    init_vbo(vertex_data); 
    set_layout();
    unbind();
}

Mesh::Mesh(const std::vector<Quad_Vertex>& vertex_data,
           const std::vector<uint32_t>& index_data)
    :_vertex_count(vertex_data.size()), 
     _index_count(index_data.size())
{
    init_vao(); 
    init_vbo(vertex_data); 
    init_ibo(index_data);
    set_layout();
    unbind();
}

Mesh::~Mesh()
{
    GL_CALL(glDeleteBuffers(1,&_ibo));
    GL_CALL(glDeleteBuffers(1,&_vbo));
    GL_CALL(glDeleteVertexArrays(1,&_vao));
}

void Mesh::init_vao()
{
    GL_CALL(glGenVertexArrays(1,&_vao));
    GL_CALL(glBindVertexArray(_vao));
}

void Mesh::init_vbo(const std::vector<Quad_Vertex>& vertex_data)
{
    GL_CALL(glGenBuffers(1,&_vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER,_vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER,
                         sizeof(Quad_Vertex)*vertex_data.size(),
                         &vertex_data[0], GL_STATIC_DRAW));
}

void Mesh::init_ibo(const std::vector<uint32_t>& index_data)
{
    GL_CALL(glGenBuffers(1,&_ibo));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(uint32_t)*index_data.size(),
                         &index_data[0], GL_STATIC_DRAW));
}

void Mesh::set_layout()
{
    int offset = 0;
    // pos
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(Quad_Vertex),(const void*)offset));
    offset += sizeof(glm::vec2); 

    // color
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Quad_Vertex),(const void*)offset));
    offset += sizeof(glm::vec4); 
    
    // texture coordinates
    GL_CALL(glEnableVertexAttribArray(2));
    GL_CALL(glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Quad_Vertex),(const void*)offset));
    offset += sizeof(glm::vec2); 
}

void Mesh::bind() const
{
    GL_CALL(glBindVertexArray(_vao));
}

void Mesh::unbind() const
{
    GL_CALL(glBindVertexArray(0));
}

uint32_t Mesh::get_index_count() const
{
    return _index_count;
}

uint32_t Mesh::get_vertex_count() const
{
    return _vertex_count;
}
