#version 460 core

out vec4 fragment_color;

uniform vec3 u_light_color;

void main()
{
    fragment_color = vec4(u_light_color, 1.0f);
}
