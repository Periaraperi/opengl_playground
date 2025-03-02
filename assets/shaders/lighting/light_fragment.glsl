#version 460 core

in VS_DATA {
    vec3 frag_pos;
    vec4 light_space_frag_pos;
    vec2 texture_coordinates;
    vec3 normal;
} vs_data;

out vec4 frag_color;

struct Shared_Data {
    vec3 norm;
    vec3 view_dir;
    vec3 sampled_diffuse;
} shared_data;

struct Directional_Light {
    vec3 direction;
    vec3 pos; // fake pos needed for shadowmapping

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spot_Light {
    vec3 pos;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cos_inner_angle;
    float cos_outer_angle;
};

struct Point_Light {
    vec3 ambient;
};

uniform sampler2D u_texture;
uniform sampler2D u_shadowmap;
uniform vec3      u_camera_pos;

uniform float     u_max_shadow_bias;
uniform float     u_min_shadow_bias;

uniform bool      u_toggle_shadows;
uniform bool      u_toggle_directional_light;
uniform bool      u_toggle_spot_lights;

uniform Directional_Light u_directional_light;
uniform Spot_Light u_spot_lights[1];
uniform Point_Light u_point_lights[1];

// get shadow value for directional and spot light shadow mapping
// 1.0f means fully lit
// 0.0f means fully in shadow
float shadow_value(vec3 norm, vec3 light_dir)
{
    // normalize to [-1, 1] range if perspective projection was used
    vec3 fp = vec3(vs_data.light_space_frag_pos) / vs_data.light_space_frag_pos.w;

    // fragment is outside of lights projection frustum, so return full light
    if (fp.z > 1.0f) {
        return 1.0f;
    }

    // transform [-1, 1] to [0, 1] because of texture space
    fp = (fp + vec3(1.0f)) * 0.5f;

    // depth value that is closest to light source
    float closest_depth = texture(u_shadowmap, fp.xy).r;

    // depth value of current fragment
    float current_depth = fp.z;

    //float bias = max(u_max_shadow_bias*(1.0f - dot(norm, light_dir)), u_min_shadow_bias);
    float bias = u_max_shadow_bias;

    float shadow = current_depth-bias > closest_depth ? 0.0f : 1.0f;
    return shadow;
}

vec3 calc_directional_light()
{
    vec3 light_direction = normalize(-u_directional_light.direction);

    vec3 ambient = u_directional_light.ambient;

    float diffuse_intensity = max(dot(shared_data.norm, light_direction), 0.0f);
    vec3 diffuse = diffuse_intensity * u_directional_light.diffuse;

    vec3 halfway = normalize(shared_data.view_dir + light_direction);
    float specular_intensity = pow(max(dot(halfway, shared_data.norm), 0.0f), 32);
    vec3 specular = specular_intensity * u_directional_light.specular;

    float shadow = 1.0f;
    if (u_toggle_shadows) {
        shadow = shadow_value(shared_data.norm, normalize(u_directional_light.pos-vs_data.frag_pos));
    }
    return (ambient + shadow*(diffuse + specular)) * shared_data.sampled_diffuse;
}

vec3 calc_spot_light(Spot_Light spot_light)
{
    vec3 light_direction = normalize(spot_light.pos-vs_data.frag_pos);

    float phi = dot(light_direction, normalize(-spot_light.direction));
    float cone_smoothing = 0.0f;

    cone_smoothing = clamp((phi-spot_light.cos_outer_angle) / (spot_light.cos_inner_angle - spot_light.cos_outer_angle), 0.0f, 1.0f);

    vec3 ambient = spot_light.ambient;

    float diffuse_intensity = max(dot(shared_data.norm, light_direction), 0.0f);
    vec3 diffuse = diffuse_intensity * spot_light.diffuse * cone_smoothing;

    vec3 halfway = normalize(shared_data.view_dir + light_direction);
    float specular_intensity = pow(max(dot(halfway, shared_data.norm), 0.0f), 32);
    vec3 specular = specular_intensity * spot_light.specular * cone_smoothing;

    float shadow = 1.0f;
    if (u_toggle_shadows) {
        shadow = shadow_value(shared_data.norm, light_direction);
    }
    return (ambient + shadow*(diffuse + specular)) * shared_data.sampled_diffuse;
}

vec3 calc_point_light(Point_Light light)
{ return vec3(0); }

void main()
{
    shared_data.norm = normalize(vs_data.normal);
    shared_data.view_dir = normalize(u_camera_pos - vs_data.frag_pos);
    shared_data.sampled_diffuse = texture(u_texture, vs_data.texture_coordinates).xyz;

    vec3 light_color = vec3(0.0f);

    if (u_toggle_directional_light) {
        light_color += calc_directional_light();
    }

    if (u_toggle_spot_lights) {
        for (int i=0; i<1; ++i) {
            light_color += calc_spot_light(u_spot_lights[i]);
        }
    }
    
    frag_color = vec4(light_color, 1.0f);
}
