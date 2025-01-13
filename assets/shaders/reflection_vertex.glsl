#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 position;
out vec3 normal;

uniform mat4 u_vp;
uniform mat4 u_model;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);
    normal = mat3(transpose(inverse(u_model))) * aNormal; 
    position = vec3(u_model*vec4(aPos, 1.0f));
}
