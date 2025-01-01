#version 460 core

in vec2 texture_coordinates;
out vec4 frag_color;

uniform sampler2D u_texture;
uniform float u_near;
uniform float u_far;

void main()
{
    //frag_color = texture(u_texture, texture_coordinates);
    float ndc = gl_FragCoord.z * 2.0f - 1.0f;
    float linear_depth = (2.0f * u_near * u_far) / (u_far + u_near - ndc*(u_far - u_near));

    frag_color = vec4(vec3(linear_depth / u_far + 0.15f), 1.0f);
}
