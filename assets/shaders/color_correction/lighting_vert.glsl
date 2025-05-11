#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_DATA {
    vec3 frag_pos;
    vec3 normal;
    vec2 texture_coordinates;
} vs_data;

uniform mat4 u_vp;
uniform mat4 u_model;

void main()
{
    vec4 world_pos = u_model*vec4(aPos, 1.0f);
    gl_Position = u_vp*world_pos;
    vs_data.texture_coordinates = aTexCoords;
    vs_data.normal = transpose(inverse(mat3(u_model)))*aNormal;
    vs_data.frag_pos = world_pos.xyz;
}
