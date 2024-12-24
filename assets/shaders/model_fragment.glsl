#version 460 core

in vec2 texture_coordinates;
out vec4 frag_color;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_diffuse2;
uniform sampler2D u_texture_diffuse3;
uniform sampler2D u_texture_specular1;
uniform sampler2D u_texture_specular2;

void main()
{
    frag_color = texture(u_texture_diffuse1, texture_coordinates);
}
