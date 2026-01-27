#version 460 core

in vec2 texture_coordinates;
out vec4 fragment_color;

uniform sampler2D u_hdr_color_texture;

uniform float u_exposure;

void main()
{
    const float gamma = 2.2f;
    vec3 hdr_color = texture(u_hdr_color_texture, texture_coordinates).rgb;
    vec3 final_color = vec3(0.0f);
    final_color = vec3(1.0f) - exp(-hdr_color*u_exposure);
    final_color = pow(final_color, vec3(1.0f / gamma));
    fragment_color = vec4(final_color, 1.0f);
}
