#version 460 core

in VS_DATA {
    vec3 frag_pos;
    vec3 normal;
    vec2 texture_coordinates;
    vec4 frag_pos_light_space;
} vs_data;
out vec4 frag_color;

struct Spot_Light {
    vec3 pos;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    // for attenuation
    float linear;
    float quadratic;

    // cosines of inner and outer cutoff angle of the spotlight cone
    float inner_cosine_angle;
    float outer_cosine_angle;
};

struct Shared_Data {
    vec3 base_diffuse_color;
    vec3 norm;
    vec3 view_direction;
} shared_data;

uniform vec3 u_view_pos;
uniform sampler2D u_diffuse_texture;
uniform sampler2D u_shadowmap;
uniform Spot_Light u_spot_light;
uniform float u_min_bias;
uniform float u_max_bias;

float get_shadow_value(vec3 norm, vec3 light_dir) // both must be passed as normalized
{
    // in the range of [-1, 1] from [-w, w]
    vec3 frag_pos_lp = vs_data.frag_pos_light_space.xyz / vs_data.frag_pos_light_space.w;
    
    // this is outside of view frustum of light so distant object don't have shadows
    if (frag_pos_lp.z > 1.0f) {
        return 1.0f;
    }

    // in the range of [0, 1]
    frag_pos_lp = frag_pos_lp*0.5f + vec3(0.5f);

    // depth value that is closest to light source on the ray that goes from current fragment to plane of light source
    float closest_depth = texture(u_shadowmap, frag_pos_lp.xy).r; 

    float current_depth = frag_pos_lp.z; // current depth of the fragment that we are rendering now

    float bias = max(u_max_bias*(1.0f - dot(norm, light_dir)), u_min_bias);
    float sh = (current_depth-bias > closest_depth) ? 0.0f : 1.0f;
    return sh;
}

vec3 calculate_spot_light(Spot_Light spl)
{
    vec3 light_direction = normalize(spl.pos - vs_data.frag_pos);
    float phi = dot(light_direction, -normalize(spl.direction));
    float cone_smoothing = clamp((phi - spl.inner_cosine_angle) / (spl.inner_cosine_angle - spl.outer_cosine_angle), 0.0f, 1.0f);

    vec3 ambient_light = spl.ambient * shared_data.base_diffuse_color;

    float diffuse_intensity = max(dot(light_direction, shared_data.norm), 0.0f);
    vec3 diffuse_light = spl.diffuse * diffuse_intensity * shared_data.base_diffuse_color * cone_smoothing;

    vec3 half_way = normalize(light_direction+shared_data.view_direction);
    float specular_intensity = pow(max(dot(half_way, shared_data.norm), 0.0f), 32.0f);
    vec3 specular_light = spl.specular * specular_intensity * shared_data.base_diffuse_color * cone_smoothing;

    float shadow = get_shadow_value(shared_data.norm, light_direction);

    return ambient_light + shadow*(diffuse_light + specular_light);
}

void main()
{
    shared_data.base_diffuse_color = texture(u_diffuse_texture, vs_data.texture_coordinates).rgb;
    shared_data.norm = normalize(vs_data.normal);
    shared_data.view_direction = normalize(u_view_pos - vs_data.frag_pos);

    vec3 light_color = calculate_spot_light(u_spot_light);

    frag_color = vec4(light_color, 1.0f);
}
