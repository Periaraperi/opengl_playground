#version 460

in vec2 texture_coordinates;
out vec4 fragment_color;

uniform sampler2D u_texture;

void main()
{
    fragment_color = texture(u_texture, texture_coordinates);
}
