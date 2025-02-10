#version 460 core

in VS_DATA {
    vec3 frag_pos;
    vec3 normal;
    vec2 texture_coordinates;
} vs_data;

out vec4 frag_color;

struct Point_Light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Shared_Data {
    vec3 base_diffuse_color;
    vec3 norm;
    vec3 view_direction;
} shared_data;

uniform vec3 u_view_pos;
uniform sampler2D u_diffuse_texture;
uniform samplerCube u_shadow_cubemap;
uniform Point_Light u_point_light;
uniform float u_far_plane;
uniform float u_min_bias;
uniform float u_max_bias;
uniform bool u_do_pcf;

float get_shadow_value(vec3 light_to_frag_direction)
{
    float sampled_depth = texture(u_shadow_cubemap, light_to_frag_direction).r;
    sampled_depth *= u_far_plane; // back to original range from [0, 1]
    //frag_color = vec4(vec3(sampled_depth / u_far_plane), 1.0f);

    float light_to_fragment_distance = length(light_to_frag_direction);

    float bias = 0.002f;
    if (light_to_fragment_distance > sampled_depth + bias) {
        return 1.0f;
    }
    else {
        return 0.0f;
    }
}

vec3 calculate_point_light(Point_Light pl)
{
    vec3 light_direction = normalize(pl.pos - vs_data.frag_pos);

    vec3 ambient_light = pl.ambient * shared_data.base_diffuse_color;

    float diffuse_intensity = max(dot(light_direction, shared_data.norm), 0.0f);
    vec3 diffuse_light = pl.diffuse * diffuse_intensity * shared_data.base_diffuse_color;

    vec3 half_way = normalize(light_direction + shared_data.view_direction);
    float specular_intensity = pow(max(dot(half_way, shared_data.norm), 0.0f), 32.0f);
    vec3 specular_light = pl.specular * specular_intensity * shared_data.base_diffuse_color;

    float shadow = get_shadow_value(vs_data.frag_pos - pl.pos);

    return ambient_light + shadow*(diffuse_light + specular_light);
}

void main()
{
    shared_data.base_diffuse_color = texture(u_diffuse_texture, vs_data.texture_coordinates).rgb;
    shared_data.norm = normalize(vs_data.normal);
    shared_data.view_direction = normalize(u_view_pos - vs_data.frag_pos);

    vec3 light_color = calculate_point_light(u_point_light);

    frag_color = vec4(light_color, 1.0f);
}
