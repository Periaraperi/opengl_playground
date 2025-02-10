#version 460 core

in vec4 frag_pos;

uniform vec3 u_light_pos; // point light pos in world space
uniform float u_far_plane;

void main()
{
    float frag_light_dis = length(vec3(frag_pos) - u_light_pos); // each cubemap face will store distance from fragment to light source.
    float frag_light_dis_normalized = frag_light_dis / u_far_plane;

    gl_FragDepth = frag_light_dis_normalized;
}
