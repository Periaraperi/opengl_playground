#version 460 core

in VS_DATA {
    vec3 frag_pos;
    vec3 normal;
    vec2 texture_coordinates;
    mat3 tbn;
} vs_data;

out vec4 frag_color;

uniform sampler2D u_wall;
uniform sampler2D u_wall_normal;
uniform vec3 u_camera_pos;

struct Shared_Data {
    vec3 view_dir;
    vec3 normal;
    vec3 sampled_diffuse;
} shared_data;

struct Point_Light {
    vec3 pos;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Point_Light u_pl;
uniform bool u_normal_mapping;

vec3 calc_point_light(Point_Light pl)
{
    vec3 light_direction = normalize(pl.pos - vs_data.frag_pos);
    float dis = distance(pl.pos, vs_data.frag_pos);

    float attn = 1.0f / (pl.constant + pl.linear*dis + pl.quadratic*dis*dis);

    // don't apply attn value for ambient
    vec3 ambient = pl.ambient;

    vec3 normal = vec3(0.0f);
    if (u_normal_mapping) {
        normal = texture(u_wall_normal, vs_data.texture_coordinates).rgb;
        normal = normal*2.0f - 1.0f;
        normal = normalize(vs_data.tbn*normal);
    }
    else {
        normal = shared_data.normal; // surface normal, use this if no normal mapping
    }

    float diffuse_intensity = max(dot(normal, light_direction), 0.0f);
    vec3 diffuse = diffuse_intensity * pl.diffuse * attn;

    vec3 halfway = normalize(shared_data.view_dir + light_direction);
    float specular_intensity = pow(max(dot(halfway, normal), 0.0f), 32);
    vec3 specular = specular_intensity * pl.specular * attn;

    return (ambient + diffuse + specular) * shared_data.sampled_diffuse;
}

void main()
{
    shared_data.view_dir = normalize(u_camera_pos - vs_data.frag_pos);
    shared_data.sampled_diffuse = texture(u_wall, vs_data.texture_coordinates).xyz;
    shared_data.normal = normalize(vs_data.normal);
    
    vec3 light_color = calc_point_light(u_pl);

    frag_color = vec4(light_color, 1.0f);
}
