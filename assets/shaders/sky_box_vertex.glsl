#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 direction; // for sampling cubemap

uniform mat4 u_vp;

void main()
{
    vec4 pos = u_vp*vec4(aPos, 1.0f);
    direction = aPos;
    gl_Position = pos.xyww; // z-trick for performace
}
