#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 normal;
    vec2 tex_coords;
} vs_out;

uniform mat4 u_vp;
uniform mat4 u_model;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);
    vs_out.normal = mat3(inverse(transpose(u_model)))*aNormal;
    vs_out.tex_coords = aTexCoords;
}
