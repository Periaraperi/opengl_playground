#version 460 core

in vec2 texture_coordinates;
out vec4 fragment_color;

uniform sampler2D u_texture;
uniform vec3 u_color;

void main()
{
    fragment_color = vec4(u_color*texture(u_texture, texture_coordinates).rgb, 1.0f);
}
