#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTanget;
layout (location = 4) in vec3 aBitangent;

uniform mat4 u_vp;
uniform mat4 u_model;
uniform vec3 u_pl_pos;
uniform vec3 u_camera_pos;

out VS_DATA {
    vec3 frag_pos;
    vec3 tanget_space_frag_pos;
    vec3 tanget_space_point_light_pos;
    vec3 tanget_space_camera_pos;
    vec3 normal;
    vec2 texture_coordinates;
} vs_data;

void main()
{
    vec4 wpos = u_model*vec4(aPos, 1.0f);
    gl_Position = u_vp*wpos;
    vs_data.frag_pos = wpos.xyz;
    vs_data.texture_coordinates = aTexCoords;

    mat3 normal_mat = inverse(transpose(mat3(u_model)));
    vs_data.normal = inverse(transpose(mat3(u_model)))*aNormal;

    vec3 T = normalize(normal_mat*aTanget);
    vec3 N = normalize(normal_mat*aNormal);
    T = normalize(T - dot(T, N)*N);
    vec3 B = cross(N, T);
    mat3 tbn_inv = transpose(mat3(T, B, N));

    vs_data.tanget_space_frag_pos = tbn_inv*wpos.xyz;
    vs_data.tanget_space_point_light_pos = tbn_inv*u_pl_pos;
    vs_data.tanget_space_camera_pos = tbn_inv*u_camera_pos;
}
