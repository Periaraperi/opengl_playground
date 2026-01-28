#version 460 core

in VS_DATA {
    vec3 frag_pos;
    vec3 normal;
    vec2 texture_coordinates;
} vs_data;

layout (location = 0) out vec4 g_position;
layout (location = 1) out vec4 g_fragment_color;
layout (location = 2) out vec4 g_normal;

uniform sampler2D u_diffuse_texture;

void main()
{
    g_position = vec4(vs_data.frag_pos, 1.0f);
    g_fragment_color = vec4(texture(u_diffuse_texture, vs_data.texture_coordinates).rgb, 1.0f);
    g_normal = vec4(normalize(vs_data.normal), 1.0f);
}
