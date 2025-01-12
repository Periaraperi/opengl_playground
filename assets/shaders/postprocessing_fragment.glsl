#version 460 core

in vec2 texture_coordinates;

out vec4 frag_color;

uniform sampler2D u_texture;
uniform float u_offset_x;
uniform float u_offset_y;

vec2 offsets[9] = vec2[](
    vec2(0.0f, 0.0f),
    vec2(u_offset_x, 0.0f),
    vec2(0.0f, u_offset_y),
    vec2(-u_offset_x, 0.0f),
    vec2(0.0f, -u_offset_y),
    vec2(u_offset_x, u_offset_y),
    vec2(-u_offset_x, u_offset_y),
    vec2(u_offset_x, -u_offset_y),
    vec2(-u_offset_x, -u_offset_y)
);

vec4 inversed_colors(vec4 t)
{ return vec4(1.0f-t.r, 1.0f-t.g, 1.0f-t.b, 1.0f); }

vec4 grey_scale(vec4 t)
{
    vec3 avg = vec3(t.r + t.g + t.b) / 3.0f;
    return vec4(avg.r, avg.g, avg.b, 1.0f);
}

vec4 grey_scale_weighted_average(vec4 t)
{
    vec3 avg = vec3(0.2126*t.r + 0.7152*t.g + 0.072*t.b) / 3.0f;
    return vec4(avg.r, avg.g, avg.b, 1.0f);
}

vec4 sharpen_effect(vec4 t)
{
    float kernel[9] = float[](
        2.0f, 2.0f,   2.0f,
        2.0f, -15.0f, 2.0f,
        2.0f, 2.0f,   2.0f
    );

    vec3 col = vec3(0.0f, 0.0f, 0.0f);
    for (int i=0; i<9; ++i) {
        vec3 sampled_value = vec3(texture(u_texture, vec2(texture_coordinates)+offsets[i]));
        col += sampled_value * kernel[i];
    }
    return vec4(col, 1.0f);
}

vec4 simple_blur(vec4 t)
{
    float kernel[9] = float[](
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    );

    vec3 col = vec3(0.0f, 0.0f, 0.0f);
    for (int i=0; i<9; ++i) {
        vec3 sampled_value = vec3(texture(u_texture, vec2(texture_coordinates)+offsets[i]));
        col += sampled_value * kernel[i];
    }
    return vec4(col, 1.0f);
}

vec4 edge_detection(vec4 t)
{
    float kernel[9] = float[](
        1.0f, 1.0f,  1.0f,
        1.0f, -8.0f, 1.0f,
        1.0f, 1.0f,  1.0f
    );

    vec3 col = vec3(0.0f, 0.0f, 0.0f);
    for (int i=0; i<9; ++i) {
        vec3 sampled_value = vec3(texture(u_texture, vec2(texture_coordinates)+offsets[i]));
        col += sampled_value * kernel[i];
    }
    return vec4(col, 1.0f);
}

void main()
{
    vec4 t = texture(u_texture, texture_coordinates);
    //frag_color = inversed_colors(t);
    //frag_color = grey_scale(t);
    //frag_color = grey_scale_weighted_average(t);
    //frag_color = sharpen_effect(t);
    //frag_color = inversed_colors(grey_scale_weighted_average(sharpen_effect(t)));
    //frag_color = simple_blur(t);
    frag_color = edge_detection(t);
}
