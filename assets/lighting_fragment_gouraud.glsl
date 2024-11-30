#version 460 core

uniform vec3 u_object_color;
in vec3 color;
out vec4 frag_color;

void main()
{
    frag_color = vec4(color*u_object_color, 1.0f);
}
