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

struct Spot_Light {
    vec3 pos;
    // float pad[1]
    vec3 direction;
    // float pad[1]
    
    vec3 ambient;
    // float pad[1]
    vec3 diffuse;
    // float pad[1]
    vec3 specular;

    float cos_inner_angle;
    float cos_outer_angle;
    // float pad[3]
};

struct Sl {
    float px, py, pz;
    float dx, dy, dz;
    float ambient_r,  ambient_g,  ambient_b;
    float diffuse_r,  diffuse_g,  diffuse_b;
    float specular_r, specular_g, specular_b;
    float cos_inner_angle;
    float cos_outer_angle;
};

#define MAX_SPOT_LIGHTS 32
layout(std140, binding = 0) uniform Spot_Lights {
    Spot_Light u_spls[MAX_SPOT_LIGHTS];
    int        u_spls_count;
};


uniform sampler2D u_texture;
uniform vec3      u_camera_pos;

vec3 calc_spot_light2(Sl spot_light)
{
    return vec3(0,0,0);
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

void main()
{
    shared_data.norm = normalize(vs_data.normal);
    shared_data.view_dir = normalize(u_camera_pos - vs_data.frag_pos);
    shared_data.sampled_diffuse = texture(u_texture, vs_data.texture_coordinates).xyz;

    vec3 light_color = vec3(0.0f);

    for (int i=0; i<u_spls_count; ++i) {
        light_color += calc_spot_light(u_spls[i]);
    }
    
    frag_color = vec4(light_color, 1.0f);
}
