#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_DATA {
    vec3 frag_pos;
    vec4 light_space_frag_pos;
    vec2 texture_coordinates;
    vec3 normal;
} vs_data;

uniform mat4 u_vp;
uniform mat4 u_light_vp;
uniform mat4 u_model;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);
    vs_data.texture_coordinates = aTexCoord;
    vs_data.frag_pos = vec3(u_model*vec4(aPos, 1.0f));
    vs_data.light_space_frag_pos = u_light_vp*vec4(vs_data.frag_pos, 1.0f);
    vs_data.texture_coordinates = aTexCoord;
    vs_data.normal = transpose(inverse(mat3(u_model)))*aNormal;
}
