#version 460 core

layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform Matrix_Block {
    mat4 u_projection;
    mat4 u_view;
    vec4 u_colors[2];
};

uniform mat4 u_model;

void main()
{
    gl_Position = u_projection*u_view*u_model*vec4(aPos, 1.0f);
}
