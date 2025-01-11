#version 460 core

in vec2 texture_coordinates;

out vec4 frag_color;

uniform sampler2D u_texture;

void main()
{
    frag_color = texture(u_texture, texture_coordinates);
}
