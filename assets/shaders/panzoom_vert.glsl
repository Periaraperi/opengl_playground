#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texture_coordinates;

uniform mat4 u_vp;
uniform mat4 u_model;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 0.0f, 1.0f);
    texture_coordinates = aTexCoords;
}
