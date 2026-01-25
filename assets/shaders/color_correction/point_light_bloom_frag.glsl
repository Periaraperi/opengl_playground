#version 460 core

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;

in VS_DATA {
    vec3 frag_pos;
    vec3 normal;
    vec2 texture_coordinates;
} vs_data;

uniform vec3 u_light_color;

void main()
{
    fragment_color = vec4(u_light_color, 1.0f);
    float brightness = dot(u_light_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if (brightness > 1.0f) {
        brightness_color = vec4(u_light_color.rgb, 1.0f);
    }
    else {
        brightness_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
