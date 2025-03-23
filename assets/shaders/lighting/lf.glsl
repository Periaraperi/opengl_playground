#version 460 core

in VS_DATA {
    vec3 frag_pos;
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
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 pos;
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
    vec3 pos;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define MAX_SPOT_LIGHTS 32
#define MAX_POINT_LIGHTS 32
layout(std140, binding = 0) uniform Spot_Lights {
    Directional_Light u_directional_light;
    Spot_Light        u_spls[MAX_SPOT_LIGHTS];
    Point_Light       u_pls[MAX_POINT_LIGHTS];
    int               u_spls_count;
    int               u_pls_count;
};

uniform sampler2D u_texture;
uniform vec3      u_camera_pos;

vec3 calc_dir_light()
{
    vec3 light_direction = normalize(-u_directional_light.direction);

    vec3 ambient = u_directional_light.ambient;

    float diffuse_intensity = max(dot(shared_data.norm, light_direction), 0.0f);
    vec3 diffuse = diffuse_intensity * u_directional_light.diffuse;

    vec3 halfway = normalize(shared_data.view_dir + light_direction);
    float specular_intensity = pow(max(dot(halfway, shared_data.norm), 0.0f), 32);
    vec3 specular = specular_intensity * u_directional_light.specular;

    return (ambient + diffuse + specular) * shared_data.sampled_diffuse;
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

    return (ambient + diffuse + specular) * shared_data.sampled_diffuse;
}

vec3 calc_point_light(Point_Light pl)
{
    vec3 light_direction = normalize(pl.pos - vs_data.frag_pos);
    float dis = distance(pl.pos, vs_data.frag_pos);

    float attn = 1.0f / (pl.constant + pl.linear*dis + pl.quadratic*dis*dis);

    // don't apply attn value for ambient
    vec3 ambient = pl.ambient;

    float diffuse_intensity = max(dot(shared_data.norm, light_direction), 0.0f);
    vec3 diffuse = diffuse_intensity * pl.diffuse * attn;

    vec3 halfway = normalize(shared_data.view_dir + light_direction);
    float specular_intensity = pow(max(dot(halfway, shared_data.norm), 0.0f), 32);
    vec3 specular = specular_intensity * pl.specular * attn;

    return (ambient + diffuse + specular) * shared_data.sampled_diffuse;
}

void main()
{
    shared_data.norm = normalize(vs_data.normal);
    shared_data.view_dir = normalize(u_camera_pos - vs_data.frag_pos);
    shared_data.sampled_diffuse = texture(u_texture, vs_data.texture_coordinates).xyz;

    vec3 light_color = vec3(0.0f);

    light_color += calc_dir_light();

    for (int i=0; i<u_spls_count; ++i) {
        light_color += calc_spot_light(u_spls[i]);
    }

    for (int i=0; i<u_pls_count; ++i) {
        light_color += calc_point_light(u_pls[i]);
    }
    
    frag_color = vec4(light_color, 1.0f);
}
