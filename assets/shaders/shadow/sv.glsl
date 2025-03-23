#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_vp; // from the perspective of light
uniform mat4 u_model;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);
}
