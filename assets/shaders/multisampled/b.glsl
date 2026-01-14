#version 460 core

in vec2 texture_coordinates;
uniform sampler2D u_texture;

out vec4 frag_color;

void main()
{
    frag_color = texture(u_texture, texture_coordinates);
}
