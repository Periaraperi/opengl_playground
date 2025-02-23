#version 460 core

in VS_DATA {
    vec2 texture_coordinates;
    vec4 color;
    float texture_slot;
} vs_data;

out vec4 fragment_color;

uniform sampler2D u_textures[8];

void main()
{
    int slot = int(vs_data.texture_slot);
    fragment_color = texture(u_textures[slot], vs_data.texture_coordinates)*vs_data.color;
}
