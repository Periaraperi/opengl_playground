#version 460 core

out vec4 final_color;

in vec2 frag_pos;
in vec2 center;
in vec3 color;
in float radius;

void main()
{
    vec2 p = frag_pos - center;
    float plen = length(p);
    float s = 1.0f-smoothstep(radius-1.01f, radius, plen);
    final_color = vec4(color*s, s);
}


