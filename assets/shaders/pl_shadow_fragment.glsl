#version 460 core

in vec4 frag_pos;

uniform vec3 u_light_pos; // point light pos in world space
uniform float u_far_plane;
uniform float u_near_plane;

void main()
{
    float frag_light_dis = length(vec3(frag_pos) - u_light_pos); // each cubemap face will store distance from fragment to light source.
    float frag_light_dis_normalized = frag_light_dis / u_far_plane;
    gl_FragDepth = frag_light_dis_normalized;

    //vec3 a = abs(vec3(frag_pos) - u_light_pos);
    //float local_z = max(a.x, max(a.y, a.z));

    //float normalized = (u_far_plane+u_near_plane) / (u_far_plane-u_near_plane) - (2*u_far_plane*u_near_plane) / (u_far_plane-u_near_plane) / local_z;
    //gl_FragDepth = (normalized + 1.0f) *0.5f;
}
