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
};

struct Spot_Light {
    vec3 ambient;
};

struct Point_Light {
    vec3 ambient;
};

uniform sampler2D u_texture;
uniform sampler2D u_shadowmap;
uniform vec3 u_camera_pos;

uniform Directional_Light u_directional_light;
uniform Spot_Light u_spot_lights[1];
uniform Point_Light u_point_lights[1];

vec3 calc_directional_light()
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

vec3 calc_spot_light(Spot_Light light)
{ return vec3(0); }

vec3 calc_point_light(Point_Light light)
{ return vec3(0); }

void main()
{
    shared_data.norm = normalize(vs_data.normal);
    shared_data.view_dir = normalize(u_camera_pos - vs_data.frag_pos);
    shared_data.sampled_diffuse = texture(u_texture, vs_data.texture_coordinates).xyz;

    vec3 light_color = vec3(0.0f);

    light_color += calc_directional_light();
    
    frag_color = vec4(light_color, 1.0f);
}
