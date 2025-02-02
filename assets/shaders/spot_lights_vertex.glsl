#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;

out VS_DATA {
    vec3 frag_pos; // world space
    vec3 normal;
    vec2 texture_coordinates;
    vec4 frag_pos_light_space;
} vs_data;

uniform mat4 u_vp;
uniform mat4 u_model;
uniform mat4 u_light_vp;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);
    vs_data.frag_pos = vec3(u_model*vec4(aPos, 1.0f));
    vs_data.texture_coordinates = aTextureCoordinates;
    vs_data.normal = inverse(transpose(mat3(u_model)))*aNormal;
    vs_data.frag_pos_light_space = u_light_vp * vec4(vs_data.frag_pos, 1.0f);
}
