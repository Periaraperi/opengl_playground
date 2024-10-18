#include "vertex_array.hpp"

#include <glad/glad.h>
#include <iostream>

namespace peria::engine {
// get size in bytes from GL enum type names
u8 gl_type_bytes(GLenum type)
{
    switch (type) {
        case GL_BYTE: case GL_UNSIGNED_BYTE:
            return 1;
        case GL_SHORT: case GL_UNSIGNED_SHORT:
            return 2;
        case GL_INT: case GL_UNSIGNED_INT: case GL_FLOAT:
            return 4;
        case GL_DOUBLE:
            return 8;
    }

    return 0;
}

// creates new vertex array object
// and binds it to currently active one
vertex_array::vertex_array(std::vector<vertex_attribute>&& vertex_attributes)
    :attributes{std::move(vertex_attributes)}
{
    std::cerr << "creating vertex_array object\n";
    glCreateVertexArrays(1, &id);

    std::size_t offset = 0;
    for (std::size_t i{}; i<attributes.size(); ++i) {
        const auto& a = attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, a.count, a.type, 
                    a.normalized ? GL_TRUE : GL_FALSE, 
                    a.stride, (const void*)offset);
        offset += (a.count * gl_type_bytes(a.type));
    }
}

vertex_array::~vertex_array()
{ std::cerr << "destroying vertex_array object\n"; glDeleteVertexArrays(1, &id); }

}
