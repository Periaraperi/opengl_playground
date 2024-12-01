#version 460 core

in vec2 tex_coordinates;
in vec4 color;
in float texture_unit;

out vec4 frag_color;

uniform sampler2D u_textures[3];

void main()
{
    frag_color = texture(u_textures[int(texture_unit)], tex_coordinates)*color;
}
