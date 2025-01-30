#version 460 core

in Vs_Out {
    vec2 texture_coordinates;
    vec3 normal;
    vec3 frag_pos;
    vec4 frag_pos_light_space;
} vs_data;

out vec4 fragment_color;

uniform sampler2D u_texture;
uniform sampler2D u_shadowmap_texture;

uniform vec3 u_view_pos;
uniform vec3 u_light_pos;
uniform float u_bias;

float get_shadow_value()
{
    // in the range of [-1, 1] from [-w, w]
    vec3 frag_pos_lp = vs_data.frag_pos_light_space.xyz / vs_data.frag_pos_light_space.w;

    // in the range of [0, 1]
    frag_pos_lp = frag_pos_lp*0.5f + vec3(0.5f);

    // depth value that is closest to light source on the ray that goes from current fragment to plane of light source
    float closest_depth = texture(u_shadowmap_texture, frag_pos_lp.xy).r; 

    float current_depth = frag_pos_lp.z; // current depth of the fragment that we are rendering now

    float sh = (current_depth-u_bias > closest_depth) ? 0.0f : 1.0f;
    return sh;
}

void main()
{
    vec3 base_color = texture(u_texture, vs_data.texture_coordinates).rgb;
    vec3 norm = normalize(vs_data.normal);
    vec3 light_dir = normalize(u_light_pos - vs_data.frag_pos);
    vec3 view_dir = normalize(u_view_pos - vs_data.frag_pos);
    vec3 half_way = normalize(light_dir + view_dir);
    vec3 light_color = vec3(1.0f);
    vec3 ambient_color = 0.15f*light_color;

    vec3 diffuse_color = max(dot(light_dir, norm), 0.0f) * light_color;
    vec3 specular_color = pow(max(dot(half_way, norm), 0.0f), 32.0f) * light_color;

    float shadow = get_shadow_value();

    fragment_color = vec4((ambient_color + shadow*(specular_color + diffuse_color))*base_color, 1.0f);

    //fragment_color = vec4(vec3(texture(u_shadowmap_texture, vs_data.texture_coordinates).r), 1.0f);
    //fragment_color = texture(u_texture, vs_data.texture_coordinates);
}
