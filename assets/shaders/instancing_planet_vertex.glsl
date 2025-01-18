#version 460 core

layout (location = 0) in vec3 aPos;
// location = 1 is normals that are not used in this demo
layout (location = 2) in vec2 aTexCoords;

out vec2 texture_coordinates;

uniform mat4 u_vp;
uniform mat4 u_model;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);
    texture_coordinates = aTexCoords;
}
