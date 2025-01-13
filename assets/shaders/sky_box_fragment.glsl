#version 460 core

in vec3 direction;

out vec4 fragment_color;

uniform samplerCube u_cubemap;

void main()
{
    fragment_color = texture(u_cubemap, direction);
}
