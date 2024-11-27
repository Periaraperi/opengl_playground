#version 460 core

uniform vec4 u_object_color;
uniform vec4 u_light_source_color;

out vec4 frag_color;

void main()
{
    frag_color = vec4(u_object_color*u_light_source_color);
}


