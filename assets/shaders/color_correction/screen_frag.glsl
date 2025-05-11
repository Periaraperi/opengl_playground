#version 460 core

in vec2 texture_coordinates;
out vec4 fragment_color;

// floating point color attachment for our scene
uniform sampler2D u_hdr_color_texture;

uniform bool  u_gamma;
uniform bool  u_hdr;
uniform float u_exposure;

void main()
{
    const float gamma = 2.2f;
    vec3 hdr_color = texture(u_hdr_color_texture, texture_coordinates).rgb;
    vec3 final_color = vec3(0.0f);

    if (u_hdr) {
        // simple reinhard tone mapper
        //final_color = hdr_color / (hdr_color + vec3(1.0f));

        // the one with exposure
        final_color = vec3(1.0f) - exp(-hdr_color*u_exposure);
    }
    else {
        final_color = hdr_color;
    }

    if (u_gamma) {
        final_color = pow(final_color, vec3(1.0f / gamma));
    }

    fragment_color = vec4(final_color, 1.0f);
}
