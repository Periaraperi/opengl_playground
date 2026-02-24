#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 aInstanceMatrix;

out vec3 color;
uniform mat4 u_vp;

void main()
{
    gl_Position = u_vp*aInstanceMatrix*vec4(aPos, 0.0f, 1.0f);
    color = aColor;
}

