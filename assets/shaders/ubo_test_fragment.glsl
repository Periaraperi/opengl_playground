#version 460 core

layout (std140, binding = 0) uniform Matrix_Block {
    mat4 u_projection;
    mat4 u_view;
    vec4 u_colors[2];
};

out vec4 fragment_color;

void main()
{
    fragment_color = u_colors[0];
}
