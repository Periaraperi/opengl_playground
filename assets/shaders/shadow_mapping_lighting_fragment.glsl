#version 460 core

// ===================================== io vars =========================================
in Vs_Out {
    vec2 texture_coordinates;
    vec3 normal;
    vec3 frag_pos;
    vec4 frag_pos_light_space;
} vs_data;

out vec4 fragment_color;
// =======================================================================================

// ===================================== structures ======================================
struct Directional_Light {
    vec3 direction;
    vec3 pos; // fake position used for shadows

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// shared data for lighting calculations
struct Shared_Data {
    vec3 norm;
    vec3 fragment_base_color; // base diffuse texture color
    vec3 view_dir;
} shared_data;
// =======================================================================================


// ===================================== uniforms ========================================
uniform sampler2D u_texture;
uniform sampler2D u_shadowmap_texture;
uniform vec3 u_view_pos;
uniform Directional_Light u_directional_light;
uniform float u_min_bias;
uniform float u_max_bias;
// =======================================================================================

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
    float closest_depth = texture(u_shadowmap_texture, frag_pos_lp.xy).r; 

    float current_depth = frag_pos_lp.z; // current depth of the fragment that we are rendering now

    //float sh = (current_depth-u_bias > closest_depth) ? 0.0f : 1.0f;
    float bias = max(u_max_bias*(1.0f - dot(norm, light_dir)), u_min_bias);
    float sh = (current_depth-bias > closest_depth) ? 0.0f : 1.0f;
    return sh;
}

vec3 calc_directional_light()
{
    vec3 light_direction = normalize(-u_directional_light.direction); // from pixel to light

    // ambient
    vec3 ambient_light = u_directional_light.ambient * shared_data.fragment_base_color;

    // diffuse
    float diffuse_intensity = max(dot(shared_data.norm, light_direction), 0.0f);
    vec3 diffuse_light = u_directional_light.diffuse * diffuse_intensity * shared_data.fragment_base_color;

    // specular
    vec3 half_way = normalize(light_direction+shared_data.view_dir);
    float specular_intensity = pow(max(dot(shared_data.norm, half_way), 0.0f), 32.0f);
    vec3 specular_light = u_directional_light.specular * specular_intensity * shared_data.fragment_base_color; // use of specular map for instead of fragment_base_color is better.

    float shadow = get_shadow_value(shared_data.norm, normalize(u_directional_light.pos - vs_data.frag_pos));

    return ambient_light + (diffuse_light + specular_light) * shadow;
}

void main()
{
    shared_data.fragment_base_color = texture(u_texture, vs_data.texture_coordinates).rgb;
    shared_data.norm = normalize(vs_data.normal);
    shared_data.view_dir = normalize(u_view_pos - vs_data.frag_pos);

    vec3 final_color = vec3(0.0f);

    final_color += calc_directional_light();

    fragment_color = vec4(final_color, 1.0f);
}
