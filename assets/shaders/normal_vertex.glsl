#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 u_vp;
uniform mat4 u_model;

out VS_DATA {
    vec3 frag_pos;
    vec2 texture_coordinates;
} vs_data;

void main()
{
    vec4 wpos = u_model*vec4(aPos, 1.0f);
    gl_Position = u_vp*wpos;
    vs_data.frag_pos = wpos.xyz;
    vs_data.texture_coordinates = aTexCoords;
}
