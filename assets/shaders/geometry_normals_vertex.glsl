#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_view;
uniform mat4 u_model;

out VS_OUT {
    vec3 norm;
} vs_out;

void main()
{
    gl_Position = u_view*u_model*vec4(aPos, 1.0f);
    vs_out.norm = normalize(mat3(inverse(transpose(u_view*u_model)))*aNormal);
}
