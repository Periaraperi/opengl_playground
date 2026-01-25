#version 460 core

in vec2 texture_coordinates;

out vec4 fragment_color;

uniform sampler2D u_texture;
uniform bool u_horizontal;

void main()
{
    const vec2 texture_size = textureSize(u_texture, 0);
    const vec2 texel_size = 1.0f/texture_size;
    const float weights[5] = float[] (0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f);

    vec3 sampled = texture(u_texture, texture_coordinates).xyz * weights[0];

    if (u_horizontal) {
        for (int i=1; i<5; ++i) {
            sampled += texture(u_texture, texture_coordinates+vec2(i*texel_size.x, 0.0f)).xyz*weights[i];
            sampled += texture(u_texture, texture_coordinates-vec2(i*texel_size.x, 0.0f)).xyz*weights[i];
        }
    }
    else {
        for (int i=1; i<5; ++i) {
            sampled += texture(u_texture, texture_coordinates+vec2(0.0f, i*texel_size.y)).xyz*weights[i];
            sampled += texture(u_texture, texture_coordinates-vec2(0.0f, i*texel_size.y)).xyz*weights[i];
        }
    }
    fragment_color = vec4(sampled, 1.0f);
}
